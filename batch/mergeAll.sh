#!/bin/bash

path=/hadoop/cms/store/user/${USER}/condor/ss_13_babies/
tag=v6.02-fs
remake=no

function run () {
    echo root -b -q merge.C\\\(\\\"${1}/\\\",\\\"$2\\\"\\\)
    if [ -e $tag/$2 ] && [ "$remake" == "yes" ]
    then 
      echo "$2 already exists, done."
    else 
      nohup nice -n 19 root -b -q merge.C\(\"${1}\",\"$2\"\) >> nohub.txt & 
    fi
}

if [ ! -d /nfs-7/userdata/ss2015/ssBabies/$tag ]; then mkdir -p /nfs-7/userdata/ss2015/ssBabies/$tag; fi

rm nohub.txt

#run $path/datadoubleegc_05oct                     $tag/DataDoubleEGC_05oct.root         
#run $path/datadoublemuonc_05oct                   $tag/DataDoubleMuonC_05oct.root       
#run $path/datamuonegc_05oct                       $tag/DataMuonEGC_05oct.root           
#run $path/datadoubleegd_05oct                     $tag/DataDoubleEGD_05oct.root         
#run $path/datadoublemuond_05oct                   $tag/DataDoubleMuonD_05oct.root       
#run $path/datamuonegd_05oct                       $tag/DataMuonEGD_05oct.root           
#run $path/datadoubleegd_v4                        $tag/DataDoubleEGD_v4.root            
#run $path/datadoublemuond_v4                      $tag/DataDoubleMuonD_v4.root          
#run $path/datamuonegd_v4                          $tag/DataMuonEGD_v4.root              
#
#run $path/dy_high                                 $tag/DY_high.root                     
#run $path/dy_low                                  $tag/DY_low.root                      
#run $path/ttbar_[0-9]*.root                       $tag/TTBAR.root                       
#run $path/wwz                                     $tag/WWZ.root                         
#run $path/zz                                      $tag/ZZ.root                          
#run $path/wjets_[0-9]*.root                       $tag/WJets.root                       
#run $path/ttzl                                    $tag/TTZL.root                        
#run $path/ttzq                                    $tag/TTZQ.root                        
#run $path/ttw                                     $tag/TTW.root                         
#run $path/ttwqq                                   $tag/TTWQQ.root                       
#run $path/wz                                      $tag/WZ.root                          
#run $path/wgtolnug                                $tag/WGToLNuG.root                    
#run $path/ttg                                     $tag/TTG.root                         
#run $path/tthtononbb                              $tag/TTHtoNonBB.root                  
#run $path/tthtononbbext                           $tag/TTHtoNonBBext.root               
#run $path/vhtononbb                               $tag/VHtoNonBB.root                   
#run $path/tzq                                     $tag/TZQ.root                         
#run $path/tttt                                    $tag/TTTT.root                        
#run $path/ttbar_ph                                $tag/TTBAR_PH.root                    
#run $path/wwdps                                   $tag/WWDPS.root                       
#run $path/wzz                                     $tag/WZZ.root                         
#run $path/t6ttww_600_425_50                       $tag/T6TTWW_600_425_50.root           
#run $path/t6ttww_650_150_50                       $tag/T6TTWW_650_150_50.root           
#run $path/t1tttt_1200                             $tag/T1TTTT_1200.root                 
#run $path/t1tttt_1500                             $tag/T1TTTT_1500.root                 
#run $path/singletop1                              $tag/SINGLETOP1.root                  
#run $path/singletop2                              $tag/SINGLETOP2.root                  
#run $path/singletop3                              $tag/SINGLETOP3.root                  
#run $path/singletop4                              $tag/SINGLETOP4.root                  
#run $path/singletop5                              $tag/SINGLETOP5.root                  
#run $path/qqww                                    $tag/QQWW.root                        
#run $path/tg                                      $tag/TG.root                          
#run $path/zg                                      $tag/ZG.root                          
#run $path/t5qqqqww_1200_1000_800_[1-9]*.root      $tag/T5qqqqWW_1200_1000_800.root      
#run $path/t5ttttdeg_1000_300_285_280_[1-9]*.root  $tag/T5ttttDeg_1000_300_285_280.root  
#run $path/t5qqqqwwdeg_1000_315_300_[1-9]*.root    $tag/T5qqqqWWDeg_1000_315_300.root    
#run $path/ttzlow                                  $tag/TTZLOW.root                      
#run $path/gghtozzto4l                             $tag/GGHtoZZto4L.root                 
#run $path/wzmg                                    $tag/WZMG.root                        
#run $path/wgmg                                    $tag/WGMG.root                        
#
#run $path/t1tttt_1100_1to775                    $tag/T1TTTT_1100_1to775.root          
#run $path/t1tttt_1175_950                       $tag/T1TTTT_1175_950.root             
#run $path/t1tttt_1275_900to975                  $tag/T1TTTT_1275_900to975.root        
#run $path/t1tttt_1300_1to1075                   $tag/T1TTTT_1300_1to1075.root         
#run $path/t1tttt_1300to1325_700to1100           $tag/T1TTTT_1300to1325_700to1100.root 
#run $path/t1tttt_1350to1375_50to1025            $tag/T1TTTT_1350to1375_50to1025.root  
#run $path/t1tttt_1500to1525_50to1125            $tag/T1TTTT_1500to1525_50to1125.root  
#run $path/t1tttt_1800to1850_1to1450             $tag/T1TTTT_1800to1850_1to1450.root   
#run $path/t1tttt_625_275to375                   $tag/T1TTTT_625_275to375.root         
#run $path/t1tttt_625to650_200to400              $tag/T1TTTT_625to650_200to400.root    
#run $path/t1tttt_650to675_250to425              $tag/T1TTTT_650to675_250to425.root    
#run $path/t1tttt_1100to1125_700to900            $tag/T1TTTT_1100to1125_700to900.root  
#run $path/t1tttt_700to750_200to500              $tag/T1TTTT_700to750_200to500.root    
#run $path/t1tttt_1150_1to800                    $tag/T1TTTT_1150_1to800.root          
#run $path/t1tttt_1150to1175_750to925            $tag/T1TTTT_1150to1175_750to925.root  
#run $path/t1tttt_1200_1to825                    $tag/T1TTTT_1200_1to825.root          
#run $path/t1tttt_1200to1225_800to1000           $tag/T1TTTT_1200to1225_800to1000.root 
#run $path/t1tttt_1250to1275_700to1050           $tag/T1TTTT_1250to1275_700to1050.root 
#run $path/t1tttt_1325to1350_1to1125             $tag/T1TTTT_1325to1350_1to1125.root   
#run $path/t1tttt_1400_1to1175                   $tag/T1TTTT_1400_1to1175.root         
#run $path/t1tttt_1400to1425_50to1100            $tag/T1TTTT_1400to1425_50to1100.root  
#run $path/t1tttt_1425to1450_1to1200             $tag/T1TTTT_1425to1450_1to1200.root   
#run $path/t1tttt_1450to1475_50to1075            $tag/T1TTTT_1450to1475_50to1075.root  
#run $path/t1tttt_1475to1500_1to1250             $tag/T1TTTT_1475to1500_1to1250.root   
#run $path/t1tttt_1525to1550_1to1300             $tag/T1TTTT_1525to1550_1to1300.root   
#run $path/t1tttt_1600to1650_1to1350             $tag/T1TTTT_1600to1650_1to1350.root   
#run $path/t1tttt_1650to1700_1to1400             $tag/T1TTTT_1650to1700_1to1400.root   
#run $path/t1tttt_1700to1750_1to1450             $tag/T1TTTT_1700to1750_1to1450.root   
#run $path/t1tttt_1750_50to1450                  $tag/T1TTTT_1750_50to1450.root        
#run $path/t1tttt_1850to1900_1to1450             $tag/T1TTTT_1850to1900_1to1450.root   
#run $path/t1tttt_1900to1950_0to1450             $tag/T1TTTT_1900to1950_0to1450.root   
#run $path/t1tttt_600_250to325                   $tag/T1TTTT_600_250to325.root         
#run $path/t1tttt_675_325to450                   $tag/T1TTTT_675_325to450.root         
#run $path/t1tttt_700_1to450                     $tag/T1TTTT_700_1to450.root           
#run $path/t1tttt_750to775_350to525              $tag/T1TTTT_750to775_350to525.root    
#run $path/t1tttt_775_475to550                   $tag/T1TTTT_775_475to550.root         
#run $path/t1tttt_800to825_1to575                $tag/T1TTTT_800to825_1to575.root      
#run $path/t1tttt_825to850_200to600              $tag/T1TTTT_825to850_200to600.root    
#run $path/t1tttt_850to875_450to625              $tag/T1TTTT_850to875_450to625.root    
#run $path/t1tttt_875to900_1to650                $tag/T1TTTT_875to900_1to650.root      
#run $path/t1tttt_950to975_350to725              $tag/T1TTTT_950to975_350to725.root    
#run $path/t1tttt_975_600to750                   $tag/T1TTTT_975_600to750.root         
#run $path/t1tttt_1375_950to1150                 $tag/T1TTTT_1375_950to1150.root       
#run $path/t1tttt_1400_1to1175                   $tag/T1TTTT_1400_1to1175.root         
#run $path/t1tttt_1000to1050_1to800              $tag/T1TTTT_1000to1050_1to800.root    
#run $path/t1tttt_1000_1to700                    $tag/T1TTTT_1000_1to700.root          
#run $path/t1tttt_1050_50to775                   $tag/T1TTTT_1050_50to775.root         
#run $path/t1tttt_1050to1075_650to850            $tag/T1TTTT_1050to1075_650to850.root  
#run $path/t1tttt_1225to1250_1to1025             $tag/T1TTTT_1225to1250_1to1025.root   
#run $path/t1tttt_1550to1575_500to1175           $tag/T1TTTT_1550to1575_500to1175.root 
#run $path/t1tttt_600_1to225                     $tag/T1TTTT_600_1to225.root           
#run $path/t1tttt_600to625_250to375              $tag/T1TTTT_600to625_250to375.root    
#run $path/t1tttt_900to950_200to700              $tag/T1TTTT_900to950_200to700.root    
#run $path/t1tttt_975_600to750                   $tag/T1TTTT_975_600to750.root         

run $path/t5qqqqvv_1000to1075_1to950   $tag/T5QQQQVV_1000to1075_1to950.root  
run $path/t5qqqqvv_1100to1175_1to1050  $tag/T5QQQQVV_1100to1175_1to1050.root 
run $path/t5qqqqvv_1200to1275_1to1150  $tag/T5QQQQVV_1200to1275_1to1150.root 
run $path/t5qqqqvv_1300to1375_1to1250  $tag/T5QQQQVV_1300to1375_1to1250.root 
run $path/t5qqqqvv_1400to1550_1to1275  $tag/T5QQQQVV_1400to1550_1to1275.root 
run $path/t5qqqqvv_1600to1750_1to950   $tag/T5QQQQVV_1600to1750_1to950.root  
run $path/t5qqqqvv_600to675_1to550     $tag/T5QQQQVV_600to675_1to550.root    
run $path/t5qqqqvv_700to775_1to650     $tag/T5QQQQVV_700to775_1to650.root    
run $path/t5qqqqvv_800to975_1to850     $tag/T5QQQQVV_800to975_1to850.root    

run $path/t6ttww_50_300to600_225to575  $tag/T6TTWW_50_300to600_225to575.root 
run $path/t6ttww_50_300to600_75to125   $tag/T6TTWW_50_300to600_75to125.root  
run $path/t6ttww_50_350to600_150to400  $tag/T6TTWW_50_350to600_150to400.root 
run $path/t6ttww_50_625to950_500to850  $tag/T6TTWW_50_625to950_500to850.root 
run $path/t6ttww_50_625to950_550to875  $tag/T6TTWW_50_625to950_550to875.root 
run $path/t6ttww_50_625to950_75to125   $tag/T6TTWW_50_625to950_75to125.root  
run $path/t6ttww_50_650to950_150to750  $tag/T6TTWW_50_650to950_150to750.root 
run $path/t6ttww_50_300to600_150to500  $tag/T6TTWW_50_300to600_150to500.root 
