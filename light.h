#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

class light {
	public:
		light();
		light(float d, float i, sf::Vector3f p, sf::Vector3f c);
		float diffuse;
		float intensity;
		sf::Vector3f pos;
		sf::Vector3f color;
};
