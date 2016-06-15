#!/bin/bash

WHICH=$1
FILE=$2
USER=$3

#Show where you are
hostname

#Environment
export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc491
source /cvmfs/cms.cern.ch/cmsset_default.sh

#Set CMSSW environment
pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_12/src/
eval `scramv1 runtime -sh`
popd

#Specify name of output file and name of dierctory in /hadoop/...
export DIRNAME=ss_babies_June10 # FIXME match submit25ns
export WHICH_SMALL=`echo ${WHICH,,}`

#backgrounds
if [ "$WHICH" == "0" ]; then WHICH_SMALL="ttbar"; fi
if [ "$WHICH" == "1" ]; then WHICH_SMALL="ttw"; fi
if [ "$WHICH" == "2" ]; then WHICH_SMALL="ttz"; fi
if [ "$WHICH" == "3" ]; then WHICH_SMALL="wz"; fi
if [ "$WHICH" == "4" ]; then WHICH_SMALL="dy_low"; fi
if [ "$WHICH" == "5" ]; then WHICH_SMALL="dy_high"; fi
if [ "$WHICH" == "6" ]; then WHICH_SMALL="wjets"; fi
if [ "$WHICH" == "7" ]; then WHICH_SMALL="ttwqq"; fi
if [ "$WHICH" == "8" ]; then WHICH_SMALL="ttzq"; fi
if [ "$WHICH" == "9" ]; then WHICH_SMALL="ttg"; fi
if [ "$WHICH" == "10" ]; then WHICH_SMALL="singletop1"; fi
if [ "$WHICH" == "11" ]; then WHICH_SMALL="singletop2"; fi
if [ "$WHICH" == "12" ]; then WHICH_SMALL="singletop3"; fi
if [ "$WHICH" == "13" ]; then WHICH_SMALL="singletop4"; fi
if [ "$WHICH" == "14" ]; then WHICH_SMALL="singletop5"; fi
if [ "$WHICH" == "15" ]; then WHICH_SMALL="qqww"; fi
if [ "$WHICH" == "16" ]; then WHICH_SMALL="tttt"; fi
if [ "$WHICH" == "17" ]; then WHICH_SMALL="wwdps"; fi
if [ "$WHICH" == "18" ]; then WHICH_SMALL="ttbar_ph"; fi
if [ "$WHICH" == "20" ]; then WHICH_SMALL="wwz"; fi
if [ "$WHICH" == "21" ]; then WHICH_SMALL="zz"; fi
if [ "$WHICH" == "22" ]; then WHICH_SMALL="tg"; fi
if [ "$WHICH" == "23" ]; then WHICH_SMALL="zg"; fi
if [ "$WHICH" == "24" ]; then WHICH_SMALL="wzz"; fi
if [ "$WHICH" == "25" ]; then WHICH_SMALL="wgtolnug"; fi
if [ "$WHICH" == "26" ]; then WHICH_SMALL="tzq"; fi
if [ "$WHICH" == "27" ]; then WHICH_SMALL="tthtononbb"; fi
if [ "$WHICH" == "28" ]; then WHICH_SMALL="vhtononbb"; fi
if [ "$WHICH" == "29" ]; then WHICH_SMALL="ttzlow"; fi
if [ "$WHICH" == "30" ]; then WHICH_SMALL="gghtozzto4l"; fi
if [ "$WHICH" == "31" ]; then WHICH_SMALL="wzmg"; fi
if [ "$WHICH" == "32" ]; then WHICH_SMALL="wgmg"; fi
if [ "$WHICH" == "33" ]; then WHICH_SMALL="tthtononbbext"; fi
if [ "$WHICH" == "34" ]; then WHICH_SMALL="zzz"; fi
if [ "$WHICH" == "35" ]; then WHICH_SMALL="www"; fi

#signals
if [ "$WHICH" == "100" ]; then WHICH_SMALL="t1tttt_1500"; fi
if [ "$WHICH" == "101" ]; then WHICH_SMALL="t1tttt_1200"; fi
if [ "$WHICH" == "102" ]; then WHICH_SMALL="t5qqqqww_1200_1000_800"; fi
if [ "$WHICH" == "103" ]; then WHICH_SMALL="t5ttttdeg_1000_300_285_280"; fi
if [ "$WHICH" == "104" ]; then WHICH_SMALL="t5qqqqwwdeg_1000_315_300"; fi
if [ "$WHICH" == "105" ]; then WHICH_SMALL="t6ttww_600_425_50"; fi
if [ "$WHICH" == "106" ]; then WHICH_SMALL="t6ttww_650_150_50"; fi

