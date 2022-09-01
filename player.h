#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../Boards/board.h"
#include "../colour.h"

template <typename Coordinate>
class Player {
protected:
    Board<Coordinate> *board;
public:
    Player(Board<Coordinate> *board): board{board} {}
    virtual ~Player() {}
    // Make a legal move on board
    virtual void makeMove() = 0;
};

#endif
