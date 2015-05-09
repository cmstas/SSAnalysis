// system include files
#include <memory>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "MCUtilities.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/WeightsInfo.h"

class producer : public edm::EDProducer {
   public:
      explicit producer(const edm::ParameterSet&);
      ~producer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      int counter;
      int highPtCounter;
      int numLeptons;

      //Input tags
      edm::InputTag genParticlesInputTag_;
      edm::InputTag genJetsInputTag_;
};

typedef math::XYZTLorentzVectorF LorentzVector;

producer::producer(const edm::ParameterSet& iConfig){
  
   genParticlesInputTag_ = iConfig.getParameter<edm::InputTag> ("genParticlesInputTag" );
   genJetsInputTag_ = iConfig.getParameter<edm::InputTag> ("genJetsInputTag" );

   //Declare gen particle branches
   produces<std::vector<LorentzVector> > ("genps").setBranchAlias("genps");
   produces<std::vector<int> > ("genpsid").setBranchAlias("gen_id");
   produces<std::vector<float> > ("genpspt").setBranchAlias("gen_pt");
   produces<std::vector<float> > ("genpseta").setBranchAlias("gen_eta");
   produces<std::vector<float> > ("genpsphi").setBranchAlias("gen_phi");
   produces<std::vector<int> > ("genpsstatus").setBranchAlias("gen_status");
   produces<std::vector<int> > ("genpsmotherid").setBranchAlias("gen_mother_id");
   produces<std::vector<LorentzVector> > ("genpsmotherp4").setBranchAlias("gen_mother_p4");
   produces<std::vector<int> > ("genpsmatchtojet").setBranchAlias("gen_matchtojet");
   produces<LorentzVector> ("lep1").setBranchAlias("lep1");
   produces<LorentzVector> ("lep2").setBranchAlias("lep2");
   produces <int> ("lep1id").setBranchAlias("id1");
   produces <int> ("lep2id").setBranchAlias("id2");
   produces <int> ("lep1gmother").setBranchAlias("gramid1");
   produces <int> ("lep2gmother").setBranchAlias("gramid2");
   produces <float> ("lep1iso").setBranchAlias("iso1");
   produces <float> ("lep2iso").setBranchAlias("iso2");
   produces<float> ("genmet").setBranchAlias("met");
   produces<float> ("genht").setBranchAlias("ht");
   produces<float> ("genmetphi").setBranchAlias("metphi");
   produces<std::vector<LorentzVector> > ("genBs").setBranchAlias("genBs");
   produces<std::vector<int> > ("genBIDs").setBranchAlias("genBIDs");
   produces<int> ("genpsgrandmotherid").setBranchAlias("gen_gmother_id");
   produces<int> ("nLeptons").setBranchAlias("nLep");
   produces<int> ("nBtags").setBranchAlias("nBtags");

   //Declare gen jet branches
   produces<std::vector<LorentzVector> > ("genjet").setBranchAlias("genjet");
   produces<std::vector<float> > ("genjetpt").setBranchAlias("jet_pt");
   produces<std::vector<float> > ("genjeteta").setBranchAlias("jet_eta");
   produces<std::vector<float> > ("genjetphi").setBranchAlias("jet_phi");
   produces<int> ("nJets40").setBranchAlias("nJets40");

   //Declare meta deta branches
   produces<float> ("weight").setBranchAlias("weight");
   produces<double> ("qScale").setBranchAlias("qScale");
   produces<bool> ("keep").setBranchAlias("keep");
   produces<std::vector<float> > ("weights").setBranchAlias("weights");
   produces<std::vector<std::string> > ("weightsID").setBranchAlias("weightsID");
   produces<std::vector<float> > ("DJRValues").setBranchAlias("DJR");
   produces<int> ("nMEPartons").setBranchAlias("nMEPart");

   //now do what ever other initialization is needed
   counter = 0;
   highPtCounter = 0;
   numLeptons = 0;
}

// ------------------Method destructor---------------------
producer::~producer(){ 
}

double deltaR(float eta1 , float phi1 , float eta2 , float phi2) {
  double dphi = std::min(::fabs(phi1 - phi2), 2 * M_PI - fabs(phi1 - phi2));
  double deta = eta1 - eta2;
  return sqrt(dphi*dphi + deta*deta);
}

