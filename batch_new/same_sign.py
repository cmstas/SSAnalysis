import os
import sys

d_ds2name = {
        "signals": {
            "/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM": "T1TTTT_main",
            "/SMS-T5qqqqVV_dM20_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM": "T5QQQQVV_dm20",
            "/SMS-T5ttcc_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3/MINIAODSIM": "T5TTCC_main",
            "/SMS-T5qqqqVV_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM": "T5QQQQVV_main",
            "/SMS-T6ttWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM": "T6TTWW_main",
            "/SMS-T5tttt_dM175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM": "T5TTTT_dm175",
            "/SMS-T1ttbb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM": "T1TTBB_main",
            "/ttH_HToTT_1J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTH_scan",
            "/tHW_HToTT_0J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "THW_scan",
            "/tHq_HToTT_0J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "THQ_scan",

            # "/THQ/tHq-scan-MINIAOD_Private80Xv2_v2/USER": "THQ_scan_old",
            # "/TTH/ttH-scan-MINIAOD_Private80Xv2_v2/USER": "TTH_scan_old",
            # "/THW/tHW-scan-MINIAOD_Private80Xv2/USER": "THW_scan_old",
            },
        "backgrounds": {
            "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZ",
            "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "DY_low",
            "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM": "DY_high",
            "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "ZG",
            "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WJets",
            "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTG",
            "/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "QQWW",
            "/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTT",
            "/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWDPS",
            "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTBAR_PH",
            "/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZ",
            "/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TG",
            "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZZ",
            "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWZ",
            "/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WGToLNuG",
            "/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TZQ",
            "/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTHtoNonBB",
            "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "VHtoNonBB",
            "/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTZLOW",
            "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "GGHtoZZto4L",
            "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZZ",
            "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWW",
            "/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "WWG",
            "/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZG",
            "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TTZnlo",
            "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM": "TTWnlo",
            "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "WJets100To200",
            "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM": "WJets200To400",
            "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WJets400To600",
            "/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WJets600To800",
            "/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "WJets800To1200",
            "/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TWZ",
            "/ttZJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTZ",
            "/ttWJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTW",
            "/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZLO",
            "/WW_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WW_inclusive",
            },
        "data": {
            # "/DoubleEG/Run2016B-PromptReco-v1/MINIAOD": "DataDoubleEGv1",
            # "/DoubleMuon/Run2016B-PromptReco-v1/MINIAOD": "DataDoubleMuonv1",
            # "/MuonEG/Run2016B-PromptReco-v1/MINIAOD": "DataMuonEGv1",
            # "/DoubleEG/Run2016B-PromptReco-v2/MINIAOD": "DataDoubleEGv2",
            # "/DoubleMuon/Run2016B-PromptReco-v2/MINIAOD": "DataDoubleMuonv2",
            # "/MuonEG/Run2016B-PromptReco-v2/MINIAOD": "DataMuonEGv2",
            # "/DoubleEG/Run2016C-PromptReco-v2/MINIAOD": "DataDoubleEGC",
            # "/DoubleMuon/Run2016C-PromptReco-v2/MINIAOD": "DataDoubleMuonC",
            # "/MuonEG/Run2016C-PromptReco-v2/MINIAOD": "DataMuonEGC",
            # "/DoubleEG/Run2016D-PromptReco-v2/MINIAOD": "DataDoubleEGD",
            # "/DoubleMuon/Run2016D-PromptReco-v2/MINIAOD": "DataDoubleMuonD",
            # "/MuonEG/Run2016D-PromptReco-v2/MINIAOD": "DataMuonEGD",
            # "/DoubleEG/Run2016E-PromptReco-v2/MINIAOD": "DataDoubleEGE",
            # "/DoubleMuon/Run2016E-PromptReco-v2/MINIAOD": "DataDoubleMuonE",
            # "/MuonEG/Run2016E-PromptReco-v2/MINIAOD": "DataMuonEGE",
            # "/DoubleEG/Run2016F-PromptReco-v1/MINIAOD": "DataDoubleEGF",
            # "/DoubleMuon/Run2016F-PromptReco-v1/MINIAOD": "DataDoubleMuonF",
            # "/MuonEG/Run2016F-PromptReco-v1/MINIAOD": "DataMuonEGF",
            # "/DoubleEG/Run2016G-PromptReco-v1/MINIAOD": "DataDoubleEGG",
            # "/DoubleMuon/Run2016G-PromptReco-v1/MINIAOD": "DataDoubleMuonG",
            # "/MuonEG/Run2016G-PromptReco-v1/MINIAOD": "DataMuonEGG",

            # "/DoubleEG/Run2016H-PromptReco-v2/MINIAOD": "DataDoubleEGH",
            # "/DoubleMuon/Run2016H-PromptReco-v2/MINIAOD": "DataDoubleMuonH",
            # "/MuonEG/Run2016H-PromptReco-v2/MINIAOD": "DataMuonEGH",
            # "/DoubleEG/Run2016H-PromptReco-v3/MINIAOD": "DataDoubleEGHv3",
            # "/DoubleMuon/Run2016H-PromptReco-v3/MINIAOD": "DataDoubleMuonHv3",
            # "/MuonEG/Run2016H-PromptReco-v3/MINIAOD": "DataMuonEGHv3",
            # "/JetHT/Run2016H-PromptReco-v2/MINIAOD": "DataJetHTH",
            # "/JetHT/Run2016H-PromptReco-v3/MINIAOD": "DataJetHTHv3",
            # "/DoubleEG/Run2016B-23Sep2016-v3/MINIAOD": "DataDoubleEGRerecoB",
            # "/DoubleMuon/Run2016B-23Sep2016-v3/MINIAOD": "DataDoubleMuonRerecoB",
            # "/MuonEG/Run2016B-23Sep2016-v3/MINIAOD": "DataMuonEGRerecoB",
            # "/DoubleEG/Run2016C-23Sep2016-v1/MINIAOD": "DataDoubleEGRerecoC",
            # "/DoubleMuon/Run2016C-23Sep2016-v1/MINIAOD": "DataDoubleMuonRerecoC",
            # "/MuonEG/Run2016C-23Sep2016-v1/MINIAOD": "DataMuonEGRerecoC",
            # "/DoubleEG/Run2016D-23Sep2016-v1/MINIAOD": "DataDoubleEGRerecoD",
            # "/DoubleMuon/Run2016D-23Sep2016-v1/MINIAOD": "DataDoubleMuonRerecoD",
            # "/MuonEG/Run2016D-23Sep2016-v1/MINIAOD": "DataMuonEGRerecoD",
            # "/DoubleEG/Run2016E-23Sep2016-v1/MINIAOD": "DataDoubleEGRerecoE",
            # "/DoubleMuon/Run2016E-23Sep2016-v1/MINIAOD": "DataDoubleMuonRerecoE",
            # "/MuonEG/Run2016E-23Sep2016-v1/MINIAOD": "DataMuonEGRerecoE",
            # "/DoubleEG/Run2016F-23Sep2016-v1/MINIAOD": "DataDoubleEGRerecoF",
            # "/DoubleMuon/Run2016F-23Sep2016-v1/MINIAOD": "DataDoubleMuonRerecoF",
            # "/MuonEG/Run2016F-23Sep2016-v1/MINIAOD": "DataMuonEGRerecoF",
            # "/DoubleEG/Run2016G-23Sep2016-v1/MINIAOD": "DataDoubleEGRerecoG",
            # "/DoubleMuon/Run2016G-23Sep2016-v1/MINIAOD": "DataDoubleMuonRerecoG",
            # "/MuonEG/Run2016G-23Sep2016-v1/MINIAOD": "DataMuonEGRerecoG",

            # "/DoubleEG/Run2016H-03Feb2017_ver2-v1/MINIAOD": "DataDoubleEGH",
            # "/DoubleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD": "DataDoubleMuonH",
            # "/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD": "DataMuonEGH",
            # "/DoubleEG/Run2016H-03Feb2017_ver3-v1/MINIAOD": "DataDoubleEGHv3",
            # "/DoubleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD": "DataDoubleMuonHv3",
            # "/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD": "DataMuonEGHv3",
            # "/JetHT/Run2016H-03Feb2017_ver2-v1/MINIAOD": "DataJetHTH",
            # "/JetHT/Run2016H-03Feb2017_ver3-v1/MINIAOD": "DataJetHTHv3",
            # "/DoubleEG/Run2016B-03Feb2017_ver2-v2/MINIAOD": "DataDoubleEGRerecoB",
            # "/DoubleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD": "DataDoubleMuonRerecoB",
            # "/MuonEG/Run2016B-03Feb2017_ver2-v2/MINIAOD": "DataMuonEGRerecoB",
            # "/DoubleEG/Run2016C-03Feb2017-v1/MINIAOD": "DataDoubleEGRerecoC",
            # "/DoubleMuon/Run2016C-03Feb2017-v1/MINIAOD": "DataDoubleMuonRerecoC",
            # "/MuonEG/Run2016C-03Feb2017-v1/MINIAOD": "DataMuonEGRerecoC",
            # "/DoubleEG/Run2016D-03Feb2017-v1/MINIAOD": "DataDoubleEGRerecoD",
            # "/DoubleMuon/Run2016D-03Feb2017-v1/MINIAOD": "DataDoubleMuonRerecoD",
            # "/MuonEG/Run2016D-03Feb2017-v1/MINIAOD": "DataMuonEGRerecoD",
            # "/DoubleEG/Run2016E-03Feb2017-v1/MINIAOD": "DataDoubleEGRerecoE",
            # "/DoubleMuon/Run2016E-03Feb2017-v1/MINIAOD": "DataDoubleMuonRerecoE",
            # "/MuonEG/Run2016E-03Feb2017-v1/MINIAOD": "DataMuonEGRerecoE",
            # "/DoubleEG/Run2016F-03Feb2017-v1/MINIAOD": "DataDoubleEGRerecoF",
            # "/DoubleMuon/Run2016F-03Feb2017-v1/MINIAOD": "DataDoubleMuonRerecoF",
            # "/MuonEG/Run2016F-03Feb2017-v1/MINIAOD": "DataMuonEGRerecoF",
            # "/DoubleEG/Run2016G-03Feb2017-v1/MINIAOD": "DataDoubleEGRerecoG",
            # "/DoubleMuon/Run2016G-03Feb2017-v1/MINIAOD": "DataDoubleMuonRerecoG",
            # "/MuonEG/Run2016G-03Feb2017-v1/MINIAOD": "DataMuonEGRerecoG",

            "/DoubleEG/Run2016H-03Feb2017_ver2-v1/CMS3": "DataDoubleEGH",
            "/DoubleMuon/Run2016H-03Feb2017_ver2-v1/CMS3": "DataDoubleMuonH",
            "/MuonEG/Run2016H-03Feb2017_ver2-v1/CMS3": "DataMuonEGH",
            "/DoubleEG/Run2016H-03Feb2017_ver3-v1/CMS3": "DataDoubleEGHv3",
            "/DoubleMuon/Run2016H-03Feb2017_ver3-v1/CMS3": "DataDoubleMuonHv3",
            "/MuonEG/Run2016H-03Feb2017_ver3-v1/CMS3": "DataMuonEGHv3",
            "/JetHT/Run2016H-03Feb2017_ver2-v1/CMS3": "DataJetHTH",
            "/JetHT/Run2016H-03Feb2017_ver3-v1/CMS3": "DataJetHTHv3",
            "/DoubleEG/Run2016B-03Feb2017_ver2-v2/CMS3": "DataDoubleEGRerecoB",
            "/DoubleMuon/Run2016B-03Feb2017_ver2-v2/CMS3": "DataDoubleMuonRerecoB",
            "/MuonEG/Run2016B-03Feb2017_ver2-v2/CMS3": "DataMuonEGRerecoB",
            "/DoubleEG/Run2016C-03Feb2017-v1/CMS3": "DataDoubleEGRerecoC",
            "/DoubleMuon/Run2016C-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoC",
            "/MuonEG/Run2016C-03Feb2017-v1/CMS3": "DataMuonEGRerecoC",
            "/DoubleEG/Run2016D-03Feb2017-v1/CMS3": "DataDoubleEGRerecoD",
            "/DoubleMuon/Run2016D-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoD",
            "/MuonEG/Run2016D-03Feb2017-v1/CMS3": "DataMuonEGRerecoD",
            "/DoubleEG/Run2016E-03Feb2017-v1/CMS3": "DataDoubleEGRerecoE",
            "/DoubleMuon/Run2016E-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoE",
            "/MuonEG/Run2016E-03Feb2017-v1/CMS3": "DataMuonEGRerecoE",
            "/DoubleEG/Run2016F-03Feb2017-v1/CMS3": "DataDoubleEGRerecoF",
            "/DoubleMuon/Run2016F-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoF",
            "/MuonEG/Run2016F-03Feb2017-v1/CMS3": "DataMuonEGRerecoF",
            "/DoubleEG/Run2016G-03Feb2017-v1/CMS3": "DataDoubleEGRerecoG",
            "/DoubleMuon/Run2016G-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoG",
            "/MuonEG/Run2016G-03Feb2017-v1/CMS3": "DataMuonEGRerecoG",

            },

            "test": {
            "/JetHT/Run2016H-03Feb2017_ver3-v1/CMS3": "TestDataJetHTHv3",
            }
        }

def dataset_to_shortname(ds):
    for typ_values in d_ds2name.values():
        for dname in typ_values:
            if dname == ds:
                return typ_values[dname]
    return "unknown"

# print dataset_to_shortname("/MuonEG/Run2016F-23Sep2016-v1/MINIAOD")
# print dataset_to_shortname("/SMS-T5qqqqVV_dM20_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM")
# print dataset_to_shortname("/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM")

inputs_path = os.path.abspath(__file__).rsplit("/",1)[0]+"/inputs/"
sweepRoot_scripts = [inputs_path+"sweeproot.sh", inputs_path+"sweeproot_macro.C"]
merging_scripts = [inputs_path+"merge_script.sh", inputs_path+"merge_macro.C"]
baby_merged_dir = "/nfs-7/userdata/${USER}/tupler_babies/merged/"

tag = "v9.07"
package = inputs_path+"package.tar.gz"
executable = inputs_path+"condor_executable.sh"
dashboard_name = "AutoTwopler_SSbabies"

if not os.path.isfile(package):
    raise Exception("Woah! Did you run make_tar.sh? I don't see the package.tar.gz file in %s" % inputs_path)


