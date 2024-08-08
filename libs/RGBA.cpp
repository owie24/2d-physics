#include "RGBA.h"

RGBA::RGBA() {
    this->r = 0.0;
    this->g = 0.0;
    this->b = 0.0;
    this->a = 1.0;
}

RGBA::RGBA(unsigned char r, unsigned char g, unsigned char b) {
    this->r = (float) r/255.0;
    this->g = (float) g/255.0;
    this->b = (float) b/255.0;
    this->a = 1.0;
}

RGBA::RGBA(unsigned char r, unsigned char g, unsigned char b, float a) {
    this->r = (float) r/255.0;
    this->g = (float) g/255.0;
    this->b = (float) b/255.0;
    if (a < 0) this->a = 0.0;
    else if (a > 1.0) this->a = 1.0;
    else this->a = a;
}


bool RGBA::operator==(const RGBA& rhs) {
    return this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a;
}

RGBA const & RGBA::operator=(RGBA const & other) {
    if (this != &other) { _copy(other); }
    return *this;
}

ostream& operator<<(ostream& os, const RGBA& obj) {
            if (obj.r == RED->r && obj.g == RED->g && RED->b == obj.b && RED->a == obj.a) os << "RED";
            else if (obj.r == ORANGE->r && obj.g == ORANGE->g && ORANGE->b == obj.b && ORANGE->a == obj.a) os << "ORANGE";
            else if (obj.r == YELLOW->r && obj.g == YELLOW->g && YELLOW->b == obj.b && YELLOW->a == obj.a) os << "YELLOW";
            else if (obj.r == GREEN->r && obj.g == GREEN->g && GREEN->b == obj.b && GREEN->a == obj.a) os << "GREEN";
            else if (obj.r == BLUE->r && obj.g == BLUE->g && BLUE->b == obj.b && BLUE->a == obj.a) os << "BLUE";
            else if (obj.r == INDIGO->r && obj.g ==INDIGO->g && INDIGO->b == obj.b && INDIGO->a == obj.a) os << "INDIGO";
            else if (obj.r == VIOLET->r && obj.g == VIOLET->g && VIOLET->b == obj.b && VIOLET->a == obj.a) os << "VIOLET";
            else {
                os << "R: " << ((int) obj.r) << ", G: " << ((int) obj.g) << ", B: " << ((int) obj.b);
            }
            return os;
        }

void RGBA::_copy(RGBA const & other) {
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}