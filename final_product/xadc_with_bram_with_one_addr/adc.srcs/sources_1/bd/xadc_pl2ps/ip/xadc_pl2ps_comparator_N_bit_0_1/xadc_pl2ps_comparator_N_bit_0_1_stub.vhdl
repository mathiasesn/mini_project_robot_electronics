-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.1 (win64) Build 2552052 Fri May 24 14:49:42 MDT 2019
-- Date        : Sun Dec 15 12:33:12 2019
-- Host        : DESKTOP-7FQ3E2M running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               c:/Users/An-ck/Desktop/xadc_with_bram_with_one_addr/adc.srcs/sources_1/bd/xadc_pl2ps/ip/xadc_pl2ps_comparator_N_bit_0_1/xadc_pl2ps_comparator_N_bit_0_1_stub.vhdl
-- Design      : xadc_pl2ps_comparator_N_bit_0_1
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity xadc_pl2ps_comparator_N_bit_0_1 is
  Port ( 
    count : in STD_LOGIC_VECTOR ( 12 downto 0 );
    threshold : in STD_LOGIC_VECTOR ( 12 downto 0 );
    OutBit : out STD_LOGIC
  );

end xadc_pl2ps_comparator_N_bit_0_1;

architecture stub of xadc_pl2ps_comparator_N_bit_0_1 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "count[12:0],threshold[12:0],OutBit";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "comparator_N_bit,Vivado 2019.1";
begin
end;
