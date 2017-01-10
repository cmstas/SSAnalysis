
def make_dict(fname):
    d = {}
    with open(fname, "r") as fhin:
        for line in fhin:
            line = line.strip()
            if len(line) < 5: continue
            parts = line.split()
            run, lumi, event = map(int,parts[:3])
            rest = map(float, parts[3:])
            d[event] = rest
    return d

did_print_already = False
def compare(f1, f2, col_names, verbose=False, print_common_disagreements=True):
    global did_print_already


    d1 = make_dict(f1)
    d2 = make_dict(f2)

    events1 = set(d1.keys())
    events2 = set(d2.keys())

    in_one_not_two = list(events1-events2)
    in_two_not_one = list(events2-events1)
    in_both = list(events2 & events1)

    if not did_print_already:
        print "# events in %s: %i" % (f1, len(events1))
        print "# events in %s and not %s: %i" % (f1, f2, len(in_one_not_two))
        if verbose:
            print "Events in %s and not %s: %s" % (f1, f2, ",".join(map(str,in_one_not_two)))
            for evt in in_one_not_two:
                print evt, "\t".join(map(str,d1[evt]))
        print

        print "# events in %s: %i" % (f2, len(events2))
        print "# events in %s and not %s: %i" % (f2, f1, len(in_two_not_one))
        if verbose:
            print "Events in %s and not %s: %s" % (f2, f1, ",".join(map(str,in_two_not_one)))
            for evt in in_two_not_one:
                print evt, "\t".join(map(str,d2[evt]))
        print

        print "# events in both: %i" % (len(in_both))
        if verbose: print "Events in both: %s" % (",".join(map(str,in_both)))

        did_print_already = True

    # key:col_name, value:# of disagreements
    d_disclosure = { name:0 for name in col_names }

    # key:col_name, value:list of events that disagree
    d_disclosure_events = { name:[] for name in col_names }

    # key:col_name, value:list of pairs of (val1,val2) for disagreeing values
    d_disclosure_pairs = { name:[] for name in col_names }

    for evt in sorted(in_both):
        for i, (v1, v2) in enumerate(zip(d1[evt], d2[evt])):
            v1_str = "%.2f" % v1
            v2_str = "%.2f" % v2
            if v1 != v2:
            # if v1_str != v2_str:
                d_disclosure[col_names[i]] += 1
                d_disclosure_events[col_names[i]].append(evt)
                d_disclosure_pairs[col_names[i]].append((v1,v2))

    if print_common_disagreements:
        print "For these common events, # of disagreements is as follows:"
        for col_name in sorted(d_disclosure.keys(), key=d_disclosure.get, reverse=True):
            if len(d_disclosure_events[col_name]) == 0: continue
            print "   %-20s %i" % (col_name, d_disclosure[col_name])
            if verbose:
                evt_strs = map(str,d_disclosure_events[col_name])
                # pair_strs = map(lambda x: "%.1f vs %.1f" % x,d_disclosure_pairs[col_name])
                pair_strs = map(lambda x: "%.3f vs %.3f" % x,d_disclosure_pairs[col_name])
                for evt_str, pair_str in zip(evt_strs, pair_strs):
                    print "     %s: %s" % (evt_str, pair_str)
                # print "      events: %s" % (",".join(map(str,d_disclosure_events[col_name])))
                # print "      pairs: %s" % (", ".join(map(lambda x: "%.1f vs %.1f" % x,d_disclosure_pairs[col_name])))
                print

if __name__ == "__main__":
    # typ = "global"
    # typ = "leptons"
    # typ = "jets"

    verbose = True # if True, show event numbers for relevant parts
    for typ in ["global"]:
    # for typ in ["global_veto"]:
    # for typ in ["global", "leptons", "jets"]:

        print "\n\n" + "#" * 40
        print "#" * 20 + typ.upper() + "#" * 20
        print "#" * 40

        # f1 = "ucsx_TTW_80X_dump_%s.txt" % typ
        # f2 = "ecop/fdump_%s_80X.txt" % typ
        f1 = "matthieu_qsplit.txt"
        f2 = "ucsx_TTW_dump_qsplit.txt"
        # f2_patt = "ufl/fdump_%s_80X.txt"

        if typ in ["global", "global_veto"]:
            col_names = ["vetoele7+vetomu5", "lep1id", "lep1pt", "lep2id", "lep2pt", "njets", "nbtags", "met", "ht", "SR"]
        if typ == "leptons":
            col_names = ["lep1pt", "lep1eta", "lep1phi", "lep1id", "lep1conept", "lep1dxy", "lep1dz", "lep1sip", "lep1miniiso", "lep1ptratio", "lep1ptrel", "lep1mva", "lep1convveto", "lep1innerlayers", "lep1tightcharge", "lep1MCmatch", "lep2pt", "lep2eta", "lep2phi", "lep2id", "lep2conept", "lep2dxy", "lep2dz", "lep2sip", "lep2miniiso", "lep2ptratio", "lep2ptrel", "lep2mva", "lep2convveto", "lep2innerlayers", "lep2tightcharge", "lep2MCmatch"]
        if typ == "jets":
            col_names = ["jet1pt", "jet1eta", "jet1phi", "jet1csvmva", "jet1csvmedium", "jet2pt", "jet2eta", "jet2phi", "jet2csvmva", "jet2csvmedium"]

        compare(f1, f2, col_names, verbose=verbose, print_common_disagreements=True)


