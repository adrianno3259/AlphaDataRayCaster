#!/bin/bash

export ADMRAY_READY=true
#echo $ADMRAY_PATH
# check if ADMRAY_PATH is defined in the environment
if [ "$ADMRAY_READY" == "true" ]; then
	echo 'Ambient configured.'
	echo 'Skipping env setup step'
else
	echo 'Ambient not configured...'
	echo 'Setting environment variables and activating conda environment'
	source setup_env.sh
fi

tools/premake/premake5 gmake

make config=release