#data
if [ "$WHICH" == "1000" ]; then WHICH_SMALL="datadoubleegv1"; fi
if [ "$WHICH" == "1001" ]; then WHICH_SMALL="datadoublemuonv1"; fi
if [ "$WHICH" == "1002" ]; then WHICH_SMALL="datamuonegv1"; fi
if [ "$WHICH" == "1003" ]; then WHICH_SMALL="datadoubleegv2"; fi
if [ "$WHICH" == "1004" ]; then WHICH_SMALL="datadoublemuonv2"; fi
if [ "$WHICH" == "1005" ]; then WHICH_SMALL="datamuonegv2"; fi
# if [ "$WHICH" == "1003" ]; then WHICH_SMALL="datadoubleegd"; fi
# if [ "$WHICH" == "1004" ]; then WHICH_SMALL="datadoublemuond"; fi
# if [ "$WHICH" == "1005" ]; then WHICH_SMALL="datamuonegd"; fi
# if [ "$WHICH" == "1006" ]; then WHICH_SMALL="datadoubleegd_v4"; fi
# if [ "$WHICH" == "1007" ]; then WHICH_SMALL="datadoublemuond_v4"; fi
# if [ "$WHICH" == "1008" ]; then WHICH_SMALL="datamuonegd_v4"; fi

