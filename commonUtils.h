#ifndef _commonUtils_h_
#define _commonUtils_h_

float getLumi() { return 1.56; }
float getLumiUnblind() { return 1.28; }

bool isUnblindRun(int run) { return run<=258714; }

TString getTag() { return "v4.04"; }

float getPUwECO(int nvtx) {
  float array[] = {1.0, 3.153000427291265, 2.4535501340758543, 2.353696182351581, 2.3718057802881676, 2.3508262193470397, 2.291773427755106, 2.129929297304804, 1.9422796094930384, 1.7126175249202766, 1.4622401225778663, 1.2063711142884181, 0.9608504360968657, 0.7484941355600901, 0.5769889517104192, 0.4315759540480359, 0.3195820162866148, 0.2306052595765186, 0.1691819686464576, 0.12324466445693416, 0.08833641123547825, 0.06306193566475429, 0.047857788612281564, 0.034655984107483044, 0.02358209210941948, 0.019535383939466185, 0.01500976654907348, 0.009407396052837736, 0.006332906010381258, 0.008524617116368491, 0.003145186350322339, 0.0, 0.0, 0.006930381551001214, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  if (nvtx<60) return array[nvtx];
  else return 1.;
}

float getPUw(int nvtx) {
   if (nvtx>=0.000000 && nvtx<2.000000) return 7.422880;
   if (nvtx>=2.000000 && nvtx<4.000000) return 2.171067;
   if (nvtx>=4.000000 && nvtx<6.000000) return 2.013241;
   if (nvtx>=6.000000 && nvtx<8.000000) return 2.013387;
   if (nvtx>=8.000000 && nvtx<10.000000) return 1.773547;
   if (nvtx>=10.000000 && nvtx<12.000000) return 1.251210;
   if (nvtx>=12.000000 && nvtx<14.000000) return 0.944763;
   if (nvtx>=14.000000 && nvtx<16.000000) return 0.633186;
   if (nvtx>=16.000000 && nvtx<18.000000) return 0.390260;
   if (nvtx>=18.000000 && nvtx<20.000000) return 0.157115;
   if (nvtx>=20.000000 && nvtx<22.000000) return 0.061071;
   if (nvtx>=22.000000 && nvtx<24.000000) return 0.073466;
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
  float scale = 1.15;
   if (eta<-1.5 && eta>-2) scale = 3.1;
   //scale = 1.;
   if (pt>=0 && pt<20 && fabs(eta)>=0 && fabs(eta)<1 ) return scale*5.70541e-06;
   if (pt>=0 && pt<20 && fabs(eta)>=1 && fabs(eta)<2 ) return scale*0.000286472;
   if (pt>=0 && pt<20 && fabs(eta)>=2 && fabs(eta)<3 ) return scale*0.000748955;
   if (pt>=20 && pt<40 && fabs(eta)>=0 && fabs(eta)<1 ) return scale*2.10834e-05;
   if (pt>=20 && pt<40 && fabs(eta)>=1 && fabs(eta)<2 ) return scale*0.000531631;
   if (pt>=20 && pt<40 && fabs(eta)>=2 && fabs(eta)<3 ) return scale*0.00126557;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<1 ) return scale*2.48083e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=1 && fabs(eta)<2 ) return scale*0.000650525;
   if (pt>=40 && pt<60 && fabs(eta)>=2 && fabs(eta)<3 ) return scale*0.0016674;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<1 ) return scale*4.09732e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=1 && fabs(eta)<2 ) return scale*0.00101227;
   if (pt>=60 && pt<80 && fabs(eta)>=2 && fabs(eta)<3 ) return scale*0.00230621;
   if (pt>=80 && fabs(eta)>=0 && fabs(eta)<1 ) return scale*7.17436e-05;
   if (pt>=80 && fabs(eta)>=1 && fabs(eta)<2 ) return scale*0.00101483;
   if (pt>=80 && fabs(eta)>=2 && fabs(eta)<3 ) return scale*0.00512076;
   return scale*0.;
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
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.369058;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.373428;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.391452;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.109952;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.120915;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.1925;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.053727;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0820309;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.111464;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0819657;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0907634;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.155371;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.10374;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.150627;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.193997;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0372898;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0275624;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0533833;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00462352;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00441003;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.010624;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00408806;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00439167;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.00991556;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00843297;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00724532;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0157347;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0205402;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0158446;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0282269;
   return 0.;
}
float electronAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.373064;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.374511;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.39302;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.1112;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.121644;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.193215;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0565135;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0834754;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.113344;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0923699;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0975798;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.161435;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.12831;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.163272;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.203985;
   return 0.;
}
float electronQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.469259;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.483903;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.412365;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0912802;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.127715;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.146606;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0392981;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.078446;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.146955;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.053499;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0753836;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.088113;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0963152;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.231562;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.143933;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.319736;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.380772;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.400211;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.106128;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0732757;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.141076;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0276585;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0312232;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0418421;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0415518;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0368066;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0484434;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0765552;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0417443;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0362827;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0247289;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.029079;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0486562;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0137791;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0107946;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0289872;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00183967;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00230473;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.00518762;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00462566;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00427611;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0132104;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0141557;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00915428;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0178407;
   return 0.;
}
float muonAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.319871;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.380873;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.400303;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.106352;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0734866;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.141342;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0283179;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0318344;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0427744;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0444587;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0394536;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0515308;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0965334;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0531804;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0540475;
   return 0.;
}
float muonQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.348702;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.410769;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.476903;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0875078;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.104708;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.145828;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0576342;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0616798;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0872456;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0537942;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0614637;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0818687;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0614632;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0596624;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.060297;
   return 0.;
}
float electronFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.555807;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.456756;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.477829;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.207089;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.232598;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.302795;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.138727;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.197831;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.247559;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.185326;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.18086;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.313244;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.270369;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.289684;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.36855;
   return 0.;
}
float electronFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0653416;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0388574;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0847252;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00961603;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00895611;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0188257;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0120563;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0113622;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0219037;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0245288;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0196741;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0324675;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0478267;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0336173;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0482586;
   return 0.;
}
float electronAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.563444;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.458463;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.480595;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.209954;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.234201;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.304216;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.148183;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.202011;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.251963;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.220735;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.204003;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.326737;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.328783;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.319168;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.38627;
   return 0.;
}
float electronQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.554025;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.602921;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.51043;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.171273;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.218209;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.231666;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0925726;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.160992;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.263657;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.146264;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.144305;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.128836;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.19416;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.399931;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.191542;
   return 0.;
}
float muonFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.349239;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.403082;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.4819;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.125321;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.118837;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.153337;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0451568;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0512726;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0571228;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0679309;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0777592;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.10213;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.114892;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0819555;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0847835;
   return 0.;
}
float muonFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0199775;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0230985;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0408467;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.013314;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0123604;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0243072;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0028949;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00356228;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.00691843;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00704031;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00711973;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0152697;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0186315;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0175067;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0415685;
   return 0.;
}
float muonAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.349417;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.403211;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.48201;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.1257;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.119195;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.153781;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0465583;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0526367;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0587792;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0742942;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0852053;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.110595;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.160886;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.111196;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.140214;
   return 0.;
}
float muonQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.370888;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.427215;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.489158;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.114684;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.133772;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.179188;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0891667;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0952179;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.118792;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0877095;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0958426;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.111298;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.103578;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.101233;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0943381;
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
