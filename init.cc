#include "init.h"

void init::zobristKeys()
{
    std::random_device rd;
    std::mt19937_64 e2(rd());
    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 120; j++) {
            PIECE_KEYS[i][j] = dist(e2);
        }
    }
    for(int i = 0; i < 16; i++) {
        CASTLE_KEYS[i] = dist(e2);
    }
    SIDE_KEY = dist(e2);
}

void init::mvvlva()
{
    for(int attacker = WP; attacker <= BK; attacker++) {
        for(int victim = WP; victim < BK; victim++) {
            movegen::MVVLVA_SCORES[victim][attacker] = VICTUM_SCORES[victim] + 6 - (VICTUM_SCORES[attacker] / 100);
        }
    }
}

void init::bitmasks()
{
    int square;
    int tSquare;
    for(int i = 0; i < 8; i++) {
        FILE_MASK[i] = 0ULL;
        RANK_MASK[i] = 0ULL;
    }
    for(int i = 0; i < 64; i++) {
        PASSED_PAWN_MASK[0][i] = 0ULL;
        PASSED_PAWN_MASK[1][i] = 0ULL;
        ISOLATED_PAWN_MASK[i] = 0ULL;
    }
    for(int rank = RANK_1; rank <= RANK_8; rank++) {
        for(int file = FILE_A; file <= FILE_H; file++) {
            square = rank * 8 + file;
            FILE_MASK[file] |= (1ULL << square);
            RANK_MASK[rank] |= (1ULL << square);
        }
    }
    for(square = 0; square < 64; square++) {
        tSquare = square + 8;
        while(tSquare < 64) {
            PASSED_PAWN_MASK[WHITE][square] |= (1ULL << tSquare);
            tSquare += 8;
        }
        tSquare = square - 8;
        while(tSquare >= 0) {
            PASSED_PAWN_MASK[BLACK][square] |= (1ULL << tSquare);
            tSquare -= 8;
        }
        if(FILES[square] > FILE_A) {
            ISOLATED_PAWN_MASK[square] |= FILE_MASK[FILES[square] - 1];
            tSquare = square + 7;
            while(tSquare < 64) {
                PASSED_PAWN_MASK[WHITE][square] |= (1ULL << tSquare);
                tSquare += 8;
            }
            tSquare = square - 9;
            while(tSquare >= 0) {
                PASSED_PAWN_MASK[BLACK][square] |= (1ULL << tSquare);
                tSquare -= 8;
            }
        }
        if(FILES[square] < FILE_H) {
            ISOLATED_PAWN_MASK[square] |= FILE_MASK[FILES[square] + 1];
            tSquare = square + 9;
            while(tSquare < 64) {
                PASSED_PAWN_MASK[WHITE][square] |= (1ULL << tSquare);
                tSquare += 8;
            }
            tSquare = square - 9;
            while(tSquare >= 0) {
                PASSED_PAWN_MASK[BLACK][square] |= (1ULL << tSquare);
                tSquare -= 8;
            }
        }
    }
}

void init::all()
{
    zobristKeys();
    mvvlva();
    bitmasks();
}