#signal
if [ "$WHICH" == "10001" ] ; then WHICH_SMALL="t1tttt_1950_700to950"        ; fi
if [ "$WHICH" == "10002" ] ; then WHICH_SMALL="t1tttt_1100_1to775"          ; fi
if [ "$WHICH" == "10003" ] ; then WHICH_SMALL="t1tttt_1175_950"             ; fi
if [ "$WHICH" == "10004" ] ; then WHICH_SMALL="t1tttt_1275_900to975"        ; fi
if [ "$WHICH" == "10005" ] ; then WHICH_SMALL="t1tttt_1300_1to1075"         ; fi
if [ "$WHICH" == "10006" ] ; then WHICH_SMALL="t1tttt_1300to1325_700to1100" ; fi
if [ "$WHICH" == "10007" ] ; then WHICH_SMALL="t1tttt_1350to1375_50to1025"  ; fi
if [ "$WHICH" == "10008" ] ; then WHICH_SMALL="t1tttt_1500to1525_50to1125"  ; fi
if [ "$WHICH" == "10009" ] ; then WHICH_SMALL="t1tttt_1800to1850_1to1450"   ; fi
if [ "$WHICH" == "10010" ] ; then WHICH_SMALL="t1tttt_1100to1125_700to900"  ; fi
if [ "$WHICH" == "10011" ] ; then WHICH_SMALL="t1tttt_625_275to375"         ; fi
if [ "$WHICH" == "10012" ] ; then WHICH_SMALL="t1tttt_625to650_200to400"    ; fi
if [ "$WHICH" == "10013" ] ; then WHICH_SMALL="t1tttt_650to675_250to425"    ; fi
if [ "$WHICH" == "10014" ] ; then WHICH_SMALL="t1tttt_700to750_200to500"    ; fi
if [ "$WHICH" == "10015" ] ; then WHICH_SMALL="t1tttt_1150_1to800"          ; fi
if [ "$WHICH" == "10016" ] ; then WHICH_SMALL="t1tttt_1150to1175_750to925"  ; fi
if [ "$WHICH" == "10017" ] ; then WHICH_SMALL="t1tttt_1200_1to825"          ; fi
if [ "$WHICH" == "10018" ] ; then WHICH_SMALL="t1tttt_1200to1225_800to1000" ; fi
if [ "$WHICH" == "10019" ] ; then WHICH_SMALL="t1tttt_1250to1275_700to1050" ; fi
if [ "$WHICH" == "10020" ] ; then WHICH_SMALL="t1tttt_1325to1350_1to1125"   ; fi
if [ "$WHICH" == "10021" ] ; then WHICH_SMALL="t1tttt_1400_1to1175"         ; fi
if [ "$WHICH" == "10022" ] ; then WHICH_SMALL="t1tttt_1400to1425_50to1100"  ; fi
if [ "$WHICH" == "10023" ] ; then WHICH_SMALL="t1tttt_1425to1450_1to1200"   ; fi
if [ "$WHICH" == "10024" ] ; then WHICH_SMALL="t1tttt_1450to1475_50to1075"  ; fi
if [ "$WHICH" == "10025" ] ; then WHICH_SMALL="t1tttt_1475to1500_1to1250"   ; fi
if [ "$WHICH" == "10026" ] ; then WHICH_SMALL="t1tttt_1525to1550_1to1300"   ; fi
if [ "$WHICH" == "10027" ] ; then WHICH_SMALL="t1tttt_1600to1650_1to1350"   ; fi
if [ "$WHICH" == "10028" ] ; then WHICH_SMALL="t1tttt_1650to1700_1to1400"   ; fi
if [ "$WHICH" == "10029" ] ; then WHICH_SMALL="t1tttt_1700to1750_1to1450"   ; fi
if [ "$WHICH" == "10030" ] ; then WHICH_SMALL="t1tttt_1750_50to1450"        ; fi
if [ "$WHICH" == "10031" ] ; then WHICH_SMALL="t1tttt_1850to1900_1to1450"   ; fi
if [ "$WHICH" == "10032" ] ; then WHICH_SMALL="t1tttt_1900to1950_0to1450"   ; fi
if [ "$WHICH" == "10033" ] ; then WHICH_SMALL="t1tttt_600_250to325"         ; fi
if [ "$WHICH" == "10034" ] ; then WHICH_SMALL="t1tttt_675_325to450"         ; fi
if [ "$WHICH" == "10035" ] ; then WHICH_SMALL="t1tttt_700_1to450"           ; fi
if [ "$WHICH" == "10036" ] ; then WHICH_SMALL="t1tttt_750to775_350to525"    ; fi
if [ "$WHICH" == "10037" ] ; then WHICH_SMALL="t1tttt_775_475to550"         ; fi
if [ "$WHICH" == "10038" ] ; then WHICH_SMALL="t1tttt_800to825_1to575"      ; fi
if [ "$WHICH" == "10039" ] ; then WHICH_SMALL="t1tttt_825to850_200to600"    ; fi
if [ "$WHICH" == "10040" ] ; then WHICH_SMALL="t1tttt_850to875_450to625"    ; fi
if [ "$WHICH" == "10041" ] ; then WHICH_SMALL="t1tttt_875to900_1to650"      ; fi
if [ "$WHICH" == "10042" ] ; then WHICH_SMALL="t1tttt_950to975_350to725"    ; fi
if [ "$WHICH" == "10043" ] ; then WHICH_SMALL="t1tttt_975_600to750"         ; fi
if [ "$WHICH" == "10044" ] ; then WHICH_SMALL="t1tttt_1375_950to1150"       ; fi
if [ "$WHICH" == "10045" ] ; then WHICH_SMALL="t1tttt_1000to1050_1to800"    ; fi
if [ "$WHICH" == "10046" ] ; then WHICH_SMALL="t1tttt_1000_1to700"          ; fi
if [ "$WHICH" == "10047" ] ; then WHICH_SMALL="t1tttt_1050_50to775"         ; fi  
if [ "$WHICH" == "10048" ] ; then WHICH_SMALL="t1tttt_1050to1075_650to850"  ; fi  
if [ "$WHICH" == "10049" ] ; then WHICH_SMALL="t1tttt_1225to1250_1to1025"   ; fi  
if [ "$WHICH" == "10050" ] ; then WHICH_SMALL="t1tttt_1550to1575_500to1175" ; fi  
if [ "$WHICH" == "10051" ] ; then WHICH_SMALL="t1tttt_600_1to225"           ; fi  
if [ "$WHICH" == "10052" ] ; then WHICH_SMALL="t1tttt_600to625_250to375"    ; fi  
if [ "$WHICH" == "10053" ] ; then WHICH_SMALL="t1tttt_900to950_200to700"    ; fi  
if [ "$WHICH" == "11000" ] ; then WHICH_SMALL="t5qqqqvv_1000to1075_1to950"  ; fi  
if [ "$WHICH" == "11001" ] ; then WHICH_SMALL="t5qqqqvv_1100to1175_1to1050" ; fi  
if [ "$WHICH" == "11002" ] ; then WHICH_SMALL="t5qqqqvv_1200to1275_1to1150" ; fi  
if [ "$WHICH" == "11003" ] ; then WHICH_SMALL="t5qqqqvv_1300to1375_1to1250" ; fi  
if [ "$WHICH" == "11004" ] ; then WHICH_SMALL="t5qqqqvv_1400to1550_1to1275" ; fi  
if [ "$WHICH" == "11005" ] ; then WHICH_SMALL="t5qqqqvv_1600to1750_1to950"  ; fi  
if [ "$WHICH" == "11006" ] ; then WHICH_SMALL="t5qqqqvv_600to675_1to550"    ; fi  
if [ "$WHICH" == "11007" ] ; then WHICH_SMALL="t5qqqqvv_700to775_1to650"    ; fi  
if [ "$WHICH" == "11008" ] ; then WHICH_SMALL="t5qqqqvv_800to975_1to850"    ; fi  
if [ "$WHICH" == "12000" ] ; then WHICH_SMALL="t6ttww_50_300to600_225to575" ; fi  
if [ "$WHICH" == "12001" ] ; then WHICH_SMALL="t6ttww_50_300to600_75to125"  ; fi  
if [ "$WHICH" == "12002" ] ; then WHICH_SMALL="t6ttww_50_350to600_150to400" ; fi  
if [ "$WHICH" == "12003" ] ; then WHICH_SMALL="t6ttww_50_625to950_500to850" ; fi  
if [ "$WHICH" == "12004" ] ; then WHICH_SMALL="t6ttww_50_625to950_550to875" ; fi  
if [ "$WHICH" == "12005" ] ; then WHICH_SMALL="t6ttww_50_625to950_75to125"  ; fi  
if [ "$WHICH" == "12006" ] ; then WHICH_SMALL="t6ttww_50_650to950_150to750" ; fi  
if [ "$WHICH" == "12007" ] ; then WHICH_SMALL="t6ttww_50_300to600_150to500" ; fi  
if [ "$WHICH" == "13001" ] ; then WHICH_SMALL="t5qqqqww_1025to1200_0to1175" ; fi
if [ "$WHICH" == "13002" ] ; then WHICH_SMALL="t5qqqqww_1225to1400_0to1175" ; fi
if [ "$WHICH" == "13003" ] ; then WHICH_SMALL="t5qqqqww_1425to1600_0to1175" ; fi
if [ "$WHICH" == "13004" ] ; then WHICH_SMALL="t5qqqqww_1650to1700_0to1150" ; fi
if [ "$WHICH" == "13005" ] ; then WHICH_SMALL="t5qqqqww_600to800_0to725"    ; fi
if [ "$WHICH" == "13006" ] ; then WHICH_SMALL="t5qqqqww_825to1000_0to925"   ; fi
if [ "$WHICH" == "14001" ] ; then WHICH_SMALL="t1ttbb_1225to1400_0to1050"   ; fi
if [ "$WHICH" == "14002" ] ; then WHICH_SMALL="t1ttbb_1425to1600_0to1250"   ; fi
if [ "$WHICH" == "14003" ] ; then WHICH_SMALL="t1ttbb_1425to1600_1100to1350"; fi
if [ "$WHICH" == "14004" ] ; then WHICH_SMALL="t1ttbb_600to800_275to575"    ; fi
if [ "$WHICH" == "14005" ] ; then WHICH_SMALL="t1ttbb_1225to1400_900to1175" ; fi      
if [ "$WHICH" == "14006" ] ; then WHICH_SMALL="t1ttbb_1650to2000_0to1450"   ; fi        
if [ "$WHICH" == "14007" ] ; then WHICH_SMALL="t1ttbb_825to1000_500to775"   ; fi        
if [ "$WHICH" == "14008" ] ; then WHICH_SMALL="t1ttbb_1650to1750_1350to1450" ; fi  
if [ "$WHICH" == "14009" ] ; then WHICH_SMALL="t1ttbb_825to1000_0to625" ; fi  
if [ "$WHICH" == "14010" ] ; then WHICH_SMALL="t1ttbb_600to800_0to450" ; fi  
if [ "$WHICH" == "14011" ] ; then WHICH_SMALL="t1ttbb_1025to1200_700to975" ; fi
if [ "$WHICH" == "14012" ] ; then WHICH_SMALL="t1ttbb_1025to1200_0to850" ; fi
if [ "$WHICH" == "15001" ] ; then WHICH_SMALL="t5ttttdm175_825to1000_500to725"   ; fi
if [ "$WHICH" == "15002" ] ; then WHICH_SMALL="t5ttttdm175_1025to1200_700to925"  ; fi  
if [ "$WHICH" == "15003" ] ; then WHICH_SMALL="t5ttttdm175_1225to1400_0to1050"   ; fi   
if [ "$WHICH" == "15004" ] ; then WHICH_SMALL="t5ttttdm175_1225to1400_900to1125" ; fi 
if [ "$WHICH" == "15005" ] ; then WHICH_SMALL="t5ttttdm175_1425to1625_1100to1300"; fi
if [ "$WHICH" == "15006" ] ; then WHICH_SMALL="t5ttttdm175_600to800_275to525"    ; fi    
if [ "$WHICH" == "15007" ] ; then WHICH_SMALL="t5ttttdm175_825to1000_0to650"     ; fi      
if [ "$WHICH" == "15008" ] ; then WHICH_SMALL="t5ttttdm175_1425to1600_0to1250"   ; fi
if [ "$WHICH" == "15009" ] ; then WHICH_SMALL="t5ttttdm175_1025to1200_0to850"   ; fi
if [ "$WHICH" == "15010" ] ; then WHICH_SMALL="t5ttttdm175_1625to1700_0to1300"   ; fi
if [ "$WHICH" == "15011" ] ; then WHICH_SMALL="t5ttttdm175_600to800_0to450"   ; fi
if [ "$WHICH" == "16001" ] ; then WHICH_SMALL="t5tttt_degen_1225to1400_1075to1275" ; fi
if [ "$WHICH" == "16002" ] ; then WHICH_SMALL="t5tttt_degen_1425to1600_1275to1375" ; fi
if [ "$WHICH" == "16003" ] ; then WHICH_SMALL="t5tttt_degen_825to1000_0to825" ; fi
if [ "$WHICH" == "16004" ] ; then WHICH_SMALL="t5tttt_degen_825to1000_675to875" ; fi
if [ "$WHICH" == "16005" ] ; then WHICH_SMALL="t5tttt_degen_1225to1400_0to1225" ; fi    
if [ "$WHICH" == "16006" ] ; then WHICH_SMALL="t5tttt_degen_600to800_450to675" ; fi    
if [ "$WHICH" == "16007" ] ; then WHICH_SMALL="t5tttt_degen_1425to1600_0to1350" ; fi    
if [ "$WHICH" == "16008" ] ; then WHICH_SMALL="t5tttt_degen_1650to1700_0to1350" ; fi    
if [ "$WHICH" == "16009" ] ; then WHICH_SMALL="t5tttt_degen_1025to1200_0to1025" ; fi    
if [ "$WHICH" == "16010" ] ; then WHICH_SMALL="t5tttt_degen_600to800_0to625"    ; fi       
if [ "$WHICH" == "16011" ] ; then WHICH_SMALL="t5tttt_degen_1025to1200_875to1075" ; fi 
if [ "$WHICH" == "17001" ] ; then WHICH_SMALL="t5ttcc_825to1000_675to875" ; fi      
if [ "$WHICH" == "17002" ] ; then WHICH_SMALL="t5ttcc_1025to1200_875to1075" ; fi      
if [ "$WHICH" == "17003" ] ; then WHICH_SMALL="t5ttcc_825to1000_0to825" ; fi      
if [ "$WHICH" == "17004" ] ; then WHICH_SMALL="t5ttcc_600to800_450to675" ; fi      
if [ "$WHICH" == "17005" ] ; then WHICH_SMALL="t5ttcc_1225to1400_1075to1225" ; fi      
if [ "$WHICH" == "17006" ] ; then WHICH_SMALL="t5ttcc_1225to1400_0to1225" ; fi      
if [ "$WHICH" == "17007" ] ; then WHICH_SMALL="t5ttcc_1650to1700_0to1350" ; fi      
if [ "$WHICH" == "17008" ] ; then WHICH_SMALL="t5ttcc_1425to1525_1275to1375" ; fi      
if [ "$WHICH" == "17009" ] ; then WHICH_SMALL="t5ttcc_1025to1200_0to1025" ; fi      
if [ "$WHICH" == "17010" ] ; then WHICH_SMALL="t5ttcc_600to800_0to625" ; fi      
if [ "$WHICH" == "17011" ] ; then WHICH_SMALL="t5ttcc_1425to1600_0to1350" ; fi      
if [ "$WHICH" == "100000" ] ; then WHICH_SMALL="ana1"; fi
if [ "$WHICH" == "100001" ] ; then WHICH_SMALL="ana2"; fi
if [ "$WHICH" == "100002" ] ; then WHICH_SMALL="ana3"; fi

if [ "$WHICH" == "101012" ] ; then WHICH_SMALL="dy_high_LO"; fi

export OUTPUT=${WHICH_SMALL}_${FILE}

#This stuff to get output back
export COPYDIR=/hadoop/cms/store/user/$USER/condor/${DIRNAME}

#Untar the zip dir
tar xzvf CORE.tar.gz
tar xzvf btagsf.tar.gz

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
echo "running: ./main.exe $WHICH $FILE"
./main.exe $WHICH $FILE
ls -l `pwd`/${OUTPUT}.root

echo "copying.  LS is: "
ls

deleteMe=`find -name "$OUTPUT.root" -size -delete -1` 
if [ "$deleteMe" != "" ];
then 
  rm $deleteMe 
  echo "THIS BITCH IS EMPTY, DELETING IT!!"
else
  lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/${OUTPUT}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}.root
fi
