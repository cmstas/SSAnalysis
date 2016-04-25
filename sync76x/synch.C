// https://indico.cern.ch/event/522579/contributions/2144554/attachments/1261789/1865889/RA5_ECOP_200416.pdf
//

/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/synch_ttw_1.root


// global

cout<<Form("%1d%9d%12d\t%2d\t%+2d%5.1f\t%+2d%5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d",
        run(),lumi(),event(),nLooseElectrons7()+nLooseMuons5(),lep1_id(),lep1_p4().pt(),lep2_id(),
        lep2_p4().pt(),njets(),nbtags(),met(),ht(),SR)<<endl;

// Selection:
// Onlytwosame_signleptonsandtwojets(pT>40GeV)
    centralleptondefinitions,(defaultPATobjectsinminiAODs)
    File:
    /store/mc/RunIIFall15MiniAODv2/TTWJetsToLNu_TuneCUETP8M1_13TeV_amcatnloFXFXmadspin_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymp_v1/60000/14C51DB0_D6B8_E511_8D9B_8CDCD4A9A484.root
    Alsoavailableonafs:/afs/cern.ch/user/m/mmarionn/public/SUSY/syncTTW76_SSDL2016.root
    // Dumpofleptonsvariables:

    cout<<Form("%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t
            %1d\t%1d\t%1d\t%1d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t
            %5.1f\t%1d\t%1d\t%1d\t%1d",
            lep1_pt(),lep1_eta(),lep1_phi(),lep1_pdgId(),lep1_conePt(),
            lep1_dxy(),lep1_dz(),lep1_SIP()lep1_miniIso(),lep1_ptRatio(),lep1_ptRel(),
            (std::abs(lep1_pdgId()==11)?(lep1_mvaId()):0),(std::abs(lep1_pdgId()==11)?(lep1_convVeto()):0),
            (std::abs(lep1_pdgId()==11)?(lep1_nPixelMiss()):0),lep1_tightCharge(),lep1_MCTruthMatched(),
            lep2_pt(),lep2_eta(),lep2_phi(),lep2_pdgId(),lep2_conePt(),lep2_dxy(),lep2_dz(),lep2_SIP(),
            lep2_miniIso(),lep2_ptRatio(),lep2_ptRel(),(std::abs(lep2_pdgId()==11)?(lep2_mvaId()):0),
            (std::abs(lep2_pdgId()==11)?(lep2_convVeto()):0),(std::abs(lep2_pdgId()==11)?(lep2_nPixelMiss()):0),
            lep2_tightCharge(),lep2_MCTruthMatched())<<endl;

    // Dumpofjetvariables:
    cout<<Form("%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d%5.1f\t%5.1f\t%5.1f\t%5.1f\t
            %1d,jet1_pt(),jet1_eta(),jet1_phi(),jet1_btagCSVmva(),jet1_btagCSVMedium(),jet2_pt(),jet2_eta(),
            jet2_phi(),jet2_btagCSVmva(),jet2_btagCSVMedium())<<endl;
