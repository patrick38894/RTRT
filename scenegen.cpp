#include "scenegen.h"
using namespace std;


scenegen::scenegen() {
	compiled = false;
	maxRaySteps = 25;
	epsilon = .005;
	source = string("");
	ambient = sf::Vector3f(0.2,0.2,0.2);
	numLights = 0;
	numPrimitives = 0;
	utility1 = string(

"uniform vec2 screen;\n\
uniform vec3 viewDir;\n\
uniform vec3 up;\n\
uniform vec3 pos;\n\
bool hit;\n\
vec3 intersect;\n\
float rand(vec2 co){\n\
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n\
}\n\
vec3 reflect(vec3 dir, vec3 normal) {\n\
	dir = -1.0*dir;\n\
	vec3 temp = dot(dir,normal)*normal;\n\
	temp = temp - dir;\n\
	return normalize(dir + 2.0 * temp);\n\
}\n\
vec3 mix(vec3 s, vec3 l) {\n\
	return clamp(l - (vec3(1.0,1.0,1.0)-s),0.0,1.0);\n\
}\n\
vec2 offset(vec2 p) {\n\
	return p - 0.5*screen;\n\
}\n");

	utility2 = string(
"vec3 derivative(vec3 p) {\n\
float diffEpsilon = epsilon * 20.0;\n\
vec3 xx = vec3(1,0,0);\n\
vec3 yy = vec3(0,1,0);\n\
vec3 zz = vec3(0,0,1);\n\
vec3 n =vec3(\n\
		DE(intersect+diffEpsilon*xx)-DE(intersect),\n\
		DE(intersect+diffEpsilon*yy)-DE(intersect),\n\
		DE(intersect+diffEpsilon*zz)-DE(intersect));\n\
float c = 0.00001;\n\
return normalize(vec3(n.x+c*rand(p.xy),n.y+c*rand(p.yz),n.z+c*rand(p.xz)));\n\
}\n\
float trace(vec3 from, vec3 dir) {\n\
	hit = false;\n\
	intersect = vec3(0.0,0.0,0.0);\n\
	float totalDistance = 0.0;\n\
	int steps = 0;\n\
	for (;steps < maxRaySteps; ++steps) {\n\
		vec3 p = from + totalDistance * dir;\n\
		float dist = DE(p);\n\
		totalDistance += dist;\n\
		if (dist < epsilon) {\n\
			intersect = p;\n\
			hit = true;\n\
			break;\n\
		}\n\
	}\n\
	return 1.0 - float(steps)/float(maxRaySteps);\n\
}\n" ); //this function is added to the source seperately as it requires DE to be defined


	maincode = string(

"vec3 right = cross(viewDir,up);\n\
vec3 realPos = pos;\n\
vec2 screenPos = offset(gl_FragCoord.xy);\n\
screenPos = screenPos * 4.0 / screen.x;\n\
float f = 0.125;\n\
vec3 x = right * f * screenPos.x;\n\
vec3 y = up * f * screenPos.y;\n\
vec3 realDir = normalize(viewDir+x+y);\n\
realPos += right * screenPos.x;\n\
realPos += up * screenPos.y;\n\
float t = trace(realPos,realDir);\n\
vec3 normal = normalize(derivative(intersect));\n");
}

string scenegen::getSource() {
	return source;
}

string scenegen::genConstants() {
	string val = string("int maxRaySteps = ");
	val += to_string(maxRaySteps) + string(";\n");
	val += string("float epsilon = ");
	val += to_string(epsilon) + string(";\n");
	return val;
}



void scenegen::addPrimitive(primitive p) {
	primitives.push_back(p);
	++numPrimitives;
}

void scenegen::addLight(light l) {
	lights.push_back(l);
	++numLights;
}

void scenegen::compile() {
	source = string("");
	source += utility1;
	
	string constants = genConstants();
	string lightDecls = combineLights();
	string de = combinePrimitives();
	string colorFunction = compileColorFunc();
	
	source += constants;
	source += de;
	source += utility2;
	source += string("void main(void) {\n");
	source += lightDecls;
	source += maincode;
	source += colorFunction;
	source += string("}\n");
	compiled = true;

}

