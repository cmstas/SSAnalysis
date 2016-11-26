//sample: 1-ttH
//sample: 2-tHW
//sample: 3-tHq
int nPoints_higgs(int sample, int mH){
    if (sample == 1){
        if (mH == 350) return 64066;
        if (mH == 370) return 103385;
        if (mH == 390) return 88928;
        if (mH == 400) return 94306;
        if (mH == 410) return 100096;
        if (mH == 430) return 99855;
        if (mH == 450) return 95305;
        if (mH == 470) return 97538;
        if (mH == 490) return 84286;
        if (mH == 500) return 95823;
        if (mH == 510) return 94902;
        if (mH == 530) return 94425;
        if (mH == 550) return 80673;
        if (mH == 600) return 85565;
        if (mH == 650) return 82394;
        if (mH == 700) return 81373;
        if (mH == 750) return 85727;
        if (mH == 800) return 72691;
        if (mH == 850) return 72784;
        if (mH == 900) return 81490;
    }
    else if (sample == 2){
        if (mH == 350) return 103403;
        if (mH == 370) return 99954;
        if (mH == 390) return 109729;
        if (mH == 410) return 107605;
        if (mH == 430) return 97224;
        if (mH == 450) return 94242;
        if (mH == 470) return 94014;
        if (mH == 490) return 99118;
        if (mH == 510) return 98514;
        if (mH == 530) return 97569;
        if (mH == 550) return 86114;
    }
    else if (sample == 3){
        if (mH == 350) return 184000;
        if (mH == 370) return 222000;
        if (mH == 390) return 237000;
        if (mH == 410) return 2000;
        if (mH == 430) return 216000;
        if (mH == 450) return 198000;
        if (mH == 470) return 170000;
        if (mH == 490) return 162000;
        if (mH == 510) return 153000;
        if (mH == 530) return 143000;
        if (mH == 550) return 163000;
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
        if (mH == 350) return 0.02270;
        if (mH == 370) return 0.02149;
        if (mH == 390) return 0.02034;
        if (mH == 410) return 0.01925;
        if (mH == 430) return 0.01818;
        if (mH == 450) return 0.01718;
        if (mH == 470) return 0.01623;
        if (mH == 490) return 0.01534;
        if (mH == 510) return 0.01450;
        if (mH == 530) return 0.01370;
        if (mH == 550) return 0.01295;
    }

    return -1.0;
}