// ------------ Method called to produce the data  ------------
void producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

   using namespace edm;
   using namespace reco;
   using namespace std;

   //Declare Gen particles
   Handle<reco::GenParticleCollection> genpsHandle;
   iEvent.getByLabel( genParticlesInputTag_, genpsHandle);
   const vector<GenParticle>* genps_coll = genpsHandle.product();

   //Declare Gen Jets
   Handle<edm::View<reco::GenJet> > genJets;
   iEvent.getByLabel( genJetsInputTag_, genJets);

   //Declare "info" (for weights)
   Handle<GenEventInfoProduct> genEvtInfo;
   iEvent.getByLabel( "generator", genEvtInfo );

   //Declare "event info" (for reweighting weights)
   Handle<LHEEventProduct> LHEEventInfo;
   iEvent.getByLabel("source", LHEEventInfo); 

   //Auto pointers for new branches -- gen particles
   auto_ptr<vector<LorentzVector> > genps(new vector<LorentzVector>);
   auto_ptr<vector<int> > gen_id(new vector<int>);
   auto_ptr<vector<float> > gen_pt(new vector<float>);
   auto_ptr<vector<float> > gen_eta(new vector<float>);
   auto_ptr<vector<float> > gen_phi(new vector<float>);
   auto_ptr<vector<int> > gen_status(new vector<int>);
   auto_ptr<vector<int> > gen_mother_id(new vector<int>);
   auto_ptr<vector<int> > gen_gmother_id(new vector<int>);
   auto_ptr<vector <LorentzVector> > gen_mother_p4(new vector<LorentzVector>);
   auto_ptr<vector<int> > gen_matchtojet(new vector<int>);
   auto_ptr<float> met(new float);
   auto_ptr<float> ht(new float);
   auto_ptr<float> metphi(new float);
   auto_ptr<LorentzVector> lep1(new LorentzVector);
   auto_ptr<LorentzVector> lep2(new LorentzVector);
   auto_ptr<int> lep1gmother(new int);
   auto_ptr<int> lep2gmother(new int);
   auto_ptr<int> lep1id(new int);
   auto_ptr<int> lep2id(new int);
   auto_ptr<float> lep1iso(new float);
   auto_ptr<float> lep2iso(new float);
   auto_ptr<bool> keep(new bool);
   auto_ptr<vector<LorentzVector> > genBs(new vector <LorentzVector>);
   auto_ptr<vector<int> > genBIDs(new vector <int>);
   auto_ptr<int> nLeptons(new int);
   auto_ptr<int> nBtags(new int);

   //Auto pointers for new branches -- gen jets 
   auto_ptr<vector<LorentzVector> > genjet(new vector<LorentzVector>);
   auto_ptr<vector<float> > jet_pt(new vector<float>);
   auto_ptr<vector<float> > jet_eta(new vector<float>);
   auto_ptr<vector<float> > jet_phi(new vector<float>);
   auto_ptr<int> nJets40(new int);

   //Auto pointers to meta data
   auto_ptr<float> weight(new float);
   auto_ptr<vector<float> > weights(new vector<float> );
   auto_ptr<vector<string> > weightsID(new vector<string> );
   auto_ptr<vector<float> > DJRValues(new vector<float> );
   auto_ptr<int> nMEPartons(new int);
   auto_ptr<double> qScale(new double);

   //Initialize auto pointers
   *lep1iso = 0;
   *lep2iso = 0;
   *nJets40 = 0;
   *nBtags = 0;

   //Event counter
   counter++;
   
   //List of jets
   vector<LorentzVector> listOfJets;

   //General Information
   *weight = genEvtInfo->weight();
   vector <gen::WeightsInfo> weightsTemp = LHEEventInfo->weights();
   for (unsigned int i = 0; i < weightsTemp.size(); i++){
     weights->push_back(weightsTemp.at(i).wgt);
     weightsID->push_back(weightsTemp.at(i).id);
   }
   *qScale = genEvtInfo->qScale();
   *DJRValues = genEvtInfo->DJRValues();
   *nMEPartons = genEvtInfo->nMEPartons(); 
   
   //Loop over Gen Jets
   int jetCounter = 0;
   for ( edm::View<reco::GenJet>::const_iterator jet = genJets->begin(); jet != genJets->end(); ++jet) {
     float pt = jet->pt();
     float eta = jet->eta();
     float phi = jet->phi();

     jetCounter++;

     listOfJets.push_back( LorentzVector( jet->p4() ) );

//     if (counter == 1) cout << Form("Gen Jets! #%2i.  pT: %5.3f. eta: %5.2f. phi: %5.2f", jetCounter, pt, eta, phi) << endl;

     jet_pt->push_back(pt); 
     jet_eta->push_back(eta); 
     jet_phi->push_back(phi); 
     genjet->push_back(LorentzVector( jet->p4() ));

   }//jet loop


   //Declare gen particle variables
   int genParticleCounter = 0;
   LorentzVector metVector;
   vector <int> leptonIDs;
   vector <LorentzVector> leptonVectors;
   vector <int> leptonGramIDs;

   //Loop over gen particles
   for(vector<GenParticle>::const_iterator genps_it = genps_coll->begin(); genps_it != genps_coll->end(); genps_it++) {
     *ht = 0; 
     int id = genps_it->pdgId();
     int status = genps_it->status();
     const GenParticle *mother = MCUtilities::motherID(*genps_it);
     int mother_id = mother->pdgId();
     //int mother_status = mother->status();
     float pt = genps_it->pt();
     float eta = genps_it->eta();
     float phi = genps_it->phi();
     float match = -1;
     int grandmother_id = MCUtilities::motherID(*mother)->pdgId();

     //If neutrino at status-1, add to MET vector
     if ( status == 1 && (abs(id) == 12 || abs(id) == 14 || abs(id) == 16)) metVector += LorentzVector( genps_it->p4() );
 
     if (status == 23 && abs(id) == 5 && pt > 20){
        genBs->push_back(LorentzVector(genps_it->p4()));
        genBIDs->push_back(id);
     }

     //Keep only protons and hard-level processes, except for final state leptons from W, t, or Tau
     if (status > 30 || status == 2 || (status == 1 && ((abs(id) != 11 && abs(id) != 13) || (abs(mother_id) != 24 && abs(mother_id) != 15 && abs(mother_id) != 6)))) continue;

     //Print out status (for debugging)
//     if (counter == 1) cout << Form("Gen Particle! #% 4i. id: % 6i. status: %3i. mother % 6i. pT %7.3f. eta % 6.3f. phi % 6.3f. gmother % 6i. motStat %3i", genParticleCounter, id, status, mother_id, pt, eta, phi, grandmother_id, mother_status) << endl;
     genParticleCounter++;

     //If it's a final-state lepton from W, t, or tau, count it as a lepton and keep it
     if (status == 1){
       if (abs(grandmother_id) > 100 && abs(grandmother_id < 100000)) continue;
       if (genps_it->pt() < 10) continue; 
       leptonIDs.push_back(id);
       leptonVectors.push_back( LorentzVector( genps_it->p4() ) );
       leptonGramIDs.push_back( grandmother_id );
       continue;
     }

     //Match gen particle to jet if within 0.50 of closest jet
     float bestDeltaR = .50;
     if (status == 23 && (abs(id) <= 6 || abs(id) == 21)){ 
       for (unsigned int i = 0; i < listOfJets.size(); i++){
         float tempDeltaR = deltaR(listOfJets.at(i).eta(), listOfJets.at(i).phi(), eta, phi);
         if (tempDeltaR < bestDeltaR){
           bestDeltaR = tempDeltaR;
           match = i;
         }
       }//match loop
     }

     //Print out matching (for debugging)
//     if (counter == 1 && match >= 0) cout << "     --> Matched To jet #" << match+1 << endl; 

     //Push back vectors
     genps->push_back(LorentzVector( genps_it->p4() ));
     gen_id->push_back(id);
     gen_status->push_back(status);
     gen_mother_id->push_back(mother_id);
     gen_gmother_id->push_back(grandmother_id);
     gen_mother_p4->push_back(LorentzVector(mother->p4()));
     gen_matchtojet->push_back(match);
     gen_pt->push_back(pt);
     gen_eta->push_back(eta);
     gen_phi->push_back(phi);

   }//gen particle loop

   //Drop events that don't have two leptons
   *keep = true;
   *nLeptons = leptonIDs.size();
   iEvent.put(nLeptons, "nLeptons");
   iEvent.put(weight, "weight");
   iEvent.put(weights, "weights");
   iEvent.put(DJRValues, "DJRValues");
   iEvent.put(nMEPartons, "nMEPartons");
   if (leptonIDs.size() != 2){
      *keep = false;
      iEvent.put(keep, "keep");
      return;
   }

   //Sort and keep the two leptons
   *lep1 = (leptonVectors.at(0).Pt() > leptonVectors.at(1).Pt()) ? leptonVectors.at(0) : leptonVectors.at(1); 
   *lep2 = (leptonVectors.at(0).Pt() <= leptonVectors.at(1).Pt()) ? leptonVectors.at(0) : leptonVectors.at(1); 
   *lep1id = (leptonVectors.at(0).Pt() > leptonVectors.at(1).Pt()) ? leptonIDs.at(0) : leptonIDs.at(1); 
   *lep2id = (leptonVectors.at(0).Pt() <= leptonVectors.at(1).Pt()) ? leptonIDs.at(0) : leptonIDs.at(1); 
   *lep1gmother = (leptonVectors.at(0).Pt() > leptonVectors.at(1).Pt()) ? leptonGramIDs.at(0) : leptonGramIDs.at(1); 
   *lep2gmother = (leptonVectors.at(0).Pt() <= leptonVectors.at(1).Pt()) ? leptonGramIDs.at(0) : leptonGramIDs.at(1); 

   float lep1eta = lep1->eta();
   float lep2eta = lep2->eta();
   float lep1phi = lep1->phi();
   float lep2phi = lep2->phi();

   //Drop events if the leptons are on top of one another
   if (deltaR(lep1eta, lep1phi, lep2eta, lep2phi) < 0.3){
     *keep = false;
     iEvent.put(keep, "keep");
     return;
   }

   for(vector<GenParticle>::const_iterator genps_it = genps_coll->begin(); genps_it != genps_coll->end(); genps_it++) {
     int status = genps_it->status();
     int id = genps_it->pdgId();
     float pt = genps_it->pt();
     if (status != 1) continue;
     if (abs(id) >= 11 && abs(id) <= 16) continue;
     float deltaRone = deltaR(lep1eta, lep1phi, genps_it->eta(), genps_it->phi());
     float deltaRtwo = deltaR(lep2eta, lep2phi, genps_it->eta(), genps_it->phi());
     if (deltaRone < 0.3) *lep1iso += pt;
     if (deltaRtwo < 0.3) *lep2iso += pt;
   }

   //Drop any jets that are on top of the leptons:
   unsigned int temp_counter = 0;
   for (unsigned int jet = 0; jet != (*genjet).size(); jet++){
     if (temp_counter >= genjet->size()) break;
     float pt = (*jet_pt).at(temp_counter);
     float deltaRone = deltaR(lep1eta, lep1phi, (*genjet).at(temp_counter).eta(), (*genjet).at(temp_counter).phi());  
     float deltaRtwo = deltaR(lep2eta, lep2phi, (*genjet).at(temp_counter).eta(), (*genjet).at(temp_counter).phi());  
     if (deltaRone < 0.3 || deltaRtwo < 0.3){
        genjet->erase(genjet->begin()+temp_counter);
        jet_pt->erase(jet_pt->begin()+temp_counter);
        jet_eta->erase(jet_eta->begin()+temp_counter);
        jet_phi->erase(jet_phi->begin()+temp_counter);
        continue;
     }
     temp_counter++;
     if (pt < 40) continue;
     (*nJets40)++; 
     *ht += pt;

     //Count number of b-tags
     for (unsigned int genB = 0; genB != (*genBs).size(); genB++){
       if (deltaR( (*genBs).at(genB).eta(), (*genBs).at(genB).phi(), (*genjet).at(temp_counter-1).eta(), (*genjet).at(temp_counter-1).phi()) > 0.1) continue;
       (*nBtags)++;
     }
   }

   //Keep the MET
   *met = metVector.mag();
   *metphi = atan2( metVector.Y(), metVector.X() );

   //Print out values (for debugging)
