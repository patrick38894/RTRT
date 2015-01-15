#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "primitive.h"
#include "light.h"
using namespace std;

class scenegen {
	public:
		string source;

		int numLights;
		int numPrimitives;

		scenegen();
		void addPrimitive(primitive p);
		void addLight(light l);
		void compile(); //mostly just links
		string getSource();

		bool write(const char * filename);
		bool isCompiled();
		
		//utility functions which may provide useful debug info
		string compileColorFunc();
		string combinePrimitives();
		string combineLights();
		string genConstants();




	protected:
		vector<primitive> primitives;
		vector<light> lights;

		bool compiled;
		int maxRaySteps;
		float epsilon;

		string utility1;
		string utility2;
		string maincode;
};
