#include "primitive.h"
using namespace std;

/**********special thanks to Inigo Quilez for coming up with these great DE functinos*********/

primitive::primitive() {
	//all placeholders
	pos = sf::Vector3f(0.0,0.0,0.0);
	color = sf::Vector3f(0.0,1.0,0.0);
	diffuse = 2.0;
	spec = 35.0;
	name = string("sphere");
	DE = string("{return length(p) - 0.5; \n}\n");
}

sphere::sphere(float r) {
	pos = sf::Vector3f(0.0,0.0,0.0);
	color = sf::Vector3f(1.0,1.0,1.0);
	diffuse = 2.0;
	spec = 35.0;
	name = string("sphere");
	DE = string("{return length(p) - ") + to_string(r) + string(";\n}\n");

}


torus::torus(float r1, float r2) {
	pos = sf::Vector3f(0.0,0.0,0.0);
	color = sf::Vector3f(1.0,1.0,1.0);
	diffuse = 2.0;
	spec = 35.0;
	name = string("torus");
	DE = string("{vec2 q = vec2(length(p.xz) -")+to_string(r1)+",p.y);\n";
	DE += string("return length(q) -")+to_string(r2)+";\n}\n";
}


box::box(float x, float y, float z) {
	pos = sf::Vector3f(0.0,0.0,0.0);
	color = sf::Vector3f(1.0,1.0,1.0);
	diffuse = 2.0;
	spec = 35.0;
	name = string("box");
	DE = string("{vec3 d = abs(p) - vec3(")
		+to_string(x) + ", " 
		+to_string(y) + ", " 
		+to_string(z) + ");\n" ;
	DE += string("return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));\n}\n");
}


cone::cone(float r, float h) {
	pos = sf::Vector3f(0.0,0.0,0.0);
	color = sf::Vector3f(1.0,1.0,1.0);
	diffuse = 2.0;
	spec = 35.0;
	name = string("cone");
	r /= sqrt(pow(r,2)+pow(h,2));
	h /= sqrt(pow(r,2)+pow(h,2));
	DE = string("{vec2 c = vec2(")+to_string(r)+", "+to_string(h)+");\n";
	DE += string("float q = length(p.xy);\n");
	DE += string("return dot(c,vec2(q,p.z));\n}\n");
}





	
