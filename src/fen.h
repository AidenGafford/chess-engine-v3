#pragma once

#include "position.h"

namespace FenUtils { 

    // FenParseError - runtime error alternate for Fen parser
    struct FenParseError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    // setFen - updates position with a fen
    inline std::optional<FenParseError> setFen(Position& position, std::string fen) { 
        // mutate temp position to allow for safe abort if error
        Position tempPosition;

        std::stringstream ss (fen);
        std::string boardFen, sideToMoveFen, castlingRightsFen, enPassantSquareFen, drawClockFen, moveNumberFen;
        
        if (!(ss >> boardFen >> sideToMoveFen >> castlingRightsFen >> enPassantSquareFen >> drawClockFen >> moveNumberFen)) {
            return FenParseError("Invalid FEN: Too few arguments");
        }
        
            // debug
            // std::cout << "Fen split successful: " << std::endl;
            // std::cout << "Board:                " << boardFen << std::endl;
            // std::cout << "Side to move:         " << sideToMoveFen << std::endl;
            // std::cout << "En Passant square:    " << enPassantSquareFen << std::endl;
            // std::cout << "Draw clock:           " << drawClockFen << std::endl;
            // std::cout << "Move number:          " << moveNumberFen << std::endl;
            


        std::string ssEnd;
        if (ss >> ssEnd) {
            return FenParseError("Invalid FEN: Too many arguments: " + ssEnd);

        }
        
        // 1. board
        size_t currFenIndex = 0;
        size_t currBoardIndex = 0;
        char currToken;

        // process char by char
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col < 8; ++col) {
                if (currFenIndex >= boardFen.size()) {
                    return FenParseError("Invalid FEN: Board token stream ended abruptly");
                }
                currToken = boardFen[currFenIndex];
                currBoardIndex = row * 8 + col;

                // debug
                // std::cout << row << " " << col << " " << currFenIndex << " " << currToken << std::endl;
                
                // number -> empty
                if (std::isdigit(currToken)) {
                    int emptySpaces = currToken - '0';
                    if (emptySpaces == 0) {
                        return FenParseError(std::string("Invalid FEN: Cannot have zero empty spaces in rank") + std::to_string(row + 1));
                    }
                    col += emptySpaces - 1;
                    if (col > 7) {
                        return FenParseError(std::string("Invalid FEN: Rank overflow in rank ") + std::to_string(row + 1));
                    }
                }

                // token -> update piece
                else {
                    Piece piece = PositionUtils::tokenToPiece(currToken);
                    if (piece == EMPTY_PIECE) {
                        return FenParseError(std::string("Invalid FEN: Invalid token ") + currToken);
                    }
                    tempPosition.board[currBoardIndex] = piece;
                    Bitboard currBit = 1ULL << currBoardIndex;
                    tempPosition.typeBB[typeOf(piece)] |= currBit;
                    tempPosition.colorBB[colorOf(piece)] |= currBit;
                }
                
                ++currFenIndex;
            }
            
