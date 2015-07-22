#!/bin/bash

#Copy files
#cp ../helper_babymaker.cc .
#cp ../helper_babymaker.h . 

#Make and fill CORE directory
if [ ! -d CORE ] 
then
  mkdir CORE 
fi
cp ../CORE/*.h CORE/
cp ../CORE/CMS3_CORE.so CORE/

#And data directory inside CORE
if [ ! -d CORE/data ] 
then
  mkdir CORE/data
fi
cp ../CORE/data/*.xml CORE/data/

#And tools directory inside CORE
if [ ! -d CORE/Tools ] 
then
  mkdir CORE/Tools
fi
cp ../Tools/utils.h           CORE/Tools/
cp ../Tools/utils.cc          CORE/Tools/
cp ../Tools/goodrun.cc        CORE/Tools/
cp ../Tools/goodrun.h         CORE/Tools/
cp ../Tools/libTools.so       CORE/Tools/
cp ../Tools/JetCorrector.h    CORE/Tools/

#And MT2 directory inside CORE
if [ ! -d CORE/Tools/MT2 ] 
then
  mkdir CORE/Tools/MT2
fi
cp ../Tools/MT2/MT2.h         CORE/Tools/MT2
cp ../Tools/MT2/MT2.cc        CORE/Tools/MT2
cp ../Tools/MT2/MT2Utility.h  CORE/Tools/MT2
cp ../Tools/MT2/MT2Utility.cc CORE/Tools/MT2

#And goodrun list directory 
if [ ! -d goodRunList ] 
then
  mkdir goodRunList
fi
cp ../goodRunList/json_*.txt goodRunList/

#Make log dir
if [ ! -d logs ] 
then
  mkdir logs
fi

#Sed commands to get babymaker to work
sed -i "s,\"Tools\/,\"CORE/Tools\/,g" helper_babymaker.h 
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.h
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.cc
#sed -i "s/%s\/%s%s/%s\/%s/g" helper_babymaker.cc
sed -i "s/output_name,\ usePtRel\ ?\ \"_ptRel\"\ :\ \"\"/output_name/" helper_babymaker.cc
sed -i "/<vector>/a #include\ \"Math/Vector4D.h\" \n#include\ \"Math/LorentzVector.h\" \n\n\#ifdef\ __MAKECINT__\n\#pragma\ link\ C++\ class\ ROOT::Math::PxPyPzE4D<float>+;\n\#pragma\ link\ C++\ class\ ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>\ >+;\n\#pragma\ link\ C++\ typedef ROOT::Math::XYZTVectorF;\n\#endif" helper_babymaker.h
sed -i "s/USERNAME/$USER/g" condorExecutable.sh

#Zip the dir
tar czf CORE.tar.gz CORE
