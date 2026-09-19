# Chess Engine Bot 
Third iteration of my chess engine, this time written in C++20 instead of Java. This iteration is being developed more rigorously, with the intent to publish a polished version to lichess.org. Some of the naming and technical conventions are inspired by Stockfish, but all code is written by me. 

## Roadmap
- [x] Bitboard-based position representation
- [x] FEN parsing with validation
- [ ] Move generation
- [ ] Search
- [ ] Evaluation heuristics
- [ ] NNUE

## Build & run
Requirements: C++20 compiler

```
clang++ -std=c++20 -Wall -Wextra tests/print_board.cpp -o print_board
./print_board
```

## Project structure
```
src/
    types.h         - Basic types: Color, Piece, PieceType, Square, bitboard helpers
    position.h      - Position struct for position representation, and position helpers
    fen.h           - FEN string parser with error reporting
tests/
    print_board.cpp - parses FEN and prints board
```

## License
MIT - see LICENSE
