#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "color.hpp"

const unsigned long int W = 1318;
const unsigned long int H = 716;

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

float distance(float x, float y, float z) {
    float tx = x+y;
    float ty = y;
    float tz = z;
    //std::cout << sqrt(tx*tx + ty*ty + tz*tz)-1.0 << "\n";
    return pow(tx*tx + ty*ty + tz*tz, 0.5)-1.0;
}

Color march(sf::Vector3f pos, sf::Vector3f dir, int iter) {
    /* Return pixel color for pos */
    float dist = distance(pos.x, pos.y, pos.z);
    Color color;
    // Create default color RGB triple
    if(dist < 0.01) {
	color = Color(2048 / (iter+8), 255, 0);
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
    for(long int x = 0; x < W; x++) {
        for(long int y = 0; y < H; y++) {
            double tx = ((double)x-W/2)/3;
            double ty = ((double)y-H/2)/3;
            sf::Vector3f dir(tx*0.001,ty*0.001,1);
            sf::Vector3f normdir;
            normdir = dir / (float)sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
            color = march(sf::Vector3f(0,0,-50), normdir, 0);
            //std::cout << (int)color << "\n";
            pixels[x*4l+y*W*4l] = color.R;
            pixels[x*4l+y*W*4l+1l] = color.B;
            pixels[x*4l+y*W*4l+2l] = color.G;
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

    for(int x = 0; x < W; x++) {
        for(int y = 0; y < H; y++) {
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
