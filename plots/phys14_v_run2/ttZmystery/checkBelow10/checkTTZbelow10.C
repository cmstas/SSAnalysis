#include "SS.h"
#include "../../../../CORE/SSSelections.h"

float getGenMass(){
  LorentzVector result = {0,0,0,0};
  int nLep = 0;
  int lep1_idx = -1;
  int lep2_idx = -1;
  for (unsigned int i = 0; i < ss::genps_id().size(); i++){
    //Consider only status-1 leptons
    if (ss::genps_status().at(i) != 23) continue;
    int id = abs(ss::genps_id().at(i));
    if (id != 11 && id != 13 && id != 15) continue; 

    //Don't consider the ones that come from W
    if (abs(ss::genps_id_mother().at(i)) == 24) continue;
    if (id == 15 && abs(ss::genps_id_grandma().at(i)) == 24) continue;

    //If we're here, have a lepton
    lep1_idx == -1 ? lep1_idx = i : lep2_idx = i;
    nLep++;
  }//gen particle loop

  //If more than 2 leptons, abort
  if (nLep > 2){
    cout << "Caution! " << nLep << " generated leptons!" << endl;
    return -1; 
  }
  if (nLep < 2) return -1;

  //These are the selected leptons
  LorentzVector lep1 = ss::genps_p4().at(lep1_idx); 
  LorentzVector lep2 = ss::genps_p4().at(lep2_idx); 
  int lep1_id = ss::genps_id().at(lep1_idx); 
  int lep2_id = ss::genps_id().at(lep2_idx); 

  //Now see if you have a third lepton that will cause a veto 
  for (unsigned int i = 0; i < ss::genps_id().size(); i++){
    //Don't consider anything you already selected
    if ((unsigned)lep1_idx == i) continue;
    if ((unsigned)lep2_idx == i) continue;

    //Consider only status-1 leptons
    if (ss::genps_status().at(i) != 23) continue;
    int id = abs(ss::genps_id().at(i));
    if (id != 11 && id != 13 && id != 15) continue; 

    //Now check for the invt mass of the 3rd SS pair
    if (sgn(lep1_id) == sgn(ss::genps_id().at(i))) return (lep1 + ss::genps_p4().at(i)).M();
    if (sgn(lep2_id) == sgn(ss::genps_id().at(i))) return (lep2 + ss::genps_p4().at(i)).M();
    return 0; 

  }

  return -1;
}

void run(TTree* tree, float arg1, float arg2){

  int numer = 0;
  int denom = 0;

  for (unsigned int event = 0; event < tree->GetEntries(); event++){
   
    //Load event  
    samesign.GetEntry(event); 

    //Only class 3
    if (ss::hyp_class() != 3) continue;

    //Apply control region
    if (ss::nbtags() < 2) continue;
    if (ss::ht() < 500 && ss::met() < 30) continue;

    //If we're here, will be in denom or numer.  

    //If no third lepton, denom
    if (ss::lep3_idx() < 0) denom++; 
 
    //If 3rd lep is SS to the selected leptons, denom
    if (sgn(ss::lep1_id()) == sgn(ss::lep3_id())) denom++;

    //Check if 3rd lepton is SF to either selected lepton
    bool sf13 = 0;
    bool sf12 = 0;
    if (ss::lep1_id() == -ss::lep3_id()) sf13 = 1;
    if (ss::lep2_id() == -ss::lep3_id()) sf23 = 1;

    //If no SF pairs, denom
    if (ss::lep3_id() == 

  }//event loop

  cout << numer << "/" << denom << " = " << 100.0*numer/denom << " \\pm " << (100.0*numer/denom)*sqrt( 1.0/numer + 1.0/denom) << "%" << endl;
}

void checkTTZbelow10(){

  TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/v3.07/TTZL.root");
  TTree *tree = (TTree*)file->Get("t"); 
  samesign.Init(tree);

  run(tree, 10, 15); 
  run(tree, 15, 20);
  run(tree, 20, 30); 
}
