#include "Picture.hpp"

using namespace std;
using namespace cv;

Picture::Picture(string path, string filename) {
    if (path != "NULL") {
        img = imgio.loadimage(path);
        fileName = filename;
    } else {
        fileName = filename;
    }
}

Picture::Picture(int width, int height) {
    img = imgio.createimage(width, height);
    threadsActive = 0;
    pixelsDone = 0;
}

Picture::Picture() {}

int Picture::getwidth() {
    return img.size().width;
}

int Picture::getheight() {
    return img.size().height;
}

Colour Picture::getpixel(int x, int y) {
    // Beware: opencv pixels are stored in a (y,x) vector, not an (x,y) vector!
    cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
    // RGB format
    Colour rgb(pixel[RED], pixel[GREEN], pixel[BLUE]);
    return rgb;
}

void Picture::setpixel(int x, int y, Colour rgb) {
    // Beware: opencv pixels are stored in a (y,x) vector, not an (x,y) vector!
    img.at<cv::Vec3b>(y, x)[BLUE] = rgb.getblue();
    img.at<cv::Vec3b>(y, x)[GREEN] = rgb.getgreen();
    img.at<cv::Vec3b>(y, x)[RED] = rgb.getred();
}

void Picture::setOnlyRed(int x, int y, int red) {
    img.at<cv::Vec3b>(y, x)[RED] = red;
}

void Picture::setOnlyBlue(int x, int y, int blue) {
    img.at<cv::Vec3b>(y, x)[BLUE] = blue;
}

void Picture::setOnlyGreen(int x, int y, int green) {
    img.at<cv::Vec3b>(y, x)[GREEN] = green;
}

bool Picture::contains(int x, int y) {
    return x >= 0 && x < img.size().width && y >= 0 && y < img.size().height;
}

Mat Picture::getimage() {
    return img;
}

void Picture::setimage(Mat cpyimg) {
    img = imgio.copyimage(cpyimg);
}

string Picture::getFileName() {
    return fileName;
}

bool Picture::savePicture(string path) {
    return imgio.saveimage(img, path);
}

void Picture::displayPicture() {
    imgio.displayimage(img);
}
