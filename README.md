# ChessAI_CodeSample

A Chess AI based on the NegaMax algorithm with alpha-beta pruning.

Uses move ordering, PV-List, and a modifierd version of killer/history heuristic to improve the performance of pruning.

The ClassicBoardPlayer uses [PeSTO](https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function)'s tapered evaluation function and piece-square tables.

Further improvement may include using a transpositional table with Zorbrsit hashing to store previous node evaluations.

Note:
The Chess AI is based on a chess program that [Michael Lapshin](https://github.com/MichaelLapshin) and I wrote for a Waterloo CS247 project.
As a result, signicant portion of the code cannot be made public, but is available upon request.
The code contained here was written later and was not submitted as part of the project.
