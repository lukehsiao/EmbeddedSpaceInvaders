/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include "platform.h"		// Needed to both initialize and cleanup our hardware platform
#include "xparameters.h"	// Defines all of the base addresses we use
#include "xaxivdma.h"		// this DMA engine transfers frames to or from the AXI Bus
#include "xio.h"			// Basic Input Output functions to hardware
#include "xtmrctr.h"        // axi Timer
#include "time.h"			
#include "unistd.h"         // Defines standard constants and types
#include "render.h"			// Our rendering file.
#include "xuartlite_l.h"	// Allows us to read from the UART for user input
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "stateMachines.h"
#include "spaceInvadersSounds.h"
#include "xac97_l.h"
#include "pit_timer.h"
#include "rangefinder.h"

XTmrCtr Timer0;

#define DEBUG

void print(char *str);

#define MAX_SILLY_TIMER 500000;

/**
 * This is invoked in response to a timer interrupt.
 * It calls all the state machines.
 */
void timer_interrupt_handler() {	//	interruptCounter++;
	u8 i;
	task* tempTask;
	tempTask = getTasks();
	for (i = 0; i < TASKS_NUM; ++i) { // Heart of the scheduler code
		if (tempTask[i].elapsedTime >= tempTask[i].period){
			int newState;
			switch (i) {
			case 0:
				newState = TankMovementAndBullet_SM(tempTask[i].state);
				setState(i, newState);
				break;
			case 1:
				newState =  TankBulletUpdate_SM(tempTask[i].state);
				setState(i, newState);
				break;
			case 2:
				newState = AlienMovementAndBullets_SM(tempTask[i].state);
				setState(i, newState);
				break;
			case 3:
				newState = AlienbulletsUpdate_SM(tempTask[i].state);
				setState(i, newState);
				break;
			case 4:
				newState = SpaceShipUpdate_SM(tempTask[i].state);
				setState(i, newState);
				break;
			case 5:
				newState = AlienDeath_SM(tempTask[i].state);
				setState(i, newState);
				break;
			default:
				break;
			}
			setElapsedTime(i, 0); // Reset the elapsed time
		}
		setElapsedTime(i, (tempTask[i].elapsedTime + 1));
	}
}

void AC97_interrupt_handler() {
	fillFIFO();
	XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
}

void DMA_interrupt_handler(){
//	xil_printf("Resume!!!\n\r");
	resumeGameDMA();
	XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_CONTROLLER_0_TRANSFER_FINISHED_MASK);
}

/**
 * Main interrupt handler, queries the interrupt controller to see what 
 * peripheral fired the interrupt and then dispatches the corresponding 
 * interrupt handler.  This routine acknowledges the interrupt at the 
 * controller level but the peripheral interrupt must be ack'd by the
 * dispatched interrupt handler.
 */
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);

	if (intc_status & XPAR_DMA_CONTROLLER_0_TRANSFER_FINISHED_MASK){
		DMA_interrupt_handler();
	}
	// Check the FIT interrupt first.
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK){
		AC97_interrupt_handler();
	}

	if (intc_status & XPAR_PIT_TIMER_0_INTR_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_TIMER_0_INTR_MASK);
		timer_interrupt_handler();
	}
}


/**
 * Main initializes the frame buffers, sets up the VDMA, intializes the
 * frame buffers black, sets global variables, then waits in while(1).
 */
int main()
{
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
	XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	// Step 2: Initialize the memory structure and the hardware.
	if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,	XPAR_AXI_VDMA_0_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
	}
	// Step 3: (optional) set the frame store number.
	if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
		xil_printf("Set Frame Store Failed.");
	}
	// Initialization is complete at this point.

	// Setup the frame counter. We want two read frames. We don't need any write frames but the
	// function generates an error if you set the write frame count to 0. We set it to 2
	// but ignore it because we don't need a write channel at all.
	XAxiVdma_FrameCounter myFrameConfig;
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount =2;
	myFrameConfig.WriteDelayTimerCount = 10;
	Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		if(Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
	}
	// Now we tell the driver about the geometry of our frame buffer and a few other things.
	// Our image is 480 x 640.
	XAxiVdma_DmaSetup myFrameBuffer;
	myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
	myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
	myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
	myFrameBuffer.FrameDelay = 0;
	myFrameBuffer.EnableCircularBuf=1;
	myFrameBuffer.EnableSync = 0;
	myFrameBuffer.PointNum = 0;
	myFrameBuffer.EnableFrameCounter = 0;
	myFrameBuffer.FixedFrameStoreAddr = 0;
	if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
	}
	// We need to give the frame buffer pointers to the memory that it will use. This memory
	// is where you will write your video data. The vdma IP/driver then streams it to the HDMI
	// IP.
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

	if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
			myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}


	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
		xil_printf("DMA START FAILED\r\n");
	}
	int frameIndex = 0;

	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}

	Status = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_PIT_TIMER_0_INTR_MASK
					| XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK
					| XPAR_AXI_AC97_0_INTERRUPT_MASK
					| XPAR_DMA_CONTROLLER_0_TRANSFER_FINISHED_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

	Status = XTmrCtr_Initialize(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID);
	XTmrCtr_SetResetValue(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID, 0);

	//Initialize the AC'97
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR,AC97_ENABLE_IN_FIFO_INTERRUPT);
	while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, 127);
	}
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);


	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");

	initStateMachines(); //setup space invaders
	initSounds();		// setup space invader sounds

	// Initialize the PIT
	PIT_TIMER_RESET;
	PIT_TIMER_SET_DELAY(DEFAULT_RELOAD);
	PIT_TIMER_WRITE_CONTROL(FORCE_LOAD);	// Force Load the Delay Value
	PIT_TIMER_WRITE_CONTROL(INTR_ENABLE | COUNTER_ENABLE | RELOAD_ENABLE);	// Run the Timer

	microblaze_enable_interrupts();


	while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, 0);
	}

	// Wait forever in while(1)
	xil_printf("Enter a 10-digit decimal number: \n\r");
	//u8 inputKey;
	//u32 newValue;
	//int i;

	u32 debugReg = RANGEFINDER_readDebug(XPAR_RANGEFINDER_0_BASEADDR);
	xil_printf("debugReg: %08x\n\r", debugReg);

	RANGEFINDER_writeDebug(XPAR_RANGEFINDER_0_BASEADDR, 0xDEADBEEF);

	debugReg = RANGEFINDER_readDebug(XPAR_RANGEFINDER_0_BASEADDR);
	xil_printf("debugReg should be Dead Beef: %08x\n\r", debugReg);
//	int delay = 0;
	Status = XTmrCtr_Initialize(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID);
	XTmrCtr_SetResetValue(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID, 0);
	while (1){
//		if (++delay == 1000000) {
//			xil_printf("Distance: %d\n\r", RANGEFINDER_readDistance(XPAR_RANGEFINDER_0_BASEADDR));
//			delay = 0;
//		}

//		newValue = 0;
//		for (i = 9; i >= 0; i--) {
//			inputKey = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
//			inputKey = inputKey & 0xF;
//			newValue = (newValue*10) + inputKey;
//		}
//		xil_printf("You entered: %d\n\r", newValue);
//		PIT_TIMER_SET_DELAY(newValue);
//		PIT_TIMER_WRITE_CONTROL(FORCE_LOAD);	// Force Load the Delay Value
//		PIT_TIMER_WRITE_CONTROL(INTR_ENABLE | COUNTER_ENABLE | RELOAD_ENABLE);	// Run the Timer
	}

	cleanup_platform();

	return 0;
}
