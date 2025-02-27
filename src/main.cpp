#include <iostream>
#include "app.h"


using namespace std;

int main()
{

	App* myApp = new App(glm::ivec2(1920, 1080), true);
	myApp->run();
	delete myApp;

	return 0;
	
}
