#!/bin/bash
# Dependencies for Interactive Videos Addon

cd ../../../addons

if [ -z $1 ]; then
PREFIX="git clone https://github.com/"
else
PREFIX="git clone git@github.com:"
fi

${PREFIX}morethanlogic/ofxMtlMapping2D
${PREFIX}rezaali/ofxUI.git




