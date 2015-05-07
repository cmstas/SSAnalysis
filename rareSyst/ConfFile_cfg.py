import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_dynam_mm_qcut60_2012way_all.root'
        'file:/home/users/cgeorge/ss2015/rare_syst_study/CMSSW_7_1_6/src/runPythia/pythiaEvents_qqww_dynam_mm_2012way.root'
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_10.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_9.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_8.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_7.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_6.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_5.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_4.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_3.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_2.root',
        #'file:/hadoop/cms/store/user/cgeorge/SS_Syst_Study/qqWW/Pythia/pythiaEvents_qqww_fixed_mm_qcut65_1.root'
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
    fileName = cms.untracked.string('qqWW_baby_dynam_mm_qcut60_2012way_noFilt.root'),
    outputCommands = cms.untracked.vstring('drop *', 
      'keep *_*_*_OWNPARTICLES'
    )
)

  
process.p = cms.Path(process.producer)
#process.p = cms.Path(process.producer*process.AGfilter)

process.e = cms.EndPath(process.out)
