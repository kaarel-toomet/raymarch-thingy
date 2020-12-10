#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "color.hpp"

/* window size.  Seems 'uint' is fine type here */
const uint W = 1300;
const uint H = 700;

//bool world[W][H];
//bool buffer[W][H];
//char input = 35;
//bool running = 0;
// int neighbors;
// bool out;
// bool cell;
// int x;
// int y;
double r[W][H];
double g[W][H];
double b[W][H];
// int i;
// int j;
sf::Uint8 pixels[W*H*4];
sf::Texture texture = sf::Texture();
sf::Sprite sprite;
//sf::Clock fpsmeter;
//sf::Font font;
//sf::Text text;
float fold(float a, float x) {return fabs(a-x)+x;}

float norm(sf::Vector3f v) {
    /* compute the Euclidean norm of vector */
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float sphere(sf::Vector3f pos) {
    /* sphere: signed distance function: the function
       that determines the shape of the rendered object
    */
    return norm(pos) - 1.0;
}

float sphereArray(sf::Vector3f pos) {
    /* Array of spheres, spaced by 4 units along all axes
     */
    float spacing = 4;  // half of the spacing
    float spacing2 = spacing/2;
    sf::Vector3f foldedPos = sf::Vector3f(fmod(pos.x - spacing2, spacing) + spacing2,
					  fmod(pos.y - spacing2, spacing) + spacing2,
					  fmod(pos.z - spacing2, spacing) + spacing2);
    return norm(foldedPos) - 1.0;
}

Color march(sf::Vector3f pos, sf::Vector3f dir, int iter) {
    /* Return pixel color for pos */
    float dist = sphereArray(pos);
    Color color;
    if(dist < 0.01) {
	color = Color(2048 / (iter+8), 100, 100);
	// R, G, B values for this pixel
    }
    else if(iter > 100) {
	color = Color(0, 0, 0);
    }
    else {
	color = march(pos+(dir*dist),dir,iter+1);
    }
    return color;
}

double clamp(double n,double min,double max) {
    n += (max - n)*(n > max);
    n += (min - n)*(n < min);
    return n;
}

void render(void) {
    Color color;
    for(uint x = 0; x < W; x++) {
        for(uint y = 0; y < H; y++) {
            double tx = ((double)x-W/2)/0.1;
            double ty = ((double)y-H/2)/0.1;
            sf::Vector3f dir(tx*0.001,ty*0.001,1);
            sf::Vector3f normdir;
            normdir = dir / (float)sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
            color = march(sf::Vector3f(0,0,-50), normdir, 0);
            // std::cout << color << "\n";
            pixels[x*4l+y*W*4l] = color.R;
            pixels[x*4l+y*W*4l+1l] = color.G;
            pixels[x*4l+y*W*4l+2l] = color.B;
        }
    }
    texture.update(pixels);
}

int main()
{
    //if (!font.loadFromFile("NimbusSans-Regular.otf")){std::cout << "Unable to load font from file\n";}

    //text.setFont(font);
    //text.setCharacterSize(24);
    //text.setFillColor(sf::Color(255,0,0));
    
    if (!texture.create((uint)W,(uint)H)) {std::cout << "Unable to create texture\n";}

    sprite.setTexture(texture);

    for(uint x = 0; x < W; x++) {
        for(uint y = 0; y < H; y++) {
            pixels[3+x*4+y*4*W] = 255;
        }
    }
    //generate();
    render();
    sf::RenderWindow window(sf::VideoMode(W, H), "jura");
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {window.close();}

            // else if (event.type == sf::Event::MouseButtonPressed)
            // {
            //     if (event.mouseButton.button == sf::Mouse::Button::Left) {
            //         sf::Vector2i pos = sf::Mouse::getPosition(window);
            //         center_r += (pos.x-W/2.0)*scale;
            //         center_i += (pos.y-H/2.0)*scale;
            //         scale /= 2.0;
            //         generate();
            //         render();
            //         //std::cout << "calculated\n";
            //     }
            //     if (event.mouseButton.button == sf::Mouse::Button::Right) {
            //         sf::Vector2i pos = sf::Mouse::getPosition(window);
            //         center_r += (pos.x-W/2.0)*scale;
            //         center_i += (pos.y-H/2.0)*scale;
            //         scale *= 2.0;
            //         generate();
            //         render();
            //     }
               // 
            //}
        }
        //     else if (event.type == sf::Event::KeyPressed) {
        //         if (event.key.code == sf::Keyboard::Enter) {
        //             running = !running;
        //         }
        //     }

        //     else if (event.type == sf::Event::TextEntered) {
        //         if (event.text.unicode == 32) {
        //             generate();
        //             render();
        //             running = false;
        //             //std::cout << "space type detected\n";
        //         }
        //         else if (event.text.unicode == 114) {
        //             for(int x = 0; x < W; x++) {
        //                 for(int y = 0; y < H; y++) {
        //                     world[x][y] = rand()%2;
            
        //                 }
        //             }
        //             render();
        //             //std::cout << "r type detected\n";
        //         }
        //     }
        //     if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //         sf::Vector2i pos = sf::Mouse::getPosition(window);
        //         world[pos.x][pos.y] = 1;
        //         //render();
        //     }
        //}
        
        //if (running) {generate(); render();}

        //text.setString(std::to_string(1.0f/fpsmeter.restart().asSeconds()));
        window.clear();
        window.draw(sprite);
        //window.draw(text);
        window.display();
        //std::cout << std::(-7.0,6.0) << "\n";
        //std::cout << 1.0f/fpsmeter.restart().asSeconds() << "\n";
    }

    return 0;
}
