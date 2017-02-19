/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2017 Pawel Koziol

Rodent is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Rodent is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#include "rodent.h"
#include "eval.h"
#include <stdio.h>

void cEngine::ClearAll(void) {

  ClearPawnHash();
  ClearEvalHash();
  ClearHist();
}

void cEngine::ClearEvalHash(void) {

  for (int e = 0; e < EVAL_HASH_SIZE; e++) {
    EvalTT[e].key = 0;
    EvalTT[e].score = 0;
  }
}

void cEngine::EvaluateMaterial(POS * p, eData *e, int sd) {

  int op = Opp(sd);

  int tmp = Par.np_table[p->cnt[sd][P]] * p->cnt[sd][N]   // knights lose value as pawns disappear
          - Par.rp_table[p->cnt[sd][P]] * p->cnt[sd][R];  // rooks gain value as pawns disappear

  if (p->cnt[sd][N] > 1) tmp -= 10;                       // knight pair
  if (p->cnt[sd][R] > 1) tmp -=  5;                       // rook pair
  if (p->cnt[sd][B] > 1) 
	 Add(e, sd, Par.bish_pair, Par.bish_pair + 10);       // bishop pair
    
  // "elephantiasis correction" for queen, idea by H.G.Mueller (nb. rookVsQueen doesn't help)

  if (p->cnt[sd][Q])
    tmp -= 5 * (p->cnt[op][N] + p->cnt[op][B]);

  Add(e, sd, tmp);
}

