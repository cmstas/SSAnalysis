#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "/home/users/cgeorge/CORE/CMS3.h"
#include "/home/users/cgeorge/CORE/Tools/utils.h"
#include "/home/users/cgeorge/CORE/SSSelections.h"
#include "/home/users/cgeorge/software/dataMCplotMaker/PlotMaker2D.h" 

void study1();
void study2();
void study3();

enum mode_s { NORMAL, TEST, DEBUG, DUMPBAD }; 
enum sample_s { T1TTTT, WZ, WG }; 

mode_s mode_in = TEST; 
sample_s sample = T1TTTT; 
int study = 3; 

void genStudies(){
  
  if (study == 1) study1(); 
  if (study == 2) study2(); 
  if (study == 3) study3(); 

}

void study1(){

  mode_s mode = mode_in == DUMPBAD ? NORMAL : mode_in;

  //Counters
  int good = 0;
  int bad = 0;

  //Set up chain
  TChain *chain = new TChain("Events"); 
  if (sample == T1TTTT) chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root"); 
  if (sample == WZ)     chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root"); 
  if (sample == WG)     chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-12/merged_ntuple_1.root"); 

  //nEvents in chain
  unsigned int nEventsProcessed = 0;
  unsigned int nEventsTotal = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    //Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    cms3.Init(tree);

    //Number of events to do
    int nEventsToDo = (mode == NORMAL ? tree->GetEntriesFast() : 1000);  

    //Loop over Events in current file
    for (int eventAG = 0; eventAG < nEventsToDo; eventAG++){

      //Load in the current event
      cms3.GetEntry(eventAG);

      //Progress Bar
      CMS3::progress(nEventsProcessed, nEventsTotal);
      nEventsProcessed++;

      //Is printing
      bool isPrinting = false; 

      //Electron counter
      int iElectron = -1; 

      //Loop through all the electrons particles
      for (unsigned int iElec = 0; iElec < tas::els_p4().size(); iElec++){

        //stupid piece of shit
        if (mode_in == DUMPBAD) mode = NORMAL; 

        //Load variables
        int event  = tas::evt_event(); 
        float pt   = tas::els_p4().at(iElec).pt();
        float aeta = fabs(tas::els_p4().at(iElec).eta()); 

        //Reject events with pT < 5 or |eta| > 2.5
        if (pt < 5) continue;
        if (aeta > 2.5) continue; 

        //The ones that get this far are electrons.  Increment the counter and print the header
        iElectron++; 
        if (mode == TEST && isPrinting == false){
          isPrinting = true; 
          cout << "------------" << endl;
          cout << "Printing pT of reco electrons in event #" << event << endl;
          cout << Form("# %8s %8s %7s hardFlag tauFlag ourFlag genMoPDGID genGmoPDGID moStat gmoStat", "reco pt", "gen pt", "deltaR") << endl;
        }

        //Find the variables for the matched gen particle
        int gen_idx  = tas::els_mc3idx().at(iElec); //<-- this must be mc3idx, otherwise it crashes
        float gen_pt = gen_idx >= 0 ? tas::genps_p4().at(gen_idx).pt() : -1; 
        int mo_id    = tas::els_mc_motherid().at(iElec); 
        int mo_idx   = tas::els_mc3_motheridx().at(iElec);
        int mo_stat  = mo_idx >= 0 ? tas::genps_status().at(mo_idx) : -1; 
        int gmo_id   = mo_idx >= 0 ?  tas::genps_id_mother().at(mo_idx) : -9999; 
        int gmo_idx  = mo_idx >= 0 ? tas::genps_idx_mother().at(mo_idx) : -9999; 
        int gmo_stat = gmo_idx >= 0 ? tas::genps_status().at(gmo_idx) : -1;
        bool gen_hardFlag = gen_idx >= 0 ? ((tas::genps_isPromptFinalState().at(gen_idx)  || tas::genps_isHardProcess().at(gen_idx)) && (abs(mo_id) == 24 || abs(mo_id) == 23)) : 0; 
        bool gen_tauFlag = gen_idx >= 0 ? ((tas::genps_isDirectHardProcessTauDecayProductFinalState().at(gen_idx) || tas::genps_isDirectPromptTauDecayProductFinalState().at(gen_idx)) && (abs(gmo_id) == 24 || abs(gmo_id) == 23)) : 0;
        int ourFlag = lepMotherID(Lep(11, iElec)); 

        //Delta-R between gen and matched
        float deltaR = gen_idx >= 0 ? DeltaR(tas::genps_p4().at(gen_idx), tas::els_p4().at(iElec)) : -1; 

        //Verdict on whether it matches or not
        bool us = (ourFlag > 0);
        bool them = (gen_hardFlag || gen_tauFlag) && abs(tas::els_mc_id().at(iElec)) == 11;
        bool verdict = (us == them);
        if (gmo_idx >= 0) verdict ? good++ : bad++; 
        if (verdict == 0 && mode_in == DUMPBAD) mode = TEST; 

        //Debug mode
        if (mode == DEBUG){
          //Get electron we want
          if (event != 257750) continue; 
          if (pt > 16.1 || pt < 16) continue;

          //Try to figure out why they fail it
          cout << "gen particle id: " << tas::els_mc_id().at(iElec) << endl;
          cout << "grandmother id: " << tas::genps_id().at(gmo_idx) << endl;
          cout << "grandmother status: " << tas::genps_status().at(gmo_idx) << endl;
          cout << "mother status: " << tas::genps_status().at(mo_idx) << endl;
          cout << "particle status: " << tas::genps_status().at(gen_idx) << endl;
          int ggmo_id   = gmo_idx >= 0 ?  tas::genps_id_mother().at(gmo_idx) : -9999; 
          cout << "great-grandmother id: " << ggmo_id << endl;
        }

        //Print out the pt and flag status
        if (mode != NORMAL) cout << Form("%i %8.4f %8.4f %7.4f %1i         %1i        % 1i        % 6i     %7i   %2i       %2i %s", iElectron, pt, gen_pt, deltaR, gen_hardFlag, gen_tauFlag, ourFlag, mo_id, gmo_id, mo_stat, gmo_stat, verdict ? "" : "<--bad!") << endl;
        if (mode != NORMAL && mo_idx >= 0) cout << " --> mother invariant mass: " << tas::genps_p4().at(mo_idx).M() << endl;
        if (mode != NORMAL && gen_idx >= 0) cout << " --> flag alone: " << (tas::genps_isPromptFinalState().at(gen_idx)  || tas::genps_isHardProcess().at(gen_idx)) << endl;

        //For debug mode, we are done
        if (mode == DEBUG) return;     

      }//electron loop 

    }//event loop

    //Close the file
    file->Close(); 

  }//file loop

  //Percentage of bad electrons in T1tttt (1200,800) fullsim
  if (mode == NORMAL){
    cout << endl << "Result: " << 100.0*bad/(good+bad) << "%" << endl;
    cout << "Number examined: " << good+bad << endl;
    cout << endl;
  }

}

