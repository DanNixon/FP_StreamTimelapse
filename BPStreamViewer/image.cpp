#include <iostream>
#include <cassert>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image.h"

using namespace cv;
using namespace std;

Image::Image(unsigned char *px, int w, int h)
{
    assert(w > 0);
    assert(h > 0);
    m_nWidth = w;
    m_nHeight = h;
    m_aPixels = new unsigned char[sizeof(px)];
    memcpy(m_aPixels, px, sizeof(px));
}

Image::~Image()
{
    delete m_aPixels;
}

Image::Image(char* szFilename)
{
    Mat cImage;
    cImage = imread(szFilename, CV_LOAD_IMAGE_COLOR);
    m_nWidth = (int)cImage.rows;
    m_nHeight = (int)cImage.cols;
    assert(m_nWidth > 0);
    assert(m_nHeight > 0);
    int nData = m_nWidth * m_nHeight * 3;
    m_aPixels = new unsigned char[nData];
    memcpy(m_aPixels, cImage.data, nData);
}

void Image::info()
{
    cout << "Width: " << m_nWidth << endl;
    cout << "Height: " << m_nHeight << endl;
    int nData = m_nWidth * m_nHeight * 3;
    cout << "Data size: " << nData << endl;
}
