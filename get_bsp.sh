#!/usr/bin/env bash
wget https://github.com/AXERA-TECH/ax-samples/releases/download/v0.3/arm_axpi_r1.22.2801.zip  -e use_proxy=on -e https_proxy=192.168.1.3:2333
unzip arm_axpi_r1.22.2801.zip -d third-party-install/ax_bsp
rm arm_axpi_r1.22.2801.zip 