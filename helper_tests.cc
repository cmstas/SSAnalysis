#include "helper_tests.h"
#include "looper.h"

#include "TFile.h"

#include "./CORE/CMS3.h"
#include "./CORE/SSSelections.h"
#include "./tools.h"

using namespace tas;

void tests::runQCDtest(looper* loop, float& weight_, vector<Lep>& fobs, vector<Lep>& goodleps, int& njets, float& met) {

  if (fobs.size()>1) return; 
  if (fobs[0].pt()<ptCutHigh) return; 
  loop->makeFillHisto1D<TH1F,int>("njets","njets",20,0,20,njets,weight_);
  for (unsigned int ipu=0;ipu<puInfo_bunchCrossing().size();ipu++)
    if (puInfo_bunchCrossing()[ipu]==0) 
      loop->makeFillHisto1D<TH1F,float>("npu_true","npu_true",20,0,100,puInfo_trueNumInteractions()[ipu],weight_);
  if (njets==0) return;
  float evtmt = mt(fobs[0].pt(),met,deltaPhi(fobs[0].p4().phi(),evt_pfmetPhi()));
  float genmt = mt(fobs[0].pt(),gen_met(),deltaPhi(fobs[0].p4().phi(),gen_metPhi()));
  float tkmet = trackerMET(0.2).met;
  float minmet = std::min(tkmet,met);
  loop->makeFillHisto1D<TH1F,float>("fo_pt","fo_pt",20,0,100,fobs[0].pt(),weight_);
  loop->makeFillHisto1D<TH1F,float>("fo_eta","fo_eta",10,-2.5,2.5,fobs[0].eta(),weight_);
  loop->makeFillHisto1D<TH1F,float>("evt_met","evt_met",10,0,100,met,weight_);
  loop->makeFillHisto1D<TH1F,float>("evt_tkmet","evt_tkmet",10,0,100,tkmet,weight_);
  loop->makeFillHisto1D<TH1F,float>("evt_minmet","evt_minmet",10,0,100,minmet,weight_);
  loop->makeFillHisto1D<TH1F,float>("evt_mt","evt_mt",20,0,200,evtmt,weight_);
  loop->makeFillHisto2D<TH2F,float>("evt_mt_vs_met","evt_mt_vs_met",10,0,100,met,20,0,200,evtmt,weight_);
  loop->makeFillHisto2D<TH2F,float>("evt_mt_vs_pt", "evt_mt_vs_pt",20,0,100,fobs[0].pt(),20,0,200,evtmt,weight_);
  loop->makeFillHisto1D<TH1F,float>("gen_met","gen_met",10,0,100,gen_met(),weight_);
  loop->makeFillHisto1D<TH1F,float>("gen_mt","gen_mt",20,0,200,genmt,weight_);
  loop->makeFillHisto2D<TH2F,float>("gen_mt_vs_met","gen_mt_vs_met",10,0,100,gen_met(),20,0,200,genmt,weight_);
  if (met<20 && evtmt<20) loop->makeFillHisto1D<TH1F,int>("pass_metmt","pass_metmt",2,0,2,1,weight_);
  else loop->makeFillHisto1D<TH1F,int>("pass_metmt","pass_metmt",2,0,2,0,weight_);
  if (minmet<20 && evtmt<20) loop->makeFillHisto1D<TH1F,int>("pass_minmetmt","pass_minmetmt",2,0,2,1,weight_);
  else loop->makeFillHisto1D<TH1F,int>("pass_minmetmt","pass_minmetmt",2,0,2,0,weight_);
  if (gen_met()<20 && genmt<20) loop->makeFillHisto1D<TH1F,int>("pass_gen_metmt","pass_gen_metmt",2,0,2,1,weight_);
  else loop->makeFillHisto1D<TH1F,int>("pass_gen_metmt","pass_gen_metmt",2,0,2,0,weight_);
  //compute fake rate
  //reco level selection
  if (met<20. && evtmt<20.) {
    for (unsigned int fo=0;fo<fobs.size();++fo) {
      //if (isFromW(fobs[fo])) continue;//fixme
      //if (!isFromB(fobs[fo])) continue;//fixme
      int pdgid = abs(fobs[fo].pdgId());
      //denominator
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den":"fr_el_den"),(pdgid==13?"fr_mu_den":"fr_el_den"),
				  10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den_now":"fr_el_den_now"),(pdgid==13?"fr_mu_den_now":"fr_el_den_now"),
				  10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),1.);
      //numerator
      for (unsigned int gl=0;gl<goodleps.size();++gl) {
	if (abs(goodleps[gl].pdgId())==pdgid && goodleps[gl].idx()==fobs[fo].idx()) {
	  //cout << "goodleps.size()=" << goodleps.size() << endl;
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num":"fr_el_num"),(pdgid==13?"fr_mu_num":"fr_el_num"),
				      10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num_now":"fr_el_num_now"),(pdgid==13?"fr_mu_num_now":"fr_el_num_now"),
				      10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),1.);
	  //std::cout << "scale1fb=" << evt_scale1fb() << endl;
	  break;
	}
      }
    }
  }
  //reco level selection (minmet)
  if (minmet<20. && evtmt<20.) {
    for (unsigned int fo=0;fo<fobs.size();++fo) {
      //if (isFromW(fobs[fo])) continue;//fixme
      //if (!isFromB(fobs[fo])) continue;//fixme
      int pdgid = abs(fobs[fo].pdgId());
      //denominator
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den_min":"fr_el_den_min"),(pdgid==13?"fr_mu_den_min":"fr_el_den_min"),
				  10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den_min_now":"fr_el_den_min_now"),(pdgid==13?"fr_mu_den_min_now":"fr_el_den_min_now"),
				  10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),1.);
      //numerator
      for (unsigned int gl=0;gl<goodleps.size();++gl) {
	if (abs(goodleps[gl].pdgId())==pdgid && goodleps[gl].idx()==fobs[fo].idx()) {
	  //cout << "goodleps.size()=" << goodleps.size() << endl;
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num_min":"fr_el_num_min"),(pdgid==13?"fr_mu_num_min":"fr_el_num_min"),
				      10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num_min_now":"fr_el_num_min_now"),(pdgid==13?"fr_mu_num_min_now":"fr_el_num_min_now"),
				      10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),1.);
	  //std::cout << "scale1fb=" << evt_scale1fb() << endl;
	  break;
	}
      }
    }
  }
  //gen level selection
  if (gen_met()<20. && genmt<20.) {
    for (unsigned int fo=0;fo<fobs.size();++fo) {
      //if (isFromW(fobs[fo])) continue;//fixme
      //if (!isFromB(fobs[fo])) continue;//fixme
      int pdgid = abs(fobs[fo].pdgId());
      //denominator
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den_gen":"fr_el_den_gen"),(pdgid==13?"fr_mu_den_gen":"fr_el_den_gen"),
				  10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den_gen_now":"fr_el_den_gen_now"),(pdgid==13?"fr_mu_den_gen_now":"fr_el_den_gen_now"),
				  10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),1.);
      //numerator
      for (unsigned int gl=0;gl<goodleps.size();++gl) {
	if (abs(goodleps[gl].pdgId())==pdgid && goodleps[gl].idx()==fobs[fo].idx()) {
	  //cout << "goodleps.size()=" << goodleps.size() << endl;
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num_gen":"fr_el_num_gen"),(pdgid==13?"fr_mu_num_gen":"fr_el_num_gen"),
				      10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num_gen_now":"fr_el_num_gen_now"),(pdgid==13?"fr_mu_num_gen_now":"fr_el_num_gen_now"),
				      10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),1.);
	  //std::cout << "scale1fb=" << evt_scale1fb() << endl;
	  break;
	}
      }
    }
  }
  
}


