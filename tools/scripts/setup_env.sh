#!/bin/bash

# Activate the conda environment to use
# the python tests
source activate admray_env

# Export the root variable of the projetct
# Used to locate the executable and tests
export ADMRAY_PATH=/home/adrianno/Documents/Projects/AlphaDataRayCaster

#Exporting variable declaring the ambient is already set
export ADMRAY_READY=true
