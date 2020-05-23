#ifndef MOSAICSTORAGE_H
#define MOSAICSTORAGE_H

// #include "./Components/headers/Tile.h"
#include "BrokenTiles.h"
#include "Mosaic.h"

#define MAX_ROWS    5

class MosaicStorage{
public:
    MosaicStorage(std::string gameMode);
    ~MosaicStorage();
    void determineGameMode(std::string gameMode, int& noOfBrokenTiles);

    //Getters
    std::shared_ptr<Tile>* getRow(unsigned const int row);
    std::shared_ptr<Tile>** getGrid();
    BrokenTiles* getBrokenTiles();
    Mosaic* getMosaic();

    //Tile movers
    void clearRow(unsigned const int row);
    void addTile(std::shared_ptr<Tile> tile, unsigned const int row);
    void addTileToDiscardedTiles(std::shared_ptr<Tile> tile);

    //End of round methods
    void endOfRoundDiscardBrokenTiles();
    void endOfRoundMove(int col);
    bool greyModeEndOfRoundMove(int row, int col);
    void moveToMosaic(std::shared_ptr<Tile> tile, unsigned const int row, int col);
    void moveToDiscardedTiles(std::shared_ptr<Tile>* tiles);

    //Broken Tile Methods
    std::vector<std::shared_ptr<Tile>>* getDiscardedTiles();
    void resetDiscardTilesVector();
    Type getRowType(unsigned const int row);
    void moveToBrokenTiles(std::shared_ptr<Tile>* tiles);

    //Utility
    std::shared_ptr<Tile>* findFullRow();
    bool isRowFull(unsigned const int row);

    bool isValidAdd(Type type, unsigned const int row);
    bool isValidStandardAdd(Type type, unsigned const int row);
    bool isValidGreyAdd(Type type, unsigned const int row);
    bool isValidSixBySixAdd(Type type, unsigned const int row);

    //Printers
    std::string rowToString(int index);
    std::string rowToSave(int index);

private:
    std::shared_ptr<Tile>** grid;
    BrokenTiles* brokenTiles;
    std::vector<std::shared_ptr<Tile>> discardedTiles;
    Mosaic* mosaic;
    bool greyMode;
    bool sixBySixMode;
    unsigned int maxNoRows;
};

#endif // MOSAICSTORAGE_H