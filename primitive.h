#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

class primitive {
	public:
		primitive();
		float diffuse;
		string DE;
		string name;
		sf::Vector3f pos;
		sf::Vector3f color;
};
