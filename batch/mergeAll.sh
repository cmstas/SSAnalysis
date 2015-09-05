#!/bin/bash

path=/hadoop/cms/store/user/cgeorge/condor/ss_13_babies

for which in 0 # 1
do
  hadd DataDoubleEG_$which.root    $path/datadoubleeg_*_$which.root     
  hadd DataDoubleMuon_$which.root  $path/datadoublemuon_*_$which.root   
  hadd DataMuonEG_$which.root      $path/datamuoneg_*_$which.root   
  hadd DataDoubleEG2_$which.root   $path/datadoubleeg2_*_$which.root     
  hadd DataDoubleMuon2_$which.root $path/datadoublemuon2_*_$which.root   
  hadd DataMuonEG2_$which.root     $path/datamuoneg2_*_$which.root   
  hadd DataDoubleEGC_$which.root   $path/datadoubleegc_*_$which.root     
  hadd DataDoubleMuonC_$which.root $path/datadoublemuonc_*_$which.root   
  hadd DataMuonEGC_$which.root     $path/datamuonegc_*_$which.root   
  hadd DY_high_$which.root         $path/dy_high_*_$which.root          
  hadd DY_low_$which.root          $path/dy_low_*_$which.root           
  hadd TTBAR_$which.root           $path/ttbar_*_$which.root            
  hadd TTPOWHEG_$which.root        $path/ttpowheg_*_$which.root         
  hadd WJets_$which.root           $path/wjets_*_$which.root            
  hadd -f WZ_$which.root           $path/wz_*_$which.root               
  hadd -f TTZL_$which.root         $path/ttzl_*_$which.root
  hadd -f TTZQ_$which.root         $path/ttzq_*_$which.root
  hadd -f TTW_$which.root          $path/ttw_*_$which.root
  hadd -f TTWQQ_$which.root          $path/ttwqq_*_$which.root
  hadd -f WZ3LNU_$which.root       $path/wz3lnu_*_$which.root
done
