#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <string>

class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    void addTileToFront(const std::shared_ptr<Tile> tile);
    std::shared_ptr<Tile> getAndRemoveFirstTile();
    std::shared_ptr<Tile> getTile() const;
    int getSize() const;
    std::string toSave() const;
    void clear();

    //for debugging purposes
    void print();
    void resetSize();


private:
    std::shared_ptr<Node> head; 
    int size;
};

#endif // LINKEDLIST_H