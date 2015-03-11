// C++
#include <iostream>
#include <vector>
#include <unistd.h> //isatty
#include <sys/types.h>
#include <sys/stat.h>

// ROOT
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TMath.h"

#include "./looper.h"
#include "./helper_tests.h"
#include "./helper_babymaker.h"
#include "./tools.h"

// CMS3
#include "./CORE/CMS3.h"

using namespace tas;
using namespace std;

int looper::ScanChain( TChain* chain, TString prefix, TString postfix, bool isData, TString whatTest, int nEvents, bool usePtRel, vector<int> evtToDebug) {

  makebaby       = 1;
  makehist       = 0;
  maketext       = 0;

  bool makeQCDtest    = 0;
  bool makeDYtest     = 0;
  bool makeWZtest     = 0;
  bool makeSSskim     = 0;
  bool makeQCDskim    = 0;
  bool makeSyncTest   = 0;
  if (whatTest=="QCDtest") makeQCDtest    = 1;
  if (whatTest=="DYtest" ) makeDYtest     = 1;
  if (whatTest=="WZtest" ) makeWZtest     = 1;
  if (whatTest=="SSskim" ){makeSSskim     = 1; makehist = 0;}
  if (whatTest=="QCDskim") makeQCDskim    = 1;
  if (whatTest=="SyncTest")makeSyncTest   = 1;
  if (whatTest=="MakeBaby"){makebaby   = 1; makehist = 0;}

  cout << "Processing " << prefix << " " << postfix << " " << whatTest << endl;

  bool debug = 0;  
  if (debug) cout << "running with flags: makeQCDtest=" << makeQCDtest << " makeDYtest=" << makeDYtest 
		  << " makeWZtest=" << makeWZtest << " makeSSskim=" << makeSSskim << " makeQCDskim=" << makeQCDskim << " makeSyncTest=" << makeSyncTest
		  << " makebaby=" << makebaby << " makehist=" << makehist << " maketext=" << maketext
		  << endl;

  if (postfix!="") postfix = "_"+postfix;
  babyMaker* bm=0;
  if (makebaby) {
    bm = new babyMaker(debug);
    bm->MakeBabyNtuple( Form( "%s%s", prefix.Data(), postfix.Data() ), usePtRel );
  }
  if (makehist) CreateOutputFile( Form( "%s_histos%s.root", prefix.Data(), postfix.Data() ) );

  TFile* fr_file=0;
  if (!makeDYtest&&!makeQCDtest&&!makeSSskim&&!makeQCDskim&&!makebaby) 
    fr_file=TFile::Open("fakeRates_qcd_pt-50to170.root");

  // File Loop
  if( nEvents == -1 ) nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("Events");

    if (debug) cout << "processing file: " << currentFile->GetTitle() << endl;

    //Skimmed output file - needs to be before cms3.Init(tree)
    TFile *skim_file = 0;
    TTree* skim_tree = 0;
    if (makeSSskim || makeQCDskim) {
      TString skim_file_name = TString(currentFile->GetTitle());
      if (makeSSskim) {
	skim_file_name.ReplaceAll(".root","_skimSS_"+prefix+".root");
	skim_file_name.Remove(0,skim_file_name.Last('/'));
	struct stat st;
	stat(prefix+"_skimSS/",&st);
	if (S_ISDIR(st.st_mode)==0) system("mkdir "+prefix+"_skimSS/");
	skim_file_name.Prepend("./"+prefix+"_skimSS/");
      } else if (makeQCDskim) {
	skim_file_name.ReplaceAll(".root","_skimQCD.root");
	skim_file_name.Remove(0,skim_file_name.First('Q'));
	skim_file_name.Remove(skim_file_name.First('T')-1,skim_file_name.Last('/')-skim_file_name.First('T')+1);
      }
      skim_file = new TFile(skim_file_name,"recreate");
      //locally
      skim_tree = (TTree*) tree->CloneTree(0, "fast");
      skim_tree->SetDirectory(skim_file);
      cms3.Init(skim_tree);
    }

    // Event Loop
    cms3.Init(tree);
    unsigned int nEvents = tree->GetEntries();
    bool newfile = true;

    for(unsigned int event = 0; event < nEvents; ++event) {
    
      // Get Event Content
      cms3.GetEntry(event);
      ++nEventsTotal;

      // progress feedback to user
      CMS3::progress(nEventsTotal, nEventsChain);

      if (makebaby) bm->InitBabyNtuple();

      //-------- DO THE ANALYSIS HERE --------//

      //print info
      if (maketext) printEvent();

      float lumi = 10.;

      if (evtToDebug.size()>0) {
	bool pass = false;
	for (unsigned int evt=0;evt<evtToDebug.size();evt++) {
	  if (evt_event()==abs(evtToDebug[evt])) {
	    pass = true;
	    break;
	  }
	}
	if (!pass) continue;
	std::cout << "event =" << evt_event() << std::endl;
	cout << "file=" << currentFile->GetTitle() << " run=" << run_ << " evt=" << evt_ << endl;      
	debug=1;
      }

      //fill baby
      if (makebaby) {	
	bm->ProcessBaby(usePtRel);
	continue;
      }

      run_   = evt_run();
      ls_    = evt_lumiBlock();
      evt_   = evt_event();
      weight_ = isData ? 1. : lumi*evt_scale1fb();
      //if (prefix=="wj") weight_*=30;//fixme using only a subset of events
      //if (prefix=="dy") weight_*=25;//fixme using only a subset of events
      if (makeSyncTest) weight_ = 1.;

      if (newfile) {
	if (debug) cout << "weight=" << weight_ << endl;
	newfile = false;
      }

      if (debug) cout << "file=" << currentFile->GetTitle() << " run=" << run_ << " evt=" << evt_ << endl;

      if (mus_dxyPV().size()!=mus_dzPV().size()) {
	cout << "run=" << run_ << " evt=" << evt_ << " mus_dxyPV().size()=" << mus_dxyPV().size() << " mus_dzPV().size()=" << mus_dzPV().size() << endl;
	continue;
      }

      //make sure there are genuine SS leptons for cut_flow_ss
      int qp=0,qn=0;
      int qpe=0,qne=0;
      int qpm=0,qnm=0;
      for (unsigned int gp=0;gp<genps_id().size();++gp) {
	int pdgid = abs(genps_id()[gp]);
	if (pdgid!=13 && pdgid!=11) continue;
	if (genps_id_mother()[gp]!=23 && abs(genps_id_mother()[gp])!=24) continue;
	if (genps_status()[gp]!=1) continue;//is this needed?
	if (fabs(genps_p4()[gp].eta())>2.4) continue;
	if (genps_p4()[gp].pt()<5) continue;
	if (pdgid==11 && genps_p4()[gp].pt()<10) continue;
	if (genps_charge()[gp]==1 ) qp++;
	if (genps_charge()[gp]==-1) qn++;
	if (pdgid==11 && genps_charge()[gp]==1 ) qpe++;
	if (pdgid==11 && genps_charge()[gp]==-1) qne++;
	if (pdgid==13 && genps_charge()[gp]==1 ) qpm++;
	if (pdgid==13 && genps_charge()[gp]==-1) qnm++;
      }
      bool isGenSS = false;
      bool isGenSSee = false;
      bool isGenSSmm = false;
      if (qp>1 || qn>1) isGenSS = true;
      if (qpe>1 || qne>1) isGenSSee = true;
      if (qpm>1 || qnm>1) isGenSSmm = true;

      if (makeSSskim&&isGenSS) {
	  cms3.LoadAllBranches();
	  skim_file->cd(); 
	  skim_tree->Fill();
	  continue;	
      }

      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,0,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,0,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,0,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,0,weight_);

      //start selection
      //vertex
      //fixme: check leptons are from this vertex!
      if (firstGoodVertex()<0) {
        if (debug) cout << "does not pass firstGoodVertex" << endl;
        continue;
      }
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,1,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,1,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,1,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,1,weight_);

      //veto leptons
      if (debug) cout << "vetoleps_noiso" << endl;
      vector<Lep> vetoleps_noiso;
      for (unsigned int elidx=0;elidx<els_p4().size();++elidx) {
	//medium electron selection
	if (isGoodVetoElectronNoIso(elidx)==0) continue;
	Lep vetoel(-1*els_charge().at(elidx)*11,elidx);
	vetoleps_noiso.push_back(vetoel);
      }
      for (unsigned int muidx=0;muidx<mus_p4().size();++muidx) {
	//veto muon selection
	if (isGoodVetoMuonNoIso(muidx)==0) continue;
	Lep vetomu(-1*mus_charge().at(muidx)*13,muidx);
	vetoleps_noiso.push_back(vetomu);
      }
      if (debug) cout << "vetoleps" << endl;
      vector<Lep> vetoleps;
      for (unsigned int vl=0;vl<vetoleps_noiso.size();++vl) {
	if (isVetoLepton(vetoleps_noiso[vl].pdgId(),vetoleps_noiso[vl].idx(),false)==0) continue;//fixme: no ptrel in veto leptons for now
      	if (debug) cout << "veto lep id=" << vetoleps_noiso[vl].pdgId() << " pt=" << vetoleps_noiso[vl].pt() 
			<< " eta=" << vetoleps_noiso[vl].eta() << " phi=" << vetoleps_noiso[vl].p4().phi() << " q=" << vetoleps_noiso[vl].charge()<< endl;
      	vetoleps.push_back(vetoleps_noiso[vl]);
      }

      //fakable objects
      if (debug) cout << "fobs noiso" << endl;
      vector<Lep> fobs_noiso;
      for (unsigned int vl=0;vl<vetoleps_noiso.size();++vl) {
	if (isDenominatorLeptonNoIso(vetoleps_noiso[vl].pdgId(),vetoleps_noiso[vl].idx())==0) continue; 
        if (debug) cout << "fob noiso id=" << vetoleps_noiso[vl].pdgId() << " pt=" << vetoleps_noiso[vl].pt()
                        << " eta=" << vetoleps_noiso[vl].eta() << " phi=" << vetoleps_noiso[vl].p4().phi() << " q=" << vetoleps_noiso[vl].charge()<< endl;
        fobs_noiso.push_back(vetoleps_noiso[vl]);
      }
      if (debug) cout << "fobs" << endl;
      vector<Lep> fobs;
      for (unsigned int vl=0;vl<vetoleps_noiso.size();++vl) {
	if (isDenominatorLepton(vetoleps_noiso[vl].pdgId(),vetoleps_noiso[vl].idx(),usePtRel)==0) continue; 
      	if (debug) cout << "fob id=" << vetoleps_noiso[vl].pdgId() << " pt=" << vetoleps_noiso[vl].pt() 
			<< " eta=" << vetoleps_noiso[vl].eta() << " phi=" << vetoleps_noiso[vl].p4().phi() << " q=" << vetoleps_noiso[vl].charge()<< endl;
      	fobs.push_back(vetoleps_noiso[vl]);
      }

      //leptons
      if (debug) cout << "goodleps" << endl;
      vector<Lep> goodleps;
      for (unsigned int fo=0;fo<fobs.size();++fo) {
	if (isGoodLepton(fobs[fo].pdgId(),fobs[fo].idx(),usePtRel)==0) continue; 
      	if (debug) cout << "good lep id=" << fobs[fo].pdgId() << " idx=" << fobs[fo].idx() << " pt=" << fobs[fo].pt() 
			<< " eta=" << fobs[fo].eta() << " phi=" << fobs[fo].p4().phi() << " q=" << fobs[fo].charge()<< endl;
      	goodleps.push_back(fobs[fo]);
      }

      //jets, ht, btags
      if (debug) cout << "jets" << endl;
      int njets = 0;
      vector<Jet> jets;
      vector<Jet> alljets;
      int nbtag = 0;
      int nbtag_pt40 = 0;
      int nbtag_pt35 = 0;
      int nbtag_pt30 = 0;
      int nbtag_pt25 = 0;
      int nbtag_pt20 = 0;
      vector<Jet> btags;
      float ht=0;
      //fixme: should add corrections
      float drcut = 0.4;
      if (makeQCDtest) drcut = 1.0;
      if (debug) {
	for (unsigned int pfjidx=0;pfjidx<pfjets_p4().size();++pfjidx) {
	  Jet jet(pfjidx);
	  cout << "jet pt=" << jet.pt() << " eta=" << jet.eta() << " phi=" << jet.phi() << endl;
	}
      }
      for (unsigned int pfjidx=0;pfjidx<pfjets_p4().size();++pfjidx) {
        Jet jet(pfjidx);
        if (debug) cout << "jet pt=" << jet.pt() << " eta=" << jet.eta() << endl;
        if (fabs(jet.eta())>2.4) continue;
        if (debug) cout << "jet pass eta" << endl;
        if (isLoosePFJet(pfjidx)==false) continue;
        if (debug) cout << "jet pass loose pf id" << endl;
        //add pu jet id pfjets_pileupJetId()>????
        bool isLep = false;
        //fixme: should be checked agains fo or good leptons?
        if (makeQCDtest) {
          for (unsigned int fo=0;fo<fobs.size();++fo) {
            if (deltaR(jet.p4(),fobs[fo].p4())<drcut) {
              isLep =true;
              break;
            }
          }
        } else {
          for (unsigned int gl=0;gl<vetoleps.size();++gl) {//fixme not sure this is the best choice
	    Lep lep = vetoleps[gl];
	    if (lep.pt()<10) continue;
            if (deltaR(jet.p4(),lep.p4())<drcut) {
              isLep =true;
	      if (debug) cout << "jet overlapping with lepton" << endl;
              break;
            }
          }
        }
        if (isLep) continue;
        if (debug) cout << "jet pass lepton clean" << endl;
        float jetpt = jet.pt();
        if (jetpt>40.) {
          if (debug) cout << "jet pass pT cut" << endl;
          njets++;
          jets.push_back(jet);
          ht+=jetpt;
        }
	alljets.push_back(jet);
	if (jetpt>20. && jet.isBtag()) nbtag_pt20++;
	if (jetpt>25. && jet.isBtag()){nbtag_pt25++; btags.push_back(jet);}
	if (jetpt>30. && jet.isBtag()) nbtag_pt30++;
	if (jetpt>35. && jet.isBtag()) nbtag_pt35++;
	if (jetpt>40. && jet.isBtag()) nbtag_pt40++;
      }
      std::sort(jets.begin(),jets.end(),jetptsort);
      std::sort(btags.begin(),btags.end(),jetptsort);
      nbtag = nbtag_pt25;
      
      if (fobs_noiso.size()==0 && !makeDYtest) continue;
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,2,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,2,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,2,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,2,weight_);

      //write skim here (only qcd)
      if (makeQCDskim) {
	if (debug) cout << "qcd skim" << endl;
	if (fobs.size()!=0) {
	  cms3.LoadAllBranches();
	  skim_file->cd(); 
	  skim_tree->Fill();
	  continue;
	}
      }

      //met
      if (debug) cout << "met" << endl;
      float met = evt_pfmet();
      if (met<30. && ht<500. && !makeQCDskim && !makeQCDtest && !makeDYtest) continue;
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,3,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,3,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,3,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,3,weight_);

      if (makeQCDtest) {
	if (debug) cout << "qcdtest" << endl;
	tests::runQCDtest(this, weight_, fobs, goodleps, njets, met);
	continue;
      }

      if (makeDYtest) {
	if (debug) cout << "dytest" << endl;
	tests::runDYtest(this, weight_, vetoleps_noiso, fobs, goodleps, njets, met, ht, usePtRel);
	continue;
      }

      if (makeWZtest) {
	if (debug) cout << "WZtest" << endl;
	tests::runWZtest(this, weight_, vetoleps, fobs, goodleps, njets, met);
	continue;
      }

      vector<Lep> hypfobs = ( makeSSskim ? getBestSSLeps(fobs_noiso) : getBestSSLeps(fobs));
      if (debug) cout << "hypfobs size=" << hypfobs.size() << endl;
      if (hypfobs.size()<2) {
	if (debug) cout << "skip, hypfobs size=" << hypfobs.size() << endl;
	if (isGenSS) {
	  tests::testLepIdFailMode( this, weight_, fobs );
	}
	continue;
      }
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,4,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,4,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,4,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,4,weight_);

      vector<Lep> hypleps = getBestSSLeps(goodleps);
      if (debug) cout << "hypleps size=" << hypleps.size() << endl;

      DilepHyp hyp = (hypleps.size()==2 ? DilepHyp(hypleps[0],hypleps[1]) : DilepHyp(hypfobs[0],hypfobs[1]) );
      if (hyp.p4().mass()<8) {
	if (debug) cout<< "skip, hyp mass=" << hyp.p4().mass() <<endl;
	continue;
      }
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,5,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,5,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,5,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,5,weight_);

      unsigned int ac_base = analysisCategory(hyp.leadLep().pt(),hyp.traiLep().pt());
      passesBaselineCuts(njets, nbtag, met, ht, ac_base);
      if (ac_base==0) {
	if (debug) {
	  cout << "skip, not passing baseline cuts" << endl;
	  cout << "njets=" << njets << " nbtag=" << nbtag << " ht=" << ht << " met=" << met << endl;
	}
	continue;
      }
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,6,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,6,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,6,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,6,weight_);

      int br = baselineRegion(nbtag);
      unsigned int ac_sig = ac_base;
      passesSignalRegionCuts(ht, met, ac_sig);
      if (debug) cout << "ac_base=" << ac_base << " ac_sig=" << ac_sig << endl;
      int sr = ac_sig!=0 ? signalRegion(njets, nbtag, met, ht) : -1;

      //write skim here (only ss)
      if (makeSSskim) {
	if (debug) cout << "ss skim" << endl;
	if (isGenSS || hyp.charge()!=0) {
	  cms3.LoadAllBranches();
	  skim_file->cd(); 
	  skim_tree->Fill();
	}
	continue;
      }

      //3rd lepton veto
      if (debug) cout << "3rd lepton veto" << endl;
      bool leptonVeto = false;
      for (unsigned int gl=0;gl<hypleps.size();++gl) {
	for (unsigned int vl=0;vl<vetoleps.size();++vl) {
	  if ( hypleps[gl].pdgId()!=-vetoleps[vl].pdgId() ) continue; 
	  float mll = (hypleps[gl].p4()+vetoleps[vl].p4()).mass();
	  if (debug) cout << "test mll=" << mll << " l1id=" << hypleps[gl].pdgId() << " pt=" << hypleps[gl].pt() << " vlid=" << vetoleps[vl].pdgId() << " pt=" << vetoleps[vl].pt() << endl;
	  if ( (fabs(mll-91.)<15&&vetoleps[vl].pt()>10. ) || (mll<12.&&vetoleps[vl].pt()>5. ) ) {
	    if (debug) cout << "fail mll=" << mll << " l1id=" << hypleps[gl].pdgId() << " pt=" << hypleps[gl].pt() << " vlid=" << vetoleps[vl].pdgId() << " pt=" << vetoleps[vl].pt() << endl;
	    leptonVeto = true;
	    break;
	  }
	}
      }
      if (leptonVeto) {
	if (debug) cout<< "skip, 3rd lepton veto" << endl;	
	if (debug) cout<< "hyp leps id=" << hypleps[0].pdgId() << " pt=" << hypleps[0].pt() << " id=" << hypleps[1].pdgId() << " pt=" << hypleps[1].pt() << endl;
	continue;
      }
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,7,weight_);
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,7,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,7,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,7,weight_);

      //compute fake rate in ss ttbar
      if (hyp.charge()!=0 && prefix=="ttbar") {
	tests::computeFakeRateAndClosure( this, weight_, fobs, goodleps, fr_file );
      }

      bool ht400met120 = false;
      if (ht>400. || met>120.) ht400met120 = true;

      TString ll = abs(hyp.leadLep().pdgId())==13 ? "mu" : "el";
      TString lt = abs(hyp.traiLep().pdgId())==13 ? "mu" : "el";
      if (hyp.leadLep().pt()>ptCutHigh && hyp.traiLep().pt()>ptCutHigh) {
	if (debug) cout << "test pt rel" << endl;
	vector<Jet> lepjets;
	for (unsigned int pfjidx=0;pfjidx<pfjets_p4().size();++pfjidx) {
	  Jet jet(pfjidx);
	  if (fabs(jet.eta())>2.4) continue;
	  if (isLoosePFJet(pfjidx)==false) continue;
	  for (unsigned int vl=0;vl<vetoleps_noiso.size();++vl) {
	    Lep lep = vetoleps_noiso[vl];
	    if (deltaR(jet.p4(),lep.p4())<0.7) {
	      lepjets.push_back(jet);
	      if (debug) cout << "found lepjet inerfering with vl" << endl;
	    }
	  }
	}
	tests::testPtRel( this, weight_, hyp, lepjets, ll, lt );
      }

      if (hypleps.size()==2) {
	makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,8,weight_);//check that passes ID
	if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,8,weight_);
	if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,8,weight_);
	if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,8,weight_);
      }
      if (hypleps.size()!=2 || hypleps[0].pt()<ptCutLow || hypleps[1].pt()<ptCutLow) {
	if (debug) {
	  cout << "skip, not passing lepton cuts" << endl; 
	  cout << "fobs size = " << fobs.size() << " pdgids=" << fobs[0].pdgId() << ", " << fobs[1].pdgId() << endl;
	  if (hypleps.size()>0) cout << "Good leptons = " << hypleps.size() << " pdgids=" << hypleps[0].pdgId() << " pt=" << hypleps[0].pt() << " eta=" << hypleps[0].eta() << endl;
	  for (unsigned int fo=0;fo<fobs.size();++fo){
	    if (abs(fobs[fo].pdgId())!=13) continue;
	    cout << "fob pt=" << fobs[fo].pt() << " eta=" << fobs[fo].eta() << endl;
	    if (mus_p4().at(fobs[fo].idx()).pt()<ptCutLow) cout << "fail pt" << endl;
	    if (isFakableMuonNoIso(fobs[fo].idx())==0) cout << "fail FO" << endl;
	    if (fobs[fo].relIso03()>1.0) cout << "fail loose iso" << endl;
	    if (isGoodMuonNoIso(fobs[fo].idx())==0) cout << "fail tight id" << endl;
	    if (isGoodMuon(fobs[fo].idx())==0 ) cout << "fail tight iso, iso=" << fobs[fo].relIso03() << endl;
	  }
	}
	continue;
      }
      makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,9,weight_);//so this is pT
      if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,9,weight_);
      if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,9,weight_);
      if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,9,weight_);

      if (debug) {
	cout << "Good leptons = " << hypleps.size() << " pdgids=" << hypleps[0].pdgId() << ", " << hypleps[1].pdgId() << endl;
	cout << "Total charge = " << hyp.charge() << " m=" << hyp.p4().mass() << endl;
      }

      //fill histos
      if (makehist) {

	int type = -1;
	if (abs(hyp.traiLep().pdgId())==13 && abs(hyp.leadLep().pdgId())==13) type=0;
	if (abs(hyp.traiLep().pdgId())==13 && abs(hyp.leadLep().pdgId())==11) type=1;
	if (abs(hyp.traiLep().pdgId())==11 && abs(hyp.leadLep().pdgId())==13) type=2;
	if (abs(hyp.traiLep().pdgId())==11 && abs(hyp.leadLep().pdgId())==11) type=3;
	if (debug) cout << "type=" << type << endl;

	float dPhi1 = deltaPhi(hyp.leadLep().p4().phi(),evt_pfmetPhi());
	float mt1  = mt(hyp.leadLep().pt(),met,dPhi1);
	float dPhi2 = deltaPhi(hyp.traiLep().p4().phi(),evt_pfmetPhi());
	float mt2  = mt(hyp.traiLep().pt(),met,dPhi2);
	float dPhill = deltaPhi(hyp.p4().phi(),evt_pfmetPhi());
	float mtll = mt(hyp.p4().pt(),met,dPhill);

	float mtmin = min(mt1,mt2);
	float mtmin12ll = min(mtmin,mtll);

	tests::makeSRplots( this, weight_, TString("all"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );

	makeFillHisto1D<TH1F,float>("hyp_mt1","hyp_mt1",15,0,300,mt1,weight_);
	makeFillHisto1D<TH1F,float>("hyp_mt2","hyp_mt2",15,0,300,mt2,weight_);
	makeFillHisto1D<TH1F,float>("hyp_mtll","hyp_mtll",15,0,300,mtll,weight_);
	makeFillHisto1D<TH1F,float>("hyp_mtmin","hyp_mtmin",15,0,300,mtmin,weight_);
	makeFillHisto1D<TH1F,float>("hyp_mtmin12ll","hyp_mtmin12ll",15,0,300,mtmin12ll,weight_);

	makeFillHisto1D<TH1F,float>("hyp_cosdPhi1","hyp_cosdPhi1",20,-1,1,cos(dPhi1),weight_);
	makeFillHisto1D<TH1F,float>("hyp_cosdPhi2","hyp_cosdPhi2",20,-1,1,cos(dPhi2),weight_);
	makeFillHisto1D<TH1F,float>("hyp_cosdPhill","hyp_cosdPhill",20,-1,1,cos(dPhill),weight_);

	if (hyp.charge()!=0) {
	  //same sign

	  /*
	  //check the sync with alex
	  hyp_result_t best_hyp_info = chooseBestHyp();
	  //int hyp_class = best_hyp_info.hyp_class;
	  int best_hyp = best_hyp_info.best_hyp;
	  if (best_hyp<0)  chooseBestHyp(true);
	  assert(best_hyp>=0);
	  int lep1_id = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
	  int lep2_id = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
	  int lep1_idx = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
	  int lep2_idx = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
	  if (lep1_id!=hyp.leadLep().pdgId() || lep1_idx!=hyp.leadLep().idx() || lep2_id!=hyp.traiLep().pdgId() || lep2_idx!=hyp.traiLep().idx() ) {
	    cout << "L1 pdg=" << lep1_id << " idx=" << lep1_idx << endl;
	    cout << "L2 pdg=" << lep2_id << " idx=" << lep2_idx << endl;	  
	    chooseBestHyp(true);
	  }
	  assert(lep1_id==hyp.leadLep().pdgId());
	  assert(lep1_idx==hyp.leadLep().idx());
	  assert(lep2_id==hyp.traiLep().pdgId());
	  assert(lep2_idx==hyp.traiLep().idx());
	  */

	  //if ( (lepMotherID(hyp.leadLep())==2 && lepMotherID(hyp.traiLep())==1) || (lepMotherID(hyp.leadLep())==1 && lepMotherID(hyp.traiLep())==2) ) continue;//FIXME //don't count events that are prompt-prompt charge flips (estimated from data)
	  //if ( !((lepMotherID(hyp.leadLep())!=1 && lepMotherID(hyp.traiLep())==1) || (lepMotherID(hyp.leadLep())==1 && lepMotherID(hyp.traiLep())!=1)) ) continue;//FIXME test single fakes
	  makeFillHisto1D<TH1F,int>("cut_flow","cut_flow",50,0,50,10,weight_);
	  if (isGenSS) makeFillHisto1D<TH1F,int>("cut_flow_ss","cut_flow_ss",50,0,50,10,weight_);
	  if (isGenSSee) makeFillHisto1D<TH1F,int>("cut_flow_ssee","cut_flow_ssee",50,0,50,10,weight_);
	  if (isGenSSmm) makeFillHisto1D<TH1F,int>("cut_flow_ssmm","cut_flow_ssmm",50,0,50,10,weight_);

	  if (debug /*|| (br==30 && hyp.leadLep().pt()>25. && hyp.traiLep().pt()>25)*/) {
	    cout << endl << "NEW SS EVENT" << endl << endl;	  
	    cout << "lead lep id=" << hyp.leadLep().pdgId() << " pt=" << hyp.leadLep().pt() << " eta=" << hyp.leadLep().eta() << " p4=" << hyp.leadLep().p4() 
		 << " mcid=" << hyp.leadLep().mc_id() << " mcp4=" << hyp.leadLep().mc_p4() << " mother_id=" << hyp.leadLep().mc_motherid()
		 << " mc3idx=" << hyp.leadLep().mc3idx() << " mc3_id=" << hyp.leadLep().mc3_id() 
		 << " mc3_motheridx=" << hyp.leadLep().mc3_motheridx() << " mc3_mother_id=" << hyp.leadLep().mc3_motherid()
		 << " genps_id_mother()[hyp.leadLep().mc3_motheridx()]=" << genps_id_mother()[hyp.leadLep().mc3_motheridx()]
		 << endl;
	    cout << "trai lep id=" << hyp.traiLep().pdgId() << " pt=" << hyp.traiLep().pt() << " eta=" << hyp.traiLep().eta() << " p4=" << hyp.traiLep().p4() 
		 << " mcid=" << hyp.traiLep().mc_id() << " mcp4=" << hyp.traiLep().mc_p4()  << " mother_id=" << hyp.traiLep().mc_motherid()
		 << " mc3idx=" << hyp.traiLep().mc3idx() << " mc3_id=" << hyp.traiLep().mc3_id() 
		 << " mc3_motheridx=" << hyp.traiLep().mc3_motheridx() << " mc3_mother_id=" << hyp.traiLep().mc3_motherid()
		 << " genps_id_mother()[hyp.traiLep().mc3_motheridx()]=" << genps_id_mother()[hyp.traiLep().mc3_motheridx()]
		 << endl;
	    cout << "njets=" << njets << " nbtag=" << nbtag << " ht=" << ht << " met=" << met << endl;
	    cout << "weight=" << weight_ << endl;
	    cout << "BR" << br << " SR" << sr << endl;
	  }

	  tests::makeSRplots( this, weight_, TString("allss"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	  makeFillHisto2D<TH2F,float>("hyp_ss_met_vs_ht","hyp_ss_met_vs_ht",40,0,2000,ht,20,0,500,met,weight_);

	  //test min dR between leptons and jets
	  float mindr = 99999.;
	  for (unsigned int j=0;j<jets.size();++j) {
	    for (unsigned int gl=0;gl<goodleps.size();++gl) {
	      float dr = deltaR(jets[j].p4(),goodleps[gl].p4());
	      if (dr<mindr) {
		mindr = dr;
	      }
	    }
	  } 
	  //test min dR between leptons and btagged jets
	  makeFillHisto1D<TH1F,float>("hyp_ss_mindRlj","hyp_ss_mindRlj",50,0,5,mindr,weight_);
	  float mindrb = 99999.;
	  for (unsigned int j=0;j<btags.size();++j) {
	    for (unsigned int gl=0;gl<goodleps.size();++gl) {
	      float dr = deltaR(btags[j].p4(),goodleps[gl].p4());
	      if (dr<mindrb) {
		mindrb = dr;
	      }
	    }
	  } 
	  makeFillHisto1D<TH1F,float>("hyp_ss_mindRlb","hyp_ss_mindRlb",50,0,5,mindrb,weight_);
	  //test min dR between jets
	  float mindrj = 99999.;
	  float mass_mindrj = 99999.;
	  for (unsigned int j=0;j<jets.size();++j) {
	    for (unsigned int i=j+1;i<jets.size();++i) {
	      float dr = deltaR(jets[j].p4(),jets[i].p4());
	      if (dr<mindrj) {
		mindrj = dr;
		mass_mindrj = (jets[j].p4()+jets[i].p4()).mass();
	      }
	    }
	  } 
	  makeFillHisto1D<TH1F,float>("hyp_ss_mindRjj","hyp_ss_mindRjj",50,0,5,mindrj,weight_);
	  makeFillHisto1D<TH1F,float>("hyp_ss_mass_mindRjj","hyp_ss_mass_mindRjj",50,0,500,mass_mindrj,weight_);
	  //test dR between leptons
	  makeFillHisto1D<TH1F,float>("hyp_ss_dRll","hyp_ss_dRll",50,0,5,deltaR(goodleps[0].p4(),goodleps[1].p4()),weight_);
	    
	  tests::testLeptonIdVariables( this, weight_, hyp, ll, lt );

	  if (ac_base & 1<<HighHigh) {
	    if (prefix=="TTWJets" && debug) cout << left << setw(10) << ls_  << " "  << setw(10) << evt_ << " "  << setw(10) << njets << " "  << setw(10) << nbtag << " "  << setw(10) << std::setprecision(8) << ht << " "  << setw(10) << std::setprecision(8) << met << " " << sr << endl;

	    if (makeSyncTest /*|| (br==30 && hyp.leadLep().pt()>25. && hyp.traiLep().pt()>25)*/) {
	      cout << Form("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f", run_, ls_, evt_, int(vetoleps.size()), hyp.leadLep().pdgId(), hyp.leadLep().pt(), 
			   hyp.traiLep().pdgId(), hyp.traiLep().pt(),  njets, nbtag, met, ht) << endl;
	    }

	    tests::makeSRplots( this, weight_, TString("hihi"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	    makeFillHisto1D<TH1F,int>("hyp_hihi_nbtag_pt20","hyp_hihi_nbtag_pt20",8,0,8,nbtag_pt20,weight_);
	    makeFillHisto1D<TH1F,int>("hyp_hihi_nbtag_pt25","hyp_hihi_nbtag_pt25",8,0,8,nbtag_pt25,weight_);
	    makeFillHisto1D<TH1F,int>("hyp_hihi_nbtag_pt30","hyp_hihi_nbtag_pt30",8,0,8,nbtag_pt30,weight_);
	    makeFillHisto1D<TH1F,int>("hyp_hihi_nbtag_pt35","hyp_hihi_nbtag_pt35",8,0,8,nbtag_pt35,weight_);
	    makeFillHisto1D<TH1F,int>("hyp_hihi_nbtag_pt40","hyp_hihi_nbtag_pt40",8,0,8,nbtag_pt40,weight_);

	    if (mtmin>100.) {
	      tests::makeSRplots( this, weight_, TString("hihi_mt100"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	    }

	    if (ht400met120) {
	      tests::makeSRplots( this, weight_, TString("hihihtmet"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	      if (mtmin>100.) {
		tests::makeSRplots( this, weight_, TString("hihihtmetmt"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	      }
	    }

	    //test gen level btag (but starting from jets with |eta|<2.4)
	    tests::testBtag( this, weight_, alljets );

	  }
	  if (ac_base & 1<<HighLow) {
	    tests::makeSRplots( this, weight_, TString("hilow"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	    if (mtmin>100.) {
	      tests::makeSRplots( this, weight_, TString("hilow_mt100"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	    }
	    if (makeSyncTest) cout << Form("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f", run_, ls_, evt_, int(vetoleps.size()), hyp.leadLep().pdgId(), hyp.leadLep().pt(), 
					   hyp.traiLep().pdgId(), hyp.traiLep().pt(),  njets, nbtag, met, ht) << endl;
	  }
	  if (ac_base & 1<<LowLow) {
	    tests::makeSRplots( this, weight_, TString("lowlow"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	    //if (mtmin>100.) {fixme!!!
	      tests::makeSRplots( this, weight_, TString("lowlow_mt100"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	      //}
	    if (makeSyncTest) cout << Form("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f", run_, ls_, evt_, int(vetoleps.size()), hyp.leadLep().pdgId(), hyp.leadLep().pt(), 
					   hyp.traiLep().pdgId(), hyp.traiLep().pt(),  njets, nbtag, met, ht) << endl;
	  }

	  //if (prefix=="ttbar") {
	  //tests::fakeStudy(this,weight_,hyp,ll,lt);
	  //}

	  /*
	  //dump genps
	  for (unsigned int gp_idx=0;gp_idx<genps_p4().size();++gp_idx) {
	    cout << "gp idx=" << gp_idx << " id=" << genps_id()[gp_idx] 
		 << " status=" << genps_status()[gp_idx]
		 << " id_mother=" << genps_id_mother()[gp_idx]
		 << " p4=" << genps_p4()[gp_idx]
		 << endl;    
	  }
	  */

	} else {
	  tests::makeSRplots( this, weight_, TString("allos"), br, sr, hyp, ht, met, mtmin, type, goodleps, fobs, vetoleps, jets, alljets, btags, ll, lt );
	}
      }//if (makehist)

    }

    if (makeSSskim || makeQCDskim) {
      skim_file->cd(); 
      skim_tree->Write(); 
      skim_file->Close();
      delete skim_file;
    }
    delete tree;
    f.Close();
  }

  if (fr_file) fr_file->Close();

  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  if (makebaby) {
    bm->CloseBabyNtuple();
    delete bm;
  }
  if (makehist) SaveHistos();

  return 0;
}

void looper::printEvent(  ostream& ostr ){
  ostr << evt_run() << " " << evt_lumiBlock() << " " << evt_event() << endl; 
}

// // Book the baby ntuple
// void looper::MakeBabyNtuple(const char *babyFilename) {
//   babyFile_ = new TFile(Form("%s", babyFilename), "RECREATE");
//   babyTree_ = new TTree("tree", "A Baby Ntuple");  
//   babyTree_->Branch("run", &run_, "run/I");
//   babyTree_->Branch("ls", &ls_, "ls/I");
//   babyTree_->Branch("evt", &evt_, "evt/I");
//   babyTree_->Branch("weight", &weight_, "weight/F");
//   babyTree_->Branch("p4", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &p4_);
// }
// // Init the baby
// void looper::InitBabyNtuple() { 
//   run_ = -9999;
//   ls_ = -9999;
//   evt_ = -9999;
//   weight_ = -9999.;
//   babyTree_->SetBranchAddress("p4",          &p4_);
//   *p4_ = LorentzVector();
// }

void looper::fillUnderOverFlow(TH1F *h1, float value, float weight){

  float min = h1->GetXaxis()->GetXmin();
  float max = h1->GetXaxis()->GetXmax();

  if (value > max) value = h1->GetBinCenter(h1->GetNbinsX());
  if (value < min) value = h1->GetBinCenter(1);

  h1->Fill(value, weight);
}

