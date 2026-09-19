#pragma once

#include <iostream>
#include <bit>
#include <bitset>
#include <array>
#include <string>
#include <cstdint>
#include <sstream>
#include <optional>
#include <unordered_map>
#include <stdexcept>

using u8 = uint8_t;
using Bitboard = uint64_t;

constexpr size_t BOARD_SIZE = 64;
inline const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// enums - represent pieces by color and/or type 
enum Color : u8 {
    WHITE, BLACK
};

enum PieceType : u8 {
    EMPTY_PIECE_TYPE, 
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

enum Piece : u8 {
    EMPTY_PIECE = 0,
    W_PAWN,     W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = 9, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

// enums - represent rank/file and square
enum Rank : u8 {
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum File : u8 {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H 
};

enum Square : u8 {
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1, 
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2, 
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3, 
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4, 
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5, 
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6, 
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7, 
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
    SQ_NONE = 64
};


// Bitboards - represent various properties of 64-square 
// chess board using 64-bit unsigned integers

// expressions
constexpr Color colorOf(Piece piece) {
    return static_cast<Color>(piece >> 3);
}

constexpr PieceType typeOf(Piece piece) {
    return static_cast<PieceType>(piece & 7);
}

constexpr Piece makePiece(Color color, PieceType pieceType) {
    return static_cast<Piece> ((color << 3) + pieceType);
} 

constexpr Rank rankOf(Square square) {
    return static_cast<Rank>(square / 8);
}

constexpr File fileOf(Square square) {
    return static_cast<File>(square % 8);
}


// rank / file bitboards
constexpr Bitboard RANK_1_BB = (1ULL << 8) - 1;
constexpr Bitboard makeRankBB(int rank) {
    return RANK_1_BB << (8 * rank);
}


// castling bitboards
enum CastlingRights {
    WHITE_OO_BB  = 0b0001,
    WHITE_OOO_BB = 0b0010,
    BLACK_OO_BB  = 0b0100,
    BLACK_OOO_BB = 0b1000
};