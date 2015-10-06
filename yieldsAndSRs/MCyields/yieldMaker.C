#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

#include "../../CORE/SSSelections.h"
#include "../../../software/tableMaker/CTable.h"
#include "SS.h"

float lumiAG = 0.0161; 
string tag = "v3.06"; 

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 

//Total
yields_t total; 

//function declaration
yields_t run(TChain *chain);

void getyields(){

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
  TChain *data_chain    = new TChain("t"); 

  //Fill chains
  ttbar_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root"          , tag.c_str())); 
  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTWQQ.root"          , tag.c_str())); 
  ttz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  ttz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZQ.root"           , tag.c_str())); 
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
  //dy_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_low.root"         , tag.c_str()));
  vhnonbb_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/VHtoNonBB.root"      , tag.c_str()));
  tzq_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TZQ.root"            , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonC.root", tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC.root"  , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC.root"    , tag.c_str()));
  
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
  yields_t data    = run(data_chain);     

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
                   ("total"  , total.EE  , total.EM  , total.MM  , total.TOTAL  )
                   ("data"   , data.EE   , data.EM   , data.MM   , data.TOTAL   );
  table.print();

}

yields_t run(TChain *chain){

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
    
      //Progress
      //SSAG::progress(nEventsTotal, nEventsChain);

      //Only keep good events
      if (ss::hyp_class() != 3) continue;

      //Require baseline selections
      int BR = baselineRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (BR < 0) continue;
   
      //Get the SR
      int SR = signalRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::mtmin(), ss::lep1_p4().pt(), ss::lep2_p4().pt());

      //Calculate the baseline yield
      if      (ss::hyp_type() == 3) result.EE    += ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;
      else if (ss::hyp_type() == 0) result.MM    += ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;
      else                          result.EM    += ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;
                                    result.TOTAL += ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;

    }//event loop
  }//file loop

  //Update total
  total.EE    += result.EE;
  total.EM    += result.EM;
  total.MM    += result.MM;
  total.TOTAL += result.TOTAL;

  //Return yield
  return result; 

}