void tests::runDYtest(looper* loop, float& weight_, vector<Lep>& vetoleps_noiso, vector<Lep>& fobs, vector<Lep>& goodleps, int& njets, float& met, float& ht, bool usePtRel) {

  //check lepton selection efficiency
  for (unsigned int gp=0;gp<genps_id().size();++gp) {
    int pdgid = abs(genps_id()[gp]);
    if (pdgid!=13 && pdgid!=11) continue;
    if (genps_id_mother()[gp]!=23 && abs(genps_id_mother()[gp])!=24) continue;
    if (genps_status()[gp]!=1) continue;//is this needed?
    if (fabs(genps_p4()[gp].eta())>2.4) continue;
    if (genps_p4()[gp].pt()<5) continue;
    if (pdgid==11 && genps_p4()[gp].pt()<10) continue;
    //denominator
    loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_mu_den":"ef_el_den"),(pdgid==13?"ef_mu_den":"ef_el_den"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
    if (fabs(genps_p4()[gp].eta())<1.0) {
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_ht_mu_den":"ef_ht_el_den"),(pdgid==13?"ef_ht_mu_den":"ef_ht_el_den"),5,0.,200.,genps_p4()[gp].pt(),6,0.,1200,ht,weight_);
    }
    //numerator
    for (unsigned int gl=0;gl<goodleps.size();++gl) {
      if (abs(goodleps[gl].pdgId())==pdgid && deltaR(goodleps[gl].p4(),genps_p4()[gp])<0.1) {
	//cout << "goodleps.size()=" << goodleps.size() << endl;
	loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_mu_num":"ef_el_num"),(pdgid==13?"ef_mu_num":"ef_el_num"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
	if (fabs(genps_p4()[gp].eta())<1.0) {
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_ht_mu_num":"ef_ht_el_num"),(pdgid==13?"ef_ht_mu_num":"ef_ht_el_num"),5,0.,200.,genps_p4()[gp].pt(),6,0.,1200,ht,weight_);
	}
	//charge flip
	loop->makeFillHisto2D<TH2F,float>((pdgid==13?"flip_mu_den":"flip_el_den"),(pdgid==13?"flip_mu_den":"flip_el_den"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
	if (goodleps[gl].pdgId()==-genps_id()[gp]) loop->makeFillHisto2D<TH2F,float>((pdgid==13?"flip_mu_num":"flip_el_num"),(pdgid==13?"flip_mu_num":"flip_el_num"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
	break;
      }
    }
    //numerator for fobs
    for (unsigned int fo=0;fo<fobs.size();++fo) {
      if (abs(fobs[fo].pdgId())==pdgid && deltaR(fobs[fo].p4(),genps_p4()[gp])<0.1) {
	//cout << "fobs.size()=" << fobs.size() << endl;
	loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_mu_num_fo":"ef_el_num_fo"),(pdgid==13?"ef_mu_num_fo":"ef_el_num_fo"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
	if (fabs(genps_p4()[gp].eta())<1.0) {
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_ht_mu_num_fo":"ef_ht_el_num_fo"),(pdgid==13?"ef_ht_mu_num_fo":"ef_ht_el_num_fo"),5,0.,200.,genps_p4()[gp].pt(),6,0.,1200,ht,weight_);
	}
	break;
      }
    }
    //numerator for tight id and fobs isolation
    for (unsigned int fo=0;fo<fobs.size();++fo) {
      if (abs(fobs[fo].pdgId())==pdgid && deltaR(fobs[fo].p4(),genps_p4()[gp])<0.1) {
	//cout << "fobs.size()=" << fobs.size() << endl;
	if (isGoodLeptonNoIso(fobs[fo].pdgId(),fobs[fo].idx())==0) continue;
	loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_mu_num_foiso":"ef_el_num_foiso"),(pdgid==13?"ef_mu_num_foiso":"ef_el_num_foiso"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
	if (fabs(genps_p4()[gp].eta())<1.0) {
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_ht_mu_num_foiso":"ef_ht_el_num_foiso"),(pdgid==13?"ef_ht_mu_num_foiso":"ef_ht_el_num_foiso"),5,0.,200.,genps_p4()[gp].pt(),6,0.,1200,ht,weight_);
	}
	break;
      }
    }
    //numerator for vetoleps_noiso
    for (unsigned int vlnoiso=0;vlnoiso<vetoleps_noiso.size();++vlnoiso) {
      if (abs(vetoleps_noiso[vlnoiso].pdgId())==pdgid && deltaR(vetoleps_noiso[vlnoiso].p4(),genps_p4()[gp])<0.1) {
	loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_mu_num_vlnoiso":"ef_el_num_vlnoiso"),(pdgid==13?"ef_mu_num_vlnoiso":"ef_el_num_vlnoiso"),5,0.,100.,genps_p4()[gp].pt(),3,0.,3.0,fabs(genps_p4()[gp].eta()),weight_);
	if (fabs(genps_p4()[gp].eta())<1.0) {
	  loop->makeFillHisto2D<TH2F,float>((pdgid==13?"ef_ht_mu_num_vlnoiso":"ef_ht_el_num_vlnoiso"),(pdgid==13?"ef_ht_mu_num_vlnoiso":"ef_ht_el_num_vlnoiso"),5,0.,200.,genps_p4()[gp].pt(),6,0.,1200,ht,weight_);
	}
	if (isGoodLepton(vetoleps_noiso[vlnoiso].pdgId(),vetoleps_noiso[vlnoiso].idx(), usePtRel)==0) {
	  plotLeptonIdVariables( loop, weight_, "vetoFailTight", vetoleps_noiso[vlnoiso].pdgId(), vetoleps_noiso[vlnoiso].idx());
	  if (isLooseIsolatedLepton(vetoleps_noiso[vlnoiso].pdgId(),vetoleps_noiso[vlnoiso].idx())) plotLeptonIdVariables( loop, weight_, "lisovetoFailTight", vetoleps_noiso[vlnoiso].pdgId(), vetoleps_noiso[vlnoiso].idx());
	  if (isIsolatedLepton(vetoleps_noiso[vlnoiso].pdgId(),vetoleps_noiso[vlnoiso].idx())) plotLeptonIdVariables( loop, weight_, "isovetoFailTight", vetoleps_noiso[vlnoiso].pdgId(), vetoleps_noiso[vlnoiso].idx());
	}
	break;
      }
    }
    if (pdgid==11){
      for (unsigned int elidx=0;elidx<els_p4().size();++elidx) {
	if (deltaR(els_p4()[elidx],genps_p4()[gp])>0.1) continue;
	if (fabs(els_etaSC().at(elidx)) <= 1.479) {
	  loop->makeFillHisto1D<TH1F,float>("el_fo_sietaieta_barrel","el_fo_sietaieta_barrel",50,0.,0.025,els_sigmaIEtaIEta_full5x5().at(elidx),weight_);
	}
      }
    }
  }

}


void tests::runWZtest(looper* loop, float& weight_, vector<Lep>& vetoleps, vector<Lep>& fobs, vector<Lep>& goodleps, int& njets, float& met) {

  weight_=1.;//fixme

  loop->makeFillHisto1D<TH1F,float>("vetoleps_size","vetoleps_size",10,-0.5,9.5,vetoleps.size(),weight_);
  loop->makeFillHisto1D<TH1F,float>("goodleps_size","goodleps_size",10,-0.5,9.5,goodleps.size(),weight_);

  if (goodleps.size()==2) {
    loop->makeFillHisto1D<TH1F,float>("vetoleps_size_g2","vetoleps_size_g2",10,-0.5,9.5,vetoleps.size(),weight_);
    if (goodleps[0].charge()!=goodleps[1].charge()) return;
    loop->makeFillHisto1D<TH1F,float>("vetoleps_size_g2ss","vetoleps_size_g2ss",10,-0.5,9.5,vetoleps.size(),weight_);

    bool noZmass = true;
    for (unsigned int gl=0;gl<goodleps.size();++gl) {
      for (unsigned int vl=0;vl<vetoleps.size();++vl) {
	if ( fabs(ROOT::Math::VectorUtil::DeltaR(goodleps[gl].p4(),vetoleps[vl].p4()))<0.001 ) continue;
	if ( goodleps[gl].pdgId()!=-vetoleps[vl].pdgId() ) continue; 
	float mll = (goodleps[gl].p4()+vetoleps[vl].p4()).mass();
	if (fabs(mll-91.2)<15) {
	  noZmass = false;
	  break;
	}
      }
    }
    if (vetoleps.size()>2) loop->makeFillHisto1D<TH1F,float>("noZmass_v3g2ss","noZmass_v3g2ss",2,0,2,noZmass,weight_);

    if (vetoleps.size()==2 || noZmass) {

      loop->makeFillHisto1D<TH1F,float>("fobs_size_noZv3g2ss","fobs_size_noZv3g2ss",10,-0.5,9.5,fobs.size(),weight_);
      if (fobs.size()>2) return;

      cout << "new gps" << endl;

      float mz = -999.;
      float mw = -999.;
      float maxAbsEta = -1;
      float minPt = 999999.;
      vector<unsigned int> gpidv;
      for (unsigned int gp=0;gp<genps_id().size();++gp) {
	int pdgid = abs(genps_id()[gp]);
	if (pdgid==23) {
	  //cout << "Z status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp]  << endl;
	  if (genps_status()[gp]==22) mz = genps_p4()[gp].mass();
	}
	if (pdgid==24) {
	  //cout << "W status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp]  << endl;
	  if (genps_status()[gp]==22) mw = genps_p4()[gp].mass();
	}
	if (pdgid!=13 && pdgid!=11 && pdgid!=15) continue;
	if (pdgid==11) cout << "electron status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp] << " pt=" << genps_p4()[gp].pt() << endl;
	if (pdgid==13) cout << "muon status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp] << " pt=" << genps_p4()[gp].pt() << endl;
	if (pdgid==15) cout << "tau status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp] << " pt=" << genps_p4()[gp].pt() << endl;
	if (genps_id_mother()[gp]!=23 && abs(genps_id_mother()[gp])!=24) continue;
	if (pdgid==11 && genps_status()[gp]!=1) continue;//is this needed?
	if (pdgid==13 && genps_status()[gp]!=1) continue;//is this needed?
	if (pdgid==15 && genps_status()[gp]!=2) continue;//is this needed?
	gpidv.push_back(gp);
	if (fabs(genps_p4()[gp].eta())>maxAbsEta) maxAbsEta=fabs(genps_p4()[gp].eta());
	if (genps_p4()[gp].pt()<minPt) minPt=genps_p4()[gp].pt();
      }

      if (gpidv.size()==1) {
	//cout << "size1" << endl;
	for (unsigned int gp=0;gp<genps_id().size();++gp) {
	  int pdgid = abs(genps_id()[gp]);
	  //cout << "id=" << genps_id()[gp] << " status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp] << " pt=" << genps_p4()[gp].pt() << endl;
	  if (genps_status()[gp]!=1 && genps_status()[gp]!=2) continue;
	  if (abs(genps_id_mother()[gp])>=24) continue;//Ws are ok (size=1), don't want other resonances, but somehow Z leptons don't have mother id=23
	  if ((pdgid==11 && genps_status()[gp]==1) || (pdgid==13 && genps_status()[gp]==1) || (pdgid==15 && genps_status()[gp]==2)) gpidv.push_back(gp);
	}
      }
      if (gpidv.size()>3) {
	cout << "size>3 is " << gpidv.size() << endl;
	for (unsigned int gp=0;gp<gpidv.size() && gpidv.size()>3;++gp) {
	  cout << "id=" << genps_id()[gpidv[gp]] << " status=" << genps_status()[gpidv[gp]] << " mother=" << genps_id_mother()[gpidv[gp]] << " pt=" << genps_p4()[gpidv[gp]].pt() << endl;
	  if (abs(genps_id()[gpidv[gp]])==15) {//extra leps can be duplicates from tau decays
	    gpidv.erase(gpidv.begin()+gp);
	    cout << "erased position=" << gp << " new size=" << gpidv.size() << endl;
	    gp--;
	  }
	}
      }
      if (gpidv.size()>3) {
	cout << "sorting size>3 is " << gpidv.size() << endl;
	//sort by pt and erase exceeding items
	std::sort(gpidv.begin(),gpidv.end(),ptsort);
	gpidv.erase(gpidv.begin()+3,gpidv.end());
	for (unsigned int gp=0;gp<gpidv.size();++gp) {
	  cout << "id=" << genps_id()[gpidv[gp]] << " status=" << genps_status()[gpidv[gp]] 
	       << " mother=" << genps_id_mother()[gpidv[gp]] << " pt=" << genps_p4()[gpidv[gp]].pt() << endl;
	}     
      }
      if (gpidv.size()!=3) {
	cout << "anomalous size=" << gpidv.size() << endl;
	for (unsigned int gp=0;gp<gpidv.size();++gp) {
	  cout << "sel id=" << genps_id()[gpidv[gp]] << " status=" << genps_status()[gpidv[gp]] << " mother=" << genps_id_mother()[gpidv[gp]] << " pt=" << genps_p4()[gpidv[gp]].pt() << endl;
	}
	for (unsigned int gp=0;gp<genps_id().size();++gp) {
	  cout << "gps id=" << genps_id()[gp] << " status=" << genps_status()[gp] << " mother=" << genps_id_mother()[gp] << " pt=" << genps_p4()[gp].pt() << endl;
	}
      }

      int glepfromZ = -1;
      for (unsigned int gl=0;gl<goodleps.size();++gl) {
	//easy to check that it is not from W
	if (isFromW(goodleps[gl].pdgId(),goodleps[gl].idx())==0 ||goodleps[gl].mc_motherid()==23) {
	  glepfromZ=gl; 
	  break;
	}
      }

      loop->makeFillHisto1D<TH1F,float>("gps_size_g2f2","gps_size_g2f2",10,-0.5,9.5,gpidv.size(),weight_);
      loop->makeFillHisto1D<TH1F,float>("mz_g2f2","mz_g2f2",100,0.,200,mz,weight_);
      loop->makeFillHisto1D<TH1F,float>("mw_g2f2","mw_g2f2",100,0.,200,mw,weight_);
      if (gpidv.size()==3) {
	loop->makeFillHisto1D<TH1F,float>("maxAbsEta_g2f2s3","maxAbsEta_g2f2s3",100,0.,5.0,maxAbsEta,weight_);
	if (maxAbsEta<2.4) loop->makeFillHisto1D<TH1F,float>("minPt_g2f2s3eta24","minPt_g2f2s3eta24",100,0.,200.0,minPt,weight_);
	else return;

	//match goodleps and vetoleps with genps
	cout << "check duplicates" << endl;
	int unmatches = 0;
	for (unsigned int gp=0;gp<gpidv.size();++gp) {
	  unsigned int gpid = gpidv[gp];
	  int pdgid = abs(genps_id()[gpid]);
	  /*
	    cout << "sel id=" << genps_id()[gpid] << " status=" << genps_status()[gpid] 
	    << " mother=" << genps_id_mother()[gpid] << " pt=" << genps_p4()[gpid].pt() << endl;
	  */
	  bool glmatch = false;
	  for (unsigned int gl=0;gl<goodleps.size();++gl) {
	    if (goodleps[gl].mc3idx()==int(gpid) || 
		fabs(ROOT::Math::VectorUtil::DeltaR(goodleps[gl].mc_p4(),genps_p4()[gpid]))<0.1 || 
		fabs(ROOT::Math::VectorUtil::DeltaR(goodleps[gl].p4(),genps_p4()[gpid]))<0.1) {
	      cout << "match lep and gp, pt=" << goodleps[gl].mc_p4().pt() << " " << genps_p4()[gpid].pt() << endl;
	      glmatch = true;
	    }
	  }
	  if (glmatch==0) {
	    unmatches++;
	    int pfidx = -1;
	    float mindpt = 0.2;
	    for (unsigned int pfi=0; pfi<pfcands_p4().size(); ++pfi){
	      if ( pfcands_charge()[pfi]==0 ) continue;
	      float dR = fabs(ROOT::Math::VectorUtil::DeltaR(pfcands_p4()[pfi],genps_p4()[gpid]));
	      float dpt = fabs((pfcands_p4()[pfi].pt()-genps_p4()[gpid].pt())/genps_p4()[gpid].pt());
	      if ( dR<0.1 && dpt<mindpt ) {
		pfidx=pfi;
		mindpt=dpt;
		//break;
	      }
	    }
	    loop->makeFillHisto1D<TH1F,float>("pdgId_noGL","pdgId_noGL",20,0.,20,pdgid,weight_);
	    loop->makeFillHisto1D<TH1F,float>("momId_noGL","momId_noGL",30,0.,30,abs(genps_id_mother()[gpid]),weight_);
	    if (pdgid==15) continue;
	    if (pfidx>=0) {
	      loop->makeFillHisto1D<TH1F,float>("deltapt_pfmatch","deltapt_pfmatch",100,-1.,1,(pfcands_p4()[pfidx].pt()-genps_p4()[gpid].pt())/genps_p4()[gpid].pt(),weight_);
	      loop->makeFillHisto1D<TH1F,float>("pfid_pfmatch","pfid_pfmatch",250,0,250,abs(pfcands_particleId()[pfidx]),weight_);
	    } else {
	      if (pdgid==11) loop->makeFillHisto2D<TH2F,float>("pteta_el_noPFnoGLeta24","pteta_el_noPFnoGLeta24",5,0.,50.0,genps_p4()[gpid].pt(),5,0.,2.5,fabs(genps_p4()[gpid].eta()),weight_);
	      if (pdgid==13) loop->makeFillHisto2D<TH2F,float>("pteta_mu_noPFnoGLeta24","pteta_mu_noPFnoGLeta24",5,0.,50.0,genps_p4()[gpid].pt(),5,0.,2.5,fabs(genps_p4()[gpid].eta()),weight_);
	    }
	    if (pdgid==11) {
	      loop->makeFillHisto1D<TH1F,float>("eta_el_noGL","eta_el_noGL",100,0.,5.0,fabs(genps_p4()[gpid].eta()),weight_);
	      if (fabs(genps_p4()[gpid].eta())<2.4) {
		loop->makeFillHisto1D<TH1F,float>("pt_el_noGLeta24","pt_el_noGLeta24",100,0.,200.0,genps_p4()[gpid].pt(),weight_);
		if (pfidx>=0&&glepfromZ>=0) {
		  //loop->makeFillHisto1D<TH1F,float>("mll_elpf_noGLeta24","mll_elpf_noGLeta24",40,0.,200.0,(goodleps[glepfromZ].p4()+pfcands_p4()[pfidx]).mass(),weight_);
		  float mass0 = (goodleps[0].p4()+pfcands_p4()[pfidx]).mass();
		  float mass1 = (goodleps[1].p4()+pfcands_p4()[pfidx]).mass();
		  loop->makeFillHisto1D<TH1F,float>("mll_elpf_noGLeta24","mll_elpf_noGLeta24",40,0.,200.0,(fabs(mass0-91.2)<fabs(mass1-91.2)?mass0:mass1),weight_);
		  loop->makeFillHisto1D<TH1F,float>("mz_elpf_noGLeta24","mz_elpf_noGLeta24",40,0.,200.0,mz,weight_);
		}
	      }
	    }		  
	    if (pdgid==13) {
	      loop->makeFillHisto1D<TH1F,float>("eta_mu_noGL","eta_mu_noGL",100,0.,5.0,fabs(genps_p4()[gpid].eta()),weight_);
	      if (fabs(genps_p4()[gpid].eta())<2.4) {
		loop->makeFillHisto1D<TH1F,float>("pt_mu_noGLeta24","pt_mu_noGLeta24",100,0.,200.0,genps_p4()[gpid].pt(),weight_);
		if (pfidx>=0&&glepfromZ>=0) {
		  //loop->makeFillHisto1D<TH1F,float>("mll_mupf_noGLeta24","mll_mupf_noGLeta24",40,0.,200.0,(goodleps[glepfromZ].p4()+pfcands_p4()[pfidx]).mass(),weight_);
		  float mass0 = (goodleps[0].p4()+pfcands_p4()[pfidx]).mass();
		  float mass1 = (goodleps[1].p4()+pfcands_p4()[pfidx]).mass();
		  loop->makeFillHisto1D<TH1F,float>("mll_mupf_noGLeta24","mll_mupf_noGLeta24",40,0.,200.0,(fabs(mass0-91.2)<fabs(mass1-91.2)?mass0:mass1),weight_);
		  loop->makeFillHisto1D<TH1F,float>("mz_mupf_noGLeta24","mz_mupf_noGLeta24",40,0.,200.0,mz,weight_);
		}
	      }
	    }		  
	  }
		
	}
	if (unmatches>1) {
	  cout << "duplcates, unmatches=" << unmatches << endl;

	  for (unsigned int gl=0;gl<goodleps.size();++gl) {
	    cout << "good lep pt=" << goodleps[gl].pt() << " eta=" << goodleps[gl].eta() << " phi=" << goodleps[gl].p4().phi() << " mcpt=" << goodleps[gl].mc_p4().pt() << endl;
	  }

	  for (unsigned int gp=0;gp<gpidv.size();++gp) {
	    cout << "id=" << genps_id()[gpidv[gp]] << " status=" << genps_status()[gpidv[gp]] << " mother=" << genps_id_mother()[gpidv[gp]] 
		 << " pt=" << genps_p4()[gpidv[gp]].pt() << " eta=" << genps_p4()[gpidv[gp]].eta() << " phi=" << genps_p4()[gpidv[gp]].phi() << endl;
	  }		
	  //return -1;

	}

      }

    }
  }

}

void tests::fakeStudy(looper* loop, float& weight_, DilepHyp& hyp, TString& ll, TString& lt) {

  bool isLeadPrompt = 0;
  bool isTrailPrompt = 0;
  int leadType = -1;
  int trailType = -1;
	    
  if ( !isFromW(hyp.traiLep().pdgId(),hyp.traiLep().idx()) ) {
    //check fakes (mother not W)
	      
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_mc","hyp_ss_faketrail_"+lt+"_mc",11001,-5500.5,5500.5,hyp.traiLep().mc_id(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_mc_mother","hyp_ss_faketrail_"+lt+"_mc_mother",11001,-5500.5,5500.5,hyp.traiLep().mc_motherid(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_mc3","hyp_ss_faketrail_"+lt+"_mc3",11001,-5500.5,5500.5,hyp.traiLep().mc3_id(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_mc3_mother","hyp_ss_faketrail_"+lt+"_mc3_mother",11001,-5500.5,5500.5,hyp.traiLep().mc3_motherid(),weight_);
	      
    loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_relIso03","hyp_ss_faketrail_"+lt+"_relIso03",100,0.,1.,hyp.traiLep().relIso03(),weight_);
	      
    if (isFromB(hyp.traiLep().pdgId(),hyp.traiLep().idx()) ) trailType=FakeB;
    else if (isFromC(hyp.traiLep().pdgId(),hyp.traiLep().idx()) ) trailType=FakeC;
    else if (isFromLight(hyp.traiLep().pdgId(),hyp.traiLep().idx())) trailType=FakeLightTrue;
    else if (isFromLightFake(hyp.traiLep().pdgId(),hyp.traiLep().idx())) trailType=FakeLightFake;
    else if (hyp.traiLep().mc_id()==22 && hyp.traiLep().mc_p4().pt()>hyp.traiLep().pt()/2.) {
      trailType=FakeHiPtGamma;
      loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hiptgamma_mc","hyp_ss_faketrail_"+lt+"_hiptgamma_mc",11001,-5500.5,5500.5,hyp.traiLep().mc_id(),weight_);
      loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hiptgamma_mc_mother","hyp_ss_faketrail_"+lt+"_hiptgamma_mc_mother",11001,-5500.5,5500.5,hyp.traiLep().mc_motherid(),weight_);
      loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hiptgamma_mc3","hyp_ss_faketrail_"+lt+"_hiptgamma_mc3",11001,-5500.5,5500.5,hyp.traiLep().mc3_id(),weight_);
      loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hiptgamma_mc3_mother","hyp_ss_faketrail_"+lt+"_hiptgamma_mc3_mother",11001,-5500.5,5500.5,hyp.traiLep().mc3_motherid(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_hiptgamma_pt","hyp_ss_faketrail_"+lt+"_hiptgamma_pt",50,0,100,hyp.traiLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_hiptgamma_ptmc","hyp_ss_faketrail_"+lt+"_hiptgamma_ptmc",50,0,100,hyp.traiLep().mc_p4().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_hiptgamma_deltaptoverpt","hyp_ss_faketrail_"+lt+"_hiptgamma_deltaptoverpt",100,-0.5,0.5,(hyp.traiLep().pt()-hyp.traiLep().mc_p4().pt())/hyp.traiLep().mc_p4().pt(),weight_);
    } else  {
      trailType=FakeUnknown;
      if (hyp.traiLep().mc_id()==22 && hyp.traiLep().mc_p4().pt()<1.) {
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_category","hyp_ss_faketrail_"+lt+"_category",End,0,End,FakeLowPtGamma,weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hilowgamma_mc","hyp_ss_faketrail_"+lt+"_hilowgamma_mc",11001,-5500.5,5500.5,hyp.traiLep().mc_id(),weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hilowgamma_mc_mother","hyp_ss_faketrail_"+lt+"_hilowgamma_mc_mother",11001,-5500.5,5500.5,hyp.traiLep().mc_motherid(),weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hilowgamma_mc3","hyp_ss_faketrail_"+lt+"_hilowgamma_mc3",11001,-5500.5,5500.5,hyp.traiLep().mc3_id(),weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_hilowgamma_mc3_mother","hyp_ss_faketrail_"+lt+"_hilowgamma_mc3_mother",11001,-5500.5,5500.5,hyp.traiLep().mc3_motherid(),weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_category_hilowgamma_pt","hyp_ss_faketrail_"+lt+"_category_hilowgamma_pt",50,0,100,hyp.traiLep().pt(),weight_);
      } else if (hyp.traiLep().mc_id()==-9999 && hyp.traiLep().mc_motherid()==-9999 && hyp.traiLep().mc3_id()==-9999 && hyp.traiLep().mc3_motherid()==-9999) {
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_category","hyp_ss_faketrail_"+lt+"_category",End,0,End,All9999,weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_category_all9999_pt","hyp_ss_faketrail_"+lt+"_category_all9999_pt",50,0,100,hyp.traiLep().pt(),weight_);
      }
      else {
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_category","hyp_ss_faketrail_"+lt+"_category",End,0,End,Other,weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_other_mc","hyp_ss_faketrail_"+lt+"_other_mc",11001,-5500.5,5500.5,hyp.traiLep().mc_id(),weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_other_mc_mother","hyp_ss_faketrail_"+lt+"_other_mc_mother",11001,-5500.5,5500.5,hyp.traiLep().mc_motherid(),weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_other_mc3","hyp_ss_faketrail_"+lt+"_other_mc3",11001,-5500.5,5500.5,hyp.traiLep().mc3_id(),weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_faketrail_"+lt+"_other_mc3_mother","hyp_ss_faketrail_"+lt+"_other_mc3_mother",11001,-5500.5,5500.5,hyp.traiLep().mc3_motherid(),weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_faketrail_"+lt+"_category_other_pt","hyp_ss_faketrail_"+lt+"_category_other_pt",50,0,100,hyp.traiLep().pt(),weight_);
	/*
	  cout << "UNKNOWN FAKE LEPTON " << lt << endl;
	  cout << "trai lep id=" << hyp.traiLep().pdgId() << " p4=" << hyp.traiLep().p4() 
	  << " mcid=" << hyp.traiLep().mc_id() << " mcp4=" << hyp.traiLep().mc_p4()  << " mother_id=" << hyp.traiLep().mc_motherid()
	  << " mc3idx=" << hyp.traiLep().mc3idx() << " mc3_id=" << hyp.traiLep().mc3_id() 
	  << " mc3_motheridx=" << hyp.traiLep().mc3_motheridx() << " mc3_mother_id=" << hyp.traiLep().mc3_motherid()
	  << " genps_id_mother()[hyp.traiLep().mc3_motheridx()]=" << genps_id_mother()[hyp.traiLep().mc3_motheridx()]
	  << endl;
	*/
      }
    }
	      
  } else {
	      
    isTrailPrompt = 1;
    if ( hyp.traiLep().pdgId()==hyp.traiLep().mc_id() ) trailType=Prompt;
    else if ( abs(hyp.traiLep().pdgId())==abs(hyp.traiLep().mc_id()) ) trailType=PromptWS;
    else if ( abs(hyp.traiLep().pdgId())==11 ? abs(hyp.traiLep().mc_id())==13 : abs(hyp.traiLep().mc_id())==11 ) trailType=PromptWF;
    else if ( hyp.traiLep().mc_id()==22) trailType=PromptFSR;
    else cout << "UNKNOWN PROMPT LEPTON " << lt << endl;
	      
  }
  if ( !isFromW(hyp.leadLep().pdgId(),hyp.leadLep().idx()) ) {
	      
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_mc","hyp_ss_fakelead_"+ll+"_mc",11001,-5500.5,5500.5,hyp.leadLep().mc_id(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_mc_mother","hyp_ss_fakelead_"+ll+"_mc_mother",11001,-5500.5,5500.5,hyp.leadLep().mc_motherid(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_mc3","hyp_ss_fakelead_"+ll+"_mc3",11001,-5500.5,5500.5,hyp.leadLep().mc3_id(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_mc3_mother","hyp_ss_fakelead_"+ll+"_mc3_mother",11001,-5500.5,5500.5,hyp.leadLep().mc3_motherid(),weight_);

    loop->makeFillHisto1D<TH1F,float>("hyp_ss_fakelead_"+lt+"_relIso03","hyp_ss_fakelead_"+lt+"_relIso03",100,0.,1.,hyp.leadLep().relIso03(),weight_);
	      
    if (isFromB(hyp.leadLep().pdgId(),hyp.leadLep().idx()) ) leadType=FakeB;
    else if (isFromC(hyp.leadLep().pdgId(),hyp.leadLep().idx()) ) leadType=FakeC;
    else if (isFromLight(hyp.leadLep().pdgId(),hyp.leadLep().idx())) leadType=FakeLightTrue;
    else if (isFromLightFake(hyp.leadLep().pdgId(),hyp.leadLep().idx())) leadType=FakeLightFake;
    else if (hyp.leadLep().mc_id()==22 && hyp.leadLep().mc_p4().pt()>hyp.leadLep().pt()/2.) leadType=FakeHiPtGamma;
    else {
      leadType=FakeUnknown;
      if (hyp.leadLep().mc_id()==22 && hyp.leadLep().mc_p4().pt()<1.) 
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_category","hyp_ss_fakelead_"+ll+"_category",End,0,End,FakeLowPtGamma,weight_);
      else if (hyp.leadLep().mc_id()==-9999 && hyp.leadLep().mc_motherid()==-9999 && hyp.leadLep().mc3_id()==-9999 && hyp.leadLep().mc3_motherid()==-9999)
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_category","hyp_ss_fakelead_"+ll+"_category",End,0,End,All9999,weight_);
      else {
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_fakelead_"+ll+"_category","hyp_ss_fakelead_"+ll+"_category",End,0,End,Other,weight_);
	/*
	  cout << "UNKNOWN FAKE LEPTON " << ll << endl;
	  cout << "lead lep id=" << hyp.leadLep().pdgId() << " p4=" << hyp.leadLep().p4() 
	  << " mcid=" << hyp.leadLep().mc_id() << " mcp4=" << hyp.leadLep().mc_p4() << " mother_id=" << hyp.leadLep().mc_motherid()
	  << " mc3idx=" << hyp.leadLep().mc3idx() << " mc3_id=" << hyp.leadLep().mc3_id() 
	  << " mc3_motheridx=" << hyp.leadLep().mc3_motheridx() << " mc3_mother_id=" << hyp.leadLep().mc3_motherid()
	  << " genps_id_mother()[hyp.leadLep().mc3_motheridx()]=" << genps_id_mother()[hyp.leadLep().mc3_motheridx()]
	  << endl;
	*/
      }
    }
	      
  } else {
	      
    isLeadPrompt = 1;
    if ( hyp.leadLep().pdgId()==hyp.leadLep().mc_id() ) leadType=Prompt;
    else if ( abs(hyp.leadLep().pdgId())==abs(hyp.leadLep().mc_id()) ) leadType=PromptWS;
    else if ( abs(hyp.leadLep().pdgId())==11 ? abs(hyp.leadLep().mc_id())==13 : abs(hyp.leadLep().mc_id())==11 ) leadType=PromptWF;
    else if ( hyp.leadLep().mc_id()==22) leadType=PromptFSR;
    else cout << "UNKNOWN PROMPT LEPTON " << ll << endl;
	      
  }
	    
  if (trailType>-1) loop->makeFillHisto1D<TH1F,int>("hyp_ss_trail_"+lt+"_category","hyp_ss_trail_"+lt+"_category",End,0,End,trailType,weight_);
  if (leadType>-1 ) loop->makeFillHisto1D<TH1F,int>("hyp_ss_lead_"+ll+"_category","hyp_ss_lead_"+ll+"_category",End,0,End,leadType,weight_);
	    
  if (abs(hyp.traiLep().pdgId())==13 && hyp.traiLep().pt()>10 && trailType>-1) loop->makeFillHisto1D<TH1F,int>("hyp_ss_trail_"+lt+"_ptGt10_category","hyp_ss_trail_"+lt+"_ptGt10_category",End,0,End,trailType,weight_);
  else if (abs(hyp.traiLep().pdgId())==13 && hyp.traiLep().pt()<10 && trailType>-1) loop->makeFillHisto1D<TH1F,int>("hyp_ss_trail_"+lt+"_ptLt10_category","hyp_ss_trail_"+lt+"_ptLt10_category",End,0,End,trailType,weight_);
	    
  int prompttype = -1;
  if (isLeadPrompt && isTrailPrompt) prompttype=0;
  if (isLeadPrompt && !isTrailPrompt) prompttype=1;
  if (!isLeadPrompt && isTrailPrompt) prompttype=2;
  if (!isLeadPrompt && !isTrailPrompt) prompttype=3;
  loop->makeFillHisto1D<TH1F,int>("hyp_prompttype","hyp_prompttype",5,0,5,prompttype,weight_);
  if (trailType==PromptWS) loop->makeFillHisto1D<TH1F,int>("hyp_fliptype","hyp_fliptype",3,0,3,1,weight_);
  else if (leadType==PromptWS) loop->makeFillHisto1D<TH1F,int>("hyp_fliptype","hyp_fliptype",3,0,3,2,weight_);
  else loop->makeFillHisto1D<TH1F,int>("hyp_fliptype","hyp_fliptype",3,0,3,0,weight_);
	    
  if (hyp.leadLep().mc_id()*hyp.traiLep().mc_id()<0) loop->makeFillHisto1D<TH1F,int>("hyp_ismcss","hyp_ismcss",3,0,3,0,weight_);
  else loop->makeFillHisto1D<TH1F,int>("hyp_ismcss","hyp_ismcss",3,0,3,1,weight_);
	    
  if (isLeadPrompt && isTrailPrompt) {
    int leadprompttype = -1;
    if (leadType==0 && trailType==0) {
      leadprompttype=0;
      //cout << "UNEXPECTED DOUBLE PROMPT" << endl;
    } else if (leadType==1 || trailType==1) leadprompttype=1;
    else if (leadType==2 || trailType==2) leadprompttype=1;
    else  leadprompttype=3;
    loop->makeFillHisto1D<TH1F,int>("hyp_leadprompttype","hyp_leadprompttype",5,0,5,leadprompttype,weight_);
  }

}

