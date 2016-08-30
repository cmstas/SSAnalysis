#ifndef _commonUtils_h_
#define _commonUtils_h_

// // 0.8
// float getLumi() { return 0.8042; }
// float getLumiUnblind() { return 0.8042; }
// bool isUnblindRun(int run) { return run <= 274240; }
// float getWZSF() { return 0.85; }
// TString getTag() { return "v8.02"; } // up to the 3.99/fb json
// #include "flip_rates.h"


// // 2.6
// float getLumi() { return 2.6; }
// float getLumiUnblind() { return 2.6; }
// bool isUnblindRun(int run) { return run <= 274442; }
// float getWZSF() { return 0.85; } 
// TString getTag() { return "v8.02"; } // up to the 3.99/fb json
// #include "flip_rates.h"

// // 3.99
// float getLumi() { return 3.99; }
// float getLumiUnblind() { return 3.99; }
// bool isUnblindRun(int run) { return run <= 275125; }
// float getWZSF() { return 1.021; } // 4/fb
// TString getTag() { return "v8.02"; } // up to the 3.99/fb json
// #include "flip_rates.h"

// // 6.26
// float getLumi() { return 6.26; }
// float getLumiUnblind() { return 6.26; }
// bool isUnblindRun(int run) { return run <= 275782; }
// float getWZSF() { return 0.948; }
// TString getTag() { return "v8.03"; } // up to the 6.26/fb json
// #include "flip_rates_6p26ifb.h"
// #include "fake_rates.h"

// 12.9
float getLumi() { return 12.9; }
float getLumiUnblind() { return 12.9; }
bool isUnblindRun(int run) { return true; }
float getWZSF() { return 0.948; } // 0.948 pm 0.210
TString getTag() { return "v8.04"; }
#include "flip_rates.h"
#include "fake_rates.h"

// // 2nd HALF
// float getLumi() { return 12.9-6.26; }
// float getLumiUnblind() { return 12.9-6.26; }
// bool isUnblindRun(int run) { return run > 275782; }
// float getWZSF() { return 0.948; } // 0.948 pm 0.210
// TString getTag() { return "v8.04"; }
// #include "flip_rates_12p9ifb.h"
// #include "fake_rates_2ndhalf.h"


bool applyThirdLeptonVeto() { return false; }


#include "hlt/HLTEfficiency.cc"


#include "pu_weights.h"
// #include "flip_rates.h"
// #include "fake_rates.h"
#include "fake_rates_noCC.h"
#include "lepton_sf.h"
#include "lepton_sf_fastsim.h"

// FIXME
#include "fake_rates_insitu.h"
// #include "fake_rates_insitu_siplt4.h"
// #include "fake_rates_insitu_miniisolt4.h"

#include "insitu_selections.h"

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

/*
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
*/

float fakeRateInSitu(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
      if (abs(id)==11) return elecFakeRateInSituHighHT(pt,eta);
      else if (abs(id)==13) return muonFakeRateInSituHighHT(pt,eta);
      else return 0.;
  } else {
      if (abs(id)==11) return elecFakeRateInSitu(pt,eta);
      else if (abs(id)==13) return muonFakeRateInSitu(pt,eta);
      else return 0.;
  }
}

float fakeRateErrorInSitu(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
      if (abs(id)==11) return elecFakeRateInSituErrorHighHT(pt,eta);
      else if (abs(id)==13) return muonFakeRateInSituErrorHighHT(pt,eta);
      else return 0.;
  } else {
      if (abs(id)==11) return elecFakeRateInSituError(pt,eta);
      else if (abs(id)==13) return muonFakeRateInSituError(pt,eta);
      else return 0.;
  }
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

float fakeRateNoCC(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRateNoCC(pt,eta);
    else if (abs(id)==13) return muonFakeRateNoCC(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRate_IsoTrigsNoCC(pt,eta);
    else if (abs(id)==13) return muonFakeRate_IsoTrigsNoCC(pt,eta);
    else return 0.;
  }
}

float fakeRateErrorNoCC(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRateErrorNoCC(pt,eta);
    else if (abs(id)==13) return muonFakeRateErrorNoCC(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRateError_IsoTrigsNoCC(pt,eta);
    else if (abs(id)==13) return muonFakeRateError_IsoTrigsNoCC(pt,eta);
    else return 0.;
  }
}

float qcdMCFakeRateNoCC(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronQCDMCFakeRateNoCC(pt,eta);
    else if (abs(id)==13) return muonQCDMCFakeRateNoCC(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronQCDMCFakeRate_IsoTrigsNoCC(pt,eta);
    else if (abs(id)==13) return muonQCDMCFakeRate_IsoTrigsNoCC(pt,eta);
    else return 0.;
  }
}


#endif
