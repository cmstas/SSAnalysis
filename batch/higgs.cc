//sample: 1-ttH
//sample: 2-tHW
//sample: 3-tHq
int nPoints_higgs(int sample, int mH){
    if (sample == 1){
        if (mH == 350) return 61983;
        if (mH == 400) return 86064;
        if (mH == 450) return 95305;
        if (mH == 500) return 88191;
        if (mH == 550) return 80673;
        if (mH == 600) return 82290;
        if (mH == 650) return 74843;
        if (mH == 700) return 81373;
        if (mH == 750) return 71852;
        if (mH == 800) return 82981;
        if (mH == 850) return 76189;
        if (mH == 900) return 74681;
    }
    else if (sample == 2){
    }
    else if (sample == 3){
    }

    return -99999999; 
}

float xsec_higgs(int sample, int mH){
    if (sample == 1){
        if (mH == 350) return 0.02287;
        if (mH == 370) return 0.01997;
        if (mH == 390) return 0.01720;
        if (mH == 410) return 0.01526;
        if (mH == 430) return 0.01323;
        if (mH == 450) return 0.01214;
        if (mH == 470) return 0.01087;
        if (mH == 490) return 0.00976;
        if (mH == 510) return 0.00904;
        if (mH == 530) return 0.00812;
        if (mH == 550) return 0.00714;
        if (mH == 600) return 0.00578;
        if (mH == 650) return 0.00465;
        if (mH == 700) return 0.00364;
        if (mH == 750) return 0.00299;
        if (mH == 800) return 0.00233;
        if (mH == 850) return 0.00195;
        if (mH == 900) return 0.00160;
    }
    else if (sample == 2){
        if (mH == 350) return 0.02057;
        if (mH == 370) return 0.01844;
        if (mH == 390) return 0.01559;
        if (mH == 410) return 0.01411;
        if (mH == 430) return 0.01240;
        if (mH == 450) return 0.01130;
        if (mH == 470) return 0.00973;
        if (mH == 490) return 0.00889;
        if (mH == 510) return 0.00803;
        if (mH == 530) return 0.00710;
        if (mH == 550) return 0.00644;
    }
    else if (sample == 3){
        if (mH == 350) return 0.01141;
        if (mH == 370) return 0.01060;
        if (mH == 390) return 0.00994;
        if (mH == 410) return 0.00944;
        if (mH == 430) return 0.00916;
        if (mH == 450) return 0.00841;
        if (mH == 470) return 0.00786;
        if (mH == 490) return 0.00751;
        if (mH == 510) return 0.00726;
        if (mH == 530) return 0.00667;
        if (mH == 550) return 0.00627;
    }

    return -1.0;
}
