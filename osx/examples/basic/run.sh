#!/bin/bash

#default parameters of shell script will precompile headers, make a static lib, compile example, and then run the example. See aluminum/linux/run.command and the scripts in aluminum/linux/scripts/ for details

#you will probably have to edit aluminum/linux/run.command to point to the include files associated with GLM

../../run.command Basic.mm
