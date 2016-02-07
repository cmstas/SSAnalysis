#ifndef SCALEFACTORS_H
#define SCALEFACTORS_H

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

enum DileptonHypType { EE, EMU, MUMU };
//enum AnalysisType { high_pt, low_pt, vlow_pt }; 

//Trigger
float DileptonTriggerScaleFactor(int hyp_type, float trailing_pt, float trailing_eta, bool is_reco = 0 );

// Lepton ID and isolatoin efficiency (lepton)
float TagAndProbeScaleFactor(int id, float pt, float eta);

#endif