void tests::testLepIdFailMode( looper* loop, float& weight_, std::vector<Lep>& fobs ) {

  bool debug = false;
  for (unsigned int gp=0;gp<genps_id().size();++gp) {
    int pdgid = abs(genps_id()[gp]);
    if (pdgid!=13 && pdgid!=11) continue;
    if (genps_id_mother()[gp]!=23 && abs(genps_id_mother()[gp])!=24) continue;
    if (genps_status()[gp]!=1) continue;//is this needed?
    if (fabs(genps_p4()[gp].eta())>2.4) continue;
    if (genps_p4()[gp].pt()<5) continue;
    if (pdgid==11 && genps_p4()[gp].pt()<10) continue;
    if (debug) cout << "gen lep id=" << genps_id()[gp] << " eta=" << genps_p4()[gp].eta() << " pt=" << genps_p4()[gp].pt() << endl;	  
    if (pdgid==11) {
      for (unsigned int elidx=0;elidx<els_p4().size();++elidx) {
	if (fabs(ROOT::Math::VectorUtil::DeltaR(els_p4()[elidx],genps_p4()[gp]))>0.1) continue;
	if (debug) cout << "el pt=" << els_p4()[elidx].pt() << " eta=" << els_p4()[elidx].eta() << " q=" << els_charge()[elidx] << " iso=" << eleRelIso03(elidx, SS)<< endl;
	int failmode = 0;
	if (isFakableElectron(elidx)==0) failmode = tests::isElectronFO_debug(elidx);
	if (threeChargeAgree(elidx)==0) {
	  failmode = 10;
	  bool wrongGsf = (genps_charge()[gp]!=els_charge()[elidx]);
	  bool wrongTrk = (genps_charge()[gp]!=els_trk_charge()[elidx]);
	  bool wrongScl = (genps_charge()[gp]!=els_sccharge()[elidx]);
	  loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,0,weight_);
	  if (wrongGsf) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,1,weight_);
	  else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,2,weight_);
	  if (wrongTrk) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,3,weight_);
	  else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,4,weight_);
	  if (wrongScl) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,5,weight_);
	  else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode","el_fo_q3fail_mode",7,0,7,6,weight_);
	  if (fabs(els_etaSC().at(elidx)) <= 1.479){
	    loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,0,weight_);
	    if (wrongGsf) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,1,weight_);
	    else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,2,weight_);
	    if (wrongTrk) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,3,weight_);
	    else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,4,weight_);
	    if (wrongScl) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,5,weight_);
	    else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_barrel","el_fo_q3fail_mode_barrel",7,0,7,6,weight_);
	  } else {
	    loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,0,weight_);
	    if (wrongGsf) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,1,weight_);
	    else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,2,weight_);
	    if (wrongTrk) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,3,weight_);
	    else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,4,weight_);
	    if (wrongScl) loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,5,weight_);
	    else loop->makeFillHisto1D<TH1F,int>("el_fo_q3fail_mode_endcap","el_fo_q3fail_mode_endcap",7,0,7,6,weight_);
	  }
	}
	if (fabs(els_etaSC().at(elidx)) <= 1.479)
	  loop->makeFillHisto1D<TH1F,float>("el_fo_fail_sietaieta_barrel","el_fo_fail_sietaieta_barrel",25,0.,0.025,els_sigmaIEtaIEta_full5x5().at(elidx),weight_);
	else
	  loop->makeFillHisto1D<TH1F,float>("el_fo_fail_sietaieta_endcap","el_fo_fail_sietaieta_endcap",25,0.,0.050,els_sigmaIEtaIEta_full5x5().at(elidx),weight_);
	if (fabs(els_p4().at(elidx).eta())>2.4) failmode = 11;
	if (els_p4().at(elidx).pt()<10.) failmode = 12;
	if (isFakableElectron(elidx)==0) {
	  loop->makeFillHisto1D<TH1F,int>("el_fo_fail_mode","el_fo_fail_mode",15,0,15,failmode,weight_);
	  if (fabs(els_etaSC().at(elidx)) <= 1.479) loop->makeFillHisto1D<TH1F,int>("el_fo_fail_mode_barrel","el_fo_fail_mode_barrel",15,0,15,failmode,weight_);
	  else loop->makeFillHisto1D<TH1F,int>("el_fo_fail_mode_endcap","el_fo_fail_mode_endcap",15,0,15,failmode,weight_);
	  continue;
	}
	if (debug) cout << "pass FO selection" << endl;
      }
    }
  }
  if (debug) {
    for (unsigned int fo=0;fo<fobs.size();++fo) {
      cout<< "fo lep id=" << fobs[fo].pdgId() << " eta=" << fobs[fo].eta() << " pt=" << fobs[fo].pt() << " relIso=" << fobs[fo].relIso03()<< endl;
    }
  }

}

