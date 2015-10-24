#ifndef _commonUtils_h_
#define _commonUtils_h_

float getLumi() { return 1.264; }
float getLumiUnblind() { return 0.133; }

bool isUnblindRun(int run) { return run<=257599; }

TString getTag() { return "v4.01"; }

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
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.363577;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.391415;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.405437;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.116412;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.129589;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.205809;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0517935;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0801971;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.117347;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.089048;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.096728;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.143847;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.113069;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.156928;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.197646;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0448513;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0338338;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0649558;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0057964;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00559119;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0131849;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00493345;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00533674;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0126692;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0105017;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00912012;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0188834;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0263702;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0195994;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0342679;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.305466;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.417147;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.362039;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.117332;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0700993;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.140192;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0266433;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.031486;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0459012;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0432821;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0395938;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0373546;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0812595;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0378713;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0560215;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0330383;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0396736;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0614766;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0194756;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0142611;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0412771;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00233347;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0031429;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.00793681;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00631932;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00632932;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0118612;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0178269;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0107039;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0315269;
   return 0.;
}
float electronFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.643571;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.418538;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.486123;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.207844;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.231203;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.300824;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.131027;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.198784;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.228487;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.182149;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.188522;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.310576;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.266728;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.307725;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.386736;
   return 0.;
}
float electronFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0803488;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0464512;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.102522;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0119047;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0111113;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0229365;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.015383;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0142409;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0273568;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0301679;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0241212;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0413127;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0611225;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0404722;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0616853;
   return 0.;
}
float muonFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.327803;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.426377;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.474965;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.143312;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.112731;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.115255;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.044913;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0525889;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0592187;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0726019;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0992866;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0868596;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.119478;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0850267;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.13836;
   return 0.;
}
float muonFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0276394;
   if (pt>=10 && pt<15 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0318795;
   if (pt>=10 && pt<15 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0540368;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0191904;
   if (pt>=15 && pt<25 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0168716;
   if (pt>=15 && pt<25 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0313794;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.00394432;
   if (pt>=25 && pt<35 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.00509942;
   if (pt>=25 && pt<35 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0101059;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.010278;
   if (pt>=35 && pt<50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0118987;
   if (pt>=35 && pt<50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0152356;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1 ) return 0.0217033;
   if (pt>=50 && fabs(eta)>=1 && fabs(eta)<2 ) return 0.0249692;
   if (pt>=50 && fabs(eta)>=2 && fabs(eta)<2.4 ) return 0.0611931;
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
 
#endif
