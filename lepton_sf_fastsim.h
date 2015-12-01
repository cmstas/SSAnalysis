#include "fastSimTriggerEff.h"

float electronScaleFactorFastSim(float pt, float eta, int nvtx) {
  if (pt>=200) pt = 199.9;
  if (fabs(eta)>=2.5) eta = 2.4;
  if (nvtx>=40) nvtx = 39;
  if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=1 && nvtx<9 ) return 0.814157;
  if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=9 && nvtx<13 ) return 0.759017;
  if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=13 && nvtx<17 ) return 0.715849;
  if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=17 && nvtx<40 ) return 0.655429;
  if (pt>=10 && pt<15 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=1 && nvtx<9 ) return 0.761708;
  if (pt>=10 && pt<15 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=9 && nvtx<13 ) return 0.683875;
  if (pt>=10 && pt<15 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=13 && nvtx<17 ) return 0.621181;
  if (pt>=10 && pt<15 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=17 && nvtx<40 ) return 0.413946;
  if (pt>=10 && pt<15 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=1 && nvtx<9 ) return 0.58436;
  if (pt>=10 && pt<15 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=9 && nvtx<13 ) return 0.504844;
  if (pt>=10 && pt<15 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=13 && nvtx<17 ) return 0.463655;
  if (pt>=10 && pt<15 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=17 && nvtx<40 ) return 0.381065;
  if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=1 && nvtx<9 ) return 0.869643;
  if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=9 && nvtx<13 ) return 0.844026;
  if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=13 && nvtx<17 ) return 0.818048;
  if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=17 && nvtx<40 ) return 0.772642;
  if (pt>=15 && pt<20 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=1 && nvtx<9 ) return 0.75549;
  if (pt>=15 && pt<20 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=9 && nvtx<13 ) return 0.764778;
  if (pt>=15 && pt<20 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=13 && nvtx<17 ) return 0.721688;
  if (pt>=15 && pt<20 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=17 && nvtx<40 ) return 0.638371;
  if (pt>=15 && pt<20 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=1 && nvtx<9 ) return 0.699227;
  if (pt>=15 && pt<20 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=9 && nvtx<13 ) return 0.623694;
  if (pt>=15 && pt<20 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=13 && nvtx<17 ) return 0.571129;
  if (pt>=15 && pt<20 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=17 && nvtx<40 ) return 0.520006;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=1 && nvtx<9 ) return 0.923976;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=9 && nvtx<13 ) return 0.903393;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=13 && nvtx<17 ) return 0.885628;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=17 && nvtx<40 ) return 0.848465;
  if (pt>=20 && pt<30 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=1 && nvtx<9 ) return 0.813383;
  if (pt>=20 && pt<30 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=9 && nvtx<13 ) return 0.727716;
  if (pt>=20 && pt<30 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=13 && nvtx<17 ) return 0.730147;
  if (pt>=20 && pt<30 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=17 && nvtx<40 ) return 0.786159;
  if (pt>=20 && pt<30 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=1 && nvtx<9 ) return 0.758499;
  if (pt>=20 && pt<30 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=9 && nvtx<13 ) return 0.720993;
  if (pt>=20 && pt<30 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=13 && nvtx<17 ) return 0.69054;
  if (pt>=20 && pt<30 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=17 && nvtx<40 ) return 0.645377;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=1 && nvtx<9 ) return 0.967405;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=9 && nvtx<13 ) return 0.942314;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=13 && nvtx<17 ) return 0.922092;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=17 && nvtx<40 ) return 0.92061;
  if (pt>=30 && pt<40 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=1 && nvtx<9 ) return 0.835257;
  if (pt>=30 && pt<40 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=9 && nvtx<13 ) return 0.81682;
  if (pt>=30 && pt<40 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=13 && nvtx<17 ) return 0.805407;
  if (pt>=30 && pt<40 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=17 && nvtx<40 ) return 0.785372;
  if (pt>=30 && pt<40 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=1 && nvtx<9 ) return 0.804173;
  if (pt>=30 && pt<40 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=9 && nvtx<13 ) return 0.775563;
  if (pt>=30 && pt<40 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=13 && nvtx<17 ) return 0.756453;
  if (pt>=30 && pt<40 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=17 && nvtx<40 ) return 0.734315;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=1 && nvtx<9 ) return 0.950196;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=9 && nvtx<13 ) return 0.950535;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=13 && nvtx<17 ) return 0.935164;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=17 && nvtx<40 ) return 0.926368;
  if (pt>=40 && pt<50 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=1 && nvtx<9 ) return 0.798578;
  if (pt>=40 && pt<50 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=9 && nvtx<13 ) return 0.795063;
  if (pt>=40 && pt<50 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=13 && nvtx<17 ) return 0.802571;
  if (pt>=40 && pt<50 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=17 && nvtx<40 ) return 0.793923;
  if (pt>=40 && pt<50 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=1 && nvtx<9 ) return 0.825028;
  if (pt>=40 && pt<50 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=9 && nvtx<13 ) return 0.794075;
  if (pt>=40 && pt<50 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=13 && nvtx<17 ) return 0.787551;
  if (pt>=40 && pt<50 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=17 && nvtx<40 ) return 0.765009;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=1 && nvtx<9 ) return 0.954838;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=9 && nvtx<13 ) return 0.951394;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=13 && nvtx<17 ) return 0.944153;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<1.442 && nvtx>=17 && nvtx<40 ) return 0.937648;
  if (pt>=50 && pt<200 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=1 && nvtx<9 ) return 0.898671;
  if (pt>=50 && pt<200 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=9 && nvtx<13 ) return 0.892684;
  if (pt>=50 && pt<200 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=13 && nvtx<17 ) return 0.8817;
  if (pt>=50 && pt<200 && fabs(eta)>=1.442 && fabs(eta)<1.556 && nvtx>=17 && nvtx<40 ) return 0.879368;
  if (pt>=50 && pt<200 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=1 && nvtx<9 ) return 0.834136;
  if (pt>=50 && pt<200 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=9 && nvtx<13 ) return 0.825359;
  if (pt>=50 && pt<200 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=13 && nvtx<17 ) return 0.821869;
  if (pt>=50 && pt<200 && fabs(eta)>=1.556 && fabs(eta)<2.5 && nvtx>=17 && nvtx<40 ) return 0.790352;
   return 0.;
}

