import math, os, sys


def divErrors(a,b,da,db):
    # return error on a/b
    return a/b,(1.0*a/b)*math.sqrt( (da/a)**2.0 + (db/b)**2.0 )

def latexNames(cutName):
    cutName = cutName.replace(">=","\\geq ")
    cutName = cutName.replace("<=","\\leq ")
    cutName = cutName.replace("ht","H_\\mathrm{T}")
    cutName = cutName.replace("mtmin","m^\\mathrm{T}_\\mathrm{min}")
    cutName = cutName.replace("njets","\\mathrm{Njets}")
    cutName = cutName.replace("met","\\met")
    cutName = cutName.replace("HH","\\mathrm{HH}")
    cutName = cutName.replace("HL","\\mathrm{HL}")
    cutName = cutName.replace("nbtags","\\mathrm{Nbtags}")
    return cutName

def countsAndErrors(sampleCounts, WZonly=False, doData=False):
    tot = 0.0
    totErr = 0.0

    if(WZonly):
        tot = sampleCounts["WZ"][0]
        totErr = sampleCounts["WZ"][1]
    else:
        if(doData):
            tot = sampleCounts["Data"][0]
            totErr = sampleCounts["Data"][1]
        else:
            tot = sum([sampleCounts[sample][0] for sample in allsamples if sample != "Data"])
            totErr = math.sqrt(sum([sampleCounts[sample][1]**2.0 for sample in allsamples if sample != "Data"]))

    return tot,totErr

def purity(sampleCounts):
    nWZ = sampleCounts["WZ"][0]
    nAll = sum([sampleCounts[sample][0] for sample in allsamples if sample != "Data"])
    return nWZ/nAll*100

lines = open("counter.txt","r").readlines()
# allsamples = ["TTBAR","DY","TTW","TTZ","WJets","WZ","ZZ","Data"] 
allsamples = ["TTBAR","DY","TTW","TTZ","WJets","WZ","ZZ","Data"] 
cutStrDict = { 1: "njets>=5", 2: "nbtags>=1", 3: "met>=200", 4: "ht>=300", 5: "mtmin>=120", 6: "HL" }
mainCutCountErr = { }

cutCounts = {}
for line in lines:
    # if("WZ" in line and ("_" not in line)): totalWZ = float(line.split()[1])
    if("_" not in line): continue
    line = line.strip()
    cut = line.split()[0].split("_")[0]
    sample = line.split()[0].split("_")[1]
    count = line.split()[1]
    error = line.split()[2]

    if(cut not in cutCounts):
        cutCounts[cut] = { }
        for all in allsamples: cutCounts[cut][all] = [ 0.0, 0.0 ]

        cutCounts[cut][sample] = [ float(count), float(error) ]
    else: 
        cutCounts[cut][sample] = [ float(count), float(error) ]


# group them into pairs
doData = True
cutCountsList = []
for cut in cutCounts.keys():
    cAndEWZ = countsAndErrors(cutCounts[cut],WZonly=True)
    cAndE = countsAndErrors(cutCounts[cut], doData=doData)

    # 0 cut
    # 1 counts total
    # 2 err total
    # 3 counts WZ
    # 4 err WZ
    # 5 purity WZ

    # cutCountsList.append( [cut, cAndE[0], cAndE[1], cAndEWZ[0], cAndEWZ[1], purity(cutCounts[cut])] )
    cutCountsList.append( [cut, cAndE[0], cAndE[1], cAndEWZ[0], cAndEWZ[1], purity(cutCounts[cut])] )

cutCountsList.sort()

cutCountsPairs = zip(cutCountsList[0::2],cutCountsList[1::2])

# move H_T pair to end
indexOfHtPair = [i for i in range(len(cutCountsPairs)) if "ht" in cutCountsPairs[i][0][0]][0]
cutCountsPairs += [cutCountsPairs.pop(indexOfHtPair)]

indexOfHHPair = [i for i in range(len(cutCountsPairs)) if "HH" in cutCountsPairs[i][0][0]][0]

total = cutCountsPairs[indexOfHHPair][0][1]
totalWZ = cutCountsPairs[indexOfHHPair][0][3]
errWZ = cutCountsPairs[indexOfHHPair][0][4]
pur = cutCountsPairs[indexOfHHPair][0][5]

pctSyst = 0.5
# compute scale factor after background subtraction
w,t = totalWZ, total
b = 100*w/pur - w # #WZ/purity - #WZ (this is just total MC - WZ)
dw, dt, db = errWZ, math.sqrt(t), pctSyst*b
s = (t-b)/w
scaleFactor,scaleFactorError = s,(1/w)*math.sqrt( dt**2 + db**2 + s**2 * dw**2 )
# print "%.2f \\pm %.2f" % (scaleFactor, scaleFactorError)
print "mrc 1 6 \\textbf{CR SF}: $%.2f \\pm %.2f$ (not applied)" % (scaleFactor, scaleFactorError)
print