void study2(){

  TH2F* plot = new TH2F("blah", "blah", 100, 0, 0.04, 100, -1, 1); 

  mode_s mode = mode_in == DUMPBAD ? NORMAL : mode_in;

  //Counters
  int good = 0;
  int bad = 0;
  int nomatch = 0; 
  int yes = 0;
  int no = 0;
  int classa = 0;
  int classb = 0;

  //Set up chain
  TChain *chain = new TChain("Events"); 
  if (sample == T1TTTT) chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root"); 
  if (sample == WZ)     chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root"); 
  if (sample == WG)     chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-12/merged_ntuple_1.root"); 

  //nEvents in chain
  unsigned int nEventsProcessed = 0;
  unsigned int nEventsTotal = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    //Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    cms3.Init(tree);

    //Number of events to do
    int nEventsToDo = (mode == NORMAL ? tree->GetEntriesFast() : 1000);  

    //Loop over Events in current file
    for (int eventAG = 0; eventAG < nEventsToDo; eventAG++){

      //Load in the current event
      cms3.GetEntry(eventAG);

      //Progress Bar
      CMS3::progress(nEventsProcessed, nEventsTotal);
      nEventsProcessed++;

      //Is printing
      bool isPrinting = false; 

      //Electron counter
      int iElectron = -1; 

      //Loop through all the electrons particles
      for (unsigned int iElec = 0; iElec < tas::els_p4().size(); iElec++){

        //Load variables
        int event  = tas::evt_event(); 
        float pt   = tas::els_p4().at(iElec).pt();
        float aeta = fabs(tas::els_p4().at(iElec).eta()); 

        //Reject events with pT < 5 or |eta| > 2.5
        if (pt < 5) continue;
        if (aeta > 2.5) continue; 

        //Reject events that fail ID
        if (!isLooseElectronPOG(iElec)) continue; 

        //Find gen idx of electron
        int gen_idx  = tas::els_mc3idx().at(iElec); 

        //If the electron is not matched, mark it
        if (gen_idx < 0){
          nomatch++; 
          continue;
        }

        //If the electron is not matched to electron, mark it
        if (abs(tas::genps_id().at(gen_idx)) == 11){
          good++; 
          continue;  
        }

        //If it's not matched to an electron, we have a mismatch!
        bad++; 

        //Figure out the delta R
        float deltaR = gen_idx >= 0 ? DeltaR(tas::genps_p4().at(gen_idx), tas::els_p4().at(iElec)) : -1; 
        //cout << "bad match!  matched to: " << tas::genps_id().at(gen_idx) << " at deltaR = " << deltaR << endl;

        //See if there is an electron within 0.2
        bool matched = false;
        for (unsigned int i = 0; i < tas::genps_p4().size(); i++){
          if (abs(tas::genps_id().at(i)) != 11) continue;
          if (abs(tas::genps_status().at(i)) != 1) continue; 
          float dR = DeltaR(tas::genps_p4().at(i), tas::els_p4().at(iElec)); 
          if (dR > 0.2) continue; 
          float dPT = (tas::els_p4().at(iElec).pt() - tas::genps_p4().at(i).pt())/tas::els_p4().at(iElec).pt();
          //cout << dPT << " " << dR << " " << abs(tas::genps_id().at(gen_idx)) << " " << tas::genps_id_mother().at(gen_idx) << endl;
          plot->Fill(dR, dPT); 
          if (dR > 0.003) continue; 
          if (abs(dPT) < 0.1) classa++; 
          else classb++; 
          matched = true;
        }
        matched ? yes++ : no++; 

      }//electron loop 

    }//event loop

    //Close the file
    file->Close(); 

    //Make the plot
    PlotMaker2D(plot, "--outputName proposal.pdf --Xaxis dR --Yaxis dPT --setTitle reco vs. gen electrons --noOverflow"); 

    //Print the mismatch rate
    cout << endl << "Fraction of reco electrons matched to something bad: " << 100.0*bad/(good+bad+nomatch) << "%" <<  endl;
    cout << "  --> Fraction of these electrons with an electron within 0.003: " << 100.0*yes/(yes+no) << "%" <<  endl;
    cout << "  --> Fraction of these electrons with pT correct to within 10\%: " << 100.0*classa/(classa+classb) << "%" <<  endl;
    cout << "Fraction of reco electrons not matched to anything: " << 100.0*nomatch/(good+bad+nomatch) << "%" << endl << endl;

  }//file loop

}