            if (row > 0) {
                if (currFenIndex >= boardFen.size()) {
                    return FenParseError("Invalid FEN: Board token stream ended abruptly");
                }   
                if (boardFen[currFenIndex] != '/') {
                    return FenParseError("Invalid FEN: too many tokens in rank" + std::to_string((row + 1)));
                }
                ++currFenIndex;
            }
            else if (currFenIndex != boardFen.size()) {
                return FenParseError("Invalid FEN: excess tokens in board stream");
            }
        }

        // - verify board -
        
        // helper bitboards
        Bitboard kingBB = tempPosition.typeBB[KING];
        Bitboard rookBB = tempPosition.typeBB[ROOK];

        // verify one king per each side
        if (!(  (std::has_single_bit(kingBB & tempPosition.colorBB[WHITE])) && 
                (std::has_single_bit(kingBB & tempPosition.colorBB[BLACK])) )) {
            return FenParseError("Unsupported format: Each side must have exactly one king");
        }
        
        // verify valid pawn ranks
        Bitboard illegalPawnBB = (makeRankBB(RANK_1) | makeRankBB(RANK_8));
        if ((illegalPawnBB & tempPosition.typeBB[PAWN]) != 0) {
            return FenParseError("Unsupported format: Pawns cannot be on 1st or 8th rank");
        }

        // 2. side to move
        if (sideToMoveFen != "w" && sideToMoveFen != "b") {
            return FenParseError("Invalid FEN: Invalid side to move " + sideToMoveFen);
        }    
        tempPosition.sideToMove = (sideToMoveFen == "w") ? WHITE : BLACK;
        
        // TODO: verify sidetoMove not giving a check - implement after movegen


        // 3. castling rights
        if (castlingRightsFen != "-") {
            for (char token : castlingRightsFen) {
                Color side;
                bool kingside;
                Bitboard castleBB;

                switch (token) {
                    case 'K' : side = WHITE; kingside = true;  castleBB = WHITE_OO_BB;  break;
                    case 'Q' : side = WHITE; kingside = false; castleBB = WHITE_OOO_BB; break;
                    case 'k' : side = BLACK; kingside = true;  castleBB = BLACK_OO_BB;  break;
                    case 'q' : side = BLACK; kingside = false; castleBB = BLACK_OOO_BB; break;
                    default:
                        return FenParseError(std::string("Invalid FEN: Invalid castling rights token ") + token);
                }


                Bitboard sideBB = tempPosition.colorBB[side];
                Bitboard rankBB = makeRankBB((side == WHITE) ? RANK_1 : RANK_8);
                Bitboard leftBB =  (kingside ? kingBB : rookBB) & sideBB & rankBB;
                Bitboard rightBB = (kingside ? rookBB : kingBB) & sideBB & rankBB;

                if ((leftBB == 0) || (rightBB == 0) || ((1ULL << std::countr_zero(leftBB)) > rightBB)) {
                    return FenParseError(std::string("Unsupported position: castling rights not permitted for token ") + token);
                }

                tempPosition.castlingRights |= castleBB;
            }
        }
        

        // 4. en passant square
        if (enPassantSquareFen == "-") {
            tempPosition.enPassantSquare = SQ_NONE;
        }
        else if (!(PositionUtils::isValidNotation(enPassantSquareFen))) {
            return FenParseError("Invalid FEN: Invalid en passant square " + enPassantSquareFen);
        }
        else {
            tempPosition.enPassantSquare = PositionUtils::notationToSquare(enPassantSquareFen);
            Bitboard enPassantBB = (1ULL << tempPosition.enPassantSquare);
            Bitboard validEnPassantBB = (tempPosition.sideToMove == WHITE) ? makeRankBB(RANK_6) : makeRankBB(RANK_3);
            if ((enPassantBB & validEnPassantBB) == 0) {
                return FenParseError("Invalid FEN: En passant square out of range " + enPassantSquareFen);
            }
        }

        // 5. halfmove draw clock
        try {
            tempPosition.drawClock = std::stoi(drawClockFen);
        }
        catch(const std::exception &e) {
            return FenParseError("Invalid FEN: Halfmove draw clock must be an integer");
        }
        if (tempPosition.drawClock < 0) {
            return FenParseError("Invalid FEN: Halfmove draw clock out of range");

        }

        // 6. fullmove number
        int fullMove;

        try {
            fullMove = std::stoi(moveNumberFen);
        }
        catch(const std::exception &e) {
            return FenParseError("Invalid FEN: Fullmove number must be an integer");
        }

        if (fullMove < 1) {
            return FenParseError("Invalid FEN: FullMove number out of range");
        }
        tempPosition.ply = (fullMove - 1) * 2 + (tempPosition.sideToMove == WHITE ? 0 : 1);
        
        position = tempPosition;
        return std::nullopt;
    }

    // TODO: makeFen from board

}