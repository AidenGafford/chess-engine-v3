#pragma once

#include "types.h"


// StateInfo - contains all non-reversable data when making move
// TODO: implement with makeMove/undoMove
// to hold castlingRights, drawClock, enPassantSquare, and capturedPiece
struct StateInfo {
    
};

// Position - holds current board state data
struct Position {
    std::array<Piece, BOARD_SIZE> board;
    std::array<Bitboard, 8> typeBB;
    std::array<Bitboard, 2> colorBB;
    // TODO: implement checker, blocker, pinner bitboards


    Color sideToMove;
    int castlingRights;
    u8 enPassantSquare;
    int drawClock;
    int ply;

    Position() {
        board.fill(EMPTY_PIECE);
        colorBB.fill(0);
        typeBB.fill(0);
        
        sideToMove = WHITE;
        castlingRights = 0;
        enPassantSquare = SQ_NONE;
        drawClock = 0;
        ply = 0;

    }

    // TODO: implement makeMove
    void makeMove() {

    }

    // TODO: implement undoMove
    void undoMove() {

    }
    
    
};

// PositionUtils - for debugging and fen parsing
namespace PositionUtils {

    inline Piece tokenToPiece(char token) {
        switch (token) {
            case 'P': return W_PAWN;
            case 'R': return W_ROOK;
            case 'N': return W_KNIGHT;
            case 'B': return W_BISHOP;
            case 'Q': return W_QUEEN;
            case 'K': return W_KING;

            case 'p': return B_PAWN;
            case 'r': return B_ROOK;
            case 'n': return B_KNIGHT;
            case 'b': return B_BISHOP;
            case 'q': return B_QUEEN;
            case 'k': return B_KING;

            default: return EMPTY_PIECE;
        }
    }

    inline char pieceToToken(Piece p) {
        switch (p) {
            case W_PAWN:   return 'P';
            case W_KNIGHT: return 'N'; 
            case W_BISHOP: return 'B'; 
            case W_ROOK:   return 'R'; 
            case W_QUEEN:  return 'Q'; 
            case W_KING:   return 'K'; 
            
            case B_PAWN:   return 'p';
            case B_KNIGHT: return 'n'; 
            case B_BISHOP: return 'b'; 
            case B_ROOK:   return 'r'; 
            case B_QUEEN:  return 'q'; 
            case B_KING:   return 'k'; 

            case EMPTY_PIECE: return '-';
        }
        return '?';
    }

    inline bool isValidNotation(std::string notation) {
        if (notation.size() != 2) return false;

        char file = notation[0];
        char rank = notation[1];

        return (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8');
    }

    // precondition: isValidNotation(notation) = true
    inline u8 notationToSquare(std::string notation) {
        int file = notation[0] - 'a';
        int rank = notation[1] - '1';
        
        return rank * 8 + file;
    }

    inline std::string squareToNotation(u8 square) {
        char rank = '1' + square / 8;
        char file = 'a' + square % 8;
        return std::string{file, rank};
    }

    inline void printBoard(const Position& position) {
        std::array<Piece, BOARD_SIZE> board = position.board;
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col < 8; ++col) {
                std::cout << pieceToToken(board[row * 8 + col]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    
}

