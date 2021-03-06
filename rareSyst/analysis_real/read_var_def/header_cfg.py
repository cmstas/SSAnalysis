import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:///home/users/cgeorge/test/findHeader/A4A78ACA-6535-E511-A22A-1CC1DE051118.root'
    ),
    duplicateCheckMode = cms.untracked.string('checkEachFile')
)
process.MessageLogger.cerr.FwkReport.reportEvery = 20000

process.header = cms.EDProducer('header'
  #genParticlesInputTag = cms.InputTag("genParticles"),
  #genJetsInputTag = cms.InputTag("ak4GenJetsNoNu")  
)

#process.out = cms.OutputModule("PoolOutputModule",
#    process.AGfilterEventSelect,
#    fileName = cms.untracked.string('myQQWWpbaby_noFilt.root'),
#    outputCommands = cms.untracked.vstring('drop *', 
#      'keep *_*_*_OWNPARTICLES'
#    )
#)

  
process.p = cms.Path(process.header)

#process.e = cms.EndPath(process.out)
