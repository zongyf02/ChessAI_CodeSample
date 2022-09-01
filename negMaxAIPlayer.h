#ifndef _NEGMAXAIPLAYER_H_
#define _NEGMAXAIPLAYER_H_
#include "player.h"
#include "../Pieces/piece.h"
#include "../Boards/board.h"
#include "../Moves/smove.h"
#include "../colour.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

// NegMax algorithm with pruning, works with only 2 players and standard board
template <typename Coordinate>
class NegMaxAIPlayer: public Player<Coordinate> {
    const int searchDepth;

    // Heuristics score for moves, range [N_INF * 2, INF * 2]
    virtual int heuristics(Colour curColour, const Move<Coordinate> &move, int depth) const = 0;

    // Move ordering heuristics
    virtual std::vector<unsigned int> orderMoves(const std::vector<Move<Coordinate>> &moves,
        int depth, bool &isPV) const = 0;
    
    // If player wins, it gets the maximum INF poitns
    // If player loses, it gets N_INF points,
    // If player draws, it gets 0 point instead.
    virtual int evaluateBoard(Colour curColour) const = 0;

    // NegMax tree search, fail-soft variant
    int negMax(int depth, int alpha, int beta,
        std::vector<SMove<Coordinate>> &newPVTable, bool &isPV) {

        // Return board evaluation if game has stopped
        // or depth is reached
        if (depth >= searchDepth || !(this->board->getIsRunning())) {
            return evaluateBoard(this->board->getCurColour());
        }

        // Get legal moves and ordered indices
        const std::vector<Move<Coordinate>> &moves = this->board->getMoves();

        // Use negMax to find best move and return its score 
        int bestEval = N_INF;
        for (auto index: orderMoves(moves, depth, isPV)) {
            auto& move = moves[index];
            this->board->tryMove(move, true); // Try a move with memoization
            // Update eval
            bestEval = std::max(bestEval, 0 - negMax(depth + 1, 0 - beta, 0 - alpha, newPVTable, isPV));
            this->board->untryMove();

            // Update alpha and pvTable
            if (bestEval > alpha) {
                alpha = bestEval;
                if (static_cast<size_t>(depth - 1) < newPVTable.size()) {
                    newPVTable[depth - 1] = SMove<Coordinate>(move);
                } else {
                    newPVTable.emplace_back(move);
                }
            }

            // Pruning
            if (bestEval >= beta) {
                // Update killer heuristic for non captures
                if (moves[index].isNonCapture()) {
                    SMove<Coordinate> temp{move};
                    killers[depth - 1][temp] += 8;
                    if (depth - 3 >= 0) killers[depth - 3][temp] += 4;
                    if (depth + 1 < searchDepth) killers[depth + 1][temp] += 4;
                }

                return bestEval;
            }
        }

        return bestEval; 
    }
protected:
    const static int N_INF = -30400;
    const static int INF = 30400;
    
    std::vector<std::unordered_map<SMove<Coordinate>, int>> killers;
    std::vector<SMove<Coordinate>> pvTable;
public:
    NegMaxAIPlayer(Board<Coordinate> *board, int searchDepth):
        Player<Coordinate>{board}, searchDepth{searchDepth},
        killers{static_cast<size_t>(searchDepth)}, pvTable{}  {
            if (board->getNColours() != 2) {
                throw std::runtime_error{"NegMaxPlayer can only be used on 2 player boards."};
            }
    }
    
    // Make the best move
    void makeMove() override {
        // Get all moves 
        const std::vector<Move<Coordinate>> &moves = this->board->getMoves();
        
        // Principal variation flag
        bool isPV = true;

        // Construct empty pv table
        std::vector<SMove<Coordinate>> newPVTable;
        newPVTable.reserve(searchDepth-1);

        // Clear killer moves
        for (auto &ply : killers) {
            ply.clear();
        }

        // Use negMax to find the move with the best eval
        int eval, alpha = N_INF, beta = INF;
        unsigned int bestIndex = 0;
        for (auto index: orderMoves(moves, 0, isPV)) {
            auto& move = moves[index];
            this->board->tryMove(move, true); // Try a move with memoization
            // Update eval
            eval = 0 - negMax(1, 0 - beta, 0 - alpha, newPVTable, isPV);
            this->board->untryMove();

            // Update alpha
            if (eval > alpha) {
                alpha = eval;
                bestIndex = index;
            }

            // No pruning is possible since beta is smallest possible
        }

        // Make best move
        this->board->make(moves[bestIndex]);
        
        // Memoize new pv table
        pvTable = std::move(newPVTable);
    }
};

#endif