void cEngine::EvaluatePieces(POS *p, eData *e, int sd) {

  U64 bb_pieces, bb_attack, bb_control, bb_contact, bb_zone, bb_file;
  int sq, cnt, tmp, mul, own_p_cnt, opp_p_cnt;
  int r_on_7th = 0;
  int mob_mg = 0;
  int mob_eg = 0;
  int tropism_mg = 0;
  int tropism_eg = 0;
  int lines_mg = 0;
  int lines_eg = 0;
  int fwd_weight = 0;
  int fwd_cnt = 0;
  int outpost = 0;
  int att = 0;
  int wood = 0;

  // Init king attack zone

  int op = Opp(sd);
  int ksq = KingSq(p, op);
  bb_zone = BB.KingAttacks(ksq);
  bb_zone = bb_zone |= BB.ShiftFwd(bb_zone, op);
  bb_zone = bb_zone &~e->p_takes[op];

  // Init helper bitboards

  U64 n_checks = BB.KnightAttacks(ksq) & ~p->cl_bb[sd] & ~e->p_takes[op];
  U64 b_checks = BB.BishAttacks(OccBb(p), ksq) & ~p->cl_bb[sd] & ~e->p_takes[op];
  U64 r_checks = BB.RookAttacks(OccBb(p), ksq) & ~p->cl_bb[sd] & ~e->p_takes[op];
  U64 q_checks = r_checks & b_checks;
  U64 bb_excluded = p->Pawns(sd);

  // Knight eval

  bb_pieces = p->Knights(sd);
  while (bb_pieces) {
    sq = BB.PopFirstBit(&bb_pieces);                    // get square

    tropism_mg += 3 * Par.dist[sq][ksq];                // king tropism (based on Gambit Fruit)
    tropism_eg += 3 * Par.dist[sq][ksq];

    if (SqBb(sq) & Mask.away[sd]) {                     // forwardness (based on Toga II 3.0)
      fwd_weight += 1;
      fwd_cnt += 1;
    }

    bb_control = BB.KnightAttacks(sq) & ~p->cl_bb[sd];  // get control bitboard
    if (!(bb_control  &~e->p_takes[op] & Mask.away[sd]))// we do not attack enemy half of the board
      Add(e, sd, -5);
    e->all_att[sd] |= BB.KnightAttacks(sq);
    e->ev_att[sd]  |= bb_control;
    if (bb_control & n_checks) att += 4;                // check threats

	bb_attack = BB.KnightAttacks(sd);
	if (bb_attack & bb_zone) {                          // king attack
	  wood++;
      att += 6 * BB.PopCnt(bb_attack & bb_zone);
	}

    cnt = BB.PopCnt(bb_control &~e->p_takes[op]);       // get mobility count
	mob_mg += Par.n_mob_mg[cnt];
	mob_eg += Par.n_mob_eg[cnt];

    ScoreOutpost(p, e, sd, N, sq, &outpost);            // outpost
  }

  // Bishop eval

  bb_pieces = p->Bishops(sd);
  while (bb_pieces) {
    sq = BB.PopFirstBit(&bb_pieces);                    // get square

    tropism_mg += 2 * Par.dist[sq][ksq];                // king tropism (based on Gambit Fruit)
    tropism_eg += 1 * Par.dist[sq][ksq];

    if (SqBb(sq) & Mask.away[sd]) {                     // forwardness (based on Toga II 3.0)
      fwd_weight += 1;
      fwd_cnt += 1;
    }

    bb_control = BB.BishAttacks(OccBb(p), sq);          // get control bitboard
    e->all_att[sd] |= bb_control;                       // update attack map
    e->ev_att[sd]  |= bb_control;
    if (!(bb_control & Mask.away[sd] )) Add(e, sd, -5); // we do not attack enemy half of the board
    if (bb_control & b_checks) att += 4;                // check threats
    bb_attack = BB.BishAttacks(OccBb(p) ^ p->Queens(sd), sq);  // get king attack bitboard

	if (bb_attack & bb_zone) {                          // evaluate king attacks
      wood++;
      att += 6 * BB.PopCnt(bb_attack & bb_zone);         
	}

    cnt = BB.PopCnt(bb_control &~e->p_takes[op] & ~bb_excluded);  // get mobility count
	mob_mg += Par.b_mob_mg[cnt];
	mob_eg += Par.b_mob_eg[cnt];

    ScoreOutpost(p, e, sd, B, sq, &outpost);           // outpost

    // Bishops side by side

    if (ShiftNorth(SqBb(sq)) & p->Bishops(sd))
      Add(e, sd, 4);
    if (ShiftEast(SqBb(sq)) & p->Bishops(sd))
      Add(e, sd, 4);

    // Pawns on the same square color as our bishop

    if (bbWhiteSq & SqBb(sq)) {
      own_p_cnt = BB.PopCnt(bbWhiteSq & p->Pawns(sd)) - 4;
      opp_p_cnt = BB.PopCnt(bbWhiteSq & p->Pawns(op)) - 4;
    } else {
      own_p_cnt = BB.PopCnt(bbBlackSq & p->Pawns(sd)) - 4;
      opp_p_cnt = BB.PopCnt(bbBlackSq & p->Pawns(op)) - 4;
    }

    Add(e, sd, -3 * own_p_cnt - opp_p_cnt);
  }

  // Rook eval

  bb_pieces = p->Rooks(sd);
  while (bb_pieces) {
    sq = BB.PopFirstBit(&bb_pieces);                    // get square

    tropism_mg += 2 * Par.dist[sq][ksq];                // king tropism (based on Gambit Fruit)
    tropism_eg += 1 * Par.dist[sq][ksq];

    if (SqBb(sq) & Mask.away[sd]) {                     // forwardness (based on Toga II 3.0)
      fwd_weight += 2;
      fwd_cnt += 1;
    }

    bb_control = BB.RookAttacks(OccBb(p), sq);          // get control bitboard
    e->all_att[sd] |= bb_control;                       // update attack map
    e->ev_att[sd] |= bb_control;

    if ((bb_control & ~p->cl_bb[sd] & r_checks)
    && p->Queens(sd)) {
      att += 9;                                         // check threat bonus
      bb_contact = (bb_control & BB.KingAttacks(ksq)) & r_checks;  // get contact check bitboard

      while (bb_contact) {
        int contactSq = BB.PopFirstBit(&bb_contact);    // find a potential contact check
        if (Swap(p, sq, contactSq) >= 0) {              // rook exchanges are also accepted
          att += 24;
          break;
        }
      }
    }

    bb_attack = BB.RookAttacks(OccBb(p) ^ p->StraightMovers(sd), sq);  // get king attack bitboard

	if (bb_attack & bb_zone) {                          // evaluate king attacks
      wood++;
      att += 9 * BB.PopCnt(bb_attack & bb_zone);
	}

    cnt = BB.PopCnt(bb_control & ~bb_excluded);         // get mobility count
	mob_mg += Par.r_mob_mg[cnt];
	mob_eg += Par.r_mob_eg[cnt];
                                                        // FILE EVALUATION:

    bb_file = BB.FillNorth(SqBb(sq)) | BB.FillSouth(SqBb(sq));   // get file
	if (bb_file & p->Queens(op)) {                      // enemy queen on rook's file
		lines_mg += 5;
		lines_eg += 5;
	}
    if (!(bb_file & p->Pawns(sd))) {                    // no own pawns on that file
		if (!(bb_file & p->Pawns(op))) {
			lines_mg += 14;
			lines_eg += 14;
		}
      else {                                            // half-open file...
		if (bb_file & (p->Pawns(op) & e->p_takes[op])) {// ...with defended enemy pawn
			lines_mg += 5;
			lines_eg += 5;
		} else {                                        // ...with undefended enemy pawn
			lines_mg += 7;
			lines_eg += 7;
		}
      }
  }

    // Rook on the 7th rank attacking pawns or cutting off enemy king

    if (SqBb(sq) & bbRelRank[sd][RANK_7]) {             // rook on 7th rank
      if (p->Pawns(op) & bbRelRank[sd][RANK_7]          // attacking enemy pawns
      || p->Kings(op) & bbRelRank[sd][RANK_8]) {        // or cutting off enemy king
		 lines_mg += 16;
		 lines_eg += 32;
         r_on_7th++;
      }
    }
  }

  // Queen eval

  bb_pieces = p->Queens(sd);
  while (bb_pieces) {
    sq = BB.PopFirstBit(&bb_pieces);                    // get square

    tropism_mg += 2 * Par.dist[sq][ksq];                // king tropism (based on Gambit Fruit)
    tropism_eg += 4 * Par.dist[sq][ksq];                

    if (SqBb(sq) & Mask.away[sd]) {                     // forwardness (based on Toga II 3.0)
      fwd_weight += 4;
      fwd_cnt += 1;
    }

    bb_control = BB.QueenAttacks(OccBb(p), sq);         // get control bitboard
    e->all_att[sd] |= bb_control;                       // update attack map
    if (bb_control & q_checks) {                        // check threat bonus
      att += 12;

      bb_contact = bb_control & BB.KingAttacks(ksq);    // queen contact checks
      while (bb_contact) {
        int contactSq = BB.PopFirstBit(&bb_contact);    // find potential contact check square 
        if (Swap(p, sq, contactSq) >= 0) {              // if check doesn't lose material, evaluate
          att += 36;
          break;
        }
      }
    }

    bb_attack  = BB.BishAttacks(OccBb(p) ^ p->DiagMovers(sd), sq);
    bb_attack |= BB.RookAttacks(OccBb(p) ^ p->StraightMovers(sd), sq);

	if (bb_attack & bb_zone) {                          // evaluate king attacks
      wood++;
      att += 15 * BB.PopCnt(bb_attack & bb_zone);
	}

    cnt = BB.PopCnt(bb_control & ~bb_excluded);         // get mobility count
	mob_mg += Par.q_mob_mg[cnt];
	mob_eg += Par.q_mob_eg[cnt];

    if (SqBb(sq) & bbRelRank[sd][RANK_7]) {             // queen on 7th rank
      if (p->Pawns(op) & bbRelRank[sd][RANK_7]          // attacking enemy pawns
      ||  p->Kings(op) & bbRelRank[sd][RANK_8]) {       // or cutting off enemy king
		 lines_mg += 4;
		 lines_eg += 8;
      }
    }
  } // end of queen eval

  // Composite factors

  if (r_on_7th > 1) {  // two rooks on 7th rank
	  lines_mg += 8;
	  lines_eg += 16;
  }

  Add(e, sd, (Par.sd_mob[sd] * mob_mg)  / 100, (Par.sd_mob[sd] * mob_eg)  / 100);
  Add(e, sd, (Par.tropism * tropism_mg) / 100, (Par.tropism * tropism_eg) / 100);
  Add(e, sd, (Par.lines * lines_mg)     / 100, (Par.lines * lines_eg)     / 100);
  Add(e, sd, (Par.forwardness * fwd_bonus[fwd_cnt] * fwd_weight) / 100, 0);
  Add(e, sd, (Par.outposts * outpost) / 100);

  // King attack eval

  if (wood > 1) {
    if (att > 399) att = 399;
    if (p->cnt[sd][Q] == 0) att = 0;
    Add(e, sd, (Par.danger[att] * Par.sd_att[sd]) / 100);
  }

}

