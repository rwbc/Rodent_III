/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2019 Pawel Koziol

Rodent is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Rodent is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/*
Rodent can use several sets of piece/square tables (currently 4).

0) Values based on Hakkapeliitta by Mike Aarnos (https://github.com/mAarnos/Hakkapeliitta).
   The original set was highly asymmetric, so probably auto-tuned. Rodent uses symmetric
   average of the values from both wings, that has been subsequently tweaked.

1) Values from older Rodent versions, based on Fruit (via Toga Log User Manual),
   also tweaked a bit. They emphasize pawn center and knight placement.

2) Values entered by Jose Velasco in my piece/square table contest as (simplified version of)
   Ippolit values. They deemphasize pawn placement, requiring usage of other eval terms,
   such as pawn phalanxes. Good for positional, neutral and hypermodern personalities.

3) Values from https://chessprogramming.wikispaces.com/Simplified+evaluation+function
   by Tomasz Michniewski with rook on 7th rank bonus removed. This set is clearly weaker

*/

static const int pstPawnMg[5][64] = {
 //A1                                H1 Midgame pawn table - wing-neutral average of Hakkapeliitta table

{ 0,   0,   0,   0,   0,   0,   0,   0,
-30, -16, -22, -27, -27, -17, -16, -30,
-25, -16, -16, -18, -18, -16, -16, -25,
-20, -17,  -7,  -2,  -2,  -7, -17, -20,
-12,  -3,   2,  16,  16,   2,  -3, -12,
  9,  22,  43,  39,  39,  43,  22,   9,
 15,  16,  76,  93,  93,  76,  16,  15,
  0,   0,   0,   0,   0,   0,   0,   0 	
},
//A8                                H8

//A1                                H1  Midgame pawn table based on Fruit
{ 0,   0,   0,   0,   0,   0,   0,   0,
-15,  -5,   5,   5,   5,   5,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
-15,  -5,  10,  25,  25,  10,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
  0,   0,   0,   0,   0,   0,   0,   0},
//A8                                H8

//A1                                H1  Midgame pawn table based on Ippolit
{ 0,   0,   0,   0,   0,   0,   0,   0,
-23, -11,  -5,   2,   2,  -5, -11, -23,
-22, -10,  -4,   3,   3,  -4, -10, -22,
-21,  -9,  -3,   4,   4,  -3,  -9, -21,
-19,  -7,  -1,   6,   6,  -1,  -7, -19,
-18,  -6,   0,   7,   7,   0,  -6, -18,
-17,  -5,   1,   8,   8,   1,  -5, -17,
  0,   0,   0,   0,   0,   0,   0,   0},
//A8                                H8

//A1                                H1  Pawn table by Tomasz Michniewski
{ 0,   0,   0,   0,   0,   0,   0,  0,
  5,  10,  10, -20, -20,  10,  10,  5,
  5,  -5, -10,   0,   0, -10,  -5,  5,
  0,   0,   0,  20,  20,   0,   0,  0,
  5,   5,  10,  25,  25,  10,   5,  5,
 10,  10,  20,  30,  30,  20,  10, 10,
 50,  50,  50,  50,  50,  50,  50, 50,
  0,   0,   0,   0,   0,   0,   0,  0
//A8                                H8
  },

{  0,   0,   0,   0,   0,   0,  0,   0, // Rofchade table with flattened last rows
 -35,  -1, -20, -23, -15,  24, 38, -22,
 -26,  -4,  -4, -10,   3,   3, 33, -12,
 -27,  -2,  -5,  12,  17,   6, 10, -25,
 -14,  13,   6,  21,  23,  12, 17, -23,
  -6,   7,  26,  31,  35,  36, 25, -20,
  38,  34,  31,  35,  38,  36, 34,  33,
  0,   0,   0,   0,   0,   0,  0,   0, }

};