pair <int, int> LesyasLepMotherID(Lep lep){
  int id_reco = lep.pdgId();  
  int idx_reco = lep.idx(); 

  //First, see if CMS3 matches the particle to the right type.  If it does, just use that
  int gen_idx = tas::els_mc3idx().at(idx_reco); 
  int ourID = tas::genps_id().at(gen_idx);
  if (abs(ourID) == abs(id_reco)) return make_pair(lepMotherID(lep), ourID);

  //Otherwise, have to loop over gen using Lesya's method
  int idx = -1;
  int id = -1; 
  bool goodOne = false;
  float dR_best = 1000;
  for (unsigned int iGen = 0; iGen < tas::genps_p4().size(); iGen++){
    float dR = DeltaR(tas::genps_p4().at(iGen), tas::els_p4().at(idx_reco));  
    //First see if this one is closer than alternatives
    if (dR < dR_best && goodOne == false){ 
      dR_best = dR; 
      idx = iGen;
      id = tas::genps_id().at(iGen); 
    }
    //Now regardless of that, see if we can get a "good" match
    if (dR > 0.2) continue;
    if (abs(tas::genps_id().at(iGen)) != abs(id_reco)) continue;
    if (abs(tas::genps_status().at(iGen)) != 1) continue; 
    if (dR < dR_best || !goodOne){
      goodOne = true;
      dR_best = dR; 
      idx = iGen;
      id = tas::genps_id().at(iGen); 
    }
  }

  //Mother & Grandma ID
  int mother_id = tas::genps_id_mother().at(idx);
  int grandma_id = tas::genps_id_mother().at(tas::genps_idx_mother().at(idx));  

  //Now we have the closest gen match, see where we are
  if (abs(id) != abs(id_reco)) return make_pair(0, id); 
  else if (tas::evt_isRealData()) return make_pair(1, id);
  else if (tas::genps_isPromptFinalState().at(idx) || tas::genps_isHardProcess().at(idx) || ((abs(id) == abs(id_reco) || abs(id) == abs(id_reco)) && (abs(mother_id) == 24 || abs(mother_id) == 23 || abs(mother_id) == 1000024 || (abs(mother_id) == 15 && (abs(grandma_id) == 24 || abs(grandma_id) == 23 || abs(grandma_id) == 1000024))))){
    if (sgn(id_reco) == sgn(id)) return make_pair(1, id);
    else return make_pair(2, id);
  }
  else if ((abs(id) == abs(id_reco) || abs(id) == abs(id_reco)) && idIsBeauty(mother_id)) return make_pair(-1, id);
  else if ((abs(id) == abs(id_reco) || abs(id) == abs(id_reco)) &&  idIsCharm(mother_id)) return make_pair(-2, id);
  return make_pair(0, id);

}

