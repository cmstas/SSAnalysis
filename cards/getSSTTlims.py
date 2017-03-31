import commands
import os
import sys
import glob



d_xsec = {}
d_xsec["tttt"] = 0.009103 # pb
d_xsec["sstt"] = 831.762/1000.0 # pb  # divide by 1000 because of line in yieldMaker for OS


def get_lim(folder, typ=None, redo_card=False, redo_log=False, fb=False, extra=""):
    if typ == "sstt":

        card_name = "card-ttos.txt"
        card_cmd = extra+" python createCard.py {0} ttbar_os br sr {1}".format(folder, card_name)

        # card_name = "card_ttbar_os_17.3ifb-hihi.txt"
        # card_cmd = "python createCard.py {0} ttbar_os".format(folder, card_name)

        log_name = "card-ttos.log".format(folder)
        combine_cmd = "combine -M Asymptotic {0}/{2}  --noFitAsimov > {0}/{1}".format(folder, log_name, card_name)
    elif typ == "tttt":

        card_cmd = extra+" python createCard.py {0} tttt".format(folder)
        # stat, out = commands.getstatusoutput(card_cmd)
        # print card_cmd
        # card_name = glob.glob(folder+"/card_tttt_*ifb-all.txt")[0].split("/")[-1]
        card_name = folder+"/card_tttt_*ifb-all.txt"
        # print card_name
        log_name = "card-tttt.log".format(folder)
        # print folder+"/"+log_name
        combine_cmd = "combine -M Asymptotic {2}  --noFitAsimov > {0}/{1}".format(folder, log_name, card_name)
        # print combine_cmd

    else:
        raise NotImplemented

    sys.stdout.flush()
    if redo_card or not os.path.isfile(folder+"/"+card_name):
        print ">>> Making card for %s" % typ
        stat, out = commands.getstatusoutput(card_cmd)
        print out
    if redo_log or not os.path.isfile(folder+"/"+log_name):
        print ">>> Running combine for %s" % typ
        stat, out = commands.getstatusoutput(combine_cmd)
        print out
    sys.stdout.flush()

    lines = []
    with open(folder+"/"+log_name, "r") as fhin:
        lines = fhin.readlines()

    return parse_lim(lines, d_xsec[typ], fb=fb)

def parse_lim(lim_lines, xsec, fb=False):
    """
    return dictionary with obs, exp, sp1, sm1 cross section limits in pb
    input xsec must be in pb
    """
    d = {}
    for line in lim_lines:
        if "Observed" in line: d["obs"] = float(line.split("<")[-1])
        elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])

    mult = 1 if not fb else 1000.0

    obs = d["obs"]*xsec*mult
    exp = d["exp_50.0"]*xsec*mult
    exp_sm1 = d["exp_16.0"]*xsec*mult
    exp_sp1 = d["exp_84.0"]*xsec*mult


    return {
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            }


def print_lims(d_lims, fb=False):

    unit = "pb" if not fb else "fb"
    print "  Obs: %.2f %s" % (d_lims["obs"], unit)
    # print "  Exp: %.2f + %.2f - %.2f pb" % (d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"])
    print "  Exp: %.2f + %.2f - %.2f %s" % (d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"], unit)


if __name__ == "__main__":

    # folder="v8.04_Oct31_17p3"

    for folder in [
            # "v8.04_July25",
            # "v8.04_Oct31_17p3",
            # "v8.04_Oct31_17p3_HHonly",
            # "v8.04_Oct31_17p3_nbgt2",
            # "v8.04_Oct31_17p3_nbgt1",
            # "v8.04_Oct31_17p3_mtmingt120_htlt300",
            # "v8.04_Oct31_17p3_plusplus",
            # "v8.04_Oct31_17p3_nbgt1_mtmingt120_htlt300",
            # "v8.04_Oct31_17p3_sr19",
            # "v8.04_Dec1_17p3",
            # "v8.07_Dec7_36p5_herewego",
            # "v8.07_Dec15_36p5_nobtagsf", # non moriond 36.5/fb
            # "v9.01_Jan16_36p5_nobtagsf_nbgeq2", # non moriond 36.5/fb
            ]:

        print "---- %s ----" % folder
        lims = get_lim(folder,typ="sstt", redo_card=False, redo_log=False)
        print lims
        print_lims(lims)
        print 

    # # folder = "v8.07_Dec15_36p5_nobtagsf" # non moriond 36.5/fb
    # folder = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs"
    # # folder = "v9.06_Mar6_35p9_reminiaod_testttz"
    # print "---- %s ----" % folder
    # lims = get_lim(folder,typ="sstt", redo_card=False, redo_log=False)
    # print lims
    # print_lims(lims)
    # print 

    # folder = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs"
    # print "---- TTTT: %s ----" % folder
    # lims = get_lim(folder,typ="tttt", redo_card=True, redo_log=True, fb=True)
    # print lims
    # print_lims(lims, fb=True)
    # print 

    # folder = "v9.06_Mar6_35p9_reminiaodfilt_tttt"
    # print "---- TTTT: %s ----" % folder
    # lims = get_lim(folder,typ="tttt", redo_card=False, redo_log=False, fb=True)
    # print lims
    # print_lims(lims, fb=True)
    # print 

    folder = "v9.06_Mar6_35p9_reminiaodttttnj7_tmp"
    print "---- TTTT: %s ----" % folder
    lims = get_lim(folder,typ="tttt", redo_card=True, redo_log=True, fb=True)
    print lims
    print_lims(lims, fb=True)
    print 

    # folder = "v9.06_Mar24_test_doublelumi_tttt"
    # print "---- TTTT: %s ----" % folder
    # lims = get_lim(folder,typ="tttt", redo_card=True, redo_log=True, fb=True, extra="")
    # print lims
    # print_lims(lims, fb=True)
    # print 

#     folder = "v9.06_Mar24_test_triplelumi_tttt"
#     print "---- TTTT: %s ----" % folder
#     lims = get_lim(folder,typ="tttt", redo_card=True, redo_log=True, fb=True, extra="")
#     print lims
#     print_lims(lims, fb=True)
#     print 
