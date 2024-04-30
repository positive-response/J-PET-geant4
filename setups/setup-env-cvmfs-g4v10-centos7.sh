#!/bin/bash
################################################
#       SETUP ENVIROMENT WITH CVMFS @ CENTOS 7 #
################################################

# how-to-run:
# source setup-env-cvmfs-centos7.sh

# ===========================================  
LCG=101
ARCH=x86_64-centos7
GCC=gcc11
GEANT4=10.07.p02
# ===========================================

LCGVIEWVERSION=/cvmfs/sft.cern.ch/lcg/views/LCG_${LCG}/${ARCH}-${GCC}-opt
GEANT4VERSION=/cvmfs/sft.cern.ch/lcg/releases/LCG_${LCG}/Geant4/${GEANT4}/${ARCH}-${GCC}-opt

echo "Setup the complete environment for this LCG view"
echo "Config script taken from ${LCGVIEWVERSION}"
source ${LCGVIEWVERSION}/setup.sh
echo

echo "Setup the appropriate Geant4 version"
echo "Geant4 taken from ${GEANT4VERSION}"
source ${GEANT4VERSION}/bin/geant4.sh
echo

export CC=$(which gcc)
export CXX=$(which g++)
which g++
g++ --version
which cmake
cmake --version
echo

#unset unnecessery paths:
export PYTHONHOME=""
export PYTHONPATH=""

echo "//////////////////////////////////////////////////////////////////"
echo "// Your enviroment is ready for J-PET-Geant4 framework !        //"
echo "//////////////////////////////////////////////////////////////////"
echo