void study3(){

  //Set up chain
  TChain *chain = new TChain("Events"); 
  if (sample == T1TTTT) chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root"); 
  if (sample == WZ)     chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root"); 
  if (sample == WG)     chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-12/merged_ntuple_1.root"); 

  //nEvents in chain
  unsigned int nEventsProcessed = 0;
  unsigned int nEventsTotal = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    //Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    cms3.Init(tree);

    //Number of events to do
    mode_s mode = mode_in; 
    int nEventsToDo = (mode == NORMAL ? tree->GetEntriesFast() : 1000);  

    //Loop over Events in current file
    for (int eventAG = 0; eventAG < nEventsToDo; eventAG++){

      //Load in the current event
      cms3.GetEntry(eventAG);

      //Progress Bar
      CMS3::progress(nEventsProcessed, nEventsTotal);
      nEventsProcessed++;

      //Loop through all the electrons particles
      for (unsigned int iElec = 0; iElec < tas::els_p4().size(); iElec++){

        //Load variables
        int event  = tas::evt_event(); 
        float pt   = tas::els_p4().at(iElec).pt();
        float aeta = fabs(tas::els_p4().at(iElec).eta()); 

        //Reject events with pT < 5 or |eta| > 2.5
        if (pt < 5) continue;
        if (aeta > 2.5) continue; 

        //Reject events that fail ID
        if (!isLooseElectronPOG(iElec)) continue; 

        //Find gen idx of electron
        int gen_idx = tas::els_mc3idx().at(iElec); 

        //Find what we match it to and what they match it to
        int us = tas::genps_id().at(gen_idx);
        int them = LesyasLepMotherID(Lep(-11*tas::els_charge().at(iElec), iElec)).second; 

        cout << -11*tas::els_charge().at(iElec) << " " << us << " " << them << endl;


      }//electron loop 

    }//event loop

    //Close the file
    file->Close(); 

  }//file loop

}
