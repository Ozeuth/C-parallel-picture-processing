#ifndef PICTURE_H
#define PICTURE_H

#include "Colour.hpp"
#include "Utils.hpp"
#include <atomic>
// The Picture class provided a wrapper for image manipulation via the opencv
// library

class Picture {

private:
    // opencv representation of an image
    Mat img;
    Utils imgio;
    string fileName; // ADD: fileName so we can find the file in storage

public:
    std::atomic<int> threadsActive; // Only initialized on temp pictures
    std::atomic<int> pixelsDone; //Only initialized on temp pictures

    enum RGB {
        BLUE, GREEN, RED
    };

    // class constructor, loads an image from a provided file
    Picture(string path, string filename);

    // class constructor, creates an image provided the size
    Picture(int width, int height);

    // default constructor/deconstructor
    Picture();

    ~Picture() {};

    // determine the dimensions of the underlying image
    int getwidth();

    int getheight();

    // extract a single pixel from the image as a Colour object
    Colour getpixel(int x, int y);

    // set a singel pixel in the  image from a Colour object
    void setpixel(int x, int y, Colour rgb);

    void setOnlyRed(int x, int y, int red);

    void setOnlyBlue(int x, int y, int blue);

    void setOnlyGreen(int x, int y, int green);

    // check if coordinates are within bounds of the stored image
    bool contains(int x, int y);

    // provides the image of the class
    Mat getimage();

    // overwrite Picture image with another image
    void setimage(Mat cpyimg);

    // Return name of file
    string getFileName();

    bool savePicture(string path);

    void displayPicture();


};

#endif
