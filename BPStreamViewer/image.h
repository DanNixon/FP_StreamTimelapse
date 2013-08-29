#ifndef IMAGE_H
#define IMAGE_H

class Image
{
public:
    unsigned char* m_aPixels;
    int m_nWidth;
    int m_nHeight;
    Image(unsigned char *, int, int);
    Image(char *);
    ~Image();
    void info();
};

#endif
