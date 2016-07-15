#!/bin/bash


# MUONS

curl -o mu_mediumID.root http://www.hep.uniovi.es/juanr/MuonSF/2.Medium/TnP_MuonID_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root
# curl -o mu_dpt0p2.root # FIXME
curl -o mu_dxy0p05dz0p1.root http://www.hep.uniovi.es/juanr/MuonSF/10.dxy0.05dz0.1_Medium/TnP_MuonID_NUM_TightIP2D_DENOM_MediumID_VAR_map_pt_eta.root
# curl -o mu_multiiso.root # FIXME
curl -o mu_miniiso0p4.root http://www.hep.uniovi.es/juanr/MuonSF/6.MiniIso0.4_Loose/TnP_MuonID_NUM_MiniIsoLoose_DENOM_LooseID_VAR_map_pt_eta.root
curl -o mu_sip3dlt4.root http://www.hep.uniovi.es/juanr/MuonSF/12.SIP3D4_Medium/TnP_MuonID_NUM_TightIP3D_DENOM_MediumID_VAR_map_pt_eta.root


# ELECTRONS

curl -o el_sfs.root http://tomc.web.cern.ch/tomc/tagAndProbe/scaleFactors.root

