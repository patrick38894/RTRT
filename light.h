#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

class light {
	public:
		light();
		float diffuse;
		float intensity;
		sf::Vector3f pos;
		sf::Vector3f color;
};