//   if (counter == 1) cout << "MET: " << *met << endl;
//   if (counter == 1) cout << "HT: " << *ht << endl;

   //Store information
   iEvent.put(keep, "keep");
   iEvent.put(genps, "genps");
   iEvent.put(gen_id, "genpsid");
   iEvent.put(gen_pt, "genpspt");
   iEvent.put(gen_eta, "genpseta");
   iEvent.put(gen_phi, "genpsphi");
   iEvent.put(gen_mother_id, "genpsmotherid");
   iEvent.put(gen_mother_p4, "genpsmotherp4");
   iEvent.put(gen_matchtojet, "genpsmatchtojet");
   iEvent.put(genBs, "genBs");
   iEvent.put(genBIDs, "genBIDs");
   iEvent.put(gen_status, "genpsstatus");
   iEvent.put(genjet, "genjet");
   iEvent.put(jet_pt, "genjetpt");
   iEvent.put(jet_eta, "genjeteta");
   iEvent.put(jet_phi, "genjetphi");
   iEvent.put(met, "genmet");
   iEvent.put(ht, "genht");
   iEvent.put(metphi, "genmetphi");
   iEvent.put(weightsID, "weightsID");
   iEvent.put(qScale, "qScale");
   iEvent.put(lep1, "lep1");
   iEvent.put(lep1id, "lep1id");
   iEvent.put(lep1gmother, "lep1gmother");
   iEvent.put(lep1iso, "lep1iso");
   iEvent.put(lep2, "lep2");
   iEvent.put(lep2id, "lep2id");
   iEvent.put(lep2gmother, "lep2gmother");
   iEvent.put(lep2iso, "lep2iso");
   iEvent.put(nJets40, "nJets40");
   iEvent.put(nBtags, "nBtags");
 
}

// ------------ Method called once each job just before starting event loop  ------------
void producer::beginJob(){
}

// ------------ Method called once each job just after ending the event loop  ------------
void producer::endJob(){
//  std::cout << "Number of events with at least one high Pt Parton: " << highPtCounter << std::endl;
}
 
// ------------ Method fills 'descriptions' with the allowed parameters for the module  ------------
void producer::fillDescriptions(edm::ConfigurationDescriptions& descriptions){
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(producer);
