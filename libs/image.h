#ifndef _IMAGE_
#define _IMAGE_

using namespace std;

class Image {
    public:
        Image(const char *imagePath);

    private:
        int width;
        int height;
        int nrChannels;
        unsigned char *data;
        unsigned int texture;
};
#endif