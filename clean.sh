#!/bin/bash

if [ -e *.*~ ] 
then rm *.*~
fi

if [ -e roostats-* ] 
then rm roostats*
fi

if [ -e higgsCombineTest.Asymptotic.mH120.root ] 
then rm higgsCombineTest.Asymptotic.mH120.root
fi