float muonScaleFactorFastSim(float pt, float eta, int nvtx) {
  if (pt>=200) pt = 199.9;
  if (fabs(eta)>=2.4) eta = 2.3;
  if (nvtx>=40) nvtx = 39;
  if (pt>=10 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=1 && nvtx<9 ) return 0.913389;
  if (pt>=10 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=9 && nvtx<13 ) return 0.901064;
  if (pt>=10 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=13 && nvtx<17 ) return 0.865614;
  if (pt>=10 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=17 && nvtx<40 ) return 0.827573;
  if (pt>=10 && pt<20 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=1 && nvtx<9 ) return 0.951636;
  if (pt>=10 && pt<20 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=9 && nvtx<13 ) return 0.867165;
  if (pt>=10 && pt<20 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=13 && nvtx<17 ) return 0.80751;
  if (pt>=10 && pt<20 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=17 && nvtx<40 ) return 0.707575;
  if (pt>=10 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=1 && nvtx<9 ) return 0.887979;
  if (pt>=10 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=9 && nvtx<13 ) return 0.847121;
  if (pt>=10 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=13 && nvtx<17 ) return 0.804171;
  if (pt>=10 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=17 && nvtx<40 ) return 0.749252;
  if (pt>=10 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=1 && nvtx<9 ) return 0.829673;
  if (pt>=10 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=9 && nvtx<13 ) return 0.82641;
  if (pt>=10 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=13 && nvtx<17 ) return 0.80143;
  if (pt>=10 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=17 && nvtx<40 ) return 0.719041;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=1 && nvtx<9 ) return 0.968791;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=9 && nvtx<13 ) return 0.954761;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=13 && nvtx<17 ) return 0.943817;
  if (pt>=20 && pt<30 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=17 && nvtx<40 ) return 0.940566;
  if (pt>=20 && pt<30 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=1 && nvtx<9 ) return 0.974052;
  if (pt>=20 && pt<30 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=9 && nvtx<13 ) return 0.964918;
  if (pt>=20 && pt<30 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=13 && nvtx<17 ) return 0.936154;
  if (pt>=20 && pt<30 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=17 && nvtx<40 ) return 0.879131;
  if (pt>=20 && pt<30 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=1 && nvtx<9 ) return 0.968941;
  if (pt>=20 && pt<30 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=9 && nvtx<13 ) return 0.943112;
  if (pt>=20 && pt<30 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=13 && nvtx<17 ) return 0.917567;
  if (pt>=20 && pt<30 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=17 && nvtx<40 ) return 0.882963;
  if (pt>=20 && pt<30 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=1 && nvtx<9 ) return 0.935514;
  if (pt>=20 && pt<30 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=9 && nvtx<13 ) return 0.926603;
  if (pt>=20 && pt<30 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=13 && nvtx<17 ) return 0.903745;
  if (pt>=20 && pt<30 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=17 && nvtx<40 ) return 0.895221;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=1 && nvtx<9 ) return 0.983126;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=9 && nvtx<13 ) return 0.977535;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=13 && nvtx<17 ) return 0.980241;
  if (pt>=30 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=17 && nvtx<40 ) return 0.962977;
  if (pt>=30 && pt<40 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=1 && nvtx<9 ) return 0.998128;
  if (pt>=30 && pt<40 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=9 && nvtx<13 ) return 0.995775;
  if (pt>=30 && pt<40 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=13 && nvtx<17 ) return 0.964481;
  if (pt>=30 && pt<40 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=17 && nvtx<40 ) return 0.945076;
  if (pt>=30 && pt<40 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=1 && nvtx<9 ) return 0.985666;
  if (pt>=30 && pt<40 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=9 && nvtx<13 ) return 0.972494;
  if (pt>=30 && pt<40 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=13 && nvtx<17 ) return 0.951573;
  if (pt>=30 && pt<40 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=17 && nvtx<40 ) return 0.943056;
  if (pt>=30 && pt<40 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=1 && nvtx<9 ) return 0.959758;
  if (pt>=30 && pt<40 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=9 && nvtx<13 ) return 0.967516;
  if (pt>=30 && pt<40 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=13 && nvtx<17 ) return 0.96017;
  if (pt>=30 && pt<40 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=17 && nvtx<40 ) return 0.94342;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=1 && nvtx<9 ) return 0.990967;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=9 && nvtx<13 ) return 0.988991;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=13 && nvtx<17 ) return 0.990408;
  if (pt>=40 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=17 && nvtx<40 ) return 0.984822;
  if (pt>=40 && pt<50 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=1 && nvtx<9 ) return 1.00183;
  if (pt>=40 && pt<50 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=9 && nvtx<13 ) return 0.99902;
  if (pt>=40 && pt<50 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=13 && nvtx<17 ) return 1.00035;
  if (pt>=40 && pt<50 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=17 && nvtx<40 ) return 0.975027;
  if (pt>=40 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=1 && nvtx<9 ) return 0.986791;
  if (pt>=40 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=9 && nvtx<13 ) return 0.979077;
  if (pt>=40 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=13 && nvtx<17 ) return 0.967807;
  if (pt>=40 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=17 && nvtx<40 ) return 0.950044;
  if (pt>=40 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=1 && nvtx<9 ) return 0.974784;
  if (pt>=40 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=9 && nvtx<13 ) return 0.975999;
  if (pt>=40 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=13 && nvtx<17 ) return 0.959763;
  if (pt>=40 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=17 && nvtx<40 ) return 0.960877;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=1 && nvtx<9 ) return 0.994381;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=9 && nvtx<13 ) return 0.994095;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=13 && nvtx<17 ) return 0.993668;
  if (pt>=50 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.9 && nvtx>=17 && nvtx<40 ) return 0.990862;
  if (pt>=50 && pt<200 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=1 && nvtx<9 ) return 1.01037;
  if (pt>=50 && pt<200 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=9 && nvtx<13 ) return 1.00339;
  if (pt>=50 && pt<200 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=13 && nvtx<17 ) return 0.996202;
  if (pt>=50 && pt<200 && fabs(eta)>=0.9 && fabs(eta)<1.2 && nvtx>=17 && nvtx<40 ) return 0.98092;
  if (pt>=50 && pt<200 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=1 && nvtx<9 ) return 0.986247;
  if (pt>=50 && pt<200 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=9 && nvtx<13 ) return 0.986691;
  if (pt>=50 && pt<200 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=13 && nvtx<17 ) return 0.978849;
  if (pt>=50 && pt<200 && fabs(eta)>=1.2 && fabs(eta)<2.1 && nvtx>=17 && nvtx<40 ) return 0.961887;
  if (pt>=50 && pt<200 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=1 && nvtx<9 ) return 0.978256;
  if (pt>=50 && pt<200 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=9 && nvtx<13 ) return 0.989626;
  if (pt>=50 && pt<200 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=13 && nvtx<17 ) return 0.97986;
  if (pt>=50 && pt<200 && fabs(eta)>=2.1 && fabs(eta)<2.4 && nvtx>=17 && nvtx<40 ) return 0.971385;
   return 0.;
}

float leptonScaleFactorFastSim(int pdgId, float pt, float eta, int nvtx) {
  if (abs(pdgId)==13) return muonScaleFactorFastSim(pt, eta, nvtx);
  else return electronScaleFactorFastSim(pt, eta, nvtx);
  return 0.;
}

float eventScaleFactorFastSim(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht, int nvtx) {
  return FastSimTriggerEfficiency(ht, pt1, pdgId1, pt2, pdgId2) * leptonScaleFactorFastSim(pdgId1, pt1, eta1, nvtx) * leptonScaleFactorFastSim(pdgId2, pt2, eta2, nvtx);
}
