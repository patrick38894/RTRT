#include "scenegen.h"
using namespace std;

// Here is a small helper for you ! Have a look.

int main(int, char const**)
{
    scenegen scene;
    primitive p1;
    primitive p2;
    primitive p3;
    primitive p4;
    primitive p5;
    p2.pos = sf::Vector3f(1.0,1.0,1.0);
    p2.color = sf::Vector3f(1.0,0.0,0.0);
    p3.pos = sf::Vector3f(0.0,-1.0,-1.0);
    p3.color = sf::Vector3f(0.5,0.5,1.0);
    p4.pos = sf::Vector3f(0.0,1.0,-1.0);
    p4.color = sf::Vector3f(1.0,1.0,0.0);
    p5.pos = sf::Vector3f(-0.5,-0.5,-2.0);
    p5.color = sf::Vector3f(1.0,1.0,1.0);
    scene.addPrimitive(p1);
    scene.addPrimitive(p2);
    scene.addPrimitive(p3);
    scene.addPrimitive(p4);
    scene.addPrimitive(p5);
    light l;
    scene.addLight(l);
    
    float pi = 3.14159265;
    // Create the main window
    sf::Vector2f screensize(1920,1080);
    sf::RenderWindow window(sf::VideoMode(screensize.x, screensize.y), "SFML window");

    sf::RectangleShape rectangle;
    rectangle.setSize(screensize);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(0, 0);
    
    //cout << sf::Shader::isAvailable() << endl;
    sf::Shader myshader;

    if (!myshader.loadFromFile("vertexshader" , sf::Shader::Vertex)) {
        cout << "failed to load vertex shader" << endl;
    }
    scene.compile();
    if (!scene.isCompiled())
	    cout << "failed to generate fragment shader" << endl;
    scene.write("shadercompileroutput");
    string fragmentsource = scene.getSource();
    //cout << scene.getSource() << endl;

    
    if (!myshader.loadFromMemory(fragmentsource, sf::Shader::Fragment)) {
        cout << "failed to load fragment shader" << endl;
    }

    sf::Vector2f s = sf::Vector2f((float)screensize.x,(float)screensize.y);
    myshader.setParameter("screen",s);

    sf::Vector3f pos = sf::Vector3f(3.0, 0.0, 0.0);
    myshader.setParameter("pos",pos);

    sf::Vector2f thetaphi = sf::Vector2f(pi,pi/2);
    sf::Vector2f dthetadphi = sf::Vector2f(0.0,0.0);
    sf::Vector3f dir = sf::Vector3f(sin(thetaphi.y)*cos(thetaphi.x), sin(thetaphi.y)*sin(thetaphi.x), cos(thetaphi.y));

    sf::Vector3f up = sf::Vector3f(sin(thetaphi.y - pi/2)*cos(thetaphi.x), sin(thetaphi.y - pi/2)*sin(thetaphi.x), cos(thetaphi.y - pi/2));

    myshader.setParameter("viewDir",dir);
    
    myshader.setParameter("up", up);
    sf::Vector2i mouseBegin(0,0);

    float speed = 0.25;
    float turnspeed = .90;

    
    
    // Start the game loop
    while (window.isOpen())
    {
	//myshader.setParameter("time",t);
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Espace pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (event.mouseWheel.delta < 0)
			pos += dir * speed;
                else if (event.mouseWheel.delta > 0)
			pos -= dir * speed;
                myshader.setParameter("pos",pos);
            }
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (mouseBegin == sf::Vector2i(0,0)) {
                    mouseBegin = sf::Mouse::getPosition();
                }
                else {
                    sf::Vector2i deltaMouse = sf::Mouse::getPosition() - mouseBegin;
		    dthetadphi = sf::Vector2f(deltaMouse.x / (screensize.x /4.), -deltaMouse.y /(screensize.x/4.));
		    dthetadphi = dthetadphi * turnspeed;
		    float t = thetaphi.x + dthetadphi.x;
		    float p = thetaphi.y + dthetadphi.y;
                    dir = sf::Vector3f(sin(p)*cos(t), sin(p)*sin(t), cos(p));
    		    up = sf::Vector3f(sin(p - pi/2)*cos(t), sin(p - pi/2)*sin(t), cos(p - pi/2));
		    myshader.setParameter("viewDir", dir);
		    myshader.setParameter("up", up);
                }
            }
            else {
                mouseBegin = sf::Vector2i(0,0);
		thetaphi += dthetadphi;
		dthetadphi = sf::Vector2f(0.0,0.0);
            }
                
            
        }
        // Clear screen
        window.clear();
        
        
        // Draw the sprite
        window.draw(rectangle, &myshader);

        // Update the window
        window.display();
//	cout << "I am at position " << pos.x << ", " << pos.y << ", " << pos.z << endl;
       // cout << "looking in direction" << dir.x << ", " << dir.y << ", " << dir.z << endl;
    }
    
    return EXIT_SUCCESS;
}