int tests::isElectronFO_debug(unsigned int elIdx){//fixme
  if(fabs(els_etaSC().at(elIdx)) <= 1.479){
    if(fabs(els_dEtaIn().at(elIdx)) >= 0.004) return 1;
    if(fabs(els_dPhiIn().at(elIdx)) >= 0.06) return 2; 
    if(els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return 3; 
    if(els_hOverE().at(elIdx) >= 0.12) return 4; 
    if(fabs(els_dxyPV().at(elIdx)) >= 0.05) return 5; //is this wrt the correct PV?
    if(fabs(els_dzPV().at(elIdx)) >= 0.1) return 5; //is this wrt the correct PV?
    if( fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return 6; // |1/E - 1/p|
    if( eleRelIso03(elIdx, SS) >= 0.5 ) return 7; 
    if( els_conv_vtx_flag().at(elIdx) ) return 8;
    if( els_exp_innerlayers().at(elIdx) > 0) return 9;
    return 0;
  } else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(els_dEtaIn().at(elIdx)) >= 0.007) return 1;
    if(fabs(els_dPhiIn().at(elIdx)) >= 0.03) return 2; 
    if(els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return 3; 
    if(els_hOverE().at(elIdx) >= 0.1) return 4; 
    if(fabs(els_dxyPV().at(elIdx)) >= 0.05) return 5; //is this wrt the correct PV?
    if(fabs(els_dzPV().at(elIdx)) >= 0.1) return 5; //is this wrt the correct PV?
    if( fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return 6; // |1/E - 1/p|
    if( eleRelIso03(elIdx, SS) >= 0.5 ) return 7; 
    if( els_conv_vtx_flag().at(elIdx) ) return 8;
    if( els_exp_innerlayers().at(elIdx) > 0) return 9;
    return 0;
  } else return -1;
}


void tests::computeFakeRateAndClosure( looper* loop, float& weight_, std::vector<Lep>& fobs, std::vector<Lep>& goodleps, TFile* fr_file ) {

  for (unsigned int fo=0;fo<fobs.size();++fo) {
    if (isFromW(fobs[fo].pdgId(),fobs[fo].idx())) continue;
    int pdgid = abs(fobs[fo].pdgId());
    //denominator
    loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_den":"fr_el_den"),(pdgid==13?"fr_mu_den":"fr_el_den"),10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
    //numerator
    bool isNumerator = false;
    for (unsigned int gl=0;gl<goodleps.size();++gl) {
      if (abs(goodleps[gl].pdgId())==pdgid && goodleps[gl].idx()==fobs[fo].idx()) {
	//cout << "goodleps.size()=" << goodleps.size() << endl;
	loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_num":"fr_el_num"),(pdgid==13?"fr_mu_num":"fr_el_num"),10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_);
	isNumerator = true;
	break;
      }
    }
    if (!isNumerator && fr_file) {
      TH2F* fr_h = (TH2F*) fr_file->Get((pdgid==13?"fr_mu_gen":"fr_el_gen"));
      float maxPt=fr_h->GetXaxis()->GetBinUpEdge(fr_h->GetXaxis()->GetNbins())-0.01; 
      float maxEta=fr_h->GetYaxis()->GetBinUpEdge(fr_h->GetYaxis()->GetNbins())-0.01;
      float pt = fobs[fo].pt();
      float eta = fabs(fobs[fo].eta());
      if (pt>maxPt) pt=maxPt;
      if (eta>maxEta) eta=maxEta;
      float fr = fr_h->GetBinContent(fr_h->FindBin(pt,eta));
      //float fre = fr_h->GetBinError(fr_h->FindBin(pt,eta));
      float frW = fr/(1.-fr);
      //std::cout << "fake rate=" << fr << " +/- " << fre << std::endl;
      loop->makeFillHisto2D<TH2F,float>((pdgid==13?"fr_mu_close":"fr_el_close"),(pdgid==13?"fr_mu_close":"fr_el_close"),10,0.,50.,fobs[fo].pt(),5,0.,2.5,fabs(fobs[fo].eta()),weight_*frW);
    }
  }

}

void tests::testPtRel( looper* loop, float& weight_, DilepHyp& hyp, std::vector<Jet>& lepjets, TString& ll, TString& lt ) {

  bool debug = false;

  //make sure all cuts pass except isolation
  if (isGoodLeptonNoIso(hyp.traiLep().pdgId(), hyp.traiLep().idx())) {
    if (isFromW(hyp.traiLep().pdgId(),hyp.traiLep().idx())) {
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWtrail_"+lt+"_relIso03","hyp_ss_foFromWtrail_"+lt+"_relIso03",20,0.,2.,hyp.traiLep().relIso03(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWtrail_"+lt+"_iso03","hyp_ss_foFromWtrail_"+lt+"_iso03",10,0.,20.,hyp.traiLep().relIso03()*hyp.traiLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWtrail_"+lt+"_pt","hyp_ss_foFromWtrail_"+lt+"_pt",10,0.,200.,hyp.traiLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWtrail_"+lt+"_dxyPV","hyp_ss_foFromWtrail_"+lt+"_dxyPV",100,0,0.1,fabs(hyp.traiLep().dxyPV()),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWtrail_"+lt+"_dzPV","hyp_ss_foFromWtrail_"+lt+"_dzPV",100,0,0.5,fabs(hyp.traiLep().dzPV()),weight_);
    } else {
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFaketrail_"+lt+"_relIso03","hyp_ss_foFaketrail_"+lt+"_relIso03",20,0.,2.,hyp.traiLep().relIso03(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFaketrail_"+lt+"_iso03","hyp_ss_foFaketrail_"+lt+"_iso03",10,0.,20.,hyp.traiLep().relIso03()*hyp.traiLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFaketrail_"+lt+"_pt","hyp_ss_foFaketrail_"+lt+"_pt",10,0.,200.,hyp.traiLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFaketrail_"+lt+"_dxyPV","hyp_ss_foFaketrail_"+lt+"_dxyPV",100,0,0.1,fabs(hyp.traiLep().dxyPV()),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFaketrail_"+lt+"_dzPV","hyp_ss_foFaketrail_"+lt+"_dzPV",100,0,0.5,fabs(hyp.traiLep().dzPV()),weight_);
    }
  }

  //make sure all cuts pass except isolation
  if (isGoodLeptonNoIso(hyp.leadLep().pdgId(), hyp.leadLep().idx())) {
    if (isFromW(hyp.leadLep().pdgId(),hyp.leadLep().idx())) {
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_relIso03","hyp_ss_foFromWlead_"+ll+"_relIso03",20,0.,2.,hyp.leadLep().relIso03(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_iso03","hyp_ss_foFromWlead_"+ll+"_iso03",10,0.,20.,hyp.leadLep().relIso03()*hyp.leadLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_pt","hyp_ss_foFromWlead_"+ll+"_pt",10,0.,200.,hyp.leadLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+lt+"_dxyPV","hyp_ss_foFromWlead_"+lt+"_dxyPV",100,0,0.1,fabs(hyp.leadLep().dxyPV()),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+lt+"_dzPV","hyp_ss_foFromWlead_"+lt+"_dzPV",100,0,0.5,fabs(hyp.leadLep().dzPV()),weight_);
	    
      if (isGoodLepton(hyp.leadLep().pdgId(), hyp.leadLep().idx(), false)==0) {
	if (ll=="mu") {
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_relIso03","hyp_ss_foFromWlead_fail_"+ll+"_relIso03",20,0.,2.,hyp.leadLep().relIso03(),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_miniRelIso","hyp_ss_foFromWlead_fail_"+ll+"_miniRelIso",20,0.,1., muMiniRelIso(hyp.leadLep().idx()),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_relIso02","hyp_ss_foFromWlead_fail_"+ll+"_relIso02",20,0.,1., muRelIsoCustomCone(hyp.leadLep().idx(),0.2),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_relIso02DB","hyp_ss_foFromWlead_fail_"+ll+"_relIso02DB",20,0.,1., muRelIsoCustomConeDB(hyp.leadLep().idx(),0.2),weight_);
	} else {
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_relIso03","hyp_ss_foFromWlead_fail_"+ll+"_relIso03",20,0.,2.,hyp.leadLep().relIso03(),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_miniRelIso","hyp_ss_foFromWlead_fail_"+ll+"_miniRelIso",20,0.,1., elMiniRelIso(hyp.leadLep().idx()),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_relIso02","hyp_ss_foFromWlead_fail_"+ll+"_relIso02",20,0.,1., elRelIsoCustomCone(hyp.leadLep().idx(),0.2),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_fail_"+ll+"_relIso02DB","hyp_ss_foFromWlead_fail_"+ll+"_relIso02DB",20,0.,1., elRelIsoCustomConeDB(hyp.leadLep().idx(),0.2),weight_);
	}
	float ptrel = getPtRel(hyp.leadLep().pdgId(), hyp.leadLep().idx(), false);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_ptrel","hyp_ss_foFromWlead_"+ll+"_ptrel",10,0.,20.,ptrel,weight_);
	float ptrelsub = getPtRel(hyp.leadLep().pdgId(), hyp.leadLep().idx(), true);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_ptrelsub","hyp_ss_foFromWlead_"+ll+"_ptrelsub",10,0.,20.,ptrelsub,weight_);
	if (ptrelsub>14) 
	  loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFromWlead_"+ll+"_ptrel14_eta_vs_pt","hyp_ss_foFromWlead_"+ll+"_ptrel14_eta_vs_pt",10,0,100,hyp.leadLep().pt(),3,0,3,hyp.leadLep().eta(),weight_);
	//test min dR between leptons and jets
	int lepjetidx = -1;
	if (debug) cout << "prompt non iso lepton with lepjets.size()=" << lepjets.size() << endl;
	float mindr = 0.7;
	int nlepjets_per_lep = 0;
	for (unsigned int j=0;j<lepjets.size();++j) {
	  float dr = deltaR(lepjets[j].p4(),hyp.leadLep().p4());
	  if (debug) cout << "dr=" << dr << endl;
	  if (dr<0.7) nlepjets_per_lep++;
	  if (dr<mindr) {
	    mindr = dr;
	    lepjetidx = j;
	  }
	} 
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_mindr","hyp_ss_foFromWlead_"+ll+"_mindr",20,0.,1.,mindr,weight_);
	loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFromWlead_"+ll+"_ptrel_vs_mindr","hyp_ss_foFromWlead_"+ll+"_ptrel_vs_mindr",10,0.,1.,mindr,10,0.,20.,ptrel,weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_foFromWlead_"+ll+"_nlepjets","hyp_ss_foFromWlead_"+ll+"_nlepjets",10,0,10,nlepjets_per_lep,weight_);
	loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFromWlead_"+ll+"_mindr_vs_nlepjets","hyp_ss_foFromWlead_"+ll+"_mindr_vs_nlepjets",10,0,10,nlepjets_per_lep,10,0,1,mindr,weight_);	      
	if (lepjetidx>=0) { 
	  if (mindr<0.1) loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_dPtMinDr01","hyp_ss_foFromWlead_"+ll+"_dPtMinDr01",20,-1.,1.,(lepjets[lepjetidx].pt()-hyp.leadLep().pt())/hyp.leadLep().pt(),weight_); 
	  if (ll=="mu") loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFromWlead_"+ll+"_ptrel_vs_lepfrac","hyp_ss_foFromWlead_"+ll+"_ptrel_vs_lepfrac",10,0.,1.,pfjets_muonE()[lepjets[lepjetidx].idx()]/(lepjets[lepjetidx].p4().E()*pfjets_undoJEC()[lepjets[lepjetidx].idx()]),10,0.,20.,ptrel,weight_);
	  else loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFromWlead_"+ll+"_ptrel_vs_lepfrac","hyp_ss_foFromWlead_"+ll+"_ptrel_vs_lepfrac",10,0.,1.,pfjets_electronE()[lepjets[lepjetidx].idx()]/(lepjets[lepjetidx].p4().E()*pfjets_undoJEC()[lepjets[lepjetidx].idx()]),10,0.,20.,ptrel,weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFromWlead_"+ll+"_sinA","hyp_ss_foFromWlead_"+ll+"_sinA",10,0.,1.,ptrel/hyp.leadLep().pt(),weight_);
	}
      }
	    
    } else {
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_relIso03","hyp_ss_foFakelead_"+ll+"_relIso03",20,0.,2.,hyp.leadLep().relIso03(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_iso03","hyp_ss_foFakelead_"+ll+"_iso03",10,0.,20.,hyp.leadLep().relIso03()*hyp.leadLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_pt","hyp_ss_foFakelead_"+ll+"_pt",10,0.,200.,hyp.leadLep().pt(),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+lt+"_dxyPV","hyp_ss_foFakelead_"+lt+"_dxyPV",100,0,0.1,fabs(hyp.leadLep().dxyPV()),weight_);
      loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+lt+"_dzPV","hyp_ss_foFakelead_"+lt+"_dzPV",100,0,0.5,fabs(hyp.leadLep().dzPV()),weight_);
	    
      if (isGoodLepton(hyp.leadLep().pdgId(), hyp.leadLep().idx(), false)==0) {
	if (ll=="mu") {
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_relIso03","hyp_ss_foFakelead_fail_"+ll+"_relIso03",20,0.,2.,hyp.leadLep().relIso03(),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_miniRelIso","hyp_ss_foFakelead_fail_"+ll+"_miniRelIso",20,0.,1., muMiniRelIso(hyp.leadLep().idx()),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_relIso02","hyp_ss_foFakelead_fail_"+ll+"_relIso02",20,0.,1., muRelIsoCustomCone(hyp.leadLep().idx(),0.2),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_relIso02DB","hyp_ss_foFakelead_fail_"+ll+"_relIso02DB",20,0.,1., muRelIsoCustomConeDB(hyp.leadLep().idx(),0.2),weight_);
	} else {
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_relIso03","hyp_ss_foFakelead_fail_"+ll+"_relIso03",20,0.,2.,hyp.leadLep().relIso03(),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_miniRelIso","hyp_ss_foFakelead_fail_"+ll+"_miniRelIso",20,0.,1., elMiniRelIso(hyp.leadLep().idx()),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_relIso02","hyp_ss_foFakelead_fail_"+ll+"_relIso02",20,0.,1., elRelIsoCustomCone(hyp.leadLep().idx(),0.2),weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_fail_"+ll+"_relIso02DB","hyp_ss_foFakelead_fail_"+ll+"_relIso02DB",20,0.,1., elRelIsoCustomConeDB(hyp.leadLep().idx(),0.2),weight_);
	}
	float ptrel = getPtRel(hyp.leadLep().pdgId(), hyp.leadLep().idx(), false);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_ptrel","hyp_ss_foFakelead_"+ll+"_ptrel",10,0.,20.,ptrel,weight_);
	float ptrelsub = getPtRel(hyp.leadLep().pdgId(), hyp.leadLep().idx(), true);
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_ptrelsub","hyp_ss_foFakelead_"+ll+"_ptrelsub",10,0.,20.,ptrelsub,weight_);
	if (ptrelsub>14) 
	  loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFakelead_"+ll+"_ptrel14_eta_vs_pt","hyp_ss_foFakelead_"+ll+"_ptrel14_eta_vs_pt",10,0,100,hyp.leadLep().pt(),3,0,3,hyp.leadLep().eta(),weight_);
	//test min dR between leptons and jets
	int lepjetidx = -1;
	if (debug) cout << "prompt non iso lepton with lepjets.size()=" << lepjets.size() << endl;
	float mindr = 0.7;
	int nlepjets_per_lep = 0;
	for (unsigned int j=0;j<lepjets.size();++j) {
	  float dr = deltaR(lepjets[j].p4(),hyp.leadLep().p4());
	  if (debug) cout << "dr=" << dr << endl;
	  if (dr<0.7) nlepjets_per_lep++;
	  if (dr<mindr) {
	    mindr = dr;
	    lepjetidx = j;
	  }
	} 
	loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_mindr","hyp_ss_foFakelead_"+ll+"_mindr",20,0.,1.,mindr,weight_);
	loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFakelead_"+ll+"_ptrel_vs_mindr","hyp_ss_foFakelead_"+ll+"_ptrel_vs_mindr",10,0.,1.,mindr,10,0.,20.,ptrel,weight_);
	loop->makeFillHisto1D<TH1F,int>("hyp_ss_foFakelead_"+ll+"_nlepjets","hyp_ss_foFakelead_"+ll+"_nlepjets",10,0,10,nlepjets_per_lep,weight_);
	loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFakelead_"+ll+"_mindr_vs_nlepjets","hyp_ss_foFakelead_"+ll+"_mindr_vs_nlepjets",10,0,10,nlepjets_per_lep,10,0,1,mindr,weight_);	      
	if (lepjetidx>=0) { 
	  if (mindr<0.1) loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_dPtMinDr01","hyp_ss_foFakelead_"+ll+"_dPtMinDr01",20,-1.,1.,(lepjets[lepjetidx].pt()-hyp.leadLep().pt())/hyp.leadLep().pt(),weight_); 
	  if (ll=="mu") loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFakelead_"+ll+"_ptrel_vs_lepfrac","hyp_ss_foFakelead_"+ll+"_ptrel_vs_lepfrac",10,0.,1.,pfjets_muonE()[lepjets[lepjetidx].idx()]/(lepjets[lepjetidx].p4().E()*pfjets_undoJEC()[lepjets[lepjetidx].idx()]),10,0.,20.,ptrel,weight_);
	  else loop->makeFillHisto2D<TH2F,float>("hyp_ss_foFakelead_"+ll+"_ptrel_vs_lepfrac","hyp_ss_foFakelead_"+ll+"_ptrel_vs_lepfrac",10,0.,1.,pfjets_electronE()[lepjets[lepjetidx].idx()]/(lepjets[lepjetidx].p4().E()*pfjets_undoJEC()[lepjets[lepjetidx].idx()]),10,0.,20.,ptrel,weight_);
	  loop->makeFillHisto1D<TH1F,float>("hyp_ss_foFakelead_"+ll+"_sinA","hyp_ss_foFakelead_"+ll+"_sinA",10,0.,1.,ptrel/hyp.leadLep().pt(),weight_);
	}
      }
    } 
  }

}

