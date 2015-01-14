#include "primitive.h"
using namespace std;

primitive::primitive() {
	//all placeholders
	pos = sf::Vector3f(0.0,0.0,0.0);
	name = string("sphere");
	DE = string("float DE(vec3 p) {return length(p) - 1.0; }\n");
}





	