void cEngine::ScoreOutpost(POS *p, eData *e, int sd, int pc, int sq, int *outpost) {

  if (SqBb(sq) & Mask.home[sd]) {
    U64 stop = BB.ShiftFwd(SqBb(sq), sd);             // get square in front of a minor
    if (stop & p->Pawns(sd))                          // is it occupied by own pawn?
    *outpost += 5;                                    // bonus for a pawn shielding a minor
  }

  int tmp = Par.sp_pst[sd][pc][sq];                   // get base outpost bonus
  if (tmp) {
    int mul = 0;                                      // reset outpost multiplier
    if (SqBb(sq) & ~e->p_can_take[Opp(sd)]) mul += 2; // is piece in hole of enemy pawn structure?
    if (SqBb(sq) & e->p_takes[sd]) mul += 1;          // is piece defended by own pawn?
    if (SqBb(sq) & e->two_pawns_take[sd]) mul += 1;   // is piece defended by two pawns?
    *outpost += (tmp * mul) / 2;                      // add outpost bonus
  }
}

void cEngine::EvaluatePawns(POS *p, eData *e, int sd) {

  U64 bb_pieces, front_span, fl_phalanx;
  int sq, fl_unopposed;
  int op = Opp(sd);

  bb_pieces = p->Pawns(sd);
  while (bb_pieces) {

    // set data and flags

    sq = BB.PopFirstBit(&bb_pieces);
    front_span = BB.GetFrontSpan(SqBb(sq), sd);
    fl_unopposed = ((front_span & p->Pawns(op)) == 0);
    fl_phalanx = (BB.ShiftSideways(SqBb(sq)) & p->Pawns(sd));

    // candidate passers

    if (fl_unopposed) {
      if (fl_phalanx) {
      if (BB.PopCnt((Mask.passed[sd][sq] & p->Pawns(op))) == 1)
        AddPawns(e, sd, passed_bonus_mg[sd][Rank(sq)] / 3, passed_bonus_eg[sd][Rank(sq)] / 3);
      }
    }

    // doubled pawn

    if (front_span & p->Pawns(sd))
      AddPawns(e, sd, Par.doubled_mg, Par.doubled_eg);

    // supported pawn

    if (fl_phalanx)                     AddPawns(e, sd, Par.sp_pst[sd][PHA_MG][sq], Par.sp_pst[sd][PHA_EG][sq]); // scores twice !!!
    else if (SqBb(sq) & e->p_takes[sd]) AddPawns(e, sd, Par.sp_pst[sd][DEF_MG][sq], Par.sp_pst[sd][DEF_EG][sq]);
	
    // isolated and weak pawn

    if (!(Mask.adjacent[File(sq)] & p->Pawns(sd)))
      AddPawns(e, sd, Par.isolated_mg + Par.isolated_open * fl_unopposed, Par.isolated_eg);
    else if (!(Mask.supported[sd][sq] & p->Pawns(sd)))
      AddPawns(e, sd, Par.backward_malus_mg[File(sq)] + Par.backward_open * fl_unopposed, Par.backward_eg);
  }
}