void tests::testLeptonIdVariables( looper* loop, float& weight_, DilepHyp& hyp, TString& ll, TString& lt ) {

  plotLeptonIdVariables( loop, weight_, "hyp_ss_alltrail_", hyp.traiLep().pdgId(), hyp.traiLep().idx());
  plotLeptonIdVariables( loop, weight_, "hyp_ss_alllead_",  hyp.leadLep().pdgId(), hyp.leadLep().idx());
}

void tests::plotLeptonIdVariables( looper* loop, float& weight_, TString prefix, int pdgId, unsigned int idx) {

  if (abs(pdgId)==11) {
    unsigned int elIdx = idx;
    
    TString ebee = "EB";
    if (fabs(els_etaSC().at(elIdx)) > 1.479) ebee = "EE";

    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_pt",prefix+"_el_"+ebee+"_pt",20,0,100,els_p4().at(elIdx).pt(),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_eta",prefix+"_el_"+ebee+"_eta",20,-3,3,els_p4().at(elIdx).eta(),weight_);

    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_dEtaIn",prefix+"_el_"+ebee+"_dEtaIn",20,0,0.02,fabs(els_dEtaIn().at(elIdx)),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_dPhiIn",prefix+"_el_"+ebee+"_dPhiIn",20,0,0.1,fabs(els_dPhiIn().at(elIdx)),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_sIEtaIEta",prefix+"_el_"+ebee+"_sIEtaIEta",20,0,0.05,els_sigmaIEtaIEta_full5x5().at(elIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_hOverE",prefix+"_el_"+ebee+"_hOverE",20,0,0.2,els_hOverE().at(elIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_EoP",prefix+"_el_"+ebee+"_EoP",100,0,0.1,fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_convflag",prefix+"_el_"+ebee+"_convflag",3,0,3,els_conv_vtx_flag().at(elIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_expinlay",prefix+"_el_"+ebee+"_expinlay",5,0,5,els_exp_innerlayers().at(elIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_3Qagree",prefix+"_el_"+ebee+"_3Qagree",3,0,3,els_isGsfCtfScPixChargeConsistent().at(elIdx),weight_);

    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_relIso03",prefix+"_el_"+ebee+"_relIso03",20,0,1.,eleRelIso03(elIdx, SS),weight_);

    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_sip3d",prefix+"_el_"+ebee+"_sip3d",20,0,10,fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_dxyPV",prefix+"_el_"+ebee+"_dxyPV",100,0,0.1,fabs(els_dxyPV().at(elIdx)),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_el_"+ebee+"_dzPV",prefix+"_el_"+ebee+"_dzPV",100,0,0.5,fabs(els_dzPV().at(elIdx)),weight_);
  } else {
    unsigned int muIdx = idx;
    bool isGlobal  = true;
    bool isTracker = true;
    if (((mus_type().at(muIdx)) & (1<<1)) == 0) isGlobal  = false;
    if (((mus_type().at(muIdx)) & (1<<2)) == 0) isTracker = false;

    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_pt",prefix+"_mu_pt",20,0,100,mus_p4().at(muIdx).pt(),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_eta",prefix+"_mu_eta",20,-3,3,mus_p4().at(muIdx).eta(),weight_);

    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_isPf",prefix+"_mu_isPf",2,0,2,mus_pid_PFMuon().at(muIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_isGl",prefix+"_mu_isGl",2,0,2,isGlobal,weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_isTk",prefix+"_mu_isTk",2,0,2,isTracker,weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_chi2",prefix+"_mu_chi2",100,0,20,mus_gfit_chi2().at(muIdx)/mus_gfit_ndof().at(muIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_vStaH",prefix+"_mu_vStaH",20,0,20,mus_gfit_validSTAHits().at(muIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_nMatchSt",prefix+"_mu_nMatchSt",20,0,20,mus_numberOfMatchedStations().at(muIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_vPixH",prefix+"_mu_vPixH",10,0,10,mus_validPixelHits().at(muIdx),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_nLay",prefix+"_mu_nLay",20,0,20,mus_nlayers().at(muIdx),weight_);

    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_relIso03",prefix+"_mu_relIso03",20,0,1.,muRelIso03(muIdx, SS),weight_);

    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_dxyPV",prefix+"_mu_dxyPV",100,0,0.1,fabs(mus_dxyPV().at(muIdx)),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_dzPV",prefix+"_mu_dzPV",100,0,0.5,fabs(mus_dzPV().at(muIdx)),weight_);
    loop->makeFillHisto1D<TH1F,float>(prefix+"_mu_sip3d",prefix+"_mu_sip3d",20,0,10,fabs(mus_ip3d().at(muIdx))/mus_ip3derr().at(muIdx),weight_);
  }

}


