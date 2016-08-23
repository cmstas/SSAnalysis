import sys

def dump(fname):
    doing_one_br = "hihi.txt" in fname or "hilow.txt" in fname or "lowlow.txt" in fname
    nproc, nsyst, nbins = None, None, None
    observations = []
    obsbinnames = []
    binnames = []
    procnames = []
    rates = []
    found_first_bin_line = False
    found_first_process_line = False
    with open(fname, "r") as fhin:
        for line in fhin:
            line = line.strip()
            if line.startswith("imax") and not nbins: nbins = int(line.replace("*","-1").split()[1])
            elif line.startswith("jmax") and not nproc: nproc = int(line.replace("*","-1").split()[1])
            elif line.startswith("kmax") and not nsyst: nsyst = int(line.replace("*","-1").split()[1])
            elif line.startswith("observation") and not observations: observations = map(lambda x: int(float(x)), line.split()[1:])
            elif line.startswith("rate") and not rates: rates = map(lambda x: float(x), line.split()[1:])

            if line.startswith("bin"): 
                if found_first_bin_line:
                    binnames = map(lambda x: x.strip(), line.split()[1:])
                else:
                    obsbinnames = map(lambda x: x.strip(), line.split()[1:])
                found_first_bin_line = True

            if line.startswith("process"):
                # if doing _all.txt cards, then the first process line gives the names
                # if doing _hihi or _hilow or whatever, then last one gives the names
                if not found_first_process_line or doing_one_br:
                    procnames = map(lambda x: x.strip(), line.split()[1:])
                found_first_process_line = True

    d_card = {}
    for bname,obs in zip(obsbinnames,observations):
        d_card[bname] = {}
        d_card[bname]["data"] = obs

    for bname,proc,rate in zip(binnames,procnames,rates):
        d_card[bname][proc] = rate

    print "#### %s ####" % fname
    print
    for channel,info in d_card.items():
        nsig = sum([info[k] for k in info if ("fs_" in k or "sig" in k) and "data" not in k])
        ndata = sum([info[k] for k in info if ("fs_" not in k) and ("sig" not in k) and ("data" in k)])
        npred = sum([info[k] for k in info if ("fs_" not in k) and ("sig" not in k) and ("data" not in k)])

        print info
        signame = ([k for k in info if "fs_" in k]+["sig"])[0]

        print "  == Channel %s ==" % channel
        print "    Obs: %i" % ndata
        print "    Pred: %.2f" % npred
        print "    %s: %.2f" % (signame, nsig)
        print


if __name__ == '__main__':
    # fname = "v8.04_July28/card_fs_t5qqqqvv_m1300_m300_12.9ifb-all.txt"
    # fname = "v8.04_July28/card_fs_t5qqqqvv_m1400_m1100_12.9ifb-all.txt"
    # fname = "v8.04_July28/card_fs_t5qqqqvv_m1000_m800_12.9ifb-all.txt"
    fname = "v8.04_July28/test_card_fs_t1tttt_m1000_m450_12.9ifb-hihi.txt"
    if len(sys.argv) == 2:
        fname = sys.argv[-1]
    dump(fname)
