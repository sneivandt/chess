#include "board/evaluate.h"

#include "board/board.h"

#include <cstdint>
#include <cstdlib>

inline bool board::evaluate::materialDraw(Board& pos)
{
    if (!pos.getPieceNum(toInt(Piece::WR)) && !pos.getPieceNum(toInt(Piece::BR)) &&
        !pos.getPieceNum(toInt(Piece::WQ)) && !pos.getPieceNum(toInt(Piece::BQ)) &&
        !pos.getPieceNum(toInt(Piece::WP)) && !pos.getPieceNum(toInt(Piece::BP))) {
        if (!pos.getPieceNum(toInt(Piece::BB)) && !pos.getPieceNum(toInt(Piece::WB))) {
            if (pos.getPieceNum(toInt(Piece::WN)) < 3 && pos.getPieceNum(toInt(Piece::BN)) < 3) {
                return true;
            }
        }
        else if (!pos.getPieceNum(toInt(Piece::WN)) && !pos.getPieceNum(toInt(Piece::BN))) {
            if (abs(pos.getPieceNum(toInt(Piece::WB)) - pos.getPieceNum(toInt(Piece::BB))) < 2) {
                return true;
            }
        }
    }
    return false;
}

inline bool board::evaluate::isEndGame(Board& pos, const int side)
{
    if (side == toInt(Color::WHITE)) {
        if (!pos.getPieceNum(toInt(Piece::BQ)) && pos.getMaterial(toInt(Color::BLACK)) < ENDGAME_MATERIAL) {
            return true;
        }
    }
    else {
        if (!pos.getPieceNum(toInt(Piece::WQ)) && pos.getMaterial(toInt(Color::WHITE)) < ENDGAME_MATERIAL) {
            return true;
        }
    }
    return false;
}

int board::evaluate::score(Board& pos)
{
    if (materialDraw(pos)) {
        return 0;
    }
    int square;
    int side;
    int score = pos.getMaterial(toInt(Color::WHITE)) - pos.getMaterial(toInt(Color::BLACK));
    if (pos.getPieceNum(toInt(Piece::WB)) == 2) {
        score += BISHOP_PAIR;
    }
    if (pos.getPieceNum(toInt(Piece::BB)) == 2) {
        score -= BISHOP_PAIR;
    }
    for (int piece = toInt(Piece::WP); piece <= toInt(Piece::BK); piece++) {
        for (int i = 0; i < pos.getPieceNum(piece); i++) {
            square = pos.getPieceList(piece)[idx(i)];
            side = Board::PIECE_COLOR[idx(piece)];
            const auto sq64 = static_cast<size_t>(Board::SQ64[idx(square)]);
            const auto pieceType = static_cast<size_t>(Board::PIECE_NO_TEAM[idx(piece)]);

            if (Board::PIECE_COLOR[idx(piece)] == toInt(Color::WHITE)) {
                score += PIECE_SQUARE_TABLE[pieceType][sq64];
            }
            else {
                const auto mirroredSq = static_cast<size_t>(MIRROR_PIECE_SQUARE_TABLE[sq64]);
                score -= PIECE_SQUARE_TABLE[pieceType][mirroredSq];
            }
            if (piece == toInt(Piece::WK)) {
                if (isEndGame(pos, toInt(Color::WHITE))) {
                    score += PIECE_SQUARE_TABLE_KING_END[sq64];
                }
                else {
                    score += PIECE_SQUARE_TABLE_KING_OPENING[sq64];
                }
            }
            else if (piece == toInt(Piece::BK)) {
                const auto mirroredSq = static_cast<size_t>(MIRROR_PIECE_SQUARE_TABLE[sq64]);
                if (isEndGame(pos, toInt(Color::BLACK))) {
                    score -= PIECE_SQUARE_TABLE_KING_END[mirroredSq];
                }
                else {
                    score -= PIECE_SQUARE_TABLE_KING_OPENING[mirroredSq];
                }
            }
            else if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WP)) {
                if (!(pos.getPawns()[idx(side)] & Board::ISOLATED_PAWN_MASK[idx(Board::SQ64[idx(square)])])) {
                    score += PAWN_ISOLATED * (side == toInt(Color::WHITE) ? 1 : -1);
                }
                if (!(pos.getPawns()[idx(side ^ 1)] &
                      Board::PASSED_PAWN_MASK[idx(side)][idx(Board::SQ64[idx(square)])])) {
                    const auto rank = static_cast<size_t>(Board::RANKS[idx(Board::SQ64[idx(square)])]);
                    score += PASSED_PAWN[rank] * (side == toInt(Color::WHITE) ? 1 : -1);
                }
            }
            else if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WR)) {
                if (!(pos.getPawns()[idx(toInt(Color::BOTH))] &
                      Board::FILE_MASK[idx(Board::FILES[idx(Board::SQ64[idx(square)])])])) {
                    score += ROOK_OPEN_FILE * (side == toInt(Color::WHITE) ? 1 : -1);
                }
                else if (!(pos.getPawns()[idx(side)] &
                           Board::FILE_MASK[idx(Board::FILES[idx(Board::SQ64[idx(square)])])])) {
                    score += ROOK_SEMI_OPEN_FILE * (side == toInt(Color::WHITE) ? 1 : -1);
                }
            }
            else if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WQ)) {
                if (!(pos.getPawns()[idx(toInt(Color::BOTH))] &
                      Board::FILE_MASK[idx(Board::FILES[idx(Board::SQ64[idx(square)])])])) {
                    score += QUEEN_OPEN_FILE * (side == toInt(Color::WHITE) ? 1 : -1);
                }
                else if (!(pos.getPawns()[idx(side)] &
                           Board::FILE_MASK[idx(Board::FILES[idx(Board::SQ64[idx(square)])])])) {
                    score += QUEEN_SEMI_OPEN_FILE * (side == toInt(Color::WHITE) ? 1 : -1);
                }
            }
        }
    }
    if (pos.getSide() == toInt(Color::BLACK)) {
        score = -score;
    }
    return score;
}
