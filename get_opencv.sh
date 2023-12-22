#!/usr/bin/env bash
mkdir third-party-install
cd third-party-install
wget https://github.com/AXERA-TECH/ax-samples/releases/download/v0.1/opencv-arm-linux-gnueabihf-gcc-7.5.0.zip -e use_proxy=on -e https_proxy=192.168.1.3:2333
unzip opencv-arm-linux-gnueabihf-gcc-7.5.0.zip
rm opencv-arm-linux-gnueabihf-gcc-7.5.0.zip
cd ..