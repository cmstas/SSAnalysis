#ifndef _commonUtils_h_
#define _commonUtils_h_

float getLumi() { return 0.2095; }

TString getTag() { return "v4.00"; }

float getPUw(int nvtx) {
  if (nvtx>=0.000000 && nvtx<2.000000) return 14.014330;
  if (nvtx>=2.000000 && nvtx<4.000000) return 3.388680;
  if (nvtx>=4.000000 && nvtx<6.000000) return 3.362378;
  if (nvtx>=6.000000 && nvtx<8.000000) return 2.350122;
  if (nvtx>=8.000000 && nvtx<10.000000) return 1.836804;
  if (nvtx>=10.000000 && nvtx<12.000000) return 1.217169;
  if (nvtx>=12.000000 && nvtx<14.000000) return 0.817244;
  if (nvtx>=14.000000 && nvtx<16.000000) return 0.408258;
  if (nvtx>=16.000000 && nvtx<18.000000) return 0.196930;
  if (nvtx>=18.000000 && nvtx<20.000000) return 0.138041;
  if (nvtx>=20.000000 && nvtx<22.000000) return 0.028379;
  if (nvtx>=22.000000 && nvtx<24.000000) return 0.081731;
  if (nvtx>=24.000000 && nvtx<26.000000) return 0.000000;
  if (nvtx>=26.000000 && nvtx<28.000000) return 0.000000;
  if (nvtx>=28.000000 && nvtx<30.000000) return 0.000000;
  if (nvtx>=30.000000 && nvtx<32.000000) return 0.000000;
  if (nvtx>=32.000000 && nvtx<34.000000) return 0.000000;
  if (nvtx>=34.000000 && nvtx<36.000000) return 0.000000;
  if (nvtx>=36.000000 && nvtx<38.000000) return 0.000000;
  if (nvtx>=38.000000 && nvtx<40.000000) return 0.000000;
  return 0.;
}

float flipRate(float pt, float eta) {
   if (pt>=0 && pt<20 && fabs(eta)>=0 && fabs(eta)<1 ) return 5.70541e-06;
   if (pt>=0 && pt<20 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.000286472;
   if (pt>=0 && pt<20 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.000748955;
   if (pt>=20 && pt<40 && fabs(eta)>=0 && fabs(eta)<1 ) return 2.10834e-05;
   if (pt>=20 && pt<40 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.000531631;
   if (pt>=20 && pt<40 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.00126557;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<1 ) return 2.48083e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.000650525;
   if (pt>=40 && pt<60 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.0016674;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<1 ) return 4.09732e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00101227;
   if (pt>=60 && pt<80 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.00230621;
   if (pt>=80 && fabs(eta)>=0 && fabs(eta)<1 ) return 7.17436e-05;
   if (pt>=80 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00101483;
   if (pt>=80 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.00512076;
   return 0.;
}
float flipRateError(float pt, float eta) {
   if (pt>=0 && pt<20 && fabs(eta)>=0 && fabs(eta)<1 ) return 5.51727e-06;
   if (pt>=0 && pt<20 && fabs(eta)>=1 && fabs(eta)<2 ) return 7.15378e-05;
   if (pt>=0 && pt<20 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.000193457;
   if (pt>=20 && pt<40 && fabs(eta)>=0 && fabs(eta)<1 ) return 5.91226e-06;
   if (pt>=20 && pt<40 && fabs(eta)>=1 && fabs(eta)<2 ) return 3.8619e-05;
   if (pt>=20 && pt<40 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.000106931;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<1 ) return 6.75759e-06;
   if (pt>=40 && pt<60 && fabs(eta)>=1 && fabs(eta)<2 ) return 4.36419e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.000141982;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<1 ) return 1.65967e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.000132439;
   if (pt>=60 && pt<80 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.000391051;
   if (pt>=80 && fabs(eta)>=0 && fabs(eta)<1 ) return 3.98353e-05;
   if (pt>=80 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.000217831;
   if (pt>=80 && fabs(eta)>=2 && fabs(eta)<3 ) return 0.00120939;
   return 0.;
}

float electronFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.427072;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.254745;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.170102;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.120354;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0638526;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.117328;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.062879;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0405136;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0651394;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0842386;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0428842;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.100071;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0873272;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.126914;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.065139;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0855434;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0661614;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0995078;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0133095;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00951482;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0228219;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00956932;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00705553;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0173703;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.016885;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0121364;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0265092;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0407728;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0272858;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0391579;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.274884;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.202058;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return -0.00134376;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.14126;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0173601;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.113918;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0235798;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0157741;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0104956;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0394259;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0479894;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.036613;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0475934;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return -0.00311868;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.167966;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0908487;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0696392;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.000466427;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0429659;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0190843;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.11044;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00571693;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00427558;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.00516554;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0160304;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0169513;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0192942;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.06244;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00601146;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.134822;
   return 0.;
}

float elecFakeRateInSitu(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.893333;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.884615;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 1;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.915254;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.863636;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 1;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.935484;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.733333;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.866667;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.92;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 1;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.733333;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.813953;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 1;
   return 0.;
}

float elecFakeRateInSituError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0356443;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0626563;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.036258;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.073165;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0441237;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.11418;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0620633;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0542586;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.043065;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0355266;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   return 0.;
}

float muonFakeRateInSitu(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.54955;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.581081;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.558824;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.544872;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.545455;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.464286;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.58;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.596774;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.692308;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.645161;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.705882;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.769231;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.644444;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.811594;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.8;
   return 0.;
}

float muonFakeRateInSituError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0333926;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0405558;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0851538;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0398705;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0474757;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0942498;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0493559;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0622993;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.128008;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.060765;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0638031;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.116855;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0374764;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0357531;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.178885;
   return 0.;
}

float fakeRate(int id, float pt, float eta) { 
  if (abs(id)==11) return electronFakeRate(pt,eta);
  else if (abs(id)==13) return muonFakeRate(pt,eta);
  else return 0.;
}

float fakeRateError(int id, float pt, float eta) { 
  if (abs(id)==11) return electronFakeRateError(pt,eta);
  else if (abs(id)==13) return muonFakeRateError(pt,eta);
  else return 0.;
}
 
#endif
