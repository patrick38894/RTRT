#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

class primitive {
	public:
		primitive();
		float diffuse;
		float spec;
		string DE;
		string name;
		sf::Vector3f pos;
		sf::Vector3f color;
};

class torus : public primitive {
	public:
		torus(float, float);
};

class sphere : public primitive {
	public:
		sphere(float);
};

class box : public primitive {
	public:
		box(float, float, float);
};

class cone : public primitive {
	public:
		cone(float, float);
};
