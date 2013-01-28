#if 1
#include "Graphics/Renderer.h"
#include "Graphics/Sprite.h"
#include "Graphics/Shader.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//#include "Utils/Interpreter.h"

int main()
{
	/*pip::LispScript script("()");
	while(true)
	{
		try
		{
			std::cout << ">>> ";
			pip::string str;
			getline(std::cin, str);
			if(str == "exit")
				break;
			script.parseCode(str);
			std::cout << "Result:" << script.run()->repr() << std::endl;
		}catch(pip::InterpreterException &e)
		{
			std::cout << "Ops: " << e.what() << std::endl;
		}
	}*/

	sf::RenderWindow App(sf::VideoMode(600, 600, 32), "SFML Graphics");

    pip::Renderer *renderer = gc_new pip::Renderer(600, 600);

    renderer->setClearColor(pip::Color(1.0,1.0,1.0,1.0));
    renderer->prepareProjection(600, 600);

    pip::Shader *shader = gc_new pip::Shader;
    std::cout << "Before trap." << std::endl;
    shader->loadShader("minimal.vert", "minimal.frag");
    std::cout << "After trap." << std::endl;

    pip::Sprite *sprite = gc_new pip::Sprite("sprite_up1.png", renderer, shader);
    sprite->load("sprite_up2.png");
    sprite->load("sprite_up3.png");
    sprite->rotationPoint = pip::Vector2D(27.0, 15.0);

    sprite->position = pip::Vector2D(300.0,300.0);
    //shader->bind();
    std::cout << "Chuj" << std::endl;

    while (App.IsOpened())
    {
    	//std::cout << 1.0/App.GetFrameTime() << std::endl;

    	sf::Event Event;
    	while (App.GetEvent(Event))
    	{
    		if (Event.Type == sf::Event::Closed)
    		{
    			App.Close();
    		}

    		if (Event.Type == sf::Event::KeyPressed)
    		{
    			if (Event.Key.Code == sf::Key::Escape)
    				App.Close();
    		}
    	}
                /*onst sf::Input& Input = App.GetInput();
                if (Input.IsKeyDown(sf::Key::A))
                {
                	posX-=App.GetFrameTime()*200.0;
                    walking = true;
                }
                else if (Input.IsKeyDown(sf::Key::D))
                {
                	posX+=App.GetFrameTime()*200.0;
                    walking = true;
                }
                else if (Input.IsKeyDown(sf::Key::S))
                {
                	posY+=App.GetFrameTime()*200.0;
                	walking = true;
                }
                else if (Input.IsKeyDown(sf::Key::W))
                {
                	posY-=App.GetFrameTime()*200.0;
                    walking = true;
                }
                else
                {
                	walking = false;
                }
                sprite.position = Vector2D(posX,posY);
                if(walking)
                {
                	if ((string)sprite.animation != "walk")
                		sprite.animation = "walk";
                }
                else
                	sprite.animation = "idle";*/

                //if(pip::isRectangleCollide(sprite, sprite2))
                //	sprite2.color = Color(1.0,0.0,0.0,1.0);
                //else
                //	sprite2.color = Color(1.0,1.0,1.0,1.0);

    		sprite->update(App.GetFrameTime());
                //sprite2.update(App.GetFrameTime());
                //std::cout << "X:" << posX << " Y:" << posY << std::endl;
            sprite->rotation += App.GetFrameTime() * 30;

        	renderer->begin();
        	    //Entity::luaMachine.execute("print(coroutine.status(co1))");
        	    //Entity::luaMachine.execute("print(coroutine.status(co2))");
                /*renderer->setTarget(tex);
                renderer->clearToColor();

                renderer->prepareProjection(tex2.width,tex2.height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
                tex2.bind();
                renderer->setColor(Color(1.0f,1.0f,1.0f,1.0f));
                renderer->drawQuad(tex2.nearestWidth,tex2.nearestHeight,tex2.width,tex2.height,Vector2D(0,0),Vector2D(tex2.width,tex2.height),Vector2D(tex2.width/2.0,tex2.height/2.0),0);

            	//renderer->setPixel(tex,Vector2D(posX,posY),Color(0.0,0.0,0.0,1.0));
            	*/
        	renderer->setTarget();
        	renderer->prepareProjection(renderer->getWidth(),renderer->getHeight());
        	renderer->clearToColor();

        	    //i+=App.GetFrameTime();
        	    //sprite2.setPixel(Vector2D(10.0,i), Color(1.0, 1.0, 1.0, 1.0));

        	    //renderer->setPixel(tex,Vector2D(posX,posY),Color(0.0,0.0,0.0,1.0));

        	    //((OpenGLRenderer*)renderer)->pixel->bind();
        	    //renderer->drawQuad(1,1,1,1,Vector2D(0.0,0.0),Vector2D(1.0,1.0),Vector2D(posX,posY),0.0);
        	    /*
                tex->bind();
                renderer->setColor(Color(1.0f,1.0f,1.0f,1.0f));
                renderer->drawQuad(tex->nearestWidth,tex->nearestHeight,tex->width,tex->height,Vector2D(0,0),Vector2D(tex->width,tex->height),Vector2D(renderer->width/2.0,renderer->height/2.0),2*i);
                */
        	//shader->bind();

        	sprite->render();
        	//shader->unbind();
        	//sprite2.render();
        	//shader->unbind();

        	std::ostringstream strs;
        	strs << 1.0/App.GetFrameTime();

        	renderer->drawText(pip::Vector2D(10.0,10.0), strs.str(), "font.ttf", 32.0f);
            renderer->end();

            App.Display();
    }
	return 0;
}

#endif
/*
#include "Utils/Interpreter.h"

int main()
{
	pip::LispScript script("()");
	while(true)
	{
		try
		{
			std::cout << ">>> ";
			pip::string str;
			getline(std::cin, str);
			if(str == "exit")
				break;
			script.parseCode(str);
			std::cout << "Result:" << script.run()->repr() << std::endl;
		}catch(pip::InterpreterException &e)
		{
			std::cout << "Ops: " << e.what() << std::endl;
		}
	}
	return 0;
}

*/