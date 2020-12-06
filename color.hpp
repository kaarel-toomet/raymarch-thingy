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
};

/* ---------- Constructors ---------- */
/* Default constructor */
Color::Color() {
    R = 0;
    G = 0;
    B = 0;
}

/* Constructor with given RGB */
Color::Color(int argR, int argG, int argB) {
    R = argR;
    G = argG;
    B = argB;
};

/* ---------- other methods ---------- */
/* print to output stream */
std::ostream& operator<<(std::ostream &out, Color const& color) {
    out << "R:" << color.R << " G:" << color.G << " B:" << color.B;
    return out;
}
