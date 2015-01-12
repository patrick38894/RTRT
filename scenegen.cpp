scenegen::scenegen() {
	vector<primitive> primitives;
	vector<light> lights;
	char * domain;

	bool isCompiled = false;

	int maxraysteps = 50;
	float epsilon = .005;

	string utility(

"uniform vec2 screen;\
uniform vec3 viewDir;\
uniform vec3 up;\
uniform int maxRaySteps;\
uniform float epsilon;\
uniform float time;\
vec3 intersect;\n\
vec2 offset(vec2 p) {\
	return p - 0.5*screen;\
}\
float trace(vec3 from, vec3 dir) {\
	float totalDistance = 0.0;\
	int steps = 0;\
	for (;steps < maxRaySteps; ++steps) {\
		vec3 p = from + totalDistance * dir;\
		float dist = DE(p);\
		totalDistance += dist;\
		if (dist < epsilon) {\
			intersect = p;\
			break;\
		}\
	}\
	return 1.0 - float(steps)/float(maxRaySteps);\
}\n" );


	string maincode(

"vec3 right = cross(dir,up);\
vec3 realPos = pos;\
vec2 screenPos = offset(gl_FragCoord.xy);\
float diffEpsilon = epsilon * 10;\
screenPos = screenPos * 4 / screen.x;\
float c = .125;\
vec3 x = right * c * screenPos.x;\
vec3 y = up * c * screenPos.y;\
vec3 realDir = normalize(viewDir+x+y);\
realPos += right * screenPos.x;\
realPos += up * screenPos.y;\
float t = trace(realPos,realDir);\
vec3 xx = vec3(1,0,0);\
vec3 yy = vec3(0,1,0);\
vec3 zz = vec3(0,0,1);\
vec3 normal = normalize(vec3(\
		DE(intersect+diffEpsilon*xx),\
		DE(intersect+diffEpsilon*yy),\
		DE(intersect+diffEpsilon*zz),\
));\n"






}


void scenegen::addPrimitive(primitive p) {
	primitives.push_back(p);
}

void scenegen::addLight(light l) {
	lights.push_back(l);
}

void scenegen::compile(string * source, int & len) {
	source = utility;
	
	string lightDecls combineLights();
	string de = combinePrimitives();
	
	source += de
	source += string("void main(void) {\n");
	source += lightDecls;
	source += maincode;







}