string scenegen::compileColorFunc() {
	//placeholder function
	return string(
"if (intersect == vec3(0.0,0.0,0.0))\n\
	gl_FragColor = vec4(0.0,0.0,0.0,1.0);\n\
else {\n\
	//vec3 surfaceColor = colormap(t,intersect);\n\
	vec3 ambient = vec3(") +
		to_string(ambient.x) + string(", ") +
		to_string(ambient.y) + string(", ") +
		to_string(ambient.z) + string(");\n") +
	string(
"	vec3 surfaceColor = primColors[colorIndex];\n\
	vec3 lightColor = vec3(0.0,0.0,0.0);\n\
	const int maxReflections = 3;\n\
	const float maxReflectionsf = 3.0;\n\
	vec3 lightStack[maxReflections];\n\
	vec3 surfStack[maxReflections];\n\
	for (int j = 0; j < maxReflections; ++j) {\n\
		lightStack[j] = vec3(0.0,0.0,0.0);\n\
		surfStack[j] = vec3(0.0,0.0,0.0);\n\
	}\n\
	for (int j = 0; j < maxReflections; ++j) {\n\
		if (!hit)\n\
			break;\n\
		lightStack[j] = ambient;\n\
		surfStack[j] = primColors[colorIndex];\n\
		for (int i = 0; i < numLights; ++i) {\n\
			if (dot(normal,lightPositions[i]) > 0.0)\n\
				lightStack[j] += lightColors[i] * lightIntensities[i] * pow(dot(normal,normalize(lightPositions[i])),primDiffuses[i]);\n\
		}\n\
		vec3 newDir = reflect(viewDir, normal);\n\
		vec3 newPos = intersect + 2.0 * epsilon * newDir;\n\
		float t = trace(newPos, newDir);\n\
		normal = derivative(intersect);\n\
	}\n\
	float factor = pow(2,1.0-maxReflectionsf);\n\
	//float factor = 1.0;\n\
	for (int i = maxReflections-1; i > 0; --i) { \n\
		lightStack[i-1] += factor * mix(lightStack[i], surfStack[i]);\n\
		factor = factor * 2.0;\n\
	}\n\
	factor = factor * 2.0;\n\
	lightColor = mix(lightStack[0],surfStack[0]);\n\
	gl_FragColor.r = lightColor.x;\n\
	gl_FragColor.g = lightColor.y;\n\
	gl_FragColor.b = lightColor.z;\n\
	gl_FragColor.a = 1.0;\n\
}\n");
}

string scenegen::combinePrimitives() {
	//placeholder function
	string code("");
	int fnum = 0;
	for (vector<primitive>::iterator it = primitives.begin() ; it != primitives.end(); ++it) {
		code += string("float DE") + to_string(fnum++) + string(" (vec3 p)\n");
		code += it->DE + string("\n");
	}
	int k = 0;
	code += string("const int numPrimitives = ") + to_string(numPrimitives) + ";\n";
	code += string("int colorIndex = 0;\n");
	code += string("float primDiffuses[numPrimitives];\n");
	code += string("vec3 primColors[numPrimitives];\n");
	code += string("float DE(vec3 pos) {\n");
	code += string("vec3 primPositions[numPrimitives];\n");
	for (vector<primitive>::iterator it = primitives.begin() ; it != primitives.end(); ++it) {
		code += string("primPositions[") + to_string(k) + "] = vec3("  + to_string(it-> pos.x) + "," + to_string(it-> pos.y) + "," + to_string(it->pos.z) + ");\n";
		code += string("primColors[") + to_string(k) + "] = vec3("  + to_string(it-> color.x) + "," + to_string(it-> color.y) + "," + to_string(it->color.z) + ");\n";
		code += string("primDiffuses[") + to_string(k++) + "] = " + to_string(it->diffuse) + ";\n";
	}
	
	code += string("float primDEresults[numPrimitives];\n");
	code += string("int j = 0;\n");
	for (int i = 0; i < numPrimitives; ++i)
		code += string("primDEresults[j] = DE") + to_string(i) + "(pos+primPositions[j]);\nj++;\n";
	code += string(
"float min = 99999.99; \n\
for (int i = 0; i < numPrimitives; ++i) \n\
	if (primDEresults[i] < min) {\n\
		min = primDEresults[i]; \n\
		colorIndex = i;\n\
	}\n\
return min; \n\
} \n" );
	return code;
}

string scenegen::combineLights() {
	int count = 0;
	string code = string("\nconst int numLights = ") + to_string(numLights) + ";\n";
	code += string("vec3 lightPositions[numLights];\n");
	code += string("float lightIntensities [numLights];\n");
	code += string("float lightDiffuses[numLights];\n");
	code += string("vec3 lightColors[numLights];\n");
	for (vector<light>::iterator it = lights.begin() ; it != lights.end(); ++it) {
		code += string("lightPositions[") + to_string(count) + "] = vec3("  + to_string(it-> pos.x) + "," + to_string(it-> pos.y) + "," + to_string(it->pos.z) + ");\n";

		code += string("lightIntensities[") + to_string(count) + "] = " + to_string(it->intensity) + ";\n";

		code += string("lightDiffuses[") + to_string(count) + "] = "  + to_string(it->diffuse) + ";\n";

		code += string("lightColors[") + to_string(count) + "] = vec3("  + to_string(it-> color.x) + "," + to_string(it-> color.y) + "," + to_string(it->color.z) + ");\n";
	}
	
	return code;
}

bool scenegen::write(const char * filename) {
	if (!compiled) {
		cout << "source has not yet been compiled" << endl;
		return false;
	}
	ofstream myfile;
	myfile.open (filename);
	myfile << (char*) source.c_str();
	myfile.close();
	return true;
}

bool scenegen::isCompiled() {
	return compiled;
}




	
