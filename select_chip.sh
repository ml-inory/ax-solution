#!/bin/bash
if [ $# < 1]; then
    echo "Please pass parameter chip type"
    exit 1
fi

CHIP_TYPE=$1