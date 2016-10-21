#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "../classFiles/v8.05/SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"
#include "../CORE/Tools/dorky/dorky.h"

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

bool jetptsort (int i, int j) { return ss::jet_pt().at(i)>ss::jet_pt().at(j); }

TH1D * evtCounter = new TH1D("","",1000,0,1000); 
map<TString, int> evtBinMap;
int evtBin = 0;
void initCounter() {
    evtCounter = new TH1D("","",1000,0,1000); 
    evtCounter->Sumw2();
    evtBinMap.clear();
}
void addToCounter(TString name, double weight=1.0) {
    if(evtBinMap.find(name) == evtBinMap.end() ) {
        evtBinMap[name] = evtBin;
        evtBin++;
    }
    evtCounter->Fill(evtBinMap[name], weight);
}
void printCounter(bool file = false) {
    ofstream outfile;
    if(file) outfile.open("counter.txt");
    cout << string(30, '-') << endl << "Counter totals: " << endl;
    for(map<TString,int>::iterator it = evtBinMap.begin(); it != evtBinMap.end(); it++) {
        int iBin = (it->second)+1;
        printf("%-15s %6.2f %6.2f\n",
                (it->first).Data(),
                evtCounter->GetBinContent(iBin),
                evtCounter->GetBinError(iBin) );
        if(file) outfile << (it->first).Data() << "  " << evtCounter->GetBinContent(iBin) << "  " << evtCounter->GetBinError(iBin) << endl;
    }
    cout << string(30, '-') << endl;
    if(file) outfile.close();
    if(file) cout << "Wrote counter to counter.txt" << endl;
}

int MCclass(int lepMotherID) {
    // map lepMotherID_v2 to sync format
    // prompt -> 0
    // prompt but charge flip -> 1
    // light flavour fake -> 2
    // heavy flavor fake -> 3
    // isolated photon conversion -> 4
    switch(lepMotherID) {
        case 1: return 0;
        case 2: return 1;

        case -4: return 2; 
        case -2: return 3;
        case -1: return 3;

        case -3: return 4;

        case 0: return -1;
        default: return -1;
    }
}

