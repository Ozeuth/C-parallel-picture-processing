//
// Created by Ozeuth on 11/10/2018.
//

#ifndef CPP_PIC_PROC_PT3717_LINKEDLIST_HPP
#define CPP_PIC_PROC_PT3717_LINKEDLIST_HPP

#include "Picture.hpp"
#include <mutex>
struct Nodes{
    Picture* picture;
    struct Nodes* next;
    mutex mtx;
};

class LinkedList {
private:
    ::Nodes* head;
public:
    explicit LinkedList(Nodes** headRef);
    ~LinkedList(){};

    static Nodes* nodeInit(Picture* picture);
    Picture* getPicture(string filename);

    void addSorted(Nodes* nodeToAdd);
    void remove(string filename);
    void printList();
    // commands

};


#endif //CPP_PIC_PROC_PT3717_LINKEDLIST_HPP
