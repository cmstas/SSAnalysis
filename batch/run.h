#ifndef run_h
#define run_h

enum sample_t { TTBAR, TTW, TTZ, WZ, T1TTTT_1500, T1TTTT_1200 };

void run(sample_t which, int file, bool ptrel);

#endif