void yields(){

    HLTEfficiency HLTEff("../hlt/HLT_Efficiencies_7p65fb_2016.root");

  // Make chains, histograms
  TChain* chain = new TChain("t");

  bool doSingleFile = true;

  if(doSingleFile) {
      chain->Add("synch_ttw_80_1.root"); 
      // chain->Add("../batch/synch_ttw_80_1.root"); 
  } else {
      chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.01-sync/DY_high.root");
      chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.01-sync/QQWW.root");
      chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.01-sync/TTBAR.root");
      chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.01-sync/TTW.root");
      chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.01-sync/WZ.root");
      chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.01-sync/TG.root");
  }


  // Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  // Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // Set up output text file
  ofstream textfile_global, textfile_leptons, textfile_jets, textfile_global_veto;
  if(doSingleFile) {
      textfile_global.open("unsorted_global.txt");
      // textfile_leptons.open("unsorted_leptons.txt");
      // textfile_jets.open("unsorted_jets.txt");
      // textfile_global_veto.open("unsorted_global_veto.txt");
  }

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TString filename(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    
      //Get Event Content
      samesign.GetEntry(ievent);
      nEventsTotal++;

      // Progress
      // SSAG::progress(nEventsTotal, nEventsChain);

      // Reject non-SS
      if (
          ss::hyp_class() != 3 
       // && ss::hyp_class() != 6 // note that class 6 must require that both leptons are tight and are SS (but fail the zveto)
       ) continue;

      // Ignore events with <2 jets
      if (ss::njets() < 2) continue;

      // Figure out region
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (BR<0) continue;

      // HH has 32 SRs and HL has 26, so add to unify the SR nums
      if(categ == HighLow && SR > 0) SR += 32;
      if(categ == LowLow  && SR > 0) SR += 32+26;


      // Now we have made all baseline cuts
      if(doSingleFile) {
          textfile_global << Form("%1d%9d%12d\t%+2d %5.1f\t%+2d %5.1f\t%2.4f\t%2.4f\t%2.4f\t%2.4f\t%2.4f\n",
              ss::run(), ss::lumi(), (int)ss::event(), 
              ss::lep1_id(), ss::lep1_p4().pt(), 
              ss::lep2_id(), ss::lep2_p4().pt(), 
              ss::weight_btagsf(), getTruePUw(ss::trueNumInt()[0]),
              HLTEff.getEfficiency(ss::lep1_p4().pt(),ss::lep1_p4().eta(), ss::lep1_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::lep2_id(), ss::ht(), 0),
              leptonScaleFactor(ss::lep1_id(),  ss::lep1_p4().pt(),  ss::lep1_p4().eta(),  ss::ht()),
              leptonScaleFactor(ss::lep2_id(),  ss::lep2_p4().pt(),  ss::lep2_p4().eta(),  ss::ht())
          );

        
        
          for (unsigned int ib = 0; ib < ss::btags_eff().size(); ib++) {
          textfile_global << Form("%1d%9d%12d\tbpt,beff,bsf\t\t%2.4f\t%2.4f\t%2.4f\n",
              ss::run(), ss::lumi(), (int)ss::event(), 
              ss::btags_effpt()[ib],
              ss::btags_eff()[ib],
              ss::btags_sf()[ib]);
          }
      }

      // // textfile_leptons << Form("%1d%9d%12d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.3f\t%1d\t%1d\t%1d\t%1d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.3f\t%1d\t%1d\t%1d\t%1d\n",
      // if(doSingleFile)
      //     textfile_leptons << Form("%1d%9d%12d\t%5.3f\t%5.3f\t%5.3f\t%+2d\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%1d\t%1d\t%1d\t%1d\t%5.3f\t%5.3f\t%5.3f\t%+2d\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%1d\t%1d\t%1d\t%1d\t%5.3f\t%5.3f\n",
      //         ss::run(), ss::lumi(), (int)ss::event(), 
      //         ss::lep1_p4().pt(),
      //         ss::lep1_p4().eta(),
      //         ss::lep1_p4().phi(),
      //         ss::lep1_id(),
      //         ss::lep1_coneCorrPt(),
      //         ss::lep1_dxyPV(),
      //         ss::lep1_dZ(),
      //         ss::lep1_sip(),
      //         ss::lep1_miniIso(),
      //         ss::lep1_p4().pt()/ss::lep1_closeJet().pt(), // pt ratio
      //         ss::lep1_ptrel_v1(),
      //         (std::abs(ss::lep1_id())==11)?ss::lep1_MVA_miniaod():0,
      //         (std::abs(ss::lep1_id())==11)?ss::lep1_el_conv_vtx_flag():0, // els_conv_vtx_flag().at(elIdx) in CMS3
      //         (std::abs(ss::lep1_id())==11)?1-ss::lep1_el_exp_innerlayers():0,
      //         (std::abs(ss::lep1_id())==11)?ss::lep1_el_threeChargeAgree():ss::lep1_mu_ptErr(),
      //         MCclass(ss::lep1_motherID()),
      //         ss::lep2_p4().pt(),
      //         ss::lep2_p4().eta(),
      //         ss::lep2_p4().phi(),
      //         ss::lep2_id(),
      //         ss::lep2_coneCorrPt(),
      //         ss::lep2_dxyPV(),
      //         ss::lep2_dZ(),
      //         ss::lep2_sip(),
      //         ss::lep2_miniIso(),
      //         ss::lep2_p4().pt()/ss::lep2_closeJet().pt(),
      //         ss::lep2_ptrel_v1(),
      //         (std::abs(ss::lep2_id())==11)?ss::lep2_MVA_miniaod():0,
      //         (std::abs(ss::lep2_id())==11)?ss::lep2_el_conv_vtx_flag():0,
      //         (std::abs(ss::lep2_id())==11)?1-ss::lep2_el_exp_innerlayers():0,
      //         (std::abs(ss::lep2_id())==11)?ss::lep2_el_threeChargeAgree():ss::lep2_mu_ptErr(),
      //         MCclass(ss::lep2_motherID()),
      //         ss::lep1_tkIso(),
      //         ss::lep2_tkIso()
      //             );

      // // reorder jets using corrected pt
      // // yes, there is one more case (where 2 overtakes 0), but that never happened in the synch
      // int idx_l = 0;
      // int idx_t = 1;
      // if(ss::jet_pt().at(1) > ss::jet_pt().at(0)) {
      //     idx_l = 1;
      //     idx_t = 0;
      // }
      // if(ss::jet_pt().size() > 2) {
      //     if(ss::jet_pt().at(2) > ss::jet_pt().at(1)) {
      //         idx_t = 2;
      //     }
      // }
      // // textfile_jets << Form("%1d%9d%12d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\n",
      // if(doSingleFile)
      //     textfile_jets << Form("%1d%9d%12d\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%1d\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%1d\n",
      //         ss::run(), ss::lumi(), (int)ss::event(), 
      //         ss::jet_pt().at(idx_l),
      //         ss::jets().at(idx_l).eta(),
      //         ss::jets().at(idx_l).phi(),
      //         ss::jets_disc_ivf().at(idx_l), // "jet1_btagCSVmva"
      //         ss::jets_disc_ivf().at(idx_l) > 0.800, // "jet1_btagCSVMedium". medium WP
      //         ss::jet_pt().at(idx_t),
      //         ss::jets().at(idx_t).eta(),
      //         ss::jets().at(idx_t).phi(),
      //         ss::jets_disc_ivf().at(idx_t),
      //         ss::jets_disc_ivf().at(idx_t) > 0.800
      //         );


      // if(ss::hyp_class() != 3) continue;
      // if(BR < 0) continue;
      // // if(!ss::fired_trigger()) continue;
      // // Now we have made all baseline cuts
      // if(doSingleFile) {
      //     textfile_global_veto << Form("%1d%9d%12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d\n", 
      //         ss::run(), ss::lumi(), (int)ss::event(), 
      //         ss::nVetoElectrons7()+ss::nVetoMuons5(),
      //         ss::lep1_id(), ss::lep1_p4().pt(), 
      //         ss::lep2_id(), ss::lep2_p4().pt(), 
      //         ss::njets(), ss::nbtags(), ss::met(), ss::ht(), SR);
      // } else {
      //     TString lep_str = abs(ss::lep1_id()) == 11 ? "ee" : "mumu";
      //     if(abs(ss::lep1_id()) != abs(ss::lep2_id())) lep_str = "emu";
      //     TString BR_str = Form("%i", BR);
      //     addToCounter(filename + "_" + BR_str + "_" + lep_str, ss::scale1fb());
      //     // addToCounter(filename + "_" + BR_str + "_" + lep_str);
      // }


    }//event loop
  }//file loop
  
  //Close output text file
  if(doSingleFile) {
      textfile_global.close();
      // textfile_leptons.close();
      // textfile_jets.close();
      // textfile_global_veto.close();

      //Sort the files
      // system("sort -n -k1 -k2 -k3 unsorted_leptons.txt > ucsx_TTW_80X_dump_leptons.txt");
      // system("sort -n -k1 -k2 -k3 unsorted_jets.txt > ucsx_TTW_80X_dump_jets.txt");
      // system("sort -n -k1 -k2 -k3 unsorted_global.txt > ucsx_TTW_80X_dump_global.txt");
      // system("sort -n -k1 -k2 -k3 unsorted_global_veto.txt > ucsx_TTW_80X_dump_global_veto.txt");
      system("sort -n -k1 -k2 -k3 unsorted_global.txt > ucsx_TTW_dump_postICHEP.txt");
      system("rm unsorted*.txt"); 
  } else {
      printCounter(true);
  }


}
