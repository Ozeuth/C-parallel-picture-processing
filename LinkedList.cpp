//
// Created by Ozeuth on 11/10/2018.
//

#include "LinkedList.hpp"
#include <mutex>

struct Nodes* LinkedList:: nodeInit(Picture* picture){
    Nodes* node = new Nodes;
    node->picture = picture;
    node->next = nullptr;
    return node;
}

LinkedList::LinkedList(struct Nodes** headRef) {
    this->head = *headRef;
    this->head->next = nodeInit(new Picture("NULL", "TAIL"));
}

Picture* LinkedList::getPicture(string filename) {
    Nodes* current = head;
    while (current->picture->getFileName() != "TAIL"){
        if (current->picture->getFileName() ==filename){
            return current->picture;
        }
        else {
            current = current->next;
        }
    }
    // We traversed to the end of the list and could not find the item
    cout << "ERROR: Attempt to get picture of Non Existent File" << endl;
    return nullptr;
}

void LinkedList::addSorted(struct Nodes* nodeToAdd){
    head->mtx.lock();
    head->next->mtx.lock();
    Nodes* current = head;
    Nodes* next = head->next; // This on a new node would be tail
    bool hasBeenAdded = false;
    while (next->picture->getFileName() != "TAIL"){
        if (nodeToAdd->picture->getFileName().compare(next->picture->getFileName()) >= 0){
            Nodes* prior = current;
            current = next;
            next->next->mtx.lock();
            next = next->next;
            prior->mtx.unlock();
        } else {
            // Implies It should be added between here and next
            current->next = nodeToAdd;
            nodeToAdd->next = next;
            current->mtx.unlock();
            next->mtx.unlock();
            hasBeenAdded = true;
            break;
        }
    }
    if (!hasBeenAdded){
        current->next = nodeToAdd;
        nodeToAdd->next = next;
        current->mtx.unlock();
        next->mtx.unlock();
    }
}

void LinkedList::remove(string filename) {
    head->mtx.lock();
    head->next->mtx.lock();
    Nodes* current = head;
    Nodes* next = current->next;
    while ((current->picture->getFileName()) != "TAIL" && next->picture->getFileName() != filename){
        Nodes* prior = current;
        current = next;
        next->next->mtx.lock();
        next = next -> next;
        prior ->mtx.unlock();
    }
    if (current->picture->getFileName() == "TAIL"){
        cout << "ERROR: Removal of Nonexistent File" << endl;
        current->mtx.unlock();
        next->mtx.unlock();
    } else {
        Nodes* prior = next;
        next->next->mtx.lock();
        Nodes* afterNext = next->next;
        current->next = afterNext;
        prior->mtx.unlock();
        current->mtx.unlock();
        afterNext->mtx.unlock();
    }

}

void LinkedList::printList() {
    struct Nodes* current = head;
    while (current->picture->getFileName() != "TAIL"){
        if (current->picture->getFileName() != "HEAD"){
            cout << current->picture->getFileName() << endl;
        }
        current = current->next;
    }
}
