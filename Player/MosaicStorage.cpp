#include "headers/MosaicStorage.h"

MosaicStorage::MosaicStorage(std::string gameMode) {


    int noOfBrokenTiles = 7;
    determineGameMode(gameMode, noOfBrokenTiles);
    
    // Creating the grid as a 2d array of pointers
    this->grid = new std::shared_ptr<Tile>*[maxNoRows];
    for (unsigned int row = 0; row < maxNoRows; ++row) {
        this->grid[row] = new std::shared_ptr<Tile>[row+1];
    }

    this->mosaic = std::make_shared<Mosaic>(gameMode);
    this->brokenTiles = std::make_shared<BrokenTiles>(noOfBrokenTiles);
    
}

MosaicStorage::~MosaicStorage() {
    brokenTiles = nullptr;

    for (unsigned int row = 0; row < maxNoRows; ++row) {
        clearRow(row);
        delete[] grid[row];
        grid[row] = nullptr;
    }
    delete[] grid;
    grid = nullptr;
}

std::shared_ptr<Tile>** MosaicStorage::getGrid() {
    return grid;
}

void MosaicStorage::resetDiscardTilesVector(){
    this->discardedTiles.clear();
};

void MosaicStorage::clearRow(unsigned const int row) {
    unsigned const int max = row+1;
    for (unsigned int col = 0; col < max; ++col) {
        grid[row][col] = nullptr;
    }
}

std::shared_ptr<Tile>* MosaicStorage::getRow(unsigned const int row) {
    return grid[row];
}

void MosaicStorage::addTileToDiscardedTiles(std::shared_ptr<Tile> tile){
    this->discardedTiles.push_back(tile);
}

bool MosaicStorage::isRowFull(unsigned const int row) {
    unsigned const int maxSizeOfRow = row+1;
    unsigned int i = 0;
    bool isFull = false;
    for (unsigned int counter = 0; counter < maxSizeOfRow; ++counter) {
        if (grid[row][i] != nullptr) {
            ++i;
        }
    }
    if (i==maxSizeOfRow) {
        isFull = true;
    }
    return isFull;
}

bool MosaicStorage::isValidAdd(Type type, unsigned const int row) {
    bool valid = false;
    
    if(this->greyMode){
        valid = isValidGreyAdd(type, row);
    }else if( this->sixBySixMode){
        valid = isValidSixBySixAdd(type, row);
    }else{
        valid = isValidStandardAdd(type, row);
    }
   
    return valid;
}

bool MosaicStorage::isValidStandardAdd(Type type, unsigned const int row){
    bool valid = false;
    int column = mosaic->getColourColumn(row, type);
    Type rowType = getRowType(row);
    
    // Row type is the same or empty
    if(type == rowType || rowType == Type::NONE){
        // Check that storage row is not full. And mosaic row SLOT is not taken. Change this for grey.
        // So check the spot they want to move to is free and that the vertical rule checks out.
        //
        if (!isRowFull(row) && mosaic->isSpaceFree(row, column)) {
            valid = true;
        }
    }
   
    return valid;
}

bool MosaicStorage::isValidGreyAdd(Type type, unsigned const int row){
    bool valid = false;
    Type rowType = getRowType(row);
    
    // Row type is the same or empty
    if(type == rowType || rowType == Type::NONE){
        // Check that storage row is not full. And mosaic row SLOT is not taken. Change this for grey.
        // So check the spot they want to move to is free and that the vertical rule checks out.
        
        if (!isRowFull(row) && !mosaic->isRowFull(row) && !mosaic->colourExistsInRow(type,row)){
            valid = true;
        }
    }
   
    return valid;
}

bool MosaicStorage::isValidSixBySixAdd(Type type, unsigned int row){
    bool valid = false;
    int column = mosaic->getColourColumn(row, type);
    Type rowType = getRowType(row);
    
    // Row type is the same or empty
    if(type == rowType || rowType == Type::NONE){
        // Check that storage row is not full. And mosaic row SLOT is not taken. Change this for grey.
        // So check the spot they want to move to is free and that the vertical rule checks out.
        //
        if (!isRowFull(row) && mosaic->isSpaceFree(row, column)) {
            valid = true;
        }
    }
    return valid;
}

//we move the broken tiles to discarded tiles so we can later move them to the box lid
void MosaicStorage::endOfRoundDiscardBrokenTiles(){
    std::shared_ptr<BrokenTiles> brokenTiles = getBrokenTiles();
    std::shared_ptr<Tile>* tiles = brokenTiles->getTiles();
    int size = brokenTiles->getSize();
    for(int i = 0; i < size; ++i){
        if(tiles[i]->getType() == Type::FIRST_PLAYER){
            tiles[i] = nullptr;
        }else{
            discardedTiles.push_back(tiles[i]);
            tiles[i] = nullptr;
        }
    }

    brokenTiles->clear();
}

