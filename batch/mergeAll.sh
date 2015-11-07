#!/bin/bash

path=/hadoop/cms/store/user/${USER}/condor/ss_13_babies

hadd DataDoubleEGC_05oct.root         $path/datadoubleegc_05oct_*_0.root     
hadd DataDoubleMuonC_05oct.root       $path/datadoublemuonc_05oct_*_0.root   
hadd DataMuonEGC_05oct.root           $path/datamuonegc_05oct_*_0.root   
hadd DataDoubleEGD_05oct.root         $path/datadoubleegd_05oct_*_0.root     
hadd DataDoubleMuonD_05oct.root       $path/datadoublemuond_05oct_*_0.root   
hadd DataMuonEGD_05oct.root           $path/datamuonegd_05oct_*_0.root   
hadd DataDoubleEGD_v4.root            $path/datadoubleegd_v4_*_0.root     
hadd DataDoubleMuonD_v4.root          $path/datadoublemuond_v4_*_0.root   
hadd DataMuonEGD_v4.root              $path/datamuonegd_v4_*_0.root   
hadd DY_high.root                     $path/dy_high_*_0.root          
hadd DY_low.root                      $path/dy_low_*_0.root           
hadd TTBAR.root                       $path/ttbar_[0-9]*_0.root            
hadd WWZ.root                         $path/wwz_*_0.root            
hadd ZZ.root                          $path/zz_*_0.root
hadd WJets.root                       $path/wjets_[0-9]*_0.root            
hadd WJets_LO.root                    $path/wjets_lo_[0-9]*_0.root            
hadd TTZL.root                        $path/ttzl_*_0.root
hadd TTZQ.root                        $path/ttzq_*_0.root
hadd TTW.root                         $path/ttw_*_0.root
hadd TTWQQ.root                       $path/ttwqq_*_0.root
hadd WZ3LNU.root                      $path/wz3lnu_*_0.root
hadd WGToLNuG.root                    $path/wgtolnug_*_0.root                        
hadd TTG.root                         $path/ttg_*_0.root 
hadd TTHtoNonBB.root                  $path/tthtononbb_*_0.root 
hadd VHtoNonBB.root                   $path/vhtononbb_*_0.root 
hadd TZQ.root                         $path/tzq_*_0.root 
hadd TTTT.root                        $path/tttt_*_0.root 
hadd TTBAR_PH.root                       $path/ttbar_ph_*_0.root 
hadd WWDPS.root                       $path/wwdps_*_0.root 
hadd WZZ.root                         $path/wzz_*_0.root 
hadd T6TTWW_600_425_50.root           $path/t6ttww_600_425_50_*_0.root 
hadd T6TTWW_650_150_50.root           $path/t6ttww_650_150_50_*_0.root 
hadd T1TTTT_1200.root                 $path/t1tttt_1200_*_0.root
hadd T1TTTT_1500.root                 $path/t1tttt_1500_*_0.root
hadd SINGLETOP1.root                  $path/singletop1_*_0.root
hadd SINGLETOP2.root                  $path/singletop2_*_0.root
hadd SINGLETOP3.root                  $path/singletop3_*_0.root
hadd SINGLETOP4.root                  $path/singletop4_*_0.root
hadd SINGLETOP5.root                  $path/singletop5_*_0.root
hadd QQWW.root                        $path/qqww_*_0.root
hadd TG.root                          $path/tg_*_0.root
hadd ZG.root                          $path/zg_*_0.root
hadd T5qqqqWW_1200_1000_800.root       $path/t5qqqqww_1200_1000_800_[1-9]*_0.root  
hadd T5ttttDeg_1000_300_285_280.root   $path/t5ttttdeg_1000_300_285_280_[1-9]*_0.root
hadd T5qqqqWWDeg_1000_315_300.root     $path/t5qqqqwwdeg_1000_315_300_[1-9]*_0.root
