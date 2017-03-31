import ROOT as r
import os
import glob
import re

def doReduceBins(fname, new_dir, kine, d_map, require_full_coverage=True):
    if not os.path.exists(new_dir):
        os.system("mkdir -p %s" % new_dir)

    # SR = int(SR)
    # print "Reducing %s to only have %sSR%i" % (fname, kine.replace("i","").replace("o","").replace("w","").upper(), SR)

    fin = r.TFile(fname)
    fout = r.TFile(new_dir+"/"+fname.split("/")[-1], "RECREATE")

    n_sr_orig = len(sum(d_map[kine].values(), []))
    n_sr_new = len(d_map[kine].keys()) 

    # print "Reducing %s to have aggregate regions (%i bins -> %i bins)" % (fname, n_sr_orig, n_sr_new)

    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        hist = fin.Get(name)

        # identify the hists like "ww_stat_hihi30Up", "ww_stat_hihi30Down" and skip them since these are added by createCards bin-by-bin
        match = re.match(".*_(hihi|hilow|lowlow)([0-9]{1,2})(Up|Down)$", name)
        if match and len(match.groups()) == 3:
            continue
            # thekine = match.group(1)
            # thesr = match.group(2)
            # if int(thesr) != int(SR): continue
            # if str(thekine) != str(kine): continue


        if require_full_coverage and n_sr_orig != hist.GetNbinsX() and name != "genMet":
            # if we want the sum of original SRs to be the same as the sum of aggregate regions
            print "[!] Error: number of input SRs (%i) does not match the expected mapping (%i)!" % (hist.GetNbinsX(), n_sr_orig)
            break



        h_temp = r.TH1F(name, name, n_sr_new, 0, n_sr_new)
        for ibin in range(1,n_sr_new+1):
            val = 0.0
            err = 0.0
            for ibinold in d_map[kine][ibin]:
                val += hist.GetBinContent(ibinold)
                err += hist.GetBinError(ibinold)**2
            err = err**0.5
            h_temp.SetBinContent(ibin, val)
            h_temp.SetBinError(ibin, err)
        h_temp.Write()


        # # don't reduce an already reduced histogram, dummy!!
        # if hist.GetNbinsX() == 1: SR = 1

        # val = hist.GetBinContent(SR)
        # err = hist.GetBinError(SR)

        # # print name, val
        # if "jesDown" in name and val == 0:
        #     val = 0.0001


    # # FIXME write new SR hist with nsig=1
    # if "sig_" in fname:
    #     srin = fout.Get("sr")
    #     print "NSIGNAL:"+str(srin.GetBinContent(1))
    #     srin.SetBinContent(1, 1.0)
    #     srin.Write()

    fout.Close()
    fin.Close()

# def doMove(fname):
#     os.system("mv {0}_new {0}".format(fname))

# takes the number of the aggregate SR (and kine) and returns list of regular SR numbers
d_agg2reg_sr = {
        "hihi": {
            1: range(1,8),
            2: range(8,11),
            3: range(11,20),
            4: range(20,23),
            5: range(23,32),
            6: range(32,35),
            7: range(35,41),
            8: range(41,42),
            9: range(42,46),
            10: range(46,52),
            },
        "hilow": {
            1: range(1,32),
            2: range(32,34),
            3: range(34,38),
            4: range(38,42),
            },
        "lowlow": {
            1: range(1,9),
            }
        }

d_agg2reg_sr_slim = {
        "hihi": {
                1: range(20,23), # 4 
                2: range(42,46), # 9
                3: range(46,52), # 10
            },
        "hilow": {
                1: range(32,34), # 2
            },
        "lowlow": {
            1: range(1,9), # 1
            }
        }

if __name__ == "__main__":

    # orig_dir = "v8.04_Nov5_17p3_qsplit"
    # out_dir = "agg_test_slim"

    # orig_dir = "v8.07_Dec1_17p3_rereco"
    # out_dir = "v8.07_Dec1_17p3_rereco_agg"

    # orig_dir = "v8.07_Dec2_17p3_rerecojetht_v1"
    # out_dir = "v8.07_Dec2_17p3_rerecojetht_v1_agg"

    # orig_dir = "v8.07_Dec5_17p3_v2"
    # out_dir = "v8.07_Dec5_17p3_v2_agg"

    # orig_dir = "v9.04_Jan31_36p8_t1tttt"
    # out_dir = "v9.04_Jan31_36p8_t1tttt_exclagg"
    # orig_dir = "v9.04_Jan31_36p8_blah"
    # out_dir = "v9.04_Jan31_36p8_blah_exclagg"

    # orig_dir = "v9.06_Mar6_35p9_reminiaodfilt_t1tttt_met"
    # out_dir = "v9.06_Mar6_35p9_reminiaodfilt_t1tttt_met_agg"

    # orig_dir = "v9.06_Mar6_35p9_reminiaodfilt_t5qqqqt6ttww_met"
    # out_dir = "v9.06_Mar6_35p9_reminiaodfilt_t5qqqqt6ttww_met_agg"

    # orig_dir = "v9.06_Mar6_35p9_reminiaodfilt_t5tttt_met"
    # out_dir = "v9.06_Mar6_35p9_reminiaodfilt_t5tttt_met_agg"

    # orig_dir = "v9.06_Mar6_35p9_reminiaodfilt_t1ttbb_met"
    # out_dir = "v9.06_Mar6_35p9_reminiaodfilt_t1ttbb_met_agg"

    orig_dir = "v9.06_Mar6_35p9_reminiaodfilt_t5ttcc_met"
    out_dir = "v9.06_Mar6_35p9_reminiaodfilt_t5ttcc_met_agg"

    # orig_dir = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs"
    # out_dir = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs_agg"
    # orig_dir = "v9.06_Mar6_35p9_reminiaod"
    # out_dir = "v9.06_Mar6_35p9_reminiaod_testagg"

    model = "" # set to empty string to do all models

    # fnames = ["%s/fs_t1tttt_m1500_m400_histos_hihi_17.3ifb.root" % orig_dir]
    # print fnames

    num_sr_agg = sum([len(v.keys()) for v in d_agg2reg_sr.values()])
    num_sr_nom =  sum([len(sum(v.values(),[])) for v in d_agg2reg_sr.values()])
    print ">>> Output directory: %s" % out_dir
    print ">>> Will reduce %i nominal bins to %i aggregate SRs in total" % (num_sr_nom, num_sr_agg)

    import tqdm

    # for fname in fnames:
    for kine in ["hihi","hilow","lowlow"]:
    # for kine in ["lowlow"]:
        # for fname in glob.glob("%s/*_%s_*.root" % (orig_dir,kine)):
        for fname in tqdm.tqdm(glob.glob("%s/*_%s_*.root" % (orig_dir,kine))):
            if "fs_" in fname:
                if model and model not in fname: continue
            doReduceBins(fname, out_dir, kine, d_agg2reg_sr)
            # doReduceBins(fname, out_dir, kine, d_agg2reg_sr_slim, require_full_coverage=False)

#     fnames = glob.glob("v8.02_cut700/*hihi*.root")
#     for fname in fnames:
#         print fname
#         doReduceBins(fname, False)
#         doMove(fname)