print "\\textbf{Unscaled} | Data | WZ Count | WZ Purity | WZ Above Threshold | $\\delta $WZ/WZ"
# print "\\textbf{SF}: $%.2f \\pm %.2f$ | Events | WZ Count | WZ Purity | WZ Above Threshold | $\\delta $WZ/WZ" % (scaleFactor,scaleFactorError)
# print " - | Events | WZ Count | WZ Purity | WZ Above Threshold | $\\delta $WZ/WZ" 
print ""
for i,pair in enumerate(cutCountsPairs):
    elem1, elem2 = pair
    cutName1 = elem1[0].split(":")[-1]
    cutName2 = elem2[0].split(":")[-1]


    # want elem1 to be the lower one
    if(">" in cutName1 or "HL" in cutName1):
        elem1, elem2 = elem2, elem1

    WZ1 = elem1[3]
    WZ2 = elem2[3]


    T1, dT1 = elem1[1], math.sqrt(elem1[1])
    T2, dT2 = elem2[1], math.sqrt(elem2[1])
    if(dT2 < 0.001): dT2 = 1.0

    NonWZ1 = T1 - WZ1
    NonWZ2 = T2 - WZ2

    dNonWZ1 = pctSyst * NonWZ1
    dNonWZ2 = pctSyst * NonWZ2

    errStat2 = math.sqrt(WZ2)
    errStatSyst2 = math.sqrt( dT2**2.0 + dNonWZ2**2.0 )
    errSyst2 = math.sqrt(errStatSyst2**2 - errStat2**2)

    relErrorWZ2 = errStatSyst2 / WZ2

    errStat1 = math.sqrt(WZ1)
    errStatSyst1 = math.sqrt( dT1**2.0 + dNonWZ1**2.0 )
    errSyst1 = math.sqrt(errStatSyst1**2 - errStat1**2)
    relErrorWZ1 = errStatSyst1 / WZ1

    if( cutName2.startswith("njets") ) : mainCutCountErr[1]  = [T1, T2, WZ1, WZ2, relErrorWZ2]
    if( cutName2.startswith("nbtags") ): mainCutCountErr[2]  = [T1, T2, WZ1, WZ2, relErrorWZ2]
    if( cutName2.startswith("met") )   : mainCutCountErr[3]  = [T1, T2, WZ1, WZ2, relErrorWZ2]
    if( cutName2.startswith("mtmin") ) : mainCutCountErr[5]  = [T1, T2, WZ1, WZ2, relErrorWZ2]
    if( cutName2.startswith("HL") )    : mainCutCountErr[6]  = [T1, T2, WZ1, WZ2, relErrorWZ2]

    if( cutName2.startswith("ht") )    : mainCutCountErr[-4] = [T1+T2, T1, WZ1+WZ2, WZ1, relErrorWZ1]
    if( cutName2.startswith("ht") )    : mainCutCountErr[4]  = [T1+T2, T2, WZ1+WZ2, WZ2, relErrorWZ2]

    cutName1 = latexNames(cutName1)
    cutName2 = latexNames(cutName2)

    if( i == len(cutCountsPairs)-1 ):
        print ""

        print "$%-35s$ | $ %.2f \\pm %.2f $ |$ %.2f $ | $ %.1f \\%% $ | $ %.2f\\pm %.2f~\\mathrm{(stat)}\\pm %.2f~\\mathrm{(sys)} $ | %.2f " \
                % (cutName1, elem1[1], dT1, elem1[3], elem1[5], WZ1, errStat1, errSyst1, relErrorWZ1)
        print "$%-35s$ | $ %.2f \\pm %.2f $ |$ %.2f $ | $ %.1f \\%% $ | $ %.2f\\pm %.2f~\\mathrm{(stat)}\\pm %.2f~\\mathrm{(sys)} $ | %.2f " \
                % (cutName2, elem2[1], dT2, elem2[3], elem2[5], WZ2, errStat2, errSyst2, relErrorWZ2)
    else:
        print "$%-35s$ | $ %.2f \\pm %.2f $ |$ %.2f $ | $ %.1f \\%% $ | mrc 2 1 $ %.2f\\pm %.2f~\\mathrm{(stat)}\\pm %.2f~\\mathrm{(sys)} $ | mrc 2 1 %.2f " \
                % (cutName1, elem1[1], dT1, elem1[3], elem1[5], WZ2, errStat2, errSyst2, relErrorWZ2)
        print "$%-35s$ | $ %.2f \\pm %.2f $ |$ %.2f $ | $ %.1f \\%% $ | $                                                               $ |     " \
                % (cutName2, elem2[1], dT2, elem2[3], elem2[5])


