#ifndef TILE_H
#define TILE_H

#include <string>
#include <memory>

#define RED_CHAR        'R'
#define YELLOW_CHAR     'Y'
#define DARKBLUE_CHAR   'B'
#define LIGHTBLUE_CHAR  'L'
#define BLACK_CHAR      'U'
#define ORANGE_CHAR     'O'
#define FIRST_CHAR      'F'
#define EMPTY           'X'

//if you want to change it to #defines or typedefs or normal strings go ahead
enum Type{
    RED,
    YELLOW,
    DARK_BLUE,
    LIGHT_BLUE,
    BLACK,
    ORANGE,
    FIRST_PLAYER,
    NONE
};

class Tile {
public:
    Tile(const Type type);
    Tile(const Tile& other);
    ~Tile();
    Type getType() const;
    char getColourType() const;
    
private:
    Type type;   
};

#endif // TILE_H