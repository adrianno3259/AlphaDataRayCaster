#include "Image.h"
#include <fstream>

Image::Image()// imagem preta, 200 x 200
{
    w = h = 200;
    Color c = Color();
    std::vector<Color> tmp = std::vector<Color>(200, c);
    for(int i = 0; i < 200; i++) pixels.push_back(tmp);

}

Image::Image(const int s) // imagem preta s x s
{
    w = h = s;
    Color c = Color();
    std::vector<Color> tmp = std::vector<Color>(s, c);
    for(int i = 0; i < s; i++) pixels.push_back(tmp);
}

Image::Image(const int wi, const int he) // imagem preta w x h
{
    w = wi;
    h = he;
    Color c = Color();
    std::vector<Color> tmp = std::vector<Color>(he, c);
    for(int i = 0; i < wi; i++) pixels.push_back(tmp);
}

Image::Image( std::vector< std::vector<Color> > pxs) : pixels(pxs) { w = pxs.size(); h = pxs[0].size(); } // imagem a partir de std::vector de cores

Color Image::getPixel(const int x, const int y) { return pixels[x][y]; } // retorna a cor de um pixel específico

void Image::setPixel(const int x, const int y, const Color& c) { pixels[x][y] = c; } //altera a cor de um pixel

int Image::getVerticalSize() const
{
    return h;
}

int Image::getHorizontalSize() const
{
    return w;
}

void Image::save(const char* filepath)
{
    std::ofstream file;

    file.open(filepath, std::ios::binary);
    file<<'P'<<'6'<<'\n'<<pixels.size()<<' '<<pixels[0].size()<<'\n'<<'2'<<'5'<<'5'<<'\n';
    for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++)
        {
            file.put(255*pixels[i][j].r);
            file.put(255*pixels[i][j].g);
            file.put(255*pixels[i][j].b);
        }
    file.close();
}
