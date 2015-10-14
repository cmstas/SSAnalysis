#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

#include "../../CORE/SSSelections.h"
#include "../../software/tableMaker/CTable.h"
#include "../../commonUtils.h"
#include "SS.h"
#include "../../CORE/Tools/dorky/dorky.h"

float lumiAG = getLumi();
string tag = getTag().Data();  

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 

//Total
yields_t total; 

//function declaration
yields_t run(TChain *chain, bool isData = 0, bool doFlips = 0, bool doFakes = 0);

void getyields(){
  
  cout << lumiAG << endl;
  cout << tag << endl;

  //Chains
  TChain *ttbar_chain   = new TChain("t");
  TChain *ttw_chain     = new TChain("t");
  TChain *ttz_chain     = new TChain("t");
  TChain *st_chain      = new TChain("t");
  TChain *wz_chain      = new TChain("t");
  TChain *wzz_chain     = new TChain("t");
  TChain *wjets_chain   = new TChain("t");
  TChain *wwdps_chain   = new TChain("t");
  TChain *ttg_chain     = new TChain("t");
  TChain *dy_chain      = new TChain("t");
  TChain *vhnonbb_chain = new TChain("t");
  TChain *tzq_chain     = new TChain("t");
  TChain *tth_chain     = new TChain("t");
  TChain *qqww_chain    = new TChain("t");
  TChain *data_chain    = new TChain("t"); 
  TChain *wgamma_chain  = new TChain("t"); 
  TChain *flips_chain    = new TChain("t"); 
  TChain *fakes_chain    = new TChain("t"); 

  //Fill chains
  ttbar_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root"          , tag.c_str())); 
  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  ttz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP1.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP2.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP3.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP4.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP5.root"     , tag.c_str()));
  wz_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  wzz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZZ.root"            , tag.c_str()));
  wjets_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root"          , tag.c_str()));
  wwdps_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWDPS.root"          , tag.c_str()));
  ttg_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTG.root"            , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_high.root"        , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_low.root"         , tag.c_str()));
  vhnonbb_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/VHtoNonBB.root"      , tag.c_str()));
  tzq_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TZQ.root"            , tag.c_str()));
  tth_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  qqww_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));
  wgamma_chain ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WGToLNuG.root"       , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD.root", tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD.root"  , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD.root"    , tag.c_str()));
  flips_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD.root"  , tag.c_str()));
  flips_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD.root"    , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD.root", tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD.root"  , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD.root"    , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));

  //Get yields
  yields_t ttbar   = run(ttbar_chain);   
  yields_t ttw     = run(ttw_chain);     
  yields_t ttz     = run(ttz_chain);     
  yields_t st      = run(st_chain);      
  yields_t wz      = run(wz_chain);      
  yields_t wzz     = run(wzz_chain);     
  yields_t wjets   = run(wjets_chain);   
  yields_t wwdps   = run(wwdps_chain);   
  yields_t ttg     = run(ttg_chain);     
  yields_t dy      = run(dy_chain);      
  yields_t vhnonbb = run(vhnonbb_chain); 
  yields_t tzq     = run(tzq_chain);     
  yields_t tth     = run(tth_chain);     
  yields_t wgamma  = run(wgamma_chain);     
  yields_t qqww    = run(qqww_chain); 
  yields_t data    = run(data_chain, 1);     
  yields_t flips   = run(flips_chain, 1, 1);     
  yields_t fakes   = run(fakes_chain, 1, 0, 1);     

  //Make yield table
  CTable table; 
  table.setPrecision(2); 
  table.setTitle("SS Baseline Yields"); 
  table.useTitle(); 
  table.setTable() (              "EE"   ,   "EM"    ,   "MM"    ,   "TOTAL"    )
                   ("ttbar"  , ttbar.EE  , ttbar.EM  , ttbar.MM  , ttbar.TOTAL  ) 
                   ("ttw"    , ttw.EE    , ttw.EM    , ttw.MM    , ttw.TOTAL    ) 
                   ("ttz"    , ttz.EE    , ttz.EM    , ttz.MM    , ttz.TOTAL    ) 
                   ("st"     , st.EE     , st.EM     , st.MM     , st.TOTAL     ) 
                   ("wz"     , wz.EE     , wz.EM     , wz.MM     , wz.TOTAL     ) 
                   ("wzz"    , wzz.EE    , wzz.EM    , wzz.MM    , wzz.TOTAL    ) 
                   ("wjets"  , wjets.EE  , wjets.EM  , wjets.MM  , wjets.TOTAL  ) 
                   ("wwdps"  , wwdps.EE  , wwdps.EM  , wwdps.MM  , wwdps.TOTAL  ) 
                   ("ttg"    , ttg.EE    , ttg.EM    , ttg.MM    , ttg.TOTAL    ) 
                   ("dy"     , dy.EE     , dy.EM     , dy.MM     , dy.TOTAL     ) 
                   ("vhnonbb", vhnonbb.EE, vhnonbb.EM, vhnonbb.MM, vhnonbb.TOTAL) 
                   ("tzq"    , tzq.EE    , tzq.EM    , tzq.MM    , tzq.TOTAL    )
                   ("tth"    , tth.EE    , tth.EM    , tth.MM    , tth.TOTAL    )
                   ("qqww"   , qqww.EE   , qqww.EM   , qqww.MM   , qqww.TOTAL   )
                   ("wgamma" , wgamma.EE , wgamma.EM , wgamma.MM , wgamma.TOTAL )
                   ("flips"  , flips.EE  , flips.EM  , flips.MM  , flips.TOTAL  )
                   ("fakes"  , fakes.EE  , fakes.EM  , fakes.MM  , fakes.TOTAL  )
                   ("total"  , total.EE  , total.EM  , total.MM  , total.TOTAL  )
                   ("data"   , data.EE   , data.EM   , data.MM   , data.TOTAL   );
  table.print();

}

