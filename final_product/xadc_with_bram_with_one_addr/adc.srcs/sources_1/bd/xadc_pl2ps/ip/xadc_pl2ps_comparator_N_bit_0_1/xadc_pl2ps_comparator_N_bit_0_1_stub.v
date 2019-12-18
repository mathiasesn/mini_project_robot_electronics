// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.1 (win64) Build 2552052 Fri May 24 14:49:42 MDT 2019
// Date        : Sun Dec 15 12:33:12 2019
// Host        : DESKTOP-7FQ3E2M running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               c:/Users/An-ck/Desktop/xadc_with_bram_with_one_addr/adc.srcs/sources_1/bd/xadc_pl2ps/ip/xadc_pl2ps_comparator_N_bit_0_1/xadc_pl2ps_comparator_N_bit_0_1_stub.v
// Design      : xadc_pl2ps_comparator_N_bit_0_1
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z020clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "comparator_N_bit,Vivado 2019.1" *)
module xadc_pl2ps_comparator_N_bit_0_1(count, threshold, OutBit)
/* synthesis syn_black_box black_box_pad_pin="count[12:0],threshold[12:0],OutBit" */;
  input [12:0]count;
  input [12:0]threshold;
  output OutBit;
endmodule