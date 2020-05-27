#ifndef MOSAIC_H
#define MOSAIC_H

#include "../../Components/headers/Tile.h"

#define MAX_ROWS        5
#define MAX_COLS        5
#define MAX_NO_TILES    5

class Mosaic{
public:
    Mosaic(std::string gameMode);
    ~Mosaic();
    void determineGameMode(std::string gameMode);

    //Getters
    std::shared_ptr<Tile>** getGrid();
    int getPointsThisRound();
    int getColourColumn(unsigned const int row, unsigned const int colour);

    //Tile movers
    bool addTile(std::shared_ptr<Tile> tile, unsigned int row, unsigned int col);
    void incrementColorCounter(Type tileType);

    //Point calculators
    int calculateEndGamePoints();
    void numberOfSequentialTiles(unsigned int row, unsigned int col);
    bool checkSequentialRows(int row, int col);
    bool checkSequentialCols(int row, int col);

    //Utilities
    bool findFullRow();
    bool findFullCol(unsigned int col);
    bool isSpaceFree(unsigned int row, unsigned int col);
    bool isRowFull(unsigned int row);
    bool colourExistsInCol(Type type, unsigned int col);
    bool colourExistsInRow(Type type, unsigned int row);

    int numFullRows();
    int numFullCols();
    int noOfFiveColours();
    void resetPoints();

    //Printers
    std::string templateRowToString(int index);
    std::string rowToString(int index);
    std::string rowToSave(int index);
   
private:
    std::shared_ptr<Tile>** grid;
    unsigned int noOfReds;
    unsigned int noOfYellows;
    unsigned int noOfDarkBlues;
    unsigned int noOfLightBlues;
    unsigned int noOfBlacks;
    unsigned int noOfOranges;
    int pointsThisRound;

    bool greyMode;
    bool sixBySixMode;
    unsigned int maxNoRows;
    unsigned int maxNoCols;
    unsigned int maxNoTiles;

    /* Represents the column in which the color belongs to
     * Col 1 = Red
     * Col 2 = Yellow
     * Col 3 = Dark Blue
     * Col 4 = Light Blue
     * Col 5 = Black
     */

    int colourColumns[MAX_ROWS][MAX_COLS] = {
        {2, 1, 0, 4, 3} ,
        {3, 2, 1, 0, 4} ,
        {4, 3, 2, 1, 0} ,
        {0, 4, 3, 2, 1} ,
        {1, 0, 4, 3, 2} ,
    };

    /* Represents the column in which the color belongs to
     * Col 0 = Red
     * Col 1 = Yellow
     * Col 2 = Dark Blue
     * Col 3 = Light Blue
     * Col 4 = Black
     * Col 5 = Orange
     */
    int colourColumnsFor6x6[6][6] = {
        {2, 1, 0, 4, 3, 5} ,
        {3, 2, 1, 5, 4, 0} ,
        {4, 3, 2, 0, 5, 1} ,
        {5, 4, 3, 1, 0, 2} ,
        {0, 5, 4, 2, 1, 3} ,
        {1, 0, 5, 3, 2, 4} ,
    };

    char colourGrid[MAX_ROWS][MAX_COLS] = {
        {'B', 'Y', 'R', 'U', 'L'} ,
        {'L', 'B', 'Y', 'R', 'U'} ,
        {'U', 'L', 'B', 'Y', 'R'} ,
        {'R', 'U', 'L', 'B', 'Y'} ,
        {'Y', 'R', 'U', 'L', 'B'} ,
    };

    const char colourGridFor6x6[6][6] = {
        {'B', 'Y', 'R', 'U', 'L', 'O'} ,
        {'O', 'B', 'Y', 'R', 'U', 'L'} ,
        {'L', 'O', 'B', 'Y', 'R', 'U'} ,
        {'U', 'L', 'O', 'B', 'Y', 'R'} ,
        {'R', 'U', 'L', 'O', 'B', 'Y'} ,
        {'Y', 'R', 'U', 'L', 'O' ,'B'}
    };

    const char toSaveColourGridFor6x6[6][6] = {
        {'b', 'y', 'r', 'u', 'l', 'o'} ,
        {'o', 'b', 'y', 'r', 'u', 'l'} ,
        {'l', 'o', 'b', 'y', 'r', 'u'} ,
        {'u', 'l', 'o', 'b', 'y', 'r'} ,
        {'r', 'u', 'l', 'o', 'b', 'y'} ,
        {'y', 'r', 'u', 'l', 'o' ,'b'}
    };

    char toSavecolourGrid[MAX_ROWS][MAX_COLS] = {
        {'b', 'y', 'r', 'u', 'l'} ,
        {'l', 'b', 'y', 'r', 'u'} ,
        {'u', 'l', 'b', 'y', 'r'} ,
        {'r', 'u', 'l', 'b', 'y'} ,
        {'y', 'r', 'u', 'l', 'b'} ,
    };
};

#endif // MOSAIC_H