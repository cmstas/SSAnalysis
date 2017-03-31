int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt){
    // anal_type_t lep_pt = analysisCategory(id1, id2, lep1pt, lep2pt); 

    if (ht >= 300) {
    // if (ht >= 0) {
        if (nbtags == 2) {
            if (njets == 2) return 1;
            if (njets == 3) return 2;
            if (njets == 4) return 3;
            if (njets == 5) return 4;
            if (njets == 6) return 5;
            if (njets >= 7) return 6;
        } else if (nbtags == 3) {
            if (njets == 2) return 7;
            if (njets == 3) return 8;
            if (njets == 4) return 9;
            if (njets == 5) return 10;
            if (njets == 6) return 11;
            if (njets >= 7) return 12;
        } else if (nbtags >= 4) {
            if (njets == 2) return 13;
            if (njets == 3) return 14;
            if (njets == 4) return 15;
            if (njets == 5) return 16;
            if (njets == 6) return 17;
            if (njets >= 7) return 18;
        }
    }

    return -1;
}
