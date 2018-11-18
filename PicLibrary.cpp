#include "PicLibrary.hpp"
#include "Colour.hpp"
#include <assert.h>
#include "LinkedList.hpp"
#include <thread>
#include <list>
#include <chrono>
#include <ctime>

PicLibrary::PicLibrary() {
    struct Nodes *head = LinkedList::nodeInit(new Picture("NULL", "HEAD"));
    pictures = new LinkedList(&head);
    strat = "row";
}

void PicLibrary::print_picturestore(int commandOrder) {
    while (commandsToRun.begin()->first != commandOrder) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to show list");
    }
    // We are working with the assumption we only use addSort func of linkedList
    pictures->printList();
    remove(commandOrder);
}

void PicLibrary::loadpicture(string path, string filename, int commandOrder) {
    if (commandOrder != -1) {
        while (priorActionsStillUndone(commandOrder, filename)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            printf("Stuck trying to remove");
        }
    }

    Picture *picture = new Picture(path, filename);
    if (picture->getimage().empty()) {
        cout << "There was no such image at the path!" << filename << endl;
    } else {
        cout << commandOrder << " is trying to initialize node" << endl;
        struct Nodes *node = LinkedList::nodeInit(picture); // Some issue with either init or addSorted
        cout << commandOrder << " is trying to add to picList" << endl;
        pictures->addSorted(node);
        cout << commandOrder << " successfully did both" << endl;
    }

    if (commandOrder != -1) {
        remove(commandOrder);
    }
}

void PicLibrary::unloadpicture(string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to unload");
    }
    pictures->remove(filename);
    remove(commandOrder);
}

void PicLibrary::savepicture(string filename, string path, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to save");
    }
    Picture *picture = pictures->getPicture(filename);
    if (picture == nullptr) {
        cout << "ERROR: Attempt to save a nonexistent picture" << endl;
    } else {
        picture->savePicture(path);
    }
    remove(commandOrder);
}

void PicLibrary::display(string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to display");
    }
    Picture *picture = pictures->getPicture(filename);
    if (picture == nullptr) {
        cout << "ERROR: Attempt to display a nonexistent picture" << endl;
    } else {
        picture->displayPicture();
    }
    remove(commandOrder);
}

void PicLibrary::invert(string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to invert");
    }
    Picture *pictureToChange = pictures->getPicture(filename);
    // Traverses, left to right THEN up to down
    for (int y = 0; y < pictureToChange->getheight(); y++) {
        for (int x = 0; x < pictureToChange->getwidth(); x++) {
            Colour color = pictureToChange->getpixel(x, y);
            color.setred(255 - color.getred());
            color.setgreen(255 - color.getgreen());
            color.setblue(255 - color.getblue());
            pictureToChange->setpixel(x, y, color);
        }
    }
    remove(commandOrder);
}

void PicLibrary::grayscale(string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to grayscale");
    }
    Picture *pictureToChange = pictures->getPicture(filename);
    for (int y = 0; y < pictureToChange->getheight(); y++) {
        for (int x = 0; x < pictureToChange->getwidth(); x++) {
            Colour color = pictureToChange->getpixel(x, y);
            int avg = ((color.getred() + color.getgreen() + color.getblue()) / 3);
            color.setred(avg);
            color.setgreen(avg);
            color.setblue(avg);
            pictureToChange->setpixel(x, y, color);
        }
    }
    remove(commandOrder);
}

void PicLibrary::rotate(int angle, string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to rotate");
    }
    assert(angle == 90 || angle == 180 || angle == 270);
    Picture *pictureToChange = pictures->getPicture(filename);
    for (int i = 0; i < angle / 90; i++) {
        Picture *newPicture = new Picture(pictureToChange->getheight(), pictureToChange->getwidth());
        rotateSmall(pictureToChange, newPicture);
    }
    remove(commandOrder);
}

void PicLibrary::rotateSmall(Picture *original, Picture *newPicture) {
    newPicture = new Picture(original->getheight(), original->getwidth());
    // columns, then rows
    for (int y = 0; y < original->getheight(); y++) {
        for (int x = 0; x < original->getwidth(); x++) {
            newPicture->setpixel((newPicture->getwidth() - 1 - y), x, original->getpixel(x, y));
        }
    }
    // We rewrite the original picture with new picture, before freeing newPicture
    original->setimage(newPicture->getimage());
    free(newPicture);
}

void PicLibrary::flipVH(char plane, string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Stuck trying to flip");
    }
    assert(plane == 'H' || plane == 'V');
    Picture *pictureToChange = pictures->getPicture(filename);
    Picture *newPicture = new Picture(pictureToChange->getwidth(), pictureToChange->getheight());
    if (plane == 'H') {
        for (int y = 0; y < pictureToChange->getheight(); y++) {
            for (int x = 0; x < pictureToChange->getwidth(); x++) {
                newPicture->setpixel((pictureToChange->getwidth() - x - 1), y, pictureToChange->getpixel(x, y));
            }
        }
    } else {
        for (int y = 0; y < pictureToChange->getheight(); y++) {
            for (int x = 0; x < pictureToChange->getwidth(); x++) {
                newPicture->setpixel(x, (pictureToChange->getheight() - y - 1), pictureToChange->getpixel(x, y));
            }
        }
    }
    // We rewrite the original picture with new picture, before freeing newPicture
    pictureToChange->setimage(newPicture->getimage());
    free(newPicture);
    remove(commandOrder);
}

