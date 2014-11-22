##############################################################################
## Filename:          C:\RH\Lab\hw/drivers/rangefinder_v1_00_a/data/rangefinder_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Sat Nov 15 15:14:12 2014 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "rangefinder" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
