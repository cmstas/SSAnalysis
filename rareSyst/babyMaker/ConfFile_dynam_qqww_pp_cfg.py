import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:///hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_dynam_pp_1.root',
        'file:///hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_dynam_pp_2.root',
        'file:///hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_dynam_pp_3.root'
    ),
    duplicateCheckMode = cms.untracked.string('checkEachFile')
)
process.MessageLogger.cerr.FwkReport.reportEvery = 20000

process.producer = cms.EDProducer('producer',
  genParticlesInputTag = cms.InputTag("genParticles"),
  genJetsInputTag = cms.InputTag("ak4GenJetsNoNu")  
)

process.AGfilter = cms.EDFilter("AGfilter");

process.AGfilterEventSelect = cms.PSet(
    SelectEvents = cms.untracked.PSet( 
       SelectEvents = cms.vstring('p')
    )
)

process.out = cms.OutputModule("PoolOutputModule",
    process.AGfilterEventSelect,
    fileName = cms.untracked.string('QQWW_pp_baby_1M_dynam_noFilt.root'),
    outputCommands = cms.untracked.vstring('drop *', 
      'keep *_*_*_OWNPARTICLES'
    )
)
  
process.p = cms.Path(process.producer)

process.e = cms.EndPath(process.out)
