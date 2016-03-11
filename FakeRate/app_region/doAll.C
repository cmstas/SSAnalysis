{
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gSystem->Load("../../software/tableMaker/libSimpleTable.so");

  gROOT->ProcessLine(".L ../../classFiles/v6.02/SS.cc+");
  gROOT->ProcessLine(".L ScanChain.C++");

  gROOT->ProcessLine(".L ../../commonUtils.h");

  TString tag1 = getTag();
  TString tag2 = "v5.01"; //no fake rate babies past v5.01 yet

  bool doData = false;

  bool doInSitu = 1; //we should remove this or fix this; the function does not handle inSitu properly

  bool highhigh   = 1;
  bool highlow    = 0;
  bool lowlow     = 0;

  bool doIsoTrigs = 0;
  bool doHTTrigs  = 0;

  //These are only for not-in-situ
  bool doConeCorr   = 1; 
  bool doJetCorr    = 0;
  bool doPtRatioCor = 0;
  bool doBonly      = 0;
  bool doConly      = 0;
  bool doLightonly  = 0;

  //For both inSitu and not-in-Situ
  bool doLooseEMVA  = 1;

  //These are only for inSitu (choose only one of these)
  bool soup         = 0;
  bool PC           = 0;   
  bool ssZ          = 0;
  bool PCssZ        = 0;

  TString option = "";

  if (doLooseEMVA) option+="_LooseEMVA";

  if (!doInSitu){
    if (doPtRatioCor) option+="_PtRatioCor";
    if (doBonly) option+="_doBonly";
    if (doConly) option+="_doConly";
    if (doLightonly) option+="_doLightonly";
    if (doIsoTrigs) option+="_IsoTrigs";
    if (doHTTrigs) option+="_HTTrigs";
  }
  else {
    if (soup) option += "_soup";
    if (PC) option += "_PC";
    if (ssZ) option += "_ssZ";
    if (PCssZ) option += "_PCssZ";
    option += "_inSitu"; 
  }

  TString ptRegion = "";
  if (highhigh) ptRegion = "HH";
  else if (highlow)  ptRegion = "HL";
  else if (lowlow)   ptRegion = "LL";

  if (doConeCorr) option+="_coneCorr";     //option only for ScanChain
  else if (doJetCorr) option+="_jetCorr";  //option only for ScanChain

  TChain *ch = new TChain("t"); 
  if(doData) {
    // option += "_data";
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag2+"-data1p280ifb/DataDoubleMuonD.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag2+"-data1p280ifb/DataDoubleEGD.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag2+"-data1p280ifb/DataMuonEGD.root");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag2+"/TTBAR.root");  
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag2+"/WJets_LO.root"); 
  } else {
    if (doLooseEMVA){
      //ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag1+"/WJets_LO.root");
      ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag1+"/TTBAR_PH.root");
      //ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag1+"/DY_high.root");
      //ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag1+"/DY_low.root");
    }
    // if (doLooseEMVA) ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag1+"/TTBAR.root");
    else ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag1+"/TTBAR.root"); // FIXME
  }
  ScanChain(ch, option, ptRegion, doData); 

  //TChain *ch_wjets = new TChain("t"); 
  //ch_wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v1.04/Wjets_baby.root"); //this one!

  // upload plots if you're Nick
  gSystem->Exec("if [ $USER == namin ]; then niceplots plots; fi");

}
