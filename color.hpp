/* 
Class Color:

Define RGB colors 
Mainly to be able to return/pass this object through functions
 */

class Color {
public:
    int R, G, B;
    Color();
    Color(int R, int G, int B);
    Color(double R, double G, double B);
};

/* ---------- Constructors ---------- */
/* Default constructor */
Color::Color() {
    R = 0;
    G = 0;
    B = 0;
}

/* Constructor with given RGB intensityes 0..255 */
Color::Color(int argR, int argG, int argB) {
    R = argR;
    G = argG;
    B = argB;
};

/* Constructor with RGB as float, given as float intensity 0..1 */
Color::Color(double argR, double argG, double argB) {
    R = (int)(argR*255.0);
    G = (int)(argG*255.0);
    B = (int)(argB*255.0);
};

/* ---------- other methods ---------- */
/* print to output stream */
std::ostream& operator<<(std::ostream &out, Color const& color) {
    out << "R:" << color.R << " G:" << color.G << " B:" << color.B;
    return out;
}
