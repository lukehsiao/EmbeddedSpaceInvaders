##############################################################################
## Filename:          C:\RH\Lab\Lab8\hw/drivers/dma_controller_v1_00_a/data/dma_controller_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Fri Dec 05 16:51:46 2014 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "dma_controller" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
