* taking SFs from https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Electrons_AN1
* fastsim SFs from https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSFMC
* go into `rootfiles` and execute `. download.sh` to hopefully download the correct ones (uncomment new ones as they become available, and download the stupid ones behind CERN firewall manually)
* then make modifications inside of `dumpScaleFactors.py` and run it. put these in the appropriate `.h` files

We consider the histograms listed (take product of various histograms since these are factorized)

            Electrons 

VETO

* Non triggering MVA FOLoose + IDEmu + MiniIso04 + TightIP2D + convVeto + missing inner hits <=1 (ra5 veto)
FOID2D * Mini4 * ConvHit1

NoIso

* Non triggering MVA Tight + IDEmu + MultiIsoTight + TightIP2D + TightIP3D + convVeto + missing inner hits ==0, charge consistency (ra5)
TightID2D3D * Multi * ConvHit0Chg

Iso

* Non triggering MVA Tight + IDISOEmu + MultiIsoTight + TightIP2D + TightIP3D + convVeto + missing inner hits ==0, charge consistency (ra5)
TightID2D3D * MultiEmu * ConvHit0Chg

            Muons

Signal

* SUS-16-020   RA5 same-sign dilepton (signal leptons)   25/10, 2.4   Medium && dpt/pt<0.2   Multi-Iso Medium wp   abs(dxy) < 0.05 && abs(dz) < 0.1 && abs(SIP3d) < 4   none   ?/YES
(Medium ID) * (dpt/pt<0.2 vs Medium ID) * (dxy0.05, dz0.1 vs Medium ID) * (MultiIsoM vs Medium ID) * (SIP3D<4 vs Medium ID)

Veto

* SUS-16-020   RA5 same-sign dilepton (veto leptons)   5, 2.4   Medium && dpt/pt<0.2   miniIso/pT < 0.4 (EA)   abs(dxy) < 0.05 && abs(dz) < 0.1
(Medium ID) * (dpt/pt<0.2 vs Medium ID) * (dxy0.05, dz0.1 vs Medium ID) * (miniIso0.4 vs Medium ID) 








