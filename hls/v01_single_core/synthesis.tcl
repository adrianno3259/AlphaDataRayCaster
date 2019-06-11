############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project v01_single_core
set_top intersectFPGA
add_files main.h
add_files main.c
add_files testbench.c
open_solution "xcku060"
set_part {xcku060-ffva1156-2-e} -tool vivado
create_clock -period 1 -name default
#csim_design -compiler gcc
csynth_design
#cosim_design -compiler gcc -tool xsim
export_design -flow impl -rtl vhdl -format ip_catalog
