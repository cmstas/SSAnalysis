#!/bin/bash

#Copy files
cp ../helper_babymaker.cc .
cp ../helper_babymaker.h . 

#Make and fill CORE directory
if [ ! -d CORE ] 
then
  mkdir CORE 
fi
cp /home/users/cgeorge/CORE/*.h CORE/
cp /home/users/cgeorge/CORE/CMS3_CORE.so CORE/

#And tools directory inside CORE
if [ ! -d CORE/Tools ] 
then
  mkdir CORE/Tools
fi
cp /home/users/cgeorge/CORE/Tools/utils.h CORE/Tools/
cp /home/users/cgeorge/CORE/Tools/utils.cc CORE/Tools/

#And MT2 directory inside CORE
if [ ! -d CORE/Tools/MT2 ] 
then
  mkdir CORE/Tools/MT2
fi
cp /home/users/cgeorge/CORE/Tools/MT2/MT2.h CORE/Tools/MT2
cp /home/users/cgeorge/CORE/Tools/MT2/MT2.cc CORE/Tools/MT2
cp /home/users/cgeorge/CORE/Tools/MT2/MT2Utility.h CORE/Tools/MT2
cp /home/users/cgeorge/CORE/Tools/MT2/MT2Utility.cc CORE/Tools/MT2

#Sed commands to get babymaker to work
sed -i "s,\"Tools\/,\"CORE/Tools\/,g" helper_babymaker.h 
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.h
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.cc
sed -i "s/%s\/%s%s/%s\/%s/g" helper_babymaker.cc
sed -i "s/output_name,\ usePtRel\ ?\ \"_ptRel\"\ :\ \"\"/output_name/" helper_babymaker.cc

#Zip the dir
tar czf CORE.tar.gz CORE