void tests::testBtag( looper* loop, float& weight_, std::vector<Jet>& alljets ) {

  int genbjets = 0;
  int genbjets_pt20 = 0;
  int genbjets_pt25 = 0;
  int genbjets_pt30 = 0;
  int genbjets_pt35 = 0;
  int genbjets_pt40 = 0;
  vector<Jet> jetsbmatch;
  for (unsigned int j=0;j<alljets.size();++j) {
    Jet jet = alljets[j];
    if (abs(jet.mc3_id())==5) {
      genbjets++;
      jetsbmatch.push_back(jet);
      if (jet.pt()>20) {
	genbjets_pt20++;
	if (jet.pt()>25) genbjets_pt25++;
	if (jet.pt()>30) genbjets_pt30++;
	if (jet.pt()>35) genbjets_pt35++;
	if (jet.pt()>40) genbjets_pt40++;
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_csv","hyp_hihi_genbjets_csv",50,0,1.,jet.csv(),weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_tche","hyp_hihi_genbjets_tche",50,-5.,5.,pfjets_trackCountingHighEffBJetTag()[jet.idx()],weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_tchp","hyp_hihi_genbjets_tchp",50,-5.,5.,pfjets_trackCountingHighPurBJetTag()[jet.idx()],weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_jbp","hyp_hihi_genbjets_jbp",50,0,10.,pfjets_jetBProbabilityBJetTag()[jet.idx()],weight_);
      }
    } else {
      if (jet.pt()>20) {
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_gennobjets_csv","hyp_hihi_gennobjets_csv",50,0,1.,jet.csv(),weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_gennobjets_tche","hyp_hihi_gennobjets_tche",50,-5.,5.,pfjets_trackCountingHighEffBJetTag()[jet.idx()],weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_gennobjets_tchp","hyp_hihi_gennobjets_tchp",50,-5.,5.,pfjets_trackCountingHighPurBJetTag()[jet.idx()],weight_);
	loop->makeFillHisto1D<TH1F,float>("hyp_hihi_gennobjets_jbp","hyp_hihi_gennobjets_jbp",50,0,10.,pfjets_jetBProbabilityBJetTag()[jet.idx()],weight_);
	/*
	  OBJ: TNamed	pfjets_combinedSecondaryVertexBJetTag	floats_pfJetMaker_pfjetscombinedSecondaryVertexBJetTag_CMS2.obj : 0 at: 0x7fb6f3a4bf20
	  OBJ: TNamed	pfjets_jetBProbabilityBJetTag	floats_pfJetMaker_pfjetsjetBProbabilityBJetTag_CMS2.obj : 0 at: 0x7fb6f3a4c540
	  OBJ: TNamed	pfjets_jetProbabilityBJetTag	floats_pfJetMaker_pfjetsjetProbabilityBJetTag_CMS2.obj : 0 at: 0x7fb6f3a4c600
	  OBJ: TNamed	pfjets_simpleSecondaryVertexHighEffBJetTag	floats_pfJetMaker_pfjetssimpleSecondaryVertexHighEffBJetTag_CMS2.obj : 0 at: 0x7fb6f3a4ca80
	  OBJ: TNamed	pfjets_trackCountingHighEffBJetTag	floats_pfJetMaker_pfjetstrackCountingHighEffBJetTag_CMS2.obj : 0 at: 0x7fb6f3a4cc40
	  OBJ: TNamed	pfjets_trackCountingHighPurBJetTag	floats_pfJetMaker_pfjetstrackCountingHighPurBJetTag_CMS2.obj : 0 at: 0x7fb6f3a4cd10
	*/
      }
    }
  }
  loop->makeFillHisto1D<TH1F,int>("hyp_hihi_genbjets","hyp_hihi_genbjets",8,0,8,genbjets,weight_);
  loop->makeFillHisto1D<TH1F,int>("hyp_hihi_genbjets_pt20","hyp_hihi_genbjets_pt20",8,0,8,genbjets_pt20,weight_);
  loop->makeFillHisto1D<TH1F,int>("hyp_hihi_genbjets_pt25","hyp_hihi_genbjets_pt25",8,0,8,genbjets_pt25,weight_);
  loop->makeFillHisto1D<TH1F,int>("hyp_hihi_genbjets_pt30","hyp_hihi_genbjets_pt30",8,0,8,genbjets_pt30,weight_);
  loop->makeFillHisto1D<TH1F,int>("hyp_hihi_genbjets_pt35","hyp_hihi_genbjets_pt35",8,0,8,genbjets_pt35,weight_);
  loop->makeFillHisto1D<TH1F,int>("hyp_hihi_genbjets_pt40","hyp_hihi_genbjets_pt40",8,0,8,genbjets_pt40,weight_);
  std::sort(jetsbmatch.begin(),jetsbmatch.end(),jetptsort);
  if (jetsbmatch.size()>0) loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_pt0","hyp_hihi_genbjets_pt0",40,0,200,jetsbmatch[0].pt(),weight_);
  if (jetsbmatch.size()>1) loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_pt1","hyp_hihi_genbjets_pt1",40,0,200,jetsbmatch[1].pt(),weight_);
  if (jetsbmatch.size()>2) loop->makeFillHisto1D<TH1F,float>("hyp_hihi_genbjets_pt2","hyp_hihi_genbjets_pt2",40,0,200,jetsbmatch[2].pt(),weight_);
  //now check the performance as a function of the btagging algo & discriminator

}