yields_t run(TChain *chain, bool isData, bool doFlips, bool doFakes){

  yields_t result;

  //Initialize
  result.EE    = 0;
  result.EM    = 0;
  result.MM    = 0;
  result.TOTAL = 0;

  //nEvents in chain
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;

      float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG*getPUw(ss::nGoodVertices());

      //Progress
      //SSAG::progress(nEventsTotal, nEventsChain);

      //Only keep good events
      if (!doFlips && !doFakes) {
	if (ss::hyp_class() != 3) continue;
	if (!isData && ss::lep1_motherID()!=1) continue;
	if (!isData && ss::lep2_motherID()!=1) continue;
      }

      if (doFlips) {
	if (ss::hyp_class() != 4) continue;
	float flipFact = 0.;
	if (abs(ss::lep1_id())==11) {
	  float flr = flipRate(ss::lep1_p4().pt(), ss::lep1_p4().eta());
	  flipFact += (flr/(1-flr));
	}
	if (abs(ss::lep2_id())==11) {
	  float flr = flipRate(ss::lep2_p4().pt(), ss::lep2_p4().eta());
	  flipFact += (flr/(1-flr));
	}
	weight *= flipFact;
      } 

      if (doFakes) {
	if (ss::hyp_class() != 2) continue;
	float fr = 0.;
	if (ss::lep1_passes_id()==0) {
	  fr = fakeRate(ss::lep1_id(),ss::lep1_p4().pt(), ss::lep1_p4().eta());
	}
	if (ss::lep2_passes_id()==0) {
	  fr = fakeRate(ss::lep2_id(),ss::lep2_p4().pt(), ss::lep2_p4().eta());
	}
	weight *= fr/(1.-fr);
	if (!ss::is_real_data()) weight *= -1.;
      } 

      //Reject duplicates (after selection otherwise flips are ignored...)
      if (isData == true){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ continue; }
      }

      //Require baseline selections
      int BR = baselineRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (BR < 0) continue;
   
      //Get the SR
      int SR = signalRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::mtmin(), ss::lep1_p4().pt(), ss::lep2_p4().pt());

      //Calculate the baseline yield
      if      (ss::hyp_type() == 3) result.EE    += weight;
      else if (ss::hyp_type() == 0) result.MM    += weight;
      else                          result.EM    += weight;
                                    result.TOTAL += weight;

    }//event loop
  }//file loop

  if (result.EE<0) result.EE=0.;
  if (result.MM<0) result.MM=0.;
  if (result.EM<0) result.EM=0.;
  if (result.TOTAL<0) result.TOTAL=0.;

  //Update total
  if (!isData || doFlips || doFakes){
    total.EE    += result.EE;
    total.EM    += result.EM;
    total.MM    += result.MM;
    total.TOTAL += result.TOTAL;
  }

  //Return yield
  return result; 

}
