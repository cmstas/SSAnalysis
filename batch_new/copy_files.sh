#!/bin/bash

echo "FIXME"
# #Copy files
# cp ../helper_babymaker.{h,cc} .

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

#Zip the dir
tar czf CORE.tar.gz CORE