void tests::makeSRplots( looper* loop, float& weight_, TString label, int& br, int& sr, DilepHyp& hyp, 
			 float& ht, float& met, float& mtmin, int& type, std::vector<Lep>& goodleps, std::vector<Lep>& fobs, 
			 std::vector<Lep>& vetoleps, std::vector<Jet>& jets, std::vector<Jet>& alljets, std::vector<Jet>& btags, 
			 TString& ll, TString& lt ) {

  loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_br","hyp_"+label+"_br",40,0,40,br,weight_);
  //if ( isFromWZ(hyp.traiLep()) && isFromWZ(hyp.leadLep()) ) {
  //  loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_br_fromWZ","hyp_"+label+"_br_fromWZ",40,0,40,br,weight_);
  //}
  if (sr>0) {
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_sr","hyp_"+label+"_sr",40,0,40,br,weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_sr","hyp_"+label+"_sr",40,0,40,sr,weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_excl_sr","hyp_"+label+"_excl_sr",40,0,40,sr,weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_excl_sr_mt100","hyp_"+label+"_excl_sr_mt100",40*2,0,40*2,sr+40*(mtmin>100),weight_);
    int srt1 = signalRegion(jets.size(), btags.size(), met, ht, 5, 200, 600);
    int srt2 = signalRegion(jets.size(), btags.size(), met, ht, 6, 250, 800);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_excl_srt1","hyp_"+label+"_excl_srt1",40,0,40,srt1,weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_excl_srt2","hyp_"+label+"_excl_srt2",40,0,40,srt2,weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_excl_srt1_mt100","hyp_"+label+"_excl_srt1_mt100",40*2,0,40*2,srt1+40*(mtmin>100),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_excl_srt2_mt100","hyp_"+label+"_excl_srt2_mt100",40*2,0,40*2,srt2+40*(mtmin>100),weight_);
    //if ( isFromWZ(hyp.traiLep()) && isFromWZ(hyp.leadLep()) ) {
    //  loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_sr_fromWZ","hyp_"+label+"_sr_fromWZ",40,0,40,br,weight_);
    //  loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_sr_fromWZ","hyp_"+label+"_sr_fromWZ",40,0,40,sr,weight_);
    //}
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_charge","hyp_"+label+"_charge",7,-3.5,3.5,hyp.charge(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_njets","hyp_"+label+"_njets",8,0,8,jets.size(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_nbtag","hyp_"+label+"_nbtag",8,0,8,btags.size(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ht","hyp_"+label+"_ht",30,80,1580,ht,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_met","hyp_"+label+"_met",20,0,500,met,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_mll","hyp_"+label+"_mll",100,0,1000,hyp.p4().mass(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ptll","hyp_"+label+"_ptll",100,0,1000,hyp.p4().pt(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_type","hyp_"+label+"_type",5,0,5,type,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ptlead","hyp_"+label+"_ptlead",40,0,200,hyp.leadLep().pt(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_pttrai","hyp_"+label+"_pttrai",40,0,200,hyp.traiLep().pt(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_etalead","hyp_"+label+"_etalead",10,0,2.5,fabs(hyp.leadLep().eta()),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_etatrai","hyp_"+label+"_etatrai",10,0,2.5,fabs(hyp.traiLep().eta()),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_mtmin","hyp_"+label+"_mtmin",15,0,300,mtmin,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ngleps","hyp_"+label+"_ngleps",10,0,10,goodleps.size(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_nfobs","hyp_"+label+"_nfobs",10,0,10,fobs.size(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_nvetos","hyp_"+label+"_nvetos",10,0,10,vetoleps.size(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_trail_"+lt+"_relIso03","hyp_"+label+"_trail_"+lt+"_relIso03",100,0.,1.,hyp.traiLep().relIso03(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_lead_"+ll+"_relIso03","hyp_"+label+"_lead_"+ll+"_relIso03",100,0.,1.,hyp.leadLep().relIso03(),weight_);
    loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_leadjetpt","hyp_"+label+"_leadjetpt",50,0,1000,jets[0].pt(),weight_);
    if (btags.size()>0) loop->makeFillHisto1D<TH1F,int>("hyp_"+label+"_leadbtagpt","hyp_"+label+"_leadbtagpt",50,0,1000,btags[0].pt(),weight_);
    float ld = computeLD(hyp, alljets, met, mtmin);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ld","hyp_"+label+"_ld",30,0,3.0,ld,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ld5bins","hyp_"+label+"_ld5bins",5,0,1.6,ld,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ld10bins","hyp_"+label+"_ld10bins",10,0,1.6,ld,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_excl_srld","hyp_"+label+"_excl_srld",30*4,0,3.0*4,std::min(ld,float(3.0))+3.0*btags.size(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_excl_srld5bins","hyp_"+label+"_excl_srld5bins",5*4,0,1.6*4.,std::min(ld,float(1.6))+1.6*btags.size(),weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_excl_srld10bins","hyp_"+label+"_excl_srld10bins",10*4,0,1.6*4.,std::min(ld,float(1.6))+1.6*btags.size(),weight_);
    //ptrel
    vector<LorentzVector> jetp4s;
    for (unsigned int pfjidx=0;pfjidx<pfjets_p4().size();++pfjidx) {
      Jet jet(pfjidx);
      if (fabs(jet.eta())>2.4) continue;
      if (isLoosePFJet(pfjidx)==false) continue;
      jetp4s.push_back(jet.p4());
    }
    float ptRelLead = ptRel(hyp.leadLep().p4(), jetp4s, true);
    float ptRelTrai = ptRel(hyp.traiLep().p4(), jetp4s, true);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ptRellead","hyp_"+label+"_ptRellead",10,0,20,ptRelLead,weight_);
    loop->makeFillHisto1D<TH1F,float>("hyp_"+label+"_ptReltrai","hyp_"+label+"_ptReltrai",10,0,20,ptRelTrai,weight_);

  }

}