void cEngine::EvaluatePassers(POS *p, eData *e, int sd) {

  U64 bb_pieces, stop, mul;
  int sq, mg_tmp, eg_tmp, fl_unopposed;
  int op = Opp(sd);
  int mg_tot = 0;
  int eg_tot = 0;

  bb_pieces = p->Pawns(sd);
  while (bb_pieces) {
    sq = BB.PopFirstBit(&bb_pieces);

    // passed pawns

    if (!(Mask.passed[sd][sq] & p->Pawns(op))) {
      mul = 100;
      stop = BB.ShiftFwd(SqBb(sq), sd);

      if (stop & OccBb(p)) mul -= 20;   // blocked passers score less

      else if ((stop & e->all_att[sd])  // our control of stop square
           && (stop & ~e->all_att[op])) mul += 10;

      mg_tmp = passed_bonus_mg[sd][Rank(sq)];
      eg_tmp = passed_bonus_eg[sd][Rank(sq)] 
             -((passed_bonus_eg[sd][Rank(sq)] * Par.dist[sq][p->king_sq[op]]) / 30);
	  mg_tot += (mg_tmp * mul) / 100;
	  eg_tot += (eg_tmp * mul) / 100;
    }
  }

  Add(e, sd, (mg_tot * Par.passers) / 100, (eg_tot * Par.passers) / 100);
}

