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
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.433033;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.258273;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.171453;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.122865;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0655743;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.113473;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.066252;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0385893;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0608895;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0847046;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0442128;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0969907;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0878908;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.131467;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.076634;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0877812;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0668629;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0993957;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0137356;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00992372;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0230367;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0099641;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00702323;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0171655;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0176193;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0124806;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0260161;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0418785;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0281236;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0419961;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.291945;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.281563;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.118751;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0213285;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0220092;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0207511;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.010387;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0402747;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0484684;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0431396;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0146869;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.179174;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.110236;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0917381;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.000236018;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0473346;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.023032;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.000828744;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00604478;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00655723;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.00594911;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.018561;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0208509;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0227949;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0811708;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00572877;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.168256;
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

float elecFakeRateInSitu(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.536232;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.590909;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.5;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.282609;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.461538;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.333333;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.151079;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.151515;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0813008;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.140845;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.333333;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0769231;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.155738;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.5;
   return 0.;
}
float elecFakeRateInSituError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0600347;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.104824;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.353553;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0383293;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0798268;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.272166;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0303759;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0624156;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0174248;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0412836;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.272166;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00955849;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0180953;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.25;
   return 0.;
}
float muonFakeRateInSitu(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.316716;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.365854;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.350877;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0896921;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.135417;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.186441;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0424178;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0898876;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0925926;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0186335;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0594595;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0862069;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0138496;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0541455;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0487805;
   return 0.;
}
float muonFakeRateInSituError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0251917;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0336412;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0632126;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0104547;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0201624;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0507036;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00656306;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0175042;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.039445;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00337016;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0122941;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0368537;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00134617;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00661453;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0336412;
   return 0.;
}

float fakeRateInSitu(int id, float pt, float eta) { 
  if (abs(id)==11) return elecFakeRateInSitu(pt,eta);
  else if (abs(id)==13) return muonFakeRateInSitu(pt,eta);
  else return 0.;
}

float fakeRateErrorInSitu(int id, float pt, float eta) { 
  if (abs(id)==11) return elecFakeRateInSituError(pt,eta);
  else if (abs(id)==13) return muonFakeRateInSituError(pt,eta);
  else return 0.;
}

bool passIsolatedFO(int id, float eta, float mva) {
  if (abs(id)==13) return true;
  float aeta = fabs(eta);
  if (aeta < 0.8) return mva > -0.155;
  if ((aeta >= 0.8 && aeta <= 1.479)) return mva > -0.56;
  if (aeta > 1.479) return mva > -0.76;
  return false;
}
 
#endif
