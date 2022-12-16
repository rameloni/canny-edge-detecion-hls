# Run original scripts
source build_base_ip.tcl
source base.tcl

# Delete arduino things
delete_bd_objs [get_bd_nets mb3_intr_ack_1] [get_bd_nets S02_ARESETN_1] [get_bd_intf_nets Vaux1_1] [get_bd_intf_nets mdm_1_MBDEBUG_2] [get_bd_intf_nets iop_arduino_arduino_direct_spi] [get_bd_intf_nets iop_arduino_GPIO] [get_bd_intf_nets S02_AXI_1] [get_bd_intf_nets ps7_0_axi_periph_M04_AXI] [get_bd_intf_nets Vaux0_1] [get_bd_intf_nets Vaux5_1] [get_bd_intf_nets Vaux6_1] [get_bd_intf_nets Vaux8_1] [get_bd_intf_nets Vp_Vn_1] [get_bd_intf_nets Vaux9_1] [get_bd_intf_nets Vaux15_1] [get_bd_intf_nets Vaux12_1] [get_bd_intf_nets Vaux13_1] [get_bd_nets mb_3_reset_Dout] [get_bd_nets iop_arduino_mb3_intr_req] [get_bd_cells iop_arduino]
delete_bd_objs [get_bd_nets slice_arduino_gpio_gpio_i] [get_bd_nets slice_arduino_gpio_gpio_t] [get_bd_intf_nets iop_arduino_arduino_gpio] [get_bd_cells slice_arduino_gpio]
delete_bd_objs [get_bd_cells mb_iop_arduino_reset]
delete_bd_objs [get_bd_cells mb_iop_arduino_intr_ack]
delete_bd_objs [get_bd_intf_ports arduino_direct_spi]
delete_bd_objs [get_bd_intf_ports arduino_gpio]
delete_bd_objs [get_bd_intf_nets iop_arduino_direct_iic] [get_bd_intf_ports arduino_direct_iic]
delete_bd_objs [get_bd_intf_ports ck_gpio]
delete_bd_objs [get_bd_nets slice_arduino_direct_iic_scl_i] [get_bd_nets slice_arduino_direct_iic_scl_t] [get_bd_nets slice_arduino_direct_iic_sda_i] [get_bd_nets slice_arduino_direct_iic_sda_t] [get_bd_cells slice_arduino_direct_iic]
delete_bd_objs [get_bd_cells constant_10bit_0]
delete_bd_objs [get_bd_nets constant_10bit_0_dout] [get_bd_nets concat_arduino_dout] [get_bd_cells concat_arduino]
delete_bd_objs [get_bd_intf_ports Vaux1] [get_bd_intf_ports Vaux0] [get_bd_intf_ports Vp_Vn] [get_bd_intf_ports Vaux9] [get_bd_intf_ports Vaux15] [get_bd_intf_ports Vaux8] [get_bd_intf_ports Vaux13] [get_bd_intf_ports Vaux6] [get_bd_intf_ports Vaux12] [get_bd_intf_ports Vaux5]

# Delete pmod stuff
delete_bd_objs [get_bd_nets mb_2_reset_Dout] [get_bd_nets iop_pmodb_intr_ack_1] [get_bd_nets iop_pmodb_iop_pmodb_intr_req] [get_bd_nets S01_ARESETN_1] [get_bd_intf_nets mdm_1_MBDEBUG_1] [get_bd_intf_nets S01_AXI_2] [get_bd_intf_nets ps7_0_axi_periph_M03_AXI] [get_bd_intf_nets iop_pmodb_pmodb_gpio] [get_bd_cells iop_pmodb]
delete_bd_objs [get_bd_nets mb_1_reset_Dout] [get_bd_nets iop_pmoda_intr_ack_Dout] [get_bd_nets iop_pmoda_iop_pmoda_intr_req] [get_bd_nets mdm_1_debug_sys_rst] [get_bd_nets S00_ARESETN_1] [get_bd_intf_nets microblaze_0_debug] [get_bd_intf_nets S00_AXI_2] [get_bd_intf_nets ps7_0_axi_periph_M02_AXI] [get_bd_cells iop_pmoda]
delete_bd_objs [get_bd_intf_nets axi_interconnect_0_M00_AXI] [get_bd_cells axi_interconnect_0]
delete_bd_objs [get_bd_cells mb_iop_pmoda_intr_ack]
delete_bd_objs [get_bd_cells mb_iop_pmoda_reset]
delete_bd_objs [get_bd_cells mb_iop_pmodb_reset]
delete_bd_objs [get_bd_nets slice_pmoda_gpio_gpio_i] [get_bd_nets slice_pmoda_gpio_gpio_t] [get_bd_intf_nets iop_pmoda_pmoda_gpio] [get_bd_cells slice_pmoda_gpio]
delete_bd_objs [get_bd_intf_ports pmoda_gpio]
delete_bd_objs [get_bd_intf_ports pmodb_gpio]
delete_bd_objs [get_bd_cells mb_iop_pmodb_intr_ack]
delete_bd_objs [get_bd_nets constant_8bit_0_dout] [get_bd_nets concat_pmods_dout] [get_bd_cells constant_8bit_0] [get_bd_cells concat_pmoda]
delete_bd_objs [get_bd_nets iop_interrupts_dout] [get_bd_cells iop_interrupts]
delete_bd_objs [get_bd_cells mdm_1]

# Delete trace analyzers
delete_bd_objs [get_bd_nets trace_analyzer_arduino_s2mm_introut] [get_bd_intf_nets S01_AXI_1] [get_bd_intf_nets ps7_0_axi_periph_1_M02_AXI] [get_bd_intf_nets ps7_0_axi_periph_1_M03_AXI] [get_bd_cells trace_analyzer_arduino]
delete_bd_objs [get_bd_nets trace_analyzer_pmoda_s2mm_introut] [get_bd_nets logic_1_dout] [get_bd_intf_nets axi_dma_0_M_AXI_S2MM] [get_bd_intf_nets ps7_0_axi_periph_1_M00_AXI] [get_bd_intf_nets ps7_0_axi_periph_1_M01_AXI] [get_bd_cells trace_analyzer_pmoda]
delete_bd_objs [get_bd_intf_nets axi_mem_intercon_M00_AXI1] [get_bd_cells axi_mem_intercon]
delete_bd_objs [get_bd_nets rst_ps7_0_fclk3_interconnect_aresetn] [get_bd_nets rst_ps7_0_fclk3_peripheral_aresetn] [get_bd_intf_nets ps7_0_M_AXI_GP1] [get_bd_cells ps7_0_axi_periph_1]
delete_bd_objs [get_bd_cells logic_1]
delete_bd_objs [get_bd_cells rst_ps7_0_fclk3]


# Configure ps7
startgroup
set_property -dict [list CONFIG.PCW_USE_S_AXI_GP0 {0}] [get_bd_cells ps7_0]
set_property -dict [list CONFIG.PCW_USE_M_AXI_GP1 {0} CONFIG.PCW_USE_S_AXI_HP2 {0}] [get_bd_cells ps7_0]
delete_bd_objs [get_bd_nets ps7_0_FCLK_CLK3] [get_bd_nets ps7_0_FCLK_CLK3] [get_bd_nets ps7_0_FCLK_CLK3]
endgroup


# Generate wrapper, add constraints
save_bd_design
make_wrapper -files [get_files ./${overlay_name}/${overlay_name}.srcs/sources_1/bd/${design_name}/${design_name}.bd] -top
add_files -norecurse ./${overlay_name}/${overlay_name}.srcs/sources_1/bd/${design_name}/hdl/${design_name}_wrapper.v
set_property top ${design_name}_wrapper [current_fileset]
import_files -fileset constrs_1 -norecurse ./vivado/constraints/${overlay_name}.xdc
update_compile_order -fileset sources_1