static const int pstPawnEg[4][64] = {
//A1                                H1 Endgame pawn table - wing-neutral average of Hakkapeliitta table
{ 0,   0,   0,   0,   0,   0,   0,   0,
-16,  -8,  -9,   2,   2,  -9,  -8, -16,
-20, -11, -18, -16, -16, -18, -11, -20,
-15,  -6, -20, -26, -26, -20,  -6, -15,
  2,   0, -10, -23, -23, -10,   0,   2,
 31,  37,  10,   0,   0,  10,  37,  31,
 49,  43,  39,  33,  33,  39,  43,  49,
  0,   0,   0,   0,   0,   0,   0,   0},
//A8                                H8

//A1                                H1
{ 0,   0,   0,   0,   0,   0,   0,   0,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  0,   0,   0,   0,   0,   0,   0,   0},
//A8                                H8

//A1                                H1 Endgame pawn table based on Ippolit
{ 0,   0,   0,   0,   0,   0,   0,   0,
 -2,  -4,  -6,  -8,  -8,  -6,  -4,  -2,
 -4,  -6,  -8, -10, -10,  -8,  -6,  -4,
 -5,  -7,  -9, -11, -11,  -9,  -7,  -5,
 -6,  -8, -10, -12, -12, -10,  -8,  -6,
 -7,  -9, -11, -13, -13, -11,  -9,  -7,
 -7,  -9, -11, -13, -13, -11,  -9,  -7,
  0,   0,   0,   0,   0,   0,   0,   0 },
//A8                                H8

//A1                                H1  Pawn table by Tomasz Michniewski
{ 0,   0,   0,   0,   0,   0,   0,   0,
  5,  10,  10, -20, -20,  10,  10,   5,
  5,  -5, -10,   0,   0, -10,  -5,   5,
  0,   0,   0,  20,  20,   0,   0,   0,
  5,   5,  10,  25,  25,  10,   5,   5,
 10,  10,  20,  30,  30,  20,  10,  10,
 50,  50,  50,  50,  50,  50,  50,  50,
  0,   0,   0,   0,   0,   0,   0,   0 }
//A8                                H8
 };

static const int pstKnightMg[4][64] = {

//A1                                 H1 Midgame knight table - wing-neutral average of Hakkapeliitta table
{-62, -33, -30, -30, -30, -30, -33, -62,
 -25, -23, -10,  -7,  -7, -10, -23, -25,
 -19,  -2,   3,  22,  22,   3,  -2, -19,
 -14,  16,  17,  20,  20,  17,  16, -14,
  16,  14,  33,  36,  36,  33,  14,  16,
  12,  34,  73,  88,  88,  73,  34,  12,
 -17,  -1,  49,  61,  61,  49,  -1, -17,
 -84, -36,  -2,  18,  18,  -2, -36, -84},
//A8                                 H8

//A1                                 H1 Midgame knight table based on Fruit
{-50, -40, -30, -20, -20, -30, -40, -50,
 -35, -25, -15,   0,   0, -15, -25, -35,
 -20, -10,   0,  10,  10,   0, -10, -20,
 -10,   0,  10,  20,  20,  10,   0, -10,
  -5,   5,  15,  25,  25,  15,   5,  -5,
  -5,   5,  15,  25,  25,  15,   5,  -5,
 -20, -10,   0,  10,  10,   0, -10, -20,
-135, -25, -15,  -5,  -5, -15, -25, -135},
//A8                                 H8

//A1                                 H1 Midgame knight table based on Ippolit
{-58, -42, -31, -27, -27, -31, -42, -58,
 -36, -20,  -9,  -5,  -5,  -9, -20, -36,
 -20,  -4,   7,  11,  11,   7,  -4, -20,
 -11,   5,  16,  20,  20,  16,   5, -11,
  -5,  11,  22,  26,  26,  22,  11,  -5,
  -7,   9,  20,  24,  24,  20,   9,  -7,
 -16,  10,  11,  15,  15,  11,  10, -16,
-120, -21, -10,  -6,  -6, -10, -21,-120 },
//A8                                 H8

//A1                                 H1 Knight table by Tomasz Michniewski
{-50,- 40, -30, -30, -30, -30, -40, -50,
 -40, -20,   0,   5,   5,   0, -20, -40,
 -30,   5,  10,  15,  15,  10,   5, -30,
 -30,   0,  15,  20,  20,  15,   0, -30,
 -30,   5,  15,  20,  20,  15,   5, -30,
 -30,   0,  10,  15,  15,  10,   0, -30,
 -40, -20,   0,   0,   0,   0, -20, -40,
 -50, -40, -30 ,-30, -30, -30, -40, -50 }
};

