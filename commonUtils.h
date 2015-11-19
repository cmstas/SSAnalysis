#ifndef _commonUtils_h_
#define _commonUtils_h_

float getLumi() { return 2.11; }
float getLumiUnblind() { return getLumi(); }

bool isUnblindRun(int run) { return true; }

TString getTag() { return "v5.01"; }

float getPUwECO(int nvtx) {
  float array[] = {1.0, 3.153000427291265, 2.4535501340758543, 2.353696182351581, 2.3718057802881676, 2.3508262193470397, 2.291773427755106, 2.129929297304804, 1.9422796094930384, 1.7126175249202766, 1.4622401225778663, 1.2063711142884181, 0.9608504360968657, 0.7484941355600901, 0.5769889517104192, 0.4315759540480359, 0.3195820162866148, 0.2306052595765186, 0.1691819686464576, 0.12324466445693416, 0.08833641123547825, 0.06306193566475429, 0.047857788612281564, 0.034655984107483044, 0.02358209210941948, 0.019535383939466185, 0.01500976654907348, 0.009407396052837736, 0.006332906010381258, 0.008524617116368491, 0.003145186350322339, 0.0, 0.0, 0.006930381551001214, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  if (nvtx<60) return array[nvtx];
  else return 1.;
}

float getPUw(int nvtx) {
  if (nvtx>=0.000000 && nvtx<2.000000) return 6.874322;
  if (nvtx>=2.000000 && nvtx<4.000000) return 2.078853;
  if (nvtx>=4.000000 && nvtx<6.000000) return 1.931197;
  if (nvtx>=6.000000 && nvtx<8.000000) return 1.983540;
  if (nvtx>=8.000000 && nvtx<10.000000) return 1.749420;
  if (nvtx>=10.000000 && nvtx<12.000000) return 1.240954;
  if (nvtx>=12.000000 && nvtx<14.000000) return 0.963793;
  if (nvtx>=14.000000 && nvtx<16.000000) return 0.642933;
  if (nvtx>=16.000000 && nvtx<18.000000) return 0.391587;
  if (nvtx>=18.000000 && nvtx<20.000000) return 0.199816;
  if (nvtx>=20.000000 && nvtx<22.000000) return 0.093767;
  if (nvtx>=22.000000 && nvtx<24.000000) return 0.076702;
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

float getPUwUnblind(int nvtx) {
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
   float scale = 1.35;
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return scale*7.36646e-06;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000108283;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.00108401;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return scale*2.34739e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000198413;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.00141664;
   if (pt>=60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return scale*0.00011247;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000301189;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.0020123;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000560358;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.00233948;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000295415;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.00395713;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.00282565;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.0127978;
   return 0.;
}
float flipRateError(float pt, float eta) {
   float scale = 1.35;
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return scale*3.6401e-06;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*2.89577e-05;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*9.84959e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return scale*1.09368e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*3.71107e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.000119599;
   if (pt>=60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return scale*5.90675e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000120319;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.00039592;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000285579;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.000686392;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.000180937;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.000965423;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return scale*0.00207696;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return scale*0.00742097;
   return 0.;
}

float electronFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.393477;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.411032;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.349077;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.102318;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.111635;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.131716;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.045717;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0712811;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0815254;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0815878;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0919277;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.110317;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0636477;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.117502;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.197005;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0648464;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0573387;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0497431;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00623306;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00644568;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00665219;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00582063;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00705615;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00548242;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0146363;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0127349;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.012136;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0233662;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0292236;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0269866;
   return 0.;
}
float electronAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.39899;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.412822;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.350539;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.10401;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.112843;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.132562;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0492882;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.073555;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0834966;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0954165;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.102385;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.118086;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.100688;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.141872;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.209497;
   return 0.;
}
float electronQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.457314;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.50988;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.39919;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0887789;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.115415;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.133353;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0376028;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.065426;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.106336;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0434347;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.100587;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.070583;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.103708;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.242643;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.15912;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.334863;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.368637;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.428784;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0932735;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0802793;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.135528;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0287337;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0300141;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0411214;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0376999;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0393224;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0559298;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.062726;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0524433;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0351502;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0203605;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0269318;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0523567;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0101815;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0110663;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0310548;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00153273;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00204762;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00606756;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0034222;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00444309;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0165941;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0107632;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0111327;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0204483;
   return 0.;
}
float muonAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.334999;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.368757;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.428866;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0935117;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0805104;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.135862;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0294634;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0306587;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0421033;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.040871;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0422572;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0595495;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0809382;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0668732;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.055606;
   return 0.;
}
float muonQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.354237;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.408219;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.472208;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0877687;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.109128;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.140858;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0573525;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0639541;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0838826;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0560742;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0598154;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0827884;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0588362;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.062513;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0620255;
   return 0.;
}
float electronFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.478167;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.562729;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.344186;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.192161;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.212703;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.243522;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.129736;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.189507;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.204526;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.174297;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.191654;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.255363;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.263744;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.226897;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.334425;
   return 0.;
}
float electronFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0903426;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.055986;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0491346;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0110744;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0111114;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0114553;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0143851;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0158143;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0132017;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.031788;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0260625;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0247228;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0582871;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0520077;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0357376;
   return 0.;
}
float electronAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.490949;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.564783;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.347202;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.195699;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.215043;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.245086;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.140572;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.195623;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.209071;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.213715;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.219477;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.275066;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.334736;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.280675;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.356879;
   return 0.;
}
float electronQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.549049;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.599392;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.521296;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.166574;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.209291;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.217433;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0861201;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.155806;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.190451;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.124124;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.218399;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.118186;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.207434;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.412609;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.245735;
   return 0.;
}
float muonFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.358295;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.403822;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.462864;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.117464;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.127019;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.141556;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0470157;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0498184;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0593555;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0645791;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0850976;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0979425;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.106374;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.103871;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0857029;
   return 0.;
}
float muonFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0160459;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0210315;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0437422;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0100717;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0121369;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0244059;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00239227;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00325623;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00834444;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00550991;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00670786;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0151476;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0143575;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0230442;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0461271;
   return 0.;
}
float muonAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.358457;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.403959;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.46296;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.11784;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.127367;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.142053;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0484354;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0511341;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0610217;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0711067;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.092683;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.105606;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.146371;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.136691;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.140053;
   return 0.;
}
float muonQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.376847;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.422187;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.48678;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.115663;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.137549;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.169286;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0885771;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0983223;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.114144;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0907041;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0926629;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.116131;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0996795;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.105579;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0913975;
   return 0.;
}

float fakeRate(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRate(pt,eta);
    else if (abs(id)==13) return muonFakeRate(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRate_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float fakeRateError(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRateError(pt,eta);
    else if (abs(id)==13) return muonFakeRateError(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRateError_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRateError_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float alternativeFakeRate(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronAlternativeFakeRate(pt,eta);
    else if (abs(id)==13) return muonAlternativeFakeRate(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronAlternativeFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonAlternativeFakeRate_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float qcdMCFakeRate(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronQCDMCFakeRate(pt,eta);
    else if (abs(id)==13) return muonQCDMCFakeRate(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronQCDMCFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonQCDMCFakeRate_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float elecFakeRateInSitu(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 1;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.666667;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.333333;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.166667;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.384615;
   return 0.;
}

float elecFakeRateInSituError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.272166;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.19245;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.152145;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.0813475;
   return 0.;
}

float muonFakeRateInSitu(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.3125;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.181818;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.1;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.0344828;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.137931;
   return 0.;
}

float muonFakeRateInSituError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.115878;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.0822304;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.0547723;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.033883;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<2.4 ) return 0.0546839;
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

bool passesNumeratorMVA(int id, float eta, float disc){
  if (abs(id) != 11) return true;
  float aeta = fabs(eta);
  if (aeta < 0.8) return disc > 0.87;
  if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.60;
  if (aeta > 1.479) return disc > 0.17;
  return false;
}

#endif
