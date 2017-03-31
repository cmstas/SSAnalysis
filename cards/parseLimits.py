# lim_tttt = """
# Observed Limit: r < 6.7701
# Expected  2.5%: r < 2.4475
# Expected 16.0%: r < 3.3773
# Expected 50.0%: r < 4.9531
# Expected 84.0%: r < 7.3816
# Expected 97.5%: r < 10.5316
# """

# v8.04_tttt_July26/card_tttt_12.9ifb-all.txt
lim_tttt = """
Observed Limit: r < 6.2128
Expected  2.5%: r < 2.5093
Expected 16.0%: r < 3.4626
Expected 50.0%: r < 5.0781
Expected 84.0%: r < 7.6083
Expected 97.5%: r < 10.9437
"""

# # v8.03_fort5qqqqvv_fix/card-ttos.txt
# lim_sstt = """
# Observed Limit: r < 1.6265
# Expected  2.5%: r < 0.6228
# Expected 16.0%: r < 0.8336
# Expected 50.0%: r < 1.1680
# Expected 84.0%: r < 1.6568
# Expected 97.5%: r < 2.2563
# """


# # v8.04_12p9/card-ttos.txt
# lim_sstt = """
#  -- Asymptotic --
# Observed Limit: r < 1.4801
# Expected  2.5%: r < 0.5355
# Expected 16.0%: r < 0.7197
# Expected 50.0%: r < 1.0117
# Expected 84.0%: r < 1.4352
# Expected 97.5%: r < 1.9673
# """

# # v8.04_July25/card-ttos.txt
lim_sstt = """
 -- Asymptotic --
Observed Limit: r < 1.7346
Expected  2.5%: r < 0.5894
Expected 16.0%: r < 0.7890
Expected 50.0%: r < 1.1055
Expected 84.0%: r < 1.5770
Expected 97.5%: r < 2.1695
"""

# v8.04_Oct31_17p3/card-ttos.txt
lim_sstt = """
 -- Asymptotic --
 Observed Limit: r < 2.0359
 Expected  2.5%: r < 0.5436
 Expected 16.0%: r < 0.7239
 Expected 50.0%: r < 1.0195
 Expected 84.0%: r < 1.4381
 Expected 97.5%: r < 1.9640
"""

xsec_tttt = 0.009103 # pb
xsec_sstt = 831.762/1000.0 # pb  # divide by 1000 because of line in yieldMaker for OS

def get_lim(lim_str, xsec, name):
    d = {}
    for line in lim_str.splitlines():
        if "Observed" in line: d["obs"] = float(line.split("<")[-1])
        elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])

    unit = "pb"
    if d["obs"]*xsec < 0.1:
        xsec *= 1000.0
        unit = "fb"
    obs = d["obs"]*xsec
    exp = d["exp_50.0"]*xsec
    exp_sm1 = d["exp_16.0"]*xsec
    exp_sp1 = d["exp_84.0"]*xsec

    print "Limits for %s" % name
    print "  Obs: %.2f %s" % (obs, unit)
    print "  Exp: %.2f + %.2f - %.2f %s" % (exp, exp_sp1-exp, exp-exp_sm1, unit)

get_lim(lim_tttt, xsec_tttt, "TTTT")
get_lim(lim_sstt, xsec_sstt, "SSTT")

# print obs
# print exp
# print exp_sp1
# print exp_sm1