static const int pstKnightEg[4][64] = {
//A1                                 H1 Endgame knight table - wing-neutral average of Hakkapeliitta table
{-33, -44, -20, -10, -10, -20, -44, -33,
 -15,  -9,   1,   1,   1,   1,  -9, -15,
 -11,  -2,   7,  28,  28,   7,  -2, -11,
  -4,  21,  37,  42,  42,  37,  21,  -4,
  18,  25,  45,  51,  51,  45,  25,  18,
   1,  34,  37,  30,  30,  37,  34,   1,
  -9,  24,   9,  32,  32,   9,  24,  -9,
 -33,  10,   9,   2,   2,   9,  10, -33},
//A8                                 H8

//A1                                 H1 Endgame knight table based on Fruit
{-40, -30, -20, -10, -10, -20, -30, -40,
 -30, -20, -10,   0,   0, -10, -20, -30,
 -20, -10,   0,  10,  10,   0, -10, -20,
 -10,   0,  10,  20,  20,  10,   0, -10,
 -10,   0,  10,  20,  20,  10,   0, -10,
 -20, -10,   0,  10,  10,   0, -10, -20,
 -30, -20, -10,   0,   0, -10, -20, -30,
 -40, -30, -20, -10, -10, -20, -30, -40 },
//A8                                 H8

//A1                                 H1 Endgame knight table based on Ippolit
{-22, -17, -12,  -9,  -9, -12, -17, -22,
 -15,  -8,  -4,  -2,  -2,  -4,  -8, -15,
 -10,  -4,   1,   3,   3,   1,  -4, -10,
  -6,  -1,   4,   8,   8,   4,  -1,  -6,
  -4,   1,   6,  10,  10,   6,   1,  -4,
  -3,   3,   8,  10,  10,   8,   3,  -3,
  -8,  -1,   3,   5,   5,   3,  -1,  -8,
 -15, -10,  -5,  -2,  -2,  -5, -10, -15 },
//A8                                 H8

//A1                                 H1 Knight table by Tomasz Michniewski
{-50, -40, -30, -30, -30, -30, -40, -50,
 -40, -20,   0,   5,   5,   0, -20, -40,
 -30,   5,  10,  15,  15,  10,   5, -30,
 -30,   0,  15,  20,  20,  15,   0, -30,
 -30,   5,  15,  20,  20,  15,   5, -30,
 -30,   0,  10,  15,  15,  10,   0, -30,
 -40, -20,   0,   0,   0,   0, -20, -40,
 -50, -40, -30, -30, -30, -30, -40, -50 }
//A8                                 H8
};

static const int pstBishopMg[4][64] = {
//A1                                 H1 Midgame bishop table - wing-neutral average of Hakkapeliitta table
{ -9,  -1, -14, -12, -12, -14,  -1,  -9,
  -5,   2,   5,  -7,  -7,   5,   2,  -5,
  -8,   7,  -2,   1,   1,  -2,   7,  -8,
   3,  -7,  -2,   9,   9,  -2,  -7,   3,
  -8,  -1,  20,  22,  22,  20,  -1,  -8,
   4,  37,  39,  25,  25,  39,  37,   4,
  -5, -15,   9, -12, -12,   9, -15,  -5,
 -31, -42, -26, -48, -48, -26, -42, -31},
//A8                                 H8

//A1                                 H1 Midgame bishop table based on Fruit
{ -2,  -5, -16,  -5,  -5, -16,  -5,  -2,
  -5,   6,   3,   6,   6,   3,   6,  -5,
  -5,   5,   8,  10,  10,   8,   5,  -5,
  -5,   0,  10,  13,  13,  10,   0,  -5,
  -5,   0,   5,  13,  13,   5,   0,  -5,
  -5,   0,   5,   5,   5,   5,   0,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5 },
//A8                                 H8

//A1                                 H1 Midgame bishop table based on Ippolit
{ -7,  -8, -11, -13, -13, -11,  -8,  -7,
  -3,   3,   0,  -2,  -2,   0,   3,  -3,
  -6,   0,   7,   6,   6,   7,   0,  -6,
  -8,  -2,   6,  15,  15,   6,  -2,  -8,
  -8,  -2,   6,  15,  15,   6,  -2,  -8,
  -6,   0,   7,   6,   6,   7,   0,  -6,
  -3,   3,   0,  -2,  -2,   0,   3,  -3,
  -2,  -3,  -6,  -8,  -8,  -6,  -3,  -2 },
//A8                                 H8

//A1                                 H1 Bishop table by Tomasz Michniewski
{-20, -10, -10, -10, -10, -10, -10, -20,
 -10,   5,   0,   0,   0,   0,   5, -10,
 -10,  10,  10,  10,  10,  10,  10, -10,
 -10,   0,  10,  10,  10,  10,   0, -10,
 -10,   5,   5,  10,  10,   5,   5, -10,
 -10,   0,   5,  10,  10,   5,   0, -10,
 -10,   0,   0,   0,   0,   0,   0, -10,
 -20, -10, -10, -10, -10, -10, -10, -20}
//A8                                 H8
};

