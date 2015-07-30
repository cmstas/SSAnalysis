#!/bin/bash

path=/hadoop/cms/store/user/cgeorge/condor/ss_13_babies

for which in 0 1
do
  hadd DataDoubleEG_$which.root    $path/datadoubleeg_*_$which.root     
  hadd DataDoubleMuon_$which.root  $path/datadoublemuon_*_$which.root   
  hadd DataMuonEG_$which.root      $path/datamueg_*_$which.root   
  hadd DY_high_$which.root         $path/dy_high_*_$which.root          
  hadd DY_low_$which.root          $path/dy_low_*_$which.root           
  hadd TTBAR_$which.root           $path/ttbar_*_$which.root            
  hadd TTPOWHEG_$which.root        $path/ttpowheg_*_$which.root         
  hadd WJets_$which.root           $path/wjets_*_$which.root            
  hadd WZ_$which.root              $path/wz_*_$which.root               
done
