#ifndef BROKENTILES_H
#define BROKENTILES_H

#include "../../Components/headers/Tile.h"
#include <vector>

class BrokenTiles{
public:
    BrokenTiles(int noOfTiles);
    ~BrokenTiles();

    //Getters
    std::shared_ptr<Tile>* getTiles();
    int getMaxSize() const;
    int getSize() const;

    //Move tiles
    void addTile(std::shared_ptr<Tile> tile);
    void clear();

    //Utility
    unsigned int calculatePointsLost();
    
    //Printers
    std::string toString();
    std::string toSave();
    
private:
    std::shared_ptr<Tile> tiles[8];
    int size;
    int maxNoOfTiles;
};

#endif // BROKENTILES_H