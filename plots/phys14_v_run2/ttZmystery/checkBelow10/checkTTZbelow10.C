#include "SS.h"
#include "../../../../CORE/SSSelections.h"
#include "../../../../software/dataMCplotMaker/dataMCplotMaker.h"

TH1F* selected = 0; 

float getGenMass(){
  LorentzVector result = {0,0,0,0};
  int nLep = 0;
  int lep1_idx = -1;
  int lep2_idx = -1;
  for (unsigned int i = 0; i < ss::genps_id().size(); i++){
    //Consider only status-23 leptons
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
  selected->Fill((lep1 + lep2).M()); 

  //Now see if you have a third lepton that will cause a veto 
  for (unsigned int i = 0; i < ss::genps_id().size(); i++){
    //Don't consider anything you already selected
    if ((unsigned)lep1_idx == i) continue;
    if ((unsigned)lep2_idx == i) continue;

    //Consider only status-23 leptons
    if (ss::genps_status().at(i) != 23) continue;
    int id = abs(ss::genps_id().at(i));
    if (id != 11 && id != 13 && id != 15) continue; 

    //If so, return mll
    return (lep1 + lep2).M(); 

  }

  return -1;
}

void run(TTree* tree, float arg1, float arg2){

  TH1F* numer = new TH1F("numer", "numer", 20, 0, 200);
  TH1F* denom = new TH1F("denom", "denom", 20, 0, 200);

  //Sumw2
  numer->Sumw2();
  denom->Sumw2();

  for (unsigned int event = 0; event < tree->GetEntries(); event++){
   
    //Load event  
    samesign.GetEntry(event); 

    //hyp-class = 3 or 6 
    if (ss::hyp_class() != 3 && ss::hyp_class() != 6) continue;

    //Apply control region
    if (ss::nbtags() < 2) continue;
    if (ss::ht() < 500 && ss::met() < 30) continue;
 
    if (ss::lep4_id() != 0) denom->Fill(getGenMass()); 
    else numer->Fill(getGenMass()); 

  }//event loop

  numer->Divide(denom); 
  vector <TH1F*> back = { numer }; 
  vector <TH1F*> back2 = { denom }; 
  vector <string> titles = {"ttz"}; 
  TH1F* null = new TH1F("","",1,0,1);
  dataMCplotMaker(null, back, titles, "lost ratio", "25ns run2 TTZ MC", "--outputName ratio --isLinear --xAxisLabel M_{ll} --noLumi --noOverflow --histoErrors --drawDots"); 
  dataMCplotMaker(null, back2, titles, "lost ratio denom", "25ns run2 TTZ MC", "--outputName denom --isLinear --xAxisLabel M_{ll} --noLumi --noOverflow --histoErrors --drawDots"); 

  cout << denom->Integral(2,2) << "/" << denom->Integral(2,21) << endl; 
  //cout << numer << "/" << denom << " = " << 100.0*numer/denom << " \\pm " << (100.0*numer/denom)*sqrt( 1.0/numer + 1.0/denom) << "%" << endl;
}

void checkTTZbelow10(){

   selected = new TH1F("sel", "sel", 100, 0, 200); 

  TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/v3.08/TTZL.root");
  TTree *tree = (TTree*)file->Get("t"); 
  samesign.Init(tree);

  selected->Draw(); 

  run(tree, 10, 15); 
  //run(tree, 15, 20);
  //run(tree, 20, 30); 
}
