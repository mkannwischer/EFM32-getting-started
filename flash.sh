#!/bin/sh

if [ $# -eq 0 ]
then
    BINARY=efm32-test.bin
else
    BINARY=$1
fi

sed "s/BINARY/${BINARY}/" < flash.jlink > build/flash.jlink

JLinkExe -device EFM32GG11B820F2048GL192 -speed 4000 -if SWD -CommanderScript build/flash.jlink