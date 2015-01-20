#include "primitive.h"
using namespace std;

primitive::primitive() {
	//all placeholders
	pos = sf::Vector3f(0.0,0.0,0.0);
	color = sf::Vector3f(0.0,1.0,0.0);
	diffuse = 7.0;
	name = string("sphere");
	DE = string("{return length(p) - 0.5; }\n");
}





	
