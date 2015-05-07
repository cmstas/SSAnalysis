// system include files
#include <memory>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "MCUtilities.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

class AGfilter : public edm::EDFilter {
  public:
    explicit AGfilter(const edm::ParameterSet&); 
    ~AGfilter();
  private:
    virtual void beginJob();
    virtual bool filter(edm::Event&, const edm::EventSetup&); 
    virtual void endJob();
};

AGfilter::AGfilter(const edm::ParameterSet& iConfig) {
}

AGfilter::~AGfilter() {}


void AGfilter::beginJob(){
}

void AGfilter::endJob(){
}

bool AGfilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;
  using namespace std;
 
  InputTag keep_tag("producer", "keep");
  Handle<bool> keepHandle;
  iEvent.getByLabel(keep_tag, keepHandle);
  const bool *keep = keepHandle.product();

  return *keep;

}

DEFINE_FWK_MODULE(AGfilter);