//moves the tiles to their corresponding mosaic spaces in the mosaics while discarding the rest
void MosaicStorage::endOfRoundMove(int col){
    for(unsigned int row = 0; row< maxNoRows; ++row){
        if(isRowFull(row)){
            std::shared_ptr<Tile>* tiles = getRow(row);
            for(unsigned int i = 0; i < (row + 1); ++i){
                if(i == 0){
                    this->moveToMosaic(tiles[i],row, col);
                    grid[row][i] = nullptr;
                }
                else{
                    this->discardedTiles.push_back(tiles[i]);
                    grid[row][i] = nullptr;
                }
            }
        }
    }
}

bool MosaicStorage::greyModeEndOfRoundMove(int row, int col){
    bool success = false;
    std::shared_ptr<Tile>* tiles = getRow(row);
    if(col == -1){
        for(int i = 0; i < (row + 1); ++i){
            if(i == 0){
                if(brokenTiles->getSize() < brokenTiles->getMaxSize()){
                    this->brokenTiles->addTile(tiles[i]);
                    grid[row][i] = nullptr;
                }else{
                    this->discardedTiles.push_back(tiles[i]);
                    grid[row][i] = nullptr;
                }
            }
            else{
                this->discardedTiles.push_back(tiles[i]);
                grid[row][i] = nullptr;
            }
            
        }  
        success = true;
    }
    else if(!mosaic->colourExistsInCol(tiles[0]->getType(),col) && mosaic->isSpaceFree(row, col)){
        for(int i = 0; i < (row + 1); ++i){
            if(i == 0){
                this->moveToMosaic(tiles[i], row, col);
                grid[row][i] = nullptr;
            }
            else{
                this->discardedTiles.push_back(tiles[i]);
                grid[row][i] = nullptr;
            }
            
        }  
        success = true;
    }

    return success;
}

void MosaicStorage::addTile(std::shared_ptr<Tile> tile, unsigned const int row) {
    if(!isRowFull(row)){
        unsigned int col = 0;
        while(grid[row][col] != nullptr) {
            ++col;
        }
        grid[row][col] = tile;
    }else{
        if(brokenTiles->getSize() < brokenTiles->getMaxSize() ){
            brokenTiles->addTile(tile);
        }else{
            discardedTiles.push_back(tile);
        }
    }
}

Type MosaicStorage::getRowType(unsigned const int row){
    Type type = Type::NONE;
    for(unsigned int i = 0; i< row + 1; ++i){
        if(this->grid[row][i] != nullptr){
            type = grid[row][i]->getType();
        }
    }

    return type;

}

std::shared_ptr<BrokenTiles> MosaicStorage::getBrokenTiles() {
    return brokenTiles;
}

void MosaicStorage::moveToMosaic(std::shared_ptr<Tile> tile, unsigned const int row, int col) {
    if(sixBySixMode){
        col = mosaic->getColourColumn(row, tile->getType());
    } else if(!this->greyMode && !this->sixBySixMode)
        col = mosaic->getColourColumn(row, tile->getType());
    mosaic->addTile(tile, row, col);
}

void MosaicStorage::moveToDiscardedTiles(std::shared_ptr<Tile>* tiles) {
    unsigned int i = 0;
    while (tiles[i] != nullptr) {
        discardedTiles.push_back(tiles[i]);
        i++;
    }

}

void MosaicStorage::moveToBrokenTiles(std::shared_ptr<Tile>* tiles) {
    unsigned int i = 0;
    while (tiles[i] != nullptr) {
        brokenTiles->addTile(tiles[i]);
        ++i;
    }
}

std::shared_ptr<Mosaic> MosaicStorage::getMosaic() {
    return mosaic;
}

std::vector<std::shared_ptr<Tile>>* MosaicStorage::getDiscardedTiles(){
    return &discardedTiles;
}

std::string MosaicStorage::rowToString(int index){
    int noOfRows = 5;
    
    if(sixBySixMode){
        noOfRows = 6;
    }
    std::string string = std::to_string(index+1) + ":";
    for(int j = 0; j<noOfRows-(index+1);j++){
            string+= "   ";
    }
    for(int i = index; i>=0; --i){
    
        if(grid[index][i] != nullptr){
            string += this->mosaic->colourTileInConsole(grid[index][i]->getColourType());
            //string += " ";
        }else{
            string += " . ";
        }
       
    }

    return string;
}

std::string MosaicStorage::rowToSave(int index){
    std::string string;
    for(int i = 0; i<= index; i++){    
        if(grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += ". ";
        }     
    }
    return string;
}

void MosaicStorage::determineGameMode(std::string gameMode, int& noOfBrokenTiles){
    if(gameMode == "grey"){
        this->greyMode = true;
        this->sixBySixMode = false;
        this->maxNoRows = 5;
    } else if(gameMode == "six"){
        this->greyMode = false;
        this->sixBySixMode = true;
        this->maxNoRows = 6;
        noOfBrokenTiles = 8;
    }else{
        this->greyMode = false;
        this->sixBySixMode = false;
        this->maxNoRows = 5;
    }
}
