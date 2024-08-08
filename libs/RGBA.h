#ifndef _RGBA_
#define _RGBA_

#include <ostream>

using namespace std;

class RGBA {
    public:
        RGBA();
        RGBA(unsigned char r, unsigned char g, unsigned char b);
        RGBA(unsigned char r, unsigned char g, unsigned char b, float a);


        bool operator==(const RGBA& rhs);
        RGBA const & operator= (RGBA const & other);
        friend ostream& operator<<(ostream& os, const RGBA& obj); 

        float r;
        float g;
        float b;
        float a;

    private:
        void _copy(RGBA const & other);
};

extern RGBA *RED;
extern RGBA *ORANGE;
extern RGBA *YELLOW;
extern RGBA *GREEN;
extern RGBA *BLUE;
extern RGBA *INDIGO;
extern RGBA *VIOLET;

#endif