static const int pstBishopEg[4][64] = {
//A1                                 H1 Endgame bishop table - wing-neutral average of Hakkapeliitta table
{-29, -13,  -4,  -6,  -6,  -4, -13, -29,
 -11, -10,  -9,  -1,  -1,  -9, -10, -11,
  -1,   7,   5,  13,  13,   5,   7,  -1,
  -7,   5,  11,  15,  15,  11,   5,  -7,
   7,   6,   7,  20,  20,   7,   6,   7,
  18,  16,   5,   7,   7,   5,  16,  18,
  -7,  11,   0,   8,   8,   0,  11,  -7,
   6,  21,  19,  19,  19,  19,  21,   6},
//A8                                 H8

//A1                                 H1 Endgame bishop table based on Fruit
{-15, -10,  -8,  -5,  -5,  -8, -10, -15,
 -10,  -8,   0,   5,   5,   0,  -8, -10,
  -5,   0,   5,  10,  10,   5,   0,  -8,
  -5,   5,  10,  15,  15,  10,   5,  -5,
  -5,   5,  10,  15,  15,  10,   5,  -5,
  -8,   0,   5,  10,  10,   5,   0,  -8,
  10,  -8,   0,   5,   5,   0,  -8, -10,
  -15, -10, -8,  -5,  -5,  -8, -10, -15 },
 //A8                                 H8

 //A1                                 H1 Endgame bishop table based on Ippolit
{  0,  -1,  -2,  -2,  -2,  -2,  -1,   0,
  -1,   1,   0,   0,   0,   0,   1,  -1,
  -2,   0,   3,   2,   2,   3,   0,  -2,
  -2,   0,   2,   5,   5,   2,   0,  -2,
  -2,   0,   2,   5,   5,   2,   0,  -2,
  -2,   0,   3,   2,   2,   3,   0,  -2,
  -1,   1,   0,   0,   0,   0,   1,  -1,
   0,  -1,  -2,  -2,  -2,  -2,  -1,   0 },
 //A8                                 H8

 //A1                                 H1 Bishop table by Tomasz Michniewski
{-20, -10, -10, -10, -10, -10, -10, -20,
 -10,   5,   0,   0,   0,   0,   5, -10,
 -10,  10,  10,  10,  10,  10,  10, -10,
 -10,   0,  10,  10,  10,  10,   0, -10,
 -10,   5,   5,  10,  10,   5,   5, -10,
 -10,   0,   5,  10,  10,   5,   0, -10,
 -10,   0,   0,   0,   0,   0,   0, -10,
 -20, -10, -10, -10, -10, -10, -10, -20}
 //A8                                H8
};

static const int pstRookMg[4][64] = {
//A1                                 H1 Midgame rook table - wing-neutral average of Hakkapeliitta table
{-15,  -1,   0,   3,   3,   0,  -1, -15,
 -33, -11, -14, -11, -11, -14, -11, -33,
 -26, -13, -15,  -2,  -2, -15, -13, -26,
 -33,  -1, -26,  -9,  -9, -26,  -1, -33,
   1,  15,  -3,   9,   9,  -3,  15,   1,
  10,  41,  30,  37,  37,  30,  41,  10,
  27,  17,  51,  48,  48,  51,  17,  27,
  44,  39,  34,  16,  16,  34,  39,  44},
 //A8                                H8

 //A1                                H1 Midgame rook table based on Fruit
{-3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3},
//A8                                H8

//A1                                H1 Midgame rook table based on Ippolit
{-4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4,
 -4,   0,   4,   8,   8,   4,   0,  -4 },
//A8                                H8

//A1                                H1 Rook table by Tomasz Michniewski
{ 0,   0,   0,   5,   5,   0,   0,   0,
 -5,   0,   0,   0,   0,   0,   0,  -5,
 -5,   0,   0,   0,   0,   0,   0,  -5,
 -5,   0,   0,   0,   0,   0,   0,  -5,
 -5,   0,   0,   0,   0,   0,   0,  -5,
 -5,   0,   0,   0,   0,   0,   0,  -5,
 -5,   0,   0,   0,   0,   0,   0,  -5,
  0,   0,   0,   0,   0,   0,   0,   0}
//A8                                H8
 };

static const int pstRookEg[4][64] = {
//A1                                 H1 Endgame rook table - wing-neutral average of Hakkapeliitta table
{-12, -16, -10, -12, -12, -10, -16, -12,
   0, -19,  -9, -12, -12,  -9, -19,   0,
   1,  -4,  -5, -12, -12,  -5,  -4,   1,
  22,  19,  21,  19,  19,  21,  19,  22,
  17,  17,  23,  13,  13,  23,  17,  17,
  22,  19,  21,  19,  19,  21,  19,  22,
  14,  26,  18,  31,  31,  18,  26,  14,
  13,  13,  18,  28,  28,  18,  13,  13 },
 //A8                                H8

//A1                                 H1 Endgame rook table based on Fruit
{  0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0},
 //A8                                H8

 //A1                                H1 Endgame rook table based on Ippolit
{  0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,
  -2,  -2,  -2   -2,  -2,  -2,  -2,  -2 },
  //A8                                H8

 //A1                                H1 Rook table by Tomasz Michniewski
{  0,   0,   0,   5,   5,   0,   0,   0,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
   0,   0,   0,   0,   0,   0,   0,   0}
 //A8                                H8
 };

static const int pstQueenMg[4][64] = {
//A1                                H1 Midgame queen table - wing-neutral average of Hakkapeliitta table
{ 4,  -8,   3,  11,  11,   3,  -8,   4,
  2,  15,  13,  13,  13,  13,  15,   2,
  5,   5,  10,   2,   2,  10,   5,   5,
 -8,  -1,  -3,  -8,  -8,  -3,  -1,  -8,
 -6, -16, -13,  -9,  -9, -13, -16,  -6,
  2,  -3,  14,   3,   3,  14,  -3,   2,
 23, -13,   3,  -9,  -9,   3, -13,  23,
  5,  30,  11,  -8,  -8,  11,  30,   5},
//A8                                H8

//A1                                H1 Midgame queen table based on Fruit
{-5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0},
//A8                                H8

//A1                                 H1 Midgame queen table based on Ippolit
{-16, -12,  -9,  -7,  -7,  -9, -12, -16,
  -7,  -1,   1,   3,   3,   1,  -1,  -7,
  -4,   1,   5,   6,   6,   5,   1,  -4,
  -2,   3,   6,   9,   9,   6,   3,  -2,
  -2,   3,   6,   9,   9,   6,   3,  -2,
  -4,   1,   5,   6,   6,   5,   1,  -4,
  -7,  -1,   1,   3,   3,   1,  -1,  -7,
 -11,  -7,  -4,  -2,  -2,  -4,  -7, -11 },
 //A8                                H8

 //A1                                H1 Queen table by Tomasz Michniewski
{-20, -10, -10,  -5,  -5, -10, -10, -20,
 -10,   0,   5,   0,   0,   0,   0, -10,
 -10,   5,   5,   5,   5,   5,   0, -10,
   0,   0,   5,   5,   5,   5,   0,  -5,
  -5,   0,   5,   5,   5,   5,   0,  -5,
 -10,   0,   5,   5,   5,   5,   0, -10,
 -10,   0,   0,   0,   0,   0,   0, -10,
 -20, -10, -10,  -5,  -5, -10, -10, -20}
 //A8                                H8
};

static const int pstQueenEg[4][64] = {
//A1                                 H1 Endgame queen table - wing-neutral average of Hakkapeliitta table
{-26, -37, -35, -14, -14, -35, -37, -26,
 -32, -34, -26,  -8,  -8, -26, -34, -32,
 -19,  -6,  -2,   0,   0,  -2,  -6, -19,
  -8,  -5,   6,  29,  29,   6,  -5,  -8,
  -7,  21,  36,  49,  49,  36,  21,  -7,
  15,  36,  37,  47,  47,  37,  36,  15,
  24,  25,  27,  52,  52,  27,  25,  24,
  15,  27,  22,  14,  14,  22,  27,  15},
//A8                                 H8

//A1                                 H1 Endgame queen table based on Fruit
{-24, -16, -12,  -8,  -8, -12, -16, -24,
 -16,  -8,  -4,   0,   0,  -4,  -8, -16,
 -12,  -4,   0,   4,   4,   0,  -4, -12,
  -8,   0,   4,   8,   8,   4,   0,  -8,
  -8,   0,   4,   8,   8,   4,   0,  -8,
 -12,  -4,   0,   4,   4,   0,  -4, -12,
 -16,  -8,  -4,   0,   0,  -4,  -8, -16,
 -24, -16, -12,  -8,  -8, -12, -16, -24 },
//A8                                 H8

//A1                                 H1 Endgame queen table based on Ippolit
{-15, -10,  -8,  -7,  -7,  -8, -10, -15,
 -10,  -5,  -3,  -2,  -2,  -3,  -5, -10,
  -8,  -3,   0,   2,   2,   0,  -3,  -8,
  -7,  -2,   2,   5,   5,   2,  -2,  -7,
  -7,  -2,   2,   5,   5,   2,  -2,  -7,
  -8,  -3,   0,   2,   2,   0,  -3,  -8,
 -10,  -5,  -3,  -2,  -2,  -3,  -5, -10,
 -15, -10,  -8,  -7,  -7,  -8, -10, -15 },
//A8                                 H8

//A1                                 H1 Queen table by Tomasz Michniewski
{-20, -10, -10,  -5,  -5, -10, -10, -20,
 -10,   0,   5,   0,   0,   0,   0, -10,
 -10,   5,   5,   5,   5,   5,   0, -10,
   0,   0,   5,   5,   5,   5,   0,  -5,
  -5,   0,   5,   5,   5,   5,   0,  -5,
 -10,   0,   5,   5,   5,   5,   0, -10,
 -10,   0,   0,   0,   0,   0,   0, -10,
 -20, -10, -10,  -5,  -5, -10, -10, -20 }
//A8                                 H8
};

static const int pstKingMg[4][64] = {
//A1                                 H1 Midgame king table - wing-neutral average of Hakkapeliitta table
{ 13,  31, -14, -33, -33, -14,  31,  13,
  28,  19,  -5, -21, -21,  -5,  19,  28,
 -31,   8,  -8, -30, -30,  -8,   8, -31,
 -56, -15, -11, -14, -14, -11, -15, -56,
 -36,   1,  -3, -28, -28,  -3,   1, -36,
 -32,  28,  13, -39, -39,  13,  28, -32, // strange positive scores, retune manually
 -11,  17,   4, -18, -18,   4,  17, -11,
 -29,   1,  -5, -22, -22,  -5,   1, -29},
//A8                                 H8

//A1                                 H1 Midgame king table based on Fruit
{ 40,  50,  30,   0,   0,  30,  50,  40,
  30,  40,  20,   0,   0,  20,  40,  30,
  10,  20,   0, -20, -20,   0,  20,  10,
   0,  10, -10, -30, -30, -10,  10,   0,
 -10,   0, -20, -40, -40, -20,   0, -10,
 -20, -10, -30, -50, -50, -30, -10, -20,
 -30, -20, -40, -60, -60, -40, -20, -30,
 -40, -30, -50, -70, -70, -50, -30, -40},
//A8                                 H8

//A1                                 H1 Midgame king table based on Ippolit
{ 44,  49,  19,  -1,  -1,  19,  49,  44,
  44,  49,  19,  -1,  -1,  19,  49,  44,
  38,  43,  13,  -7,  -7,  13,  43,  38,
  35,  40,  10, -10, -10,  10,  40,  35,
  30,  35,   5, -15, -15,   5,  35,  30,
  25,  30,   0, -20, -20,   0,  30,  25,
  15,  20, -10, -30, -30, -10,  20,  15,
   5,  10, -20, -40, -40, -20,  10,   5 },
//A8                                 H8

//A1                                 H1 Midgame king table by Tomasz Michniewski
{ 20,  30,  10,   0,   0,  10,  30,  20,
  20,  20,   0,   0,   0,   0,  20,  20,
 -10, -20, -20, -20, -20, -20, -20, -10,
 -20, -30, -30, -40, -40, -30, -30, -20,
 -30, -40, -40, -50, -50, -40, -40, -30,
 -30, -40, -40, -50, -50, -40, -40, -30,
 -30, -40, -40, -50, -50, -40, -40, -30,
 -30, -40, -40, -50, -50, -40, -40, -30 }
//A8                                 H8
};

static const int pstKingEg[4][64] = {
//A1                                 H1 Endgame king table - wing-neutral average of Hakkapeliitta table
{-71, -29, -23, -32, -32, -23, -29, -71,
 -32, -14,  -3,   2,   2,  -3, -14, -32,
 -15,  -2,  11,  23,  23,  11,  -2, -15,
  -2,  20,  35,  40,  40,  35,  20,  -2,
  14,  39,  49,  48,  48,  49,  39,  14,
  26,  61,  76,  57,  57,  76,  61,  26,
  -6,  51,  58,  62,  62,  58,  51,  -6,
 -19,  36,  51,  54,  54,  51,  36, -19},
//A8                                 H8

//A1                                 H1 Endgame king table based on Fruit
{-72, -48, -36, -24, -24, -36, -48, -72,
 -48, -24, -12,   0,   0, -12, -24, -48,
 -36, -12,   0,  12,  12,   0, -12, -36,
 -24,   0,  12,  24,  24,  12,   0, -24,
 -24,   0,  12,  24,  24,  12,   0, -24,
 -36, -12,   0,  12,  12,   0, -12, -36,
 -48, -24, -12,   0,   0, -12, -24, -48,
 -72, -48, -36, -24, -24, -36, -48, -72 },
//A8                                 H8

//A1                                 H1 Endgame king table based on Ippolit
{-73, -50, -34, -28, -28, -34, -50, -73,
 -40, -15,  -3,   3,   3,  -3, -15, -40,
 -29,  -8,   7,  13,  13,   7,  -8, -29,
 -23,  -2,  13,  22,  22,  13,  -2, -23,
 -18,   3,  18,  27,  27,  18,   3, -18,
 -24,  -3,  12,  18,  18,  12,  -3, -24,
 -35, -10,   2,   8,   8,   2, -10, -35,
 -53, -30, -14,  -8,  -8, -14, -30, -53 },
//A8                                 H8

//A1                                 H1 Endgame king table by Tomasz Michniewski
{-50, -30, -30, -30, -30, -30, -30, -50,
 -30, -30,   0,   0,   0,   0, -30, -30,
 -30, -10,  20,  30,  30,  20, -10, -30,
 -30, -10,  30,  40,  40,  30, -10, -30,
 -30, -10,  30,  40,  40,  30, -10, -30,
 -30, -10,  20,  30,  30,  20, -10, -30,
 -30, -20, -10,   0,   0, -10, -20, -30,
 -50, -40, -30, -20, -20, -30, -40, -50}
//A8                                 H8
};

static const int pstKnightOutpost[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   2,   3,   6,   6,   3,   2,   0,
  0,   2,   6,   9,   9,   6,   2,   0,
  0,   3,   9,  12,  12,   9,   3,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0
//A8                                H8
};

