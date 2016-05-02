#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "../classFiles/v8.01/SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"
#include "../CORE/Tools/dorky/dorky.h"

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

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

  // Make chains, histograms
  TChain* chain = new TChain("t");
  // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/synch_ttw_1.root"); 
  chain->Add("synch_ttw_1.root"); 
  // chain->Add("synch_ttw_1_noiso.root"); 

  // Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  // Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // Set up output text file
  ofstream textfile_global, textfile_leptons, textfile_jets;
  // textfile_global.open("ucsx_TTW_76X_HighHigh_dump_global.txt");
  // textfile_leptons.open("ucsx_TTW_76X_HighHigh_dump_leptons.txt");
  // textfile_jets.open("ucsx_TTW_76X_HighHigh_dump_jets.txt");
  textfile_global.open("unsorted_global.txt");
  textfile_leptons.open("unsorted_leptons.txt");
  textfile_jets.open("unsorted_jets.txt");

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
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
       // && ss::hyp_class() != 5
       && ss::hyp_class() != 6
       ) continue;

      // if( ss::hyp_class() == 6) {
      //     // if it's a class 6, ignore events that don't have one lepton being tight and the other loose
      //     if (!((ss::lep1_tight() == 1 && ss::lep2_fo() == 0) || (ss::lep2_tight() == 1 && ss::lep1_fo() == 0))) {
      //         continue;
      //     }
      // }



      // Ignore events with <2 jets
      if (ss::njets() < 2) continue;

      // Figure out region
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int SR = max(0, signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()));
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      
      // HH has 32 SRs and HL has 26, so add to unify the SR nums
      if(categ == HighLow && SR > 0) SR += 32;
      if(categ == LowLow  && SR > 0) SR += 32+26;

      // if (ss::event() == 150146 || ss::event() == 151479 || ss::event() == 151939 || ss::event() == 151964 || ss::event() == 152103 || ss::event() == 152230 || ss::event() == 152736 || ss::event() == 156134 || ss::event() == 156207 || ss::event() == 156249 || ss::event() == 156255 || ss::event() == 157039 || ss::event() == 157076 || ss::event() == 157425 || ss::event() == 157428 || ss::event() == 157511 || ss::event() == 157547 || ss::event() == 158166 || ss::event() == 158332 || ss::event() == 158368 || ss::event() == 158453 || ss::event() == 158536 || ss::event() == 158546 || ss::event() == 159762 || ss::event() == 160661 || ss::event() == 160687 || ss::event() == 160717 || ss::event() == 160733 || ss::event() == 160806 || ss::event() == 160909 || ss::event() == 180191 || ss::event() == 180281 || ss::event() == 180401 || ss::event() == 181179 || ss::event() == 181441 || ss::event() == 204416 || ss::event() == 204517 || ss::event() == 204520 || ss::event() == 204584 || ss::event() == 206892 || ss::event() == 222520 || ss::event() == 224153 || ss::event() == 224256 || ss::event() == 224364 || ss::event() == 226021 || ss::event() == 226562 || ss::event() == 227632 || ss::event() == 227744 || ss::event() == 227953 || ss::event() == 228019 || ss::event() == 228514 || ss::event() == 228567 || ss::event() == 231162 || ss::event() == 233426 || ss::event() == 233442 || ss::event() == 233524 || ss::event() == 233679 || ss::event() == 233697 || ss::event() == 234064 || ss::event() == 235187 || ss::event() == 235296 || ss::event() == 236558 || ss::event() == 236696 || ss::event() == 236703 || ss::event() == 237093 || ss::event() == 237117 || ss::event() == 237170 || ss::event() == 237424 || ss::event() == 237542 || ss::event() == 237610 || ss::event() == 237630 || ss::event() == 237726 || ss::event() == 247598 || ss::event() == 247841 || ss::event() == 251994 || ss::event() == 252058 || ss::event() == 252187 || ss::event() == 253990 || ss::event() == 254127 || ss::event() == 254137 || ss::event() == 254213 || ss::event() == 257988 || ss::event() == 258082 || ss::event() == 258954 || ss::event() == 259118 || ss::event() == 259140 || ss::event() == 259443 || ss::event() == 260319 || ss::event() == 260580 || ss::event() == 260597 || ss::event() == 260630 || ss::event() == 260632 || ss::event() == 261471 || ss::event() == 261496 || ss::event() == 261695 || ss::event() == 261728 || ss::event() == 261792 || ss::event() == 262239 || ss::event() == 262263 || ss::event() == 262273 || ss::event() == 362249 || ss::event() == 362339 || ss::event() == 362458 || ss::event() == 362652 || ss::event() == 362864 || ss::event() == 424056 || ss::event() == 424159 || ss::event() == 424581 || ss::event() == 424626) {
      //     std::cout << ss::event() << " " << ss::hyp_class() << " " << SR << " " << std::endl;
      // }



      // if ((categ == HighHigh || categ == HighLow || categ == LowLow) && BR >= 0 && SR >= 0) {


          textfile_global << Form("%1d%9d%12d\t%2d\t%+2d %5.1f\t%+2d%5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d\n", 
                  ss::run(), ss::lumi(), ss::event(), 
                  ss::nVetoElectrons7()+ss::nVetoMuons5(),
                  ss::lep1_id(), ss::lep1_p4().pt(), 
                  ss::lep2_id(), ss::lep2_p4().pt(), 
                  ss::njets(), ss::nbtags(), ss::met(), ss::ht(), SR);


          textfile_leptons << Form("%1d%9d%12d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%1d\t%1d\t%1d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%1d\t%1d\t%1d\n",
                  ss::run(), ss::lumi(), ss::event(), 
                  ss::lep1_p4().pt(),
                  ss::lep1_p4().eta(),
                  ss::lep1_p4().phi(),
                  ss::lep1_id(),
                  ss::lep1_coneCorrPt(),
                  ss::lep1_dxyPV(),
                  ss::lep1_dZ(),
                  ss::lep1_sip(),
                  ss::lep1_miniIso(),
                  ss::lep1_p4().pt()/ss::lep1_closeJet().pt(), // pt ratio
                  ss::lep1_ptrel_v1(),
                  (std::abs(ss::lep1_id())==11)?ss::lep1_MVA():0,
                  (std::abs(ss::lep1_id())==11)?ss::lep1_el_conv_vtx_flag():0, // els_conv_vtx_flag().at(elIdx) in CMS3
                  (std::abs(ss::lep1_id())==11)?1-ss::lep1_el_exp_innerlayers():0,
                  (std::abs(ss::lep1_id())==11)?ss::lep1_el_threeChargeAgree():ss::lep1_mu_ptErr(),
                  MCclass(ss::lep1_motherID()),
                  ss::lep2_p4().pt(),
                  ss::lep2_p4().eta(),
                  ss::lep2_p4().phi(),
                  ss::lep2_id(),
                  ss::lep2_coneCorrPt(),
                  ss::lep2_dxyPV(),
                  ss::lep2_dZ(),
                  ss::lep2_sip(),
                  ss::lep2_miniIso(),
                  ss::lep2_p4().pt()/ss::lep2_closeJet().pt(),
                  ss::lep2_ptrel_v1(),
                  (std::abs(ss::lep2_id())==11)?ss::lep2_MVA():0,
                  (std::abs(ss::lep2_id())==11)?ss::lep2_el_conv_vtx_flag():0,
                  (std::abs(ss::lep2_id())==11)?1-ss::lep2_el_exp_innerlayers():0,
                  (std::abs(ss::lep2_id())==11)?ss::lep2_el_threeChargeAgree():ss::lep2_mu_ptErr(),
                  MCclass(ss::lep2_motherID()));

          int idx_l = 0;
          int idx_t = 1;
          if(ss::jet_pt().at(1) > ss::jet_pt().at(0)) {
              idx_l = 1;
              idx_t = 0;
          }
          textfile_jets << Form("%1d%9d%12d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\n",
                  ss::run(), ss::lumi(), ss::event(), 
                  ss::jet_pt().at(idx_l),
                  // ss::jets().at(0).pt()*ss::jets_undoJEC().at(0)*ss::jets_JEC().at(0),
                  ss::jets().at(idx_l).eta(),
                  ss::jets().at(idx_l).phi(),
                  ss::jets_disc_ivf().at(idx_l), // "jet1_btagCSVmva"
                  ss::jets_disc().at(idx_l) > 0.800, // "jet1_btagCSVMedium". medium WP
                  ss::jet_pt().at(idx_t),
                  // ss::jets().at(1).pt()*ss::jets_undoJEC().at(1)*ss::jets_JEC().at(1),
                  ss::jets().at(idx_t).eta(),
                  ss::jets().at(idx_t).phi(),
                  ss::jets_disc_ivf().at(idx_t),
                  ss::jets_disc().at(idx_t) > 0.800);

      // }

    }//event loop
  }//file loop
  
  //Close output text file
  textfile_global.close();
  textfile_leptons.close();
  textfile_jets.close();

  //Sort the files
  system("sort -n -k1 -k2 -k3 unsorted_leptons.txt > ucsx_TTW_76X_dump_leptons.txt");
  system("sort -n -k1 -k2 -k3 unsorted_jets.txt > ucsx_TTW_76X_dump_jets.txt");
  system("sort -n -k1 -k2 -k3 unsorted_global.txt > ucsx_TTW_76X_dump_global.txt");
  system("rm unsorted*.txt"); 


}
