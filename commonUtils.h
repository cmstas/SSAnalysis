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

 
#endif