static const int pstBishopOutpost[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   1,   2,   4,   4,   2,   1,   0,
  0,   1,   4,   6,   6,   4,   1,   0,
  0,   2,   6,   8,   8,   6,   2,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0
//A8                                H8
};

static const int pstDefendedPawnMg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  1,   1,   1,   1,   1,   1,   1,   1,
  3,   3,   3,   3,   3,   3,   3,   3,
  5,   5,   5,   5,   5,   5,   5,   5,
  8,   8,   8,   8,   8,   8,   8,   8,
 12,  12,  12,  12,  12,  12,  12,  12,
  0,   0,   0,   0,   0,   0,   0,   0
//A8                                H8
};

static const int pstPhalanxPawnMg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  2,   2,   2,   2,   2,   2,   2,   2,
  2,   2,   2,   5,   5,   2,   2,   2,
  6,   6,   8,  13,  13,   8,   6,   6,
 10,  10,  10,  15,  15,  10,  10,  10,
 16,  16,  16,  16,  16,  16,  16,  16,
 24,  24,  24,  24,  24,  24,  24,  24,
  0,   0,   0,   0,   0,   0,   0,   0,
//A8                                H8
};

static const int pstDefendedPawnEg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  1,   1,   1,   1,   1,   1,   1,   1,
  2,   2,   2,   2,   2,   2,   2,   2,
  3,   3,   3,   3,   3,   3,   3,   3,
  4,   4,   4,   4,   4,   4,   4,   4,
  5,   5,   5,   5,   5,   5,   5,   5,
  0,   0,   0,   0,   0,   0,   0,   0
//A8                                H8
};

static const int pstPhalanxPawnEg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  1,   1,   1,   0,   0,   1,   1,   1,
  1,   1,   1,   2,   2,   1,   1,   1,
  1,   2,   2,   3,   3,   2,   2,   1,
  2,   3,   3,   4,   4,   3,   3,   2,
  3,   4,   5,   5,   5,   5,   4,   3,
  5,   6,   6,   6,   6,   6,   6,   5,
  0,   0,   0,   0,   0,   0,   0,   0,
//A8                                H8
};

static const int pst_empty_ks[64] = {
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30,
   0,   0,   0,   0,   0, -10, -20, -30};

static const int pst_empty_qs[64] = {
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0,
 -30, -20, -10,   0,   0,   0,   0,   0};

static const int adj[9] = { -3, -3, -2, -1, 0,  1,  2,  3,  4 };

// map of a table for evaluating material imbalance

static const int imbalance_data[9][9] = {
    /* n=-4  n=-3  n=-2  n=-1  n=0   n=+1  n=+2  n=+3  n=+4 */
    { -A_ALL, -A_ALL, -A_ALL, -A_ALL,  -A_MAJ,      0,      0,      0,      0 }, // R = -4
    { -A_ALL, -A_ALL, -A_ALL, -A_ALL,  -A_MAJ,      0,      0,      0,      0 }, // R = -3
    { -A_ALL, -A_ALL, -A_ALL, -A_ALL,  -A_MAJ,      0,      0,      0,      0 }, // R = -2
    { -A_ALL, -A_ALL, -A_ALL, -A_ALL,  -A_MAJ, -A_EXC,  A_TWO,      0,      0 }, // R = -1
    {-A_MIN , -A_MIN, -A_MIN, -A_MIN,       0,  A_MIN,  A_MIN,  A_MIN,  A_MIN }, // R =  0
    {      0,      0, -A_TWO,  A_EXC,   A_MAJ,  A_ALL,  A_ALL,  A_ALL,  A_ALL }, // R = +1
    {      0,      0,      0,      0,   A_MAJ,  A_ALL,  A_ALL,  A_ALL,  A_ALL }, // R = +2
    {      0,      0,      0,      0,   A_MAJ,  A_ALL,  A_ALL,  A_ALL,  A_ALL }, // R = +3
    {      0,      0,      0,      0,   A_MAJ,  A_ALL,  A_ALL,  A_ALL,  A_ALL }  // R = +4
};

static const int fwd_bonus[16] = {
    0, 2, 5, 8, 12, 15, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
};