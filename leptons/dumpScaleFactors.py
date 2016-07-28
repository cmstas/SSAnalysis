import ROOT as r

def get_first_hist(f1):
    return f1.Get(f1.GetListOfKeys()[0].GetName())

def dump_bins(h2, name, stupid=False):
    print "float %s(float pt, float eta) {" % name
    for ix in range(1,h2.GetNbinsX()+1):
        for iy in range(1,h2.GetNbinsY()+1):
            if stupid:
                val = h2.GetBinContent(ix,iy)
                if iy != h2.GetNbinsY():
                    print "  if (pt >= %.0f && pt < %.0f && eta >= %.3f && eta < %.3f) return %.4f;" \
                            % (h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), val)
                else:
                    print "  if (pt >= %.0f  && eta >= %.3f && eta < %.3f) return %.4f;" \
                            % (h2.GetYaxis().GetBinLowEdge(iy), h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), val)
            else:
                val = h2.GetBinContent(ix,iy)
                if ix != h2.GetNbinsX():
                    print "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;" \
                            % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
                else:
                    print "  if (pt >= %.0f  && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;" \
                            % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
    print "  return 0.;"
    print "}"

# def dump_bins_fastsim(h3, name):
#     print "float %s(float pt, float eta, int nvtx) {" % name
#     print "  if (pt>=200) pt = 199.9;"
#     if "electron" in name:
#         print "  if (fabs(eta)>=2.5) eta = 2.4;"
#     else:
#         print "  if (fabs(eta)>=2.4) eta = 2.3;"
#     print "  if (nvtx>=40) nvtx = 39;"
#     for ix in range(1,h3.GetNbinsX()+1):
#         for iy in range(1,h3.GetNbinsY()+1):
#             for iz in range(1,h3.GetNbinsZ()+1):
#                 val = h3.GetBinContent(ix,iy,iz)
#                 ptlow = h3.GetXaxis().GetBinLowEdge(ix)
#                 pthigh = h3.GetXaxis().GetBinUpEdge(ix)
#                 etalow = h3.GetYaxis().GetBinLowEdge(iy)
#                 etahigh = h3.GetYaxis().GetBinUpEdge(iy)
#                 nvtxlow = h3.GetZaxis().GetBinLowEdge(iz)
#                 nvtxhigh = h3.GetZaxis().GetBinUpEdge(iz)
#                 print "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f && nvtx >= %i && nvtx < %i) return %.4f;" \
#                         % (ptlow, pthigh, etalow, etahigh, nvtxlow, nvtxhigh, val)
#     print "  return 0.;"
#     print "}"

# el_sfs.root


if True:
    # FULLSIM ELECTRONS
    electron_file = r.TFile("rootfiles/el_sfs.root")

    tightid2d3d = electron_file.Get("GsfElectronToTightID2D3D")
    multi = electron_file.Get("MVATightElectronToMultiIsoT")
    convhit0chg = electron_file.Get("MVATightConvIHit0ElectronToConvIHit0Chg")
    multiemu = electron_file.Get("MVATightElectronToMultiIsoEmu")

    el_noiso = tightid2d3d.Clone("el_noiso")
    el_noiso.Multiply(multi)
    el_noiso.Multiply(convhit0chg)

    el_iso = tightid2d3d.Clone("el_noiso")
    el_iso.Multiply(multiemu)
    el_iso.Multiply(convhit0chg)

    f_gsf = r.TFile("rootfiles/egammaEffi.txt_SF2D.root")
    h_gsf = get_first_hist(f_gsf)

    # FULLSIM MUONS
    muon_file_dxy = r.TFile("rootfiles/mu_dxy0p05dz0p1.root")
    muon_file_mediumid = r.TFile("rootfiles/mu_mediumID.root")
    muon_file_sip3d = r.TFile("rootfiles/mu_sip3dlt4.root")
    muon_file_multiiso = r.TFile("rootfiles/mu_multiiso.root") 
    muon_file_dpt = r.TFile("rootfiles/mu_dpt0p2.root") 

    dxy0p05dz0p1 = get_first_hist(muon_file_dxy)
    mediumID = get_first_hist(muon_file_mediumid)
    sip3dlt4 = get_first_hist(muon_file_sip3d)
    multiiso = get_first_hist(muon_file_multiiso) 
    dpt = get_first_hist(muon_file_dpt) 

    mu = dxy0p05dz0p1.Clone("mu")
    mu.Multiply(mediumID)
    mu.Multiply(sip3dlt4)
    mu.Multiply(multiiso)
    mu.Multiply(dpt) 

    # FASTSIM ELECTRONS
    el_fastsim_file = r.TFile("rootfiles/el_fs.root")
    el_fastsim = el_fastsim_file.Get("histo2D")

    el_fastsim_iso_file = r.TFile("rootfiles/el_fs_iso.root")
    el_fastsim_iso = el_fastsim_iso_file.Get("histo2D")


    # FASTSIM MUONS
    muon_fs_file_dxy = r.TFile("rootfiles/mu_fs_dxy0p05dz0p1.root")
    muon_fs_file_mediumid = r.TFile("rootfiles/mu_fs_mediumID.root")
    muon_fs_file_sip3d = r.TFile("rootfiles/mu_fs_sip3dlt4.root")
    muon_fs_file_multiiso = r.TFile("rootfiles/mu_fs_multiiso.root") 
    muon_fs_file_dpt = r.TFile("rootfiles/mu_fs_dpt0p2.root") 

    dxy0p05dz0p1_fs = get_first_hist(muon_fs_file_dxy)
    mediumID_fs = get_first_hist(muon_fs_file_mediumid)
    sip3dlt4_fs = get_first_hist(muon_fs_file_sip3d)
    multiiso_fs = get_first_hist(muon_fs_file_multiiso) 
    dpt_fs = get_first_hist(muon_fs_file_dpt) 

    mu_fastsim = dxy0p05dz0p1_fs.Clone("mu")
    mu_fastsim.Multiply(mediumID_fs)
    mu_fastsim.Multiply(sip3dlt4_fs)
    mu_fastsim.Multiply(multiiso_fs)
    mu_fastsim.Multiply(dpt_fs) 

    ### DO THE DUMPING

    dump_bins(el_noiso, name="electronScaleFactorHighHT")
    dump_bins(el_iso, name="electronScaleFactorLowHT")
    dump_bins(mu, name="muonScaleFactor")
    dump_bins(h_gsf,"electronGSF", stupid=True)
    dump_bins(el_fastsim, name="electronScaleFactorFastSimHighHT")
    dump_bins(el_fastsim_iso, name="electronScaleFactorFastSimLowHT")
    dump_bins(mu_fastsim, name="muonScaleFactorFastSim")
