#include "scenegen.h"
using namespace std;


scenegen::scenegen() {
	compiled = false;
	maxRaySteps = 50;
	epsilon = .0005;
	source = string("");
	numLights = 0;
	numPrimitives = 0;
	utility1 = string(

"uniform vec2 screen;\n\
uniform vec3 viewDir;\n\
uniform vec3 up;\n\
uniform vec3 pos;\n\
vec3 intersect;\n\
vec2 offset(vec2 p) {\n\
	return p - 0.5*screen;\n\
}\n");

	utility2 = string(

"float trace(vec3 from, vec3 dir) {\n\
	intersect = vec3(0.0,0.0,0.0);\n\
	float totalDistance = 0.0;\n\
	int steps = 0;\n\
	for (;steps < maxRaySteps; ++steps) {\n\
		vec3 p = from + totalDistance * dir;\n\
		float dist = DE(p);\n\
		totalDistance += dist;\n\
		if (dist < epsilon) {\n\
			intersect = p;\n\
			break;\n\
		}\n\
	}\n\
	return 1.0 - float(steps)/float(maxRaySteps);\n\
}\n" ); //this function is added to the source seperately as it requires DE to be defined


	maincode = string(

"vec3 right = cross(viewDir,up);\n\
vec3 realPos = pos;\n\
vec2 screenPos = offset(gl_FragCoord.xy);\n\
float diffEpsilon = epsilon * 10.0;\n\
screenPos = screenPos * 4.0 / screen.x;\n\
float c = 0.0;\n\
vec3 x = right * c * screenPos.x;\n\
vec3 y = up * c * screenPos.y;\n\
vec3 realDir = normalize(viewDir+x+y);\n\
realPos += right * screenPos.x;\n\
realPos += up * screenPos.y;\n\
float t = trace(realPos,realDir);\n\
vec3 xx = vec3(1,0,0);\n\
vec3 yy = vec3(0,1,0);\n\
vec3 zz = vec3(0,0,1);\n\
vec3 normal = normalize(vec3(\n\
		DE(intersect+diffEpsilon*xx)-DE(intersect),\n\
		DE(intersect+diffEpsilon*yy)-DE(intersect),\n\
		DE(intersect+diffEpsilon*zz)-DE(intersect)));\n");
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
	vec3 surfaceColor = vec3(1.0,0.0,0.0);\n\
	vec3 lightColor = vec3(0.0,0.0,0.0);\n\
	for (int i = 0; i < numLights; ++i) {\n\
		if (dot(normal,lightPos[i]) > 0)\n\
			lightColor += lightColors[i] * lightIntensities[i] * pow(dot(normal,normalize(lightPos[i])),lightDiffuses[i]);\n\
	}\n\
	gl_FragColor.r = (lightColor.x+surfaceColor.x)/2.0;\n\
	gl_FragColor.g = (lightColor.y+surfaceColor.y)/2.0;\n\
	gl_FragColor.b = (lightColor.z+surfaceColor.z)/2.0;\n\
	gl_FragColor.a = 1.0;\n\
}\n");
}

string scenegen::combinePrimitives() {
	//placeholder function
	return primitives[0].DE;
}

string scenegen::combineLights() {
	int count = 0;
	string code = string("\nconst int numlights = ") + to_string(numLights) + ";\n";
	code += string("vec3 lightPositions[numlights];\n");
	code += string("float lightIntensities [numlights];\n");
	code += string("float lightDiffuses[numlights];\n");
	code += string("vec3 lightColors[numlights];\n");
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




	