void PicLibrary::blur(string filename, int commandOrder) {
    while (priorActionsStillUndone(commandOrder, filename)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //printf("Stuck trying to blur");
    }
    // ADD: Timer Check
    clock_t start = std :: clock();
    Picture *pictureToChange = pictures->getPicture(filename);
    Picture *newPicture = new Picture(pictureToChange->getwidth(), pictureToChange->getheight());
    if (strat == "sequential") {
        // This is the sequential implementation
        blurSmall(pictureToChange, newPicture, 0, pictureToChange->getheight(), 0, pictureToChange->getwidth(),
                  ColorRef::All);
    } else {
        // These are all the parallel implementations
        list<thread> workerThreadList;
        int maximumThreads = (int) std::thread::hardware_concurrency();
        cout << "We spawn " << maximumThreads << " threads" << endl;
        if (strat == "pixel"){
            int totalPixels = newPicture->getheight() * newPicture->getwidth();
            while (newPicture->pixelsDone != totalPixels) {
                while (newPicture->threadsActive < maximumThreads) {
                    if (newPicture->pixelsDone == totalPixels){
                        break;
                    }
                    newPicture->threadsActive++;
                    int yStart = (int) floor(newPicture->pixelsDone/newPicture->getwidth());
                    int xStart = (int) ((newPicture->pixelsDone) - (newPicture->getwidth() * floor((newPicture->pixelsDone) / newPicture->getwidth())));

                    std::thread workerThread(&PicLibrary::blurSmall, this, pictureToChange, newPicture,
                            yStart, (yStart + 1), xStart, (xStart+1), ColorRef::All);
                    workerThreadList.push_back(std::move(workerThread));
                    newPicture->pixelsDone++; // This only matters for pixel blur
                }
                std::list<thread>::iterator itera;
                for (itera = workerThreadList.begin(); itera != workerThreadList.end(); itera++) {
                    itera->join();
                    newPicture->threadsActive--;
                }
                workerThreadList.clear();
            }
        }
        else if (strat == "row") {
            int segment = (int) floor(pictureToChange->getheight() / maximumThreads);
            for (int i = 0; i < (maximumThreads - 1); i++) {
                std::thread workerThread(&PicLibrary::blurSegmenter, this, pictureToChange, newPicture, (i * segment), segment, "row");
                workerThreadList.push_back(std::move(workerThread));
            }
            std::thread workerThread(&PicLibrary::blurSegmenter, this, pictureToChange, newPicture, ((maximumThreads-1) * segment), (pictureToChange->getheight() - ((maximumThreads-1) * segment)), "row");
            workerThreadList.push_back(std::move(workerThread));
        } else if (strat == "column") {
            int segment = (int) floor(pictureToChange->getwidth() / maximumThreads);
            for (int i = 0; i < (maximumThreads-1); i++){
                std::thread workerThread(&PicLibrary::blurSegmenter, this, pictureToChange, newPicture, (i*segment), segment, "column");
                workerThreadList.push_back(std::move(workerThread));
            }
            std::thread workerThread(&PicLibrary::blurSegmenter, this, pictureToChange, newPicture, ((maximumThreads-1)*segment), (pictureToChange->getwidth() - ((maximumThreads-1)*segment)), "column");
            workerThreadList.push_back(std::move(workerThread));
        } else if (strat == "color") {
            // This will run a color-based threading
            std::thread redThread(&PicLibrary::blurSmall, this, pictureToChange, newPicture, 0,
                                  pictureToChange->getheight(), 0, pictureToChange->getwidth(), ColorRef::Red);
            std::thread blueThread(&PicLibrary::blurSmall, this, pictureToChange, newPicture, 0,
                                   pictureToChange->getheight(), 0, pictureToChange->getwidth(), ColorRef::Blue);
            std::thread greenThread(&PicLibrary::blurSmall, this, pictureToChange, newPicture, 0,
                                    pictureToChange->getheight(), 0, pictureToChange->getwidth(), ColorRef::Green);
            workerThreadList.push_back(std::move(redThread));
            workerThreadList.push_back(std::move(blueThread));
            workerThreadList.push_back(std::move(greenThread));
        }
        // Now we join all the threads
        std::list<thread>::iterator its;
        for (its = workerThreadList.begin(); its != workerThreadList.end(); its++) {
            its->join();
        }
    }
    // We rewrite the original picture with new picture, before freeing newPicture
    pictureToChange->setimage(newPicture->getimage());
    free(newPicture);
    remove(commandOrder);
    // ADD: Timer Check
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}

