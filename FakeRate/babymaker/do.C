#include "TROOT.h"


int do(){

  gROOT->ProcessLine(".L ../../CORE/CMS3.cc+");
  gROOT->ProcessLine(".L ../../CORE/Base.cc+");
  gROOT->ProcessLine(".L ../../CORE/ElectronSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/MuonSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/MCSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/JetSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/IsolationTools.cc+");
  gROOT->ProcessLine(".L ../../CORE/SSSelections.cc+");
  gROOT->ProcessLine(".L fakeratelooper.C++");   //looper.C++

  int file = 0;

  babyMaker *mylooper = new babyMaker();

  //QCD
  TChain *qcd = new TChain("Events");
  qcd->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v3/V07-02-08/merged_ntuple_1.root");//test w/ just 1 file

  //QCD_MuEnriched
  TChain *qcd_MuEnriched = new TChain("Events");
  qcd_MuEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v3/V07-02-08/merged_ntuple_*.root");
  qcd_MuEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-30to50_MuEnrichedPt5_PionKaonDecay_Tune4C_13TeV_pythia8_Phys14DR-AVE20BX25_tsg_PHYS14_25_V3-v2/V07-02-08/merged_ntuple_*.root");
  qcd_MuEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-50to80_MuEnrichedPt5_PionKaonDecay_Tune4C_13TeV_pythia8_Phys14DR-AVE20BX25_tsg_PHYS14_25_V3-v1/V07-02-08/merged_ntuple_*.root");
  qcd_MuEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-80to120_MuEnrichedPt5_PionKaonDecay_Tune4C_13TeV_pythia8_Phys14DR-AVE20BX25_tsg_PHYS14_25_V3-v1/V07-02-08/merged_ntuple_*.root");

  //QCD_EM_Enriched
  TChain *qcd_EMEnriched = new TChain("Events");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt_20to30_bcToE_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt_30to80_bcToE_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt_80to170_bcToE_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt_170toInf_bcToE_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-5to10_EMEnriched_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-10to20_EMEnriched_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-20to30_EMEnriched_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-30to80_EMEnriched_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_EMEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-80to170_EMEnriched_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");

  //QCD non-Enriched
  TChain *qcd_nonEnriched = new TChain("Events");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-15to30_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-30to50_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-50to80_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-80to120_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-120to170_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-170to300_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-06/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-470to600_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-1800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v2/V07-02-06/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-3200_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_nonEnriched->Add("/hadoop/cms/store/group/snt/phys14/QCD_Pt-15to3000_Tune4C_Flat_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");

  //QCD HT
  TChain *qcd_HT = new TChain("Events");
  qcd_HT->Add("/hadoop/cms/store/group/snt/phys14/QCD_HT-100To250_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_HT->Add("/hadoop/cms/store/group/snt/phys14/QCD_HT-500To1000_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_HT->Add("/hadoop/cms/store/group/snt/phys14/QCD_HT_1000ToInf_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1/V07-02-08/merged_ntuple_*.root");
  qcd_HT->Add("/hadoop/cms/store/group/snt/phys14/QCD_HT_250To500_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v2/V07-02-08/merged_ntuple_*.root");

  //ttbar
  TChain *ttbar = new TChain("Events");
  ttbar->Add("/hadoop/cms/store/group/snt/phys14/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/V07-02-08/merged_ntuple_*.root");
  
  // mylooper->looper(qcd,"qcd1", -1);
  mylooper->looper(qcd_MuEnriched,"qcd_MuEnriched", -1);
  mylooper->looper(qcd_EMEnriched,"qcd_EMEnriched", -1);
  // mylooper->looper(qcd_EMEnriched,"qcd_EMEnriched-noBCtoE", -1);
  // mylooper->looper(qcd_nonEnriched,"qcd_nonEnriched", -1);
  // mylooper->looper(qcd_HT,"qcd_HT", -1);
  // mylooper->looper(ttbar,"ttbar", -1);

}
