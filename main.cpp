#include "scenegen.h"
using namespace std;

// Here is a small helper for you ! Have a look.
//
sf::Vector3f cross(sf::Vector3f a, sf::Vector3f b) {
	return sf::Vector3f(
			a.y*b.z-a.z*b.y,
			a.z*b.x-a.x*b.z,
			a.x*b.y-a.y*b.x );
}

int main(int, char const**)
{
    sf::Clock clock;
    scenegen scene;
    primitive p1;
    primitive p2;
    primitive p3;
    box p4(0.5,0.5,0.5);
    torus p5(1.0,0.2);
    cone p6(0.705,0.705);
    p2.pos = sf::Vector3f(1.0,1.0,1.0);
    p2.color = sf::Vector3f(1.0,0.0,0.0);
    p3.pos = sf::Vector3f(0.0,-1.0,-1.0);
    p3.color = sf::Vector3f(0.9,0.5,0.7);
    p4.pos = sf::Vector3f(0.0,1.0,-2.0);
    p4.color = sf::Vector3f(1.0,1.0,0.0);
    p5.pos = sf::Vector3f(-0.5,-0.5,-2.0);
    p5.color = sf::Vector3f(1.0,1.0,1.0);
    p6.pos = sf::Vector3f(1.5,0.5,1.0);
    p6.color = sf::Vector3f(1.0,0.2,0.7);
    scene.addPrimitive(p1);
    scene.addPrimitive(p2);
    scene.addPrimitive(p3);
    scene.addPrimitive(p4);
    scene.addPrimitive(p5);
    scene.addPrimitive(p6);
    light l1;
    l1.color = sf::Vector3f(1.0,1.0,0.0);
    l1.pos = sf::Vector3f(3.0,7.0,0.0);
    light l2;
    l2.color = sf::Vector3f(0.0,1.0,1.0);
    l2.pos = sf::Vector3f(-3.0,0.0,8.0);
    light l3;
    l3.color = sf::Vector3f(0.5,1.0,0.5);
    l3.pos = sf::Vector3f(0.0,-4.0,-5.0);
    light l4;
    l4.color = sf::Vector3f(1.0,1.0,1.0);
    l4.pos = sf::Vector3f(5.0,5.0,5.0);
    scene.addLight(l1);
    scene.addLight(l2);
    scene.addLight(l3);
    scene.addLight(l4);
    
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
    sf::Vector3f right = cross(dir,up);

    myshader.setParameter("viewDir",dir);
    
    myshader.setParameter("up", up);
    sf::Vector2i mouseBegin(0,0);

    float speed = 0.25;
    float turnspeed = .90;

    
    int frames = 0;
    // Start the game loop
    while (window.isOpen())
    {
	    ++frames;
	    sf::Time time = clock.getElapsedTime();
	    if (time.asSeconds() >= 1) {
		    cout << "fps = " << frames << endl;
		    frames = 0;
		    clock.restart();
	    }
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
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
		pos += right* speed;
                myshader.setParameter("pos",pos);
	    }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
		pos -= right* speed;
                myshader.setParameter("pos",pos);
	    }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
		pos += up * speed;
                myshader.setParameter("pos",pos);
	    }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
		pos -= up* speed;
                myshader.setParameter("pos",pos);
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
    	            right = cross(dir,up);
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
