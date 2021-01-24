#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <omp.h>

#include "color.hpp"

/* DEBUG macro can be used to print stuff
   and thereafter to switch off all printing at once */
//# define DEBUG(x) std::cout << x;
#define DEBUG(x)


/* window size.  Seems 'uint' is fine type here */
const uint W = 1300;
const uint H = 700;
const float zoom = 100;
const sf::Vector3f center((float)W/2.0, (float)H/2.0, 0);
// the center point of the scene
const sf::Vector3f camera = center + sf::Vector3f(1.0, 1.5, -25);
// put camera at negative z, it will look toward the origin.
// Also a little off from z-axis to show the arrays from a little sideways
const int maxIter = 1000;  // maximum number of marching iterations
const float maxDist = 1000;  // if the ray get this far from everything, we put background there

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

/* print Vector3f to output stream
   mainly for debugging */
std::ostream& operator<<(std::ostream &out, sf::Vector3f const& v) {
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}

float dotProduct(sf::Vector3f u, sf::Vector3f v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

float fold(float a, float x) {return fabs(a-x)+x;}

float norm(sf::Vector3f v) {
    /* compute the Euclidean norm of vector */
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float sphere(sf::Vector3f pos, sf::Vector3f center) {
    /* sphere: signed distance function: the function
       that determines the shape of the rendered object
    */
    return norm(pos - center) - 1.0;
}

float torus(sf::Vector3f pos, sf::Vector3f center) {
    /* Distance from torus, centered at origin and located on x-y plane.
     */
    const float R = 1;  // large radius of the torus
    const float r = 0.3;  // small radius
    sf::Vector3f centeredPos = pos - center;
    sf::Vector3f t(centeredPos.x, centeredPos.y, 0);
    t = t/norm(t)*R;
    return norm(t - centeredPos) - r;
}

float objectArray(sf::Vector3f pos) {
    /* Array of spheres, spaced by 4 units along all axes
       Negative z values are empty to make camera placement easier
     */
    const sf::Vector3f foldCenter(0.0, 0.0, 0.0);
    float spacing = 4;  // half of the spacing
    float xf = fabs(fmod(pos.x, spacing));
    float yf = fabs(fmod(pos.y, spacing));
    float zf = pos.z;
    /* negative z is empty */
    if(zf > 0) {
	zf = fabs(fmod(zf, spacing));
    }
    sf::Vector3f foldedPos = sf::Vector3f(fmin(xf, spacing - xf),
					  fmin(yf, spacing - yf),
					  fmin(zf, spacing - zf));
    /* replace by your desired object */
    return torus(foldedPos, foldCenter);
}

Color march(sf::Vector3f pos, sf::Vector3f dir, int iter) {
    /* 
       Look from position 'pos' toward direction 'dir'.
       If position is not on surface, take 
       largest safe step in direction and repeat.
       Return pixel color for pos 
     */
    Color color;
    /* Distance to the closest pixel */
    float dist = objectArray(pos);
    // float dist = sphereArray(pos);
    DEBUG("  iter " << iter << "  pos: " << pos << " dist: " << dist << "\n");
    if(dist < 0.01) {
	float it = (float)iter;
	/* use exponential decay to be able to display colors for
	   a larger range of iterations */
	color = Color(exp(-it/20.0f), exp(-it/60.0f), exp(-it/120.0f));
	// R, G, B values for this pixel
	DEBUG(iter << ": " << pos << ": " << dist << "\n");
    }
    else if(iter > maxIter) {
	color = Color(0, 0, 0);
    }
    else if(dist > maxDist) {
	/* shoot past everything, dark screen behind */
	color = Color(0, 0, 0);
    }
    else {
	color = march(pos + (dir*dist), dir, iter+1);
    }
    return color;
}

double clamp(double n,double min,double max) {
    n += (max - n)*(n > max);
    n += (min - n)*(n < min);
    return n;
}

void render(sf::Vector3f camera) {
    /* loop over pixels in the window using parallel loop */
#pragma omp parallel for
    for(uint x = 0; x < W; x++) {
        for(uint y = 0; y < H; y++) {
	    /* ray vector */
            sf::Vector3f ray((x - camera.x)/zoom, (y - camera.y)/zoom, 0 - camera.z);
	    DEBUG("--new ray at " << x << " " << y << ": " << ray << "\n")
	    ray = ray/norm(ray);
	    // norm the ray in order to use it with distance later
	    // We can do that in 'march()' too but we call march iteratively
	    // so it is better to do it once here.
	    DEBUG("  camera: " << camera << "   normed ray: " << ray << "\n")
	    /* take a ray from camera toward the scene and start from there */
            Color color = march(camera + ray, ray, 0);
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
    //text.setCharacterSize(24);
    //text.setFillColor(sf::Color(255,0,0));
    
    if (!texture.create((uint)W,(uint)H)) {std::cout << "Unable to create texture\n";}

    sprite.setTexture(texture);
    for(uint x = 0; x < W; x++) {
	for(uint y = 0; y < H; y++) {
	    pixels[3+x*4+y*4*W] = 255;
	}
    }
    render(camera);
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
        //std::cout << 1.0f/fpsmeter.restart().asSeconds() << "\n";
    }

    return 0;
}
