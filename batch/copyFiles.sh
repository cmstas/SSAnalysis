#!/bin/bash

#Copy files
cp ../helper_babymaker.cc .
cp ../helper_babymaker.h . 

#Make and fill CORE directory
if [ ! -d CORE ] 
then
  mkdir CORE 
fi
cp ../CORE/*.h CORE/
cp ../CORE/*.cc CORE/

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
cp -r ../CORE/Tools               CORE/

#And data directory inside jetcorr
if [ ! -d CORE/Tools/jetcorr/data/run2_50ns ] 
then
  mkdir -p CORE/Tools/jetcorr/data/run2_50ns
fi
cp ../CORE/Tools/jetcorr/data/run2_50ns/*.txt CORE/Tools/jetcorr/data/run2_50ns/

#And goodrun list directory 
if [ ! -d goodRunList ] 
then
  mkdir goodRunList
fi
cp ../goodRunList/*.txt        goodRunList/

#Linkdef
cp ../LinkDef.h . 

#Make log dir
if [ ! -d logs ] 
then
  mkdir logs
fi

#Sed commands to get babymaker to work
sed -i "s,\"Tools\/,\"CORE/Tools\/,g" helper_babymaker.h 
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.h
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.cc
sed -i "s/output_name,\ usePtRel\ ?\ \"_ptRel\"\ :\ \"\"/output_name/" helper_babymaker.cc
sed -i "/<vector>/a #include\ \"Math/Vector4D.h\" \n#include\ \"Math/LorentzVector.h\" \n\n\#ifdef\ __MAKECINT__\n\#pragma\ link\ C++\ class\ ROOT::Math::PxPyPzE4D<float>+;\n\#pragma\ link\ C++\ class\ ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>\ >+;\n\#pragma\ link\ C++\ typedef ROOT::Math::XYZTVectorF;\n\#endif" helper_babymaker.h
sed -i "s/USERNAME/$USER/g" condorExecutable.sh

#Zip the dir
tar czf CORE.tar.gz CORE