void cEngine::ScoreUnstoppable(eData *e, POS * p) {

  U64 bb_pieces, bb_span;
  int w_dist = 8;
  int b_dist = 8;
  int sq, ksq, psq, tempo, prom_dist;

  // White unstoppable passers

  if (p->cnt[BC][N] + p->cnt[BC][B] + p->cnt[BC][R] + p->cnt[BC][Q] == 0) {
    ksq = KingSq(p, BC);
    if (p->side == BC) tempo = 1; else tempo = 0;
    bb_pieces = p->Pawns(WC);
    while (bb_pieces) {
      sq = BB.PopFirstBit(&bb_pieces);
      if (!(Mask.passed[WC][sq] & p->Pawns(BC))) {
        bb_span = BB.GetFrontSpan(SqBb(sq), WC);
        psq = ((WC - 1) & 56) + (sq & 7);
        prom_dist = Min(5, Par.chebyshev_dist[sq][psq]);

        if (prom_dist < (Par.chebyshev_dist[ksq][psq] - tempo)) {
          if (bb_span & p->Kings(WC)) prom_dist++;
          w_dist = Min(w_dist, prom_dist);
        }
      }
    }
  }

  // Black unstoppable passers

  if (p->cnt[WC][N] + p->cnt[WC][B] + p->cnt[WC][R] + p->cnt[WC][Q] == 0) {
    ksq = KingSq(p, WC);
    if (p->side == WC) tempo = 1; else tempo = 0;
    bb_pieces = p->Pawns(BC);
    while (bb_pieces) {
      sq = BB.PopFirstBit(&bb_pieces);
      if (!(Mask.passed[BC][sq] & p->Pawns(WC))) {
        bb_span = BB.GetFrontSpan(SqBb(sq), BC);
        psq = ((BC - 1) & 56) + (sq & 7);
        prom_dist = Min(5, Par.chebyshev_dist[sq][psq]);

        if (prom_dist < (Par.chebyshev_dist[ksq][psq] - tempo)) {
          if (bb_span & p->Kings(BC)) prom_dist++;
          b_dist = Min(b_dist, prom_dist);
        }
      }
    }
  }

  if (w_dist < b_dist - 1) Add(e, WC, 0, 500);
  if (b_dist < w_dist - 1) Add(e, BC, 0, 500);
}


void cEngine::Add(eData *e, int sd, int mg_val, int eg_val) {

  e->mg[sd] += mg_val;
  e->eg[sd] += eg_val;
}

void cEngine::Add(eData *e, int sd, int val) {

  e->mg[sd] += val;
  e->eg[sd] += val;
}

void cEngine::AddPawns(eData *e, int sd, int mg_val, int eg_val) {

  e->mg_pawns[sd] += mg_val;
  e->eg_pawns[sd] += eg_val;
}

int cEngine::Interpolate(POS * p, eData * e) {

  int mg_tot = e->mg[WC] - e->mg[BC];
  int eg_tot = e->eg[WC] - e->eg[BC];
  int mg_phase = Min(p->phase, 24);
  int eg_phase = 24 - mg_phase;

   return (mg_tot * mg_phase + eg_tot * eg_phase) / 24;
}

void cEngine::ScoreThreats(POS *p, eData *e, int sd) {

  int pc, sq, sc;
  int mg = 0;
  int eg = 0;
  int op = Opp(sd);
  U64 bb_hanging = p->cl_bb[op]    & ~e->p_takes[op];
  U64 bb_threatened = p->cl_bb[op] & e->p_takes[sd];
  bb_hanging |= bb_threatened;     // piece attacked by our pawn isn't well defended
  bb_hanging &= e->all_att[sd];    // hanging piece has to be attacked
  bb_hanging &= ~p->Pawns(op);     // currently we don't evaluate threats against pawns

  U64 bb_defended = p->cl_bb[op] & e->all_att[op];
  bb_defended &= e->ev_att[sd];    // N, B, R attacks (pieces attacked by pawns are scored as hanging)
  bb_defended &= ~e->p_takes[sd];  // no defense against pawn attack
  bb_defended &= ~p->Pawns(op);    // currently we don't evaluate threats against pawns

  // hanging pieces (attacked and undefended, based on DiscoCheck)

  while (bb_hanging) {
    sq = BB.PopFirstBit(&bb_hanging);
    pc = TpOnSq(p, sq);
    sc = tp_value[pc] / 64;
	mg += 10 + sc;
	eg += 18 + sc;
  }

  // defended pieces under attack

  while (bb_defended) {
    sq = BB.PopFirstBit(&bb_defended);
    pc = TpOnSq(p, sq);
    sc = tp_value[pc] / 96;
	mg += 5 + sc;
	eg += 9 + sc;
  }

  Add(e, sd, (Par.threats * mg) / 100, (Par.threats * eg) / 100);
}

