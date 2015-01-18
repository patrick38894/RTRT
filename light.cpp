#include "light.h"
using namespace std;

light::light() {
	//placeholder function
	pos = sf::Vector3f(3,3,3);
	diffuse = 5.0;
	intensity = 1.0;
	color = sf::Vector3f(1.0,1.0,1.0);
}

light::light(float d, float i, sf::Vector3f p, sf::Vector3f c) {
	//placeholder function
	pos = p;
	diffuse = d;
	intensity = i;
	color = c;
}

