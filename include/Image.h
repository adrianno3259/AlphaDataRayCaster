#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <iostream>
#include "Color.h"

class Image
{
public:

    Image();// imagem preta, 200 x 200

    Image(const int s);
    Image(const int wi, const int he);

    Image( std::vector< std::vector<Color> > pxs);

    Color getPixel(const int x, const int y);

    void setPixel(const int x, const int y, const Color& c);

    void save(const char* filepath);

    int getVerticalSize() const;

    int getHorizontalSize() const;

private:

    std::vector< std::vector< Color> > pixels;
    int w, h;


};


#endif // IMAGE_H
