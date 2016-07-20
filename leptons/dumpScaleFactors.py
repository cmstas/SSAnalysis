import ROOT as r

def get_first_hist(f1):
    return f1.Get(f1.GetListOfKeys()[0].GetName())

def dump_bins(h2, name):
    print "float %s(float pt, float eta) {" % name
    for ix in range(1,h2.GetNbinsX()+1):
        for iy in range(1,h2.GetNbinsY()+1):
            val = h2.GetBinContent(ix,iy)
            if ix != h2.GetNbinsX():
                print "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;" \
                        % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
            else:
                print "  if (pt >= %.0f  && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;" \
                        % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
    print "}"

def dump_bins_fastsim(h3, name):
    print "float %s(float pt, float eta, int nvtx) {" % name
    print "  if (pt>=200) pt = 199.9;"
    print "  if (fabs(eta)>=2.4) eta = 2.3;"
    print "  if (nvtx>=40) nvtx = 39;"
    for ix in range(1,h3.GetNbinsX()+1):
        for iy in range(1,h3.GetNbinsY()+1):
            for iz in range(1,h3.GetNbinsZ()+1):
                val = h3.GetBinContent(ix,iy,iz)
                ptlow = h3.GetXaxis().GetBinLowEdge(ix)
                pthigh = h3.GetXaxis().GetBinUpEdge(ix)
                etalow = h3.GetYaxis().GetBinLowEdge(iy)
                etahigh = h3.GetYaxis().GetBinUpEdge(iy)
                nvtxlow = h3.GetZaxis().GetBinLowEdge(iz)
                nvtxhigh = h3.GetZaxis().GetBinUpEdge(iz)
                print "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f && nvtx >= %i && nvtx < %i) return %.4f;" \
                        % (ptlow, pthigh, etalow, etahigh, nvtxlow, nvtxhigh, val)
    print "}"

# el_sfs.root


# electrons
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

# muons
muon_file_dxy = r.TFile("rootfiles/mu_dxy0p05dz0p1.root")
muon_file_mediumid = r.TFile("rootfiles/mu_mediumID.root")
muon_file_miniiso = r.TFile("rootfiles/mu_miniiso0p4.root")
muon_file_sip3d = r.TFile("rootfiles/mu_sip3dlt4.root")
# muon_file_multiiso = r.TFile("rootfiles/mu_multiiso.root") # don't have yet # FIXME
# muon_file_dpt = r.TFile("rootfiles/mu_dpt0p2.root") # don't have yet # FIXME

dxy0p05dz0p1 = get_first_hist(muon_file_dxy)
mediumID = get_first_hist(muon_file_mediumid)
miniiso0p4 = get_first_hist(muon_file_miniiso)
sip3dlt4 = get_first_hist(muon_file_sip3d)
# multiiso = get_first_hist(muon_file_multiiso) # FIXME
# dpt = get_first_hist(muon_file_dpt) # FIXME

mu = dxy0p05dz0p1.Clone("mu")
mu.Multiply(mediumID)
mu.Multiply(sip3dlt4)
# mu.Multiply(multiiso) # FIXME
# mu.Multiply(dpt) # FIXME

# fastsim
el_fastsim_file = r.TFile("rootfiles/el_fastsim.root")
mu_fastsim_file = r.TFile("rootfiles/mu_fastsim.root")
el_fastsim = el_fastsim_file.Get("histo3D")
mu_fastsim = mu_fastsim_file.Get("histo3D")

### DO THE DUMPING

dump_bins(el_noiso, name="electronScaleFactorHighHT")
dump_bins(el_iso, name="electronScaleFactorLowHT")
dump_bins(mu, name="muonScaleFactor")

dump_bins_fastsim(el_fastsim, name="electronScaleFactorFastSim")
dump_bins_fastsim(mu_fastsim, name="muonScaleFactorFastSim")