int cEngine::Evaluate(POS *p, eData *e) {

  // try retrieving score from per-thread eval hashtable

  int addr = p->hash_key % EVAL_HASH_SIZE;

  if (EvalTT[addr].key == p->hash_key) {
    int sc = EvalTT[addr].score;
    return p->side == WC ? sc : -sc;
  }

  // clear eval data  

  e->mg[WC] = p->mg_sc[WC];
  e->mg[BC] = p->mg_sc[BC];
  e->eg[WC] = p->eg_sc[WC];
  e->eg[BC] = p->eg_sc[BC];

  // init helper bitboards

  e->p_takes[WC] = BB.GetWPControl(p->Pawns(WC));
  e->p_takes[BC] = BB.GetBPControl(p->Pawns(BC));
  e->p_can_take[WC] = BB.FillNorth(e->p_takes[WC]);
  e->p_can_take[BC] = BB.FillSouth(e->p_takes[BC]);
  e->two_pawns_take[WC] = BB.GetDoubleWPControl(p->Pawns(WC));
  e->two_pawns_take[BC] = BB.GetDoubleBPControl(p->Pawns(BC));
  e->all_att[WC] = e->p_takes[WC] | BB.KingAttacks(KingSq(p, WC));
  e->all_att[BC] = e->p_takes[BC] | BB.KingAttacks(KingSq(p, BC));
  e->ev_att[WC] = 0ULL;
  e->ev_att[BC] = 0ULL;

  // run all the evaluation subroutines

  EvaluateMaterial(p, e, WC);
  EvaluateMaterial(p, e, BC);
  EvaluatePieces(p, e, WC);
  EvaluatePieces(p, e, BC);
  ScorePawnStruct(p, e);
  EvaluatePassers(p, e, WC);
  EvaluatePassers(p, e, BC);
  EvaluatePatterns(p, e);
  ScoreUnstoppable(e, p);
  ScoreThreats(p, e, WC);
  ScoreThreats(p, e, BC);
  Add(e, p->side, 14, 7); // tempo bonus

  e->mg[WC] += e->mg_pawns[WC];
  e->mg[BC] += e->mg_pawns[BC];
  e->eg[WC] += e->eg_pawns[WC];
  e->eg[BC] += e->eg_pawns[BC];

  // Add asymmetric bonus for keeping certain type of pieces

  e->mg[Par.prog_side] += Par.keep_pc[Q] * p->cnt[Par.prog_side][Q];
  e->mg[Par.prog_side] += Par.keep_pc[R] * p->cnt[Par.prog_side][R];
  e->mg[Par.prog_side] += Par.keep_pc[B] * p->cnt[Par.prog_side][B];
  e->mg[Par.prog_side] += Par.keep_pc[N] * p->cnt[Par.prog_side][N];
  e->mg[Par.prog_side] += Par.keep_pc[P] * p->cnt[Par.prog_side][P];
  
  // Interpolate between midgame and endgame scores

  int score = Interpolate(p, e);

  // Material imbalance evaluation (based on Crafty)

  int minorBalance = p->cnt[WC][N] - p->cnt[BC][N] + p->cnt[WC][B] - p->cnt[BC][B];
  int majorBalance = p->cnt[WC][R] - p->cnt[BC][R] + 2 * p->cnt[WC][Q] - 2 * p->cnt[BC][Q];

  int x = Max(majorBalance + 4, 0);
  if (x > 8) x = 8;

  int y = Max(minorBalance + 4, 0);
  if (y > 8) y = 8;

  score += Par.imbalance[x][y];

  // Weakening: add pseudo-random value to eval score

  if (Par.eval_blur) {
    int rand_mod = (Par.eval_blur / 2) - (p->hash_key % Par.eval_blur);
    score += rand_mod;
  }

  // Special case code for KBN vs K checkmate

  score += CheckmateHelper(p);

  // Decrease score for drawish endgames

  int draw_factor = 64;
  if (score > 0) draw_factor = GetDrawFactor(p, WC);
  if (score < 0) draw_factor = GetDrawFactor(p, BC);
  score = (score * draw_factor) / 64;

  // Ensure that returned value doesn't exceed mate score

  score = Clip(score, MAX_EVAL);

  // Save eval score in the evaluation hash table

  EvalTT[addr].key = p->hash_key;
  EvalTT[addr].score = score;

  // Return score relative to the side to move

  return p->side == WC ? score : -score;
}