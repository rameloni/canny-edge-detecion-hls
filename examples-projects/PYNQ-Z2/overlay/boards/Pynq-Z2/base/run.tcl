# Run original scripts
source build_base_ip.tcl
source base.tcl

# Delete unused components
delete_bd_objs [get_bd_intf_nets ps7_0_axi_periph1_M00_AXI] [get_bd_intf_nets microblaze_0_debug] [get_bd_intf_nets S00_AXI_2] [get_bd_intf_nets iop_pmoda_io] [get_bd_intf_nets mdm_1_MBDEBUG_1] [get_bd_intf_nets S01_AXI_2] [get_bd_intf_nets ps7_0_axi_periph1_M01_AXI] [get_bd_intf_nets S03_AXI_1] [get_bd_intf_nets iop_rpi_io] [get_bd_intf_nets mdm_1_MBDEBUG_3] [get_bd_intf_nets ps7_0_axi_periph1_M03_AXI] [get_bd_nets mb_1_reset_Dout] [get_bd_nets iop1_intr_ack_Dout] [get_bd_nets iop2_intr_ack_1] [get_bd_nets S01_ARESETN_1] [get_bd_nets mb_iop4_reset_Dout] [get_bd_nets S03_ARESETN_1] [get_bd_nets iop1_iop1_intr_req] [get_bd_nets S00_ARESETN_1] [get_bd_nets mb_2_reset_Dout] [get_bd_nets iop2_iop2_intr_req] [get_bd_nets mb_iop4_intr_ack_Dout] [get_bd_nets iop4_mb4_intr_req] [get_bd_cells iop_pmoda] [get_bd_cells iop_pmodb] [get_bd_cells iop_rpi]
delete_bd_objs [get_bd_intf_nets mdm_1_MBDEBUG_2] [get_bd_intf_nets iop_pmodb_pmodb_gpio] [get_bd_nets ps7_0_GPIO_O] [get_bd_nets wire_distribution_network_dout] [get_bd_nets pmoda_rp_pin_sel_Dout] [get_bd_nets iop3_mb3_intr_req] [get_bd_nets iop_interrupts_dout] [get_bd_nets clk_wiz_10MHz_clk_out1] [get_bd_nets interface_slice_0_gpio_i] [get_bd_nets mb_3_reset_Dout] [get_bd_nets wire_distribution_network_wire_o_t] [get_bd_nets concat_rp_dout] [get_bd_nets mb3_intr_ack_1] [get_bd_nets mdm_1_debug_sys_rst] [get_bd_nets interface_slice_0_gpio_t] [get_bd_cells mb_iop_pmoda_intr_ack] [get_bd_cells mb_iop_pmodb_intr_ack] [get_bd_cells concat_rp] [get_bd_cells mb_iop_arduino_intr_ack] [get_bd_cells pmoda_rp_pin_sel] [get_bd_cells iop_interrupts] [get_bd_cells mb_iop_rpi_reset] [get_bd_cells clk_wiz_10MHz] [get_bd_cells mb_iop_arduino_reset] [get_bd_cells mdm_1] [get_bd_cells mb_iop_pmoda_reset] [get_bd_cells mb_iop_rpi_intr_ack] [get_bd_cells mb_iop_pmodb_reset] [get_bd_cells constant_8bit_0] [get_bd_cells slice_pmodb_gpio]
delete_bd_objs [get_bd_intf_nets Vaux6_1] [get_bd_intf_nets ps7_0_axi_periph1_M02_AXI] [get_bd_intf_nets ps7_0_M_AXI_GP1] [get_bd_intf_nets iop_arduino_arduino_gpio1] [get_bd_intf_nets ps7_0_axi_periph_1_M00_AXI] [get_bd_intf_nets ps7_0_axi_periph_1_M01_AXI] [get_bd_intf_nets collector_pmoda_rpi_gpio_output] [get_bd_intf_nets axi_interconnect_0_M00_AXI] [get_bd_intf_nets distributor_rpi_27_8_gpio_output] [get_bd_intf_nets S02_AXI_1] [get_bd_intf_nets S_AXI_1] [get_bd_intf_nets Vaux1_1] [get_bd_intf_nets Vaux5_1] [get_bd_intf_nets axi_mem_intercon_M00_AXI1] [get_bd_intf_nets Vaux9_1] [get_bd_intf_nets S_AXI_LITE_1] [get_bd_intf_nets Vaux13_1] [get_bd_intf_nets Vaux15_1] [get_bd_intf_nets Vp_Vn_1] [get_bd_intf_nets iop_arduino_IIC_0] [get_bd_intf_nets iop3_spi_sw_shield] [get_bd_nets rst_ps7_0_fclk3_peripheral_aresetn] [get_bd_nets logic_1_dout] [get_bd_nets concat_pmodb_dout] [get_bd_nets trace_analyzer_pmodb_s2mm_introut] [get_bd_intf_nets ps7_0_axi_periph_M06_AXI] [get_bd_nets S02_ARESETN_1] [get_bd_intf_nets trace_analyzer_pi_M_AXI] [get_bd_nets trace_analyzer_pi_s2mm_introut] [get_bd_intf_nets trace_analyzer_pmodb_M_AXI_S2MM] [get_bd_nets rst_ps7_0_fclk3_interconnect_aresetn] [get_bd_cells ps7_0_axi_periph1] [get_bd_cells wire_distribution_network] [get_bd_cells iop_arduino] [get_bd_cells trace_analyzer_pi] [get_bd_cells axi_mem_intercon] [get_bd_cells ps7_0_axi_periph_1] [get_bd_cells axi_interconnect_0] [get_bd_cells trace_analyzer_pmodb]
delete_bd_objs [get_bd_intf_ports arduino_gpio] [get_bd_intf_ports arduino_direct_spi] [get_bd_intf_ports Vaux1] [get_bd_intf_ports arduino_direct_iic] [get_bd_intf_ports Vaux9] [get_bd_intf_ports Vaux15] [get_bd_intf_ports pmoda_rpi_gpio] [get_bd_intf_ports Vaux13] [get_bd_intf_ports rpi_gpio] [get_bd_intf_ports Vaux6] [get_bd_intf_ports Vaux5] [get_bd_intf_ports Vp_Vn]
delete_bd_objs [get_bd_ports audio_clk_10MHz]
delete_bd_objs [get_bd_nets constant_8bit_0_dout] [get_bd_cells logic_1] [get_bd_cells concat_pmodb] [get_bd_cells rst_ps7_0_fclk3]
delete_bd_objs [get_bd_intf_ports pmodb_gpio]

# Configure PS
startgroup
set_property -dict [list CONFIG.PCW_USE_M_AXI_GP1 {0} CONFIG.PCW_USE_S_AXI_GP0 {0} CONFIG.PCW_USE_S_AXI_HP2 {0} CONFIG.PCW_EN_CLK3_PORT {0}] [get_bd_cells ps7_0]
delete_bd_objs [get_bd_nets ps7_0_FCLK_CLK3] [get_bd_nets ps7_0_FCLK_CLK3] [get_bd_nets ps7_0_FCLK_CLK3]
endgroup

# Make wrapper, add constraints
save_bd_design

make_wrapper -files [get_files ./${overlay_name}/${overlay_name}.srcs/sources_1/bd/${design_name}/${design_name}.bd] -top
add_files -norecurse ./${overlay_name}/${overlay_name}.srcs/sources_1/bd/${design_name}/hdl/${design_name}_wrapper.v

add_files -fileset constrs_1 -norecurse ./vivado/constraints/${overlay_name}.xdc

