#ifndef _CLASSICBOARDPLAYER_H_
#define _CLASSICBOARDPLAYER_H_

#include "../coords2d.h"
#include "negMaxAIPlayer.h"

class ClassicBoardPlayer: public NegMaxAIPlayer<Coords2D> {
    const static std::vector<int> phaseValue;
    const static std::vector<int> mgPieceValue;
    const static std::vector<int> egPieceValue;
    const static std::vector<std::vector<std::vector<int>>> mgTable;
    const static std::vector<std::vector<std::vector<int>>> egTable;

    // Struct for move ordering
    struct Zip {
        int score;
        unsigned int index;

        Zip();
        Zip(int score, unsigned int index);

        bool operator<(const Zip &other) const; 
    };
    
    int heuristics(Colour curColour, const Move<Coords2D> &move, int depth) const override;

    std::vector<unsigned int> orderMoves(const std::vector<Move<Coords2D>> &moves,
        int depth, bool &isPV) const override;
    
    int evaluateBoard(Colour curColour) const override;

public:
    ClassicBoardPlayer(Board<Coords2D> *board, int searchDepth);
};


#endif
