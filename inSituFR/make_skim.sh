#!/bin/bash

dir="/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.03/output/"
outdir="skims_Jan25"

branches="is_real_data,lep1_miniIso,lep2_miniIso,fired_trigger,lep1_closeJet,lep1_el_etaSC,lep1_ip3d_err,lep1_motherID,lep1_mu_ptErr,lep1_multiIso,lep1_ptrel_v1,lep2_closeJet,lep2_el_etaSC,lep2_ip3d_err,lep2_motherID,lep2_mu_ptErr,lep2_multiIso,lep2_ptrel_v1,jet_close_lep1,jet_close_lep2,lep1_passes_id,lep2_passes_id,lep1_mediumMuonPOG,lep2_mediumMuonPOG,lep1_el_conv_vtx_flag,lep2_el_conv_vtx_flag,lep1_el_exp_innerlayers,lep2_el_exp_innerlayers,lep1_el_threeChargeAgree,lep2_el_threeChargeAgree,ht,mt,met,run,lumi,event,mt_l2,njets,metPhi,nbtags,lep1_dZ,lep1_id,lep1_p4,lep2_dZ,lep2_id,lep2_p4,lep1_MVA,lep1_iso,lep2_MVA,lep2_iso,scale1fb,hyp_class,lep1_ip3d,lep2_ip3d,lep1_dxyPV,lep2_dxyPV,is_real_data,lep1_miniIso,lep2_miniIso,fired_trigger,lep1_closeJet,lep1_el_etaSC,lep1_motherID,lep1_mu_ptErr,lep1_multiIso,lep1_ptrel_v1,lep2_closeJet,lep2_el_etaSC,lep2_motherID,lep2_mu_ptErr,lep2_multiIso,lep2_ptrel_v1,lep1_passes_id,lep2_passes_id,lep1_coneCorrPt,lep2_coneCorrPt,lep1_mediumMuonPOG,lep2_mediumMuonPOG,lep1_el_conv_vtx_flag,lep2_el_conv_vtx_flag,lep1_el_exp_innerlayers,lep2_el_exp_innerlayers,lep1_el_threeChargeAgree,lep2_el_threeChargeAgree,ht,met,run,lumi,event,njets,nbtags,lep1_dZ,lep1_id,lep1_p4,lep2_dZ,lep2_id,lep2_p4,lep1_MVA,lep1_sip,lep2_MVA,lep2_sip,scale1fb,hyp_class"


mkdir -p $outdir

# dir=/nfs-7/userdata/ss2015/ssBabies/${tag}/

for sample in TTBAR_PH WJets WJets_HTbinned ; do 
    echo "===== ${sample%.*} ====="
    # echo skim.py -c "njets>=2" -b "$branches" -t t -o "$sample" $dir/$sample
    skim.py -b "$branches" -t t -o "${outdir}/${sample}_skim.root" $dir/${sample}.root
done

echo "===== Data ====="
skim.py -b "$branches" -t t -o "${outdir}/Data_skim.root" "$dir/Data*.root"
