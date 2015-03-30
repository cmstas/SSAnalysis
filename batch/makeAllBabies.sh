#!/bin/bash

while (( "1" == "1" )); do . submitAll.sh; if [ "$?" == "1" ]; then break; fi; done
