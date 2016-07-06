#!/bin/bash

#Copy files
cp ../helper_babymaker.{h,cc} .

#Make and fill CORE directory
mkdir -p CORE 
cp ../CORE/*.{h,cc} CORE/

#And data directory inside CORE
mkdir -p CORE/data
cp ../CORE/data/*.xml CORE/data/

#And tools directory inside CORE
mkdir -p CORE/Tools
cp -r ../CORE/Tools CORE/

mkdir -p CORE/Tools/btagsf

#And goodrun list directory 
mkdir -p goodRunList
cp ../goodRunList/*.txt goodRunList/

#And btagsSF dir
cp -r ../btagsf . 

tar -czf btagsf.tar.gz btagsf

#Linkdef
cp ../LinkDef.h . 

#Make log dir
mkdir -p logs

#Sed commands to get babymaker to work
sed -i "s,\"Tools\/,\"CORE/Tools\/,g" helper_babymaker.h 
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.h
sed -i "s/output_name,\ bool\ usePtRel)/output_name)/" helper_babymaker.cc
sed -i "s/output_name,\ usePtRel\ ?\ \"_ptRel\"\ :\ \"\"/output_name/" helper_babymaker.cc
sed -i "/<vector>/a #include\ \"Math/Vector4D.h\" \n#include\ \"Math/LorentzVector.h\" \n\n\#ifdef\ __MAKECINT__\n\#pragma\ link\ C++\ class\ ROOT::Math::PxPyPzE4D<float>+;\n\#pragma\ link\ C++\ class\ ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>\ >+;\n\#pragma\ link\ C++\ typedef ROOT::Math::XYZTVectorF;\n\#endif" helper_babymaker.h
sed -i "s/USERNAME/$USER/g" condorExecutable.sh

#Zip the dir
tar czf CORE.tar.gz CORE
