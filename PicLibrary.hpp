#ifndef PICLIB_H
#define PICLIB_H

#include <mutex>
#include "Picture.hpp"
#include "Utils.hpp"
#include "LinkedList.hpp"

class PicLibrary {

  private:
  // TODO: define internal picture storage
  LinkedList* pictures;
  std::map<int, string> commandsToRun;
  mutex commandMtx;

  enum ColorRef {Red, Green, Blue, All};
  string strat;

  public:
  // defaiult constructor/deconstructor
  PicLibrary();
  ~PicLibrary(){};

  // command-line interpreter routines
  void print_picturestore(int commandOrder);
  void loadpicture(string path, string filename, int commandOrder);
  void unloadpicture(string filename, int commandOrder);
  void savepicture(string filename, string path, int commandOrder);
  void display(string filename, int commandOrder);
  
  // picture transformation routines
  void invert(string filename, int commandOrder);
  void grayscale(string filename, int commandOrder);
  void rotate(int angle, string filename, int commandOrder);
  void rotateSmall(Picture* original, Picture* newPicture); // Helper for rotate
  void flipVH(char plane, string filename, int commandOrder);
  void blur(string filename, int commandOrder);

  // to synchronize commands
  void add(int commandOrder, string filename);
  void remove(int commandOrder);
  bool priorActionsStillUndone(int commandOrder, string filename);

  // to synchronise blur
  void changeStrat(string strat);
  void blurSmall(Picture* pictureToChange, Picture* newPicture, int yStart, int yEnd, int xStart, int xEnd, ColorRef colorRef); // Helper for blur
  void blurSegmenter(Picture* pictureToChange, Picture* newPicture, int indexStart, int boundary, string segmentType); // Helper for blurring by segmentation
};

#endif

