/*****************************************************************************
* Filename:          C:\RH\Lab\Lab8\hw/drivers/dma_controller_v1_00_a/src/dma_controller.c
* Version:           1.00.a
* Description:       dma_controller Driver Source File
* Date:              Fri Dec 05 16:51:46 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "dma_controller.h"

/************************** Function Definitions ***************************/


/**
 * User logic master module to copy data from one location in memory to another.
 * Because the hardware uses the AXI4-Lite protocol, one word is read from src
 * and written to destination at a time. This process is then repeated for
 * 'Length' words.
 *
 * @param   BaseAddress is the base address of the DMA_CONTROLLER device.
 * @param   SrcBaseAddr is the base address of where to copy from.
 * @param   DestBaseAddr is the base address of where to copy to.
 * @param   Length is the number of words to transfer (should be > 0).
 */
void DMA_CONTROLLER_CopyData(Xuint32 BaseAddress, Xuint32 SrcBaseAddr, Xuint32 DestBaseAddr, Xuint32 Length)
{
  if (Length == 0) {
    xil_printf("Length should be greater than zero.\n\r");
    return;
  }
	// Write Source
	DMA_CONTROLLER_mWriteSlaveReg0(BaseAddress, 0, SrcBaseAddr);

	// Write Dest
	DMA_CONTROLLER_mWriteSlaveReg1(BaseAddress, 0, DestBaseAddr);

	// Write Length
	DMA_CONTROLLER_mWriteSlaveReg2(BaseAddress, 0, Length);

	// Start the transfer
	Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_GO_PORT_OFFSET, MST_START);
}