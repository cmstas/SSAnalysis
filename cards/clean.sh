#!/bin/bash

for f in *.*~; do
    if [ -e $f ] 
    then rm *.*~
	break
    fi
done

for f in roostats-*; do
    if [ -e $f ] 
    then rm roostats-*
	break
    fi
done

if [ -e higgsCombineTest.Asymptotic.mH120.root ] 
then rm higgsCombineTest.Asymptotic.mH120.root
fi

if [ -e higgsCombineTest.ProfileLikelihood.mH120.root ] 
then rm higgsCombineTest.ProfileLikelihood.mH120.root
fi