void PicLibrary::blurSegmenter(Picture *pictureToChange, Picture *newPicture, int indexStart, int boundary, string segmentType) {
    if (segmentType == "row") {
        for (int y = indexStart; y < indexStart + boundary; y++) {
            blurSmall(pictureToChange, newPicture, y, (y + 1), 0, pictureToChange->getwidth(), ColorRef::All);
        }
    } else if (segmentType == "column") {
        for (int x = indexStart; x < indexStart + boundary; x++){
            blurSmall(pictureToChange, newPicture, 0, pictureToChange->getheight(), x, (x+1), ColorRef::All);
        }
    } else {
        cout << "No such segmentation!" << endl;
    }
}

void PicLibrary::blurSmall(Picture *pictureToChange, Picture *newPicture, int yStart, int yEnd, int xStart, int xEnd,
                           ColorRef colorRef) {
    // NOTE: pictureToChange is the original picture
    for (int y = yStart; y < yEnd; y++) {
        for (int x = xStart; x < xEnd; x++) {
            if (!(newPicture->contains(x, y))){
                printf("HERE IS THE ERROR");
            }
            if (x != 0 && x != (pictureToChange->getwidth() - 1) && y != 0 &&
                y != (pictureToChange->getheight() - 1)) {

                if (colorRef == ColorRef::Red || colorRef == ColorRef::All) {
                    int avgRed =
                            (pictureToChange->getpixel(x, y).getred() + pictureToChange->getpixel((x - 1), y).getred()
                             + pictureToChange->getpixel((x + 1), y).getred() +
                             pictureToChange->getpixel(x, (y + 1)).getred()
                             + pictureToChange->getpixel((x - 1), (y + 1)).getred() +
                             pictureToChange->getpixel((x + 1), (y + 1)).getred()
                             + pictureToChange->getpixel(x, (y - 1)).getred() +
                             pictureToChange->getpixel((x - 1), (y - 1)).getred()
                             + pictureToChange->getpixel((x + 1), (y - 1)).getred()) / 9;
                    newPicture->setOnlyRed(x, y, avgRed);
                }
                if (colorRef == ColorRef::Blue || colorRef == ColorRef::All) {
                    int avgBlue =
                            (pictureToChange->getpixel(x, y).getblue() + pictureToChange->getpixel((x - 1), y).getblue()
                             + pictureToChange->getpixel((x + 1), y).getblue() +
                             pictureToChange->getpixel(x, (y + 1)).getblue()
                             + pictureToChange->getpixel((x - 1), (y + 1)).getblue() +
                             pictureToChange->getpixel((x + 1), (y + 1)).getblue()
                             + pictureToChange->getpixel(x, (y - 1)).getblue() +
                             pictureToChange->getpixel((x - 1), (y - 1)).getblue()
                             + pictureToChange->getpixel((x + 1), (y - 1)).getblue()) / 9;
                    newPicture->setOnlyBlue(x, y, avgBlue);
                }
                if (colorRef == ColorRef::Green || colorRef == ColorRef::All) {
                    int avgGreen =
                            (pictureToChange->getpixel(x, y).getgreen() +
                             pictureToChange->getpixel((x - 1), y).getgreen()
                             + pictureToChange->getpixel((x + 1), y).getgreen() +
                             pictureToChange->getpixel(x, (y + 1)).getgreen()
                             + pictureToChange->getpixel((x - 1), (y + 1)).getgreen() +
                             pictureToChange->getpixel((x + 1), (y + 1)).getgreen()
                             + pictureToChange->getpixel(x, (y - 1)).getgreen() +
                             pictureToChange->getpixel((x - 1), (y - 1)).getgreen()
                             + pictureToChange->getpixel((x + 1), (y - 1)).getgreen()) / 9;
                    newPicture->setOnlyGreen(x, y, avgGreen);
                }
            } else {
                newPicture->setpixel(x, y, pictureToChange->getpixel(x, y));
            }
        }
    }
}

void PicLibrary::add(int commandOrder, string filename) {
    commandMtx.lock();
    commandsToRun.insert(std::make_pair(commandOrder, filename));
    commandMtx.unlock();
}

void PicLibrary::remove(int commandOrder) {
    commandMtx.lock();
    commandsToRun.erase(commandOrder);
    commandMtx.unlock();
    cout << "I have removed " << commandOrder << endl;
}

bool PicLibrary::priorActionsStillUndone(int commandOrder, string filename) {
    commandMtx.lock();
    map<int, string>::iterator iter;
    for (iter = commandsToRun.begin(); iter != commandsToRun.end(); iter++) {
        if (iter->first == commandOrder) {
            // Then we have reached where our current action in the queue is without hitting a prior action on same file
            commandMtx.unlock();
            return false;
        } else if (iter->second == filename || iter->second == "NULL") {
            // Then we have reached a prior action on the same file,
            // OR we have hit liststore. That must complete before anything happens
            commandMtx.unlock();
            return true;
        } else {
            // We continue searching
        }
    }
    commandMtx.unlock();
    cout << "priorActionCheck: This should not happen!" << endl;
    return false;
}

void PicLibrary::changeStrat(string strat) {
    if (strat == "sequential" || strat == "pixel" || strat == "row"
        || strat == "column" || strat == "color") {
        PicLibrary::strat = strat;
    } else {
        cout << "There is no such parallel strategy, keeping to original version" << endl;
    }
}