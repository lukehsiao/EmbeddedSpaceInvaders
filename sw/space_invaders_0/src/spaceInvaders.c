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
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
//#include "time.h"	//is this needed?
#include "unistd.h"
#include "render.h"			// Our rendering file.
#include "xuartlite_l.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "stateMachines.h"
#define DEBUG

//void print(char *str);

#define MAX_SILLY_TIMER 500000;



// This is invoked in response to a timer interrupt.
// It calls all the state machines for timing.
void timer_interrupt_handler() {
	unsigned char tasksNum = 4;
//	unsigned long tasksPeriodGCD = 1;
	u32 tempWcet = 0;
	u8 i;
	for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
		if (tasks[i].elapsedTime >= tasks[i].period){
//			XTmrCtr_SetResetValue(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID, 0);
			// Task is ready to tick, so call its tick function
//			XTmrCtr_Start(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID);
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
//			XTmrCtr_Stop(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID);
//			tempWcet = XTmrCtr_GetValue(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID);
			tasks[i].elapsedTime = 0; // Reset the elapsed time
		}
		tasks[i].elapsedTime += 1;
		if(tempWcet > tasks[i].wcet)
		{
			tasks[i].wcet = tempWcet;
		}

	}

//	timerFlag = 1;
	// Clear interrupt status bit in control register
//	XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR, 0, XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR, 0));
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
}


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

     if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
    		               myFrameBuffer.FrameStoreStartAddr)) {
    	 xil_printf("DMA Set Address Failed Failed\r\n");
     }
     // Print a sanity message if you get this far.
     xil_printf("Woohoo! I made it through initialization.\n\r");

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

     //	unsigned char taski=0;
     //	//ADC
     //	tasks[taski].state = -1;
     //	tasks[taski].period = 1;
     //	tasks[taski].elapsedTime = 1;
     //	tasks[taski].TickFct = &ADC_SM;
     //	++taski;
     //
     //	tasks[taski].state = -1;
     //	tasks[taski].period = 1;
     //	tasks[taski].elapsedTime = 0;
     //	tasks[taski].TickFct = &Life_SM;
     //	++taski;
     //
     //	//DAC
     //	tasks[taski].state = -1;
     //	tasks[taski].period = 1;
     //	tasks[taski].elapsedTime = 1;
     //	tasks[taski].TickFct = &DAC_SM;
     //	++taski;
     //
     //	//trigger
     //	tasks[taski].state = -1;
     //	tasks[taski].period = 1;
     //	tasks[taski].elapsedTime = 1;
     //	tasks[taski].TickFct = &trigger;
     //	++taski;

     initGlobals(); //setup space invaders
     blankScreen(); // erase old data
     render();      // draw initialized game
     u8 inputKey;
     u32 userInput;
     u32 sillyTimer = MAX_SILLY_TIMER;  // Just a cheap delay between frames.
     while (1) {
    	 sillyTimer--;
    	 inputKey = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
    	 if (inputKey == '2') {
    		 userInput = 0;
    		 xil_printf("\n\rEnter a two-digit alien number between 00-54 to kill: ");
    		 inputKey = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
    		 userInput = (inputKey - 48) * 10;		//-48 to compensate for the ascii input
    		 inputKey = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
    		 userInput = userInput + (inputKey - 48);
    		 if (userInput > 54) {
    			 xil_printf("\n\r!!! You entered a number that was too big!");
    		 }
    		 else {
        		 xil_printf("\n\r    Killing alien #%d", userInput);
        		 parseKey('2', sillyTimer, userInput);
    		 }
    	 }
    	 else if (inputKey == '7') {
    		 userInput = 0;
    		 xil_printf("\n\rEnter a bunker number between 0-3 to hit: ");
    		 inputKey = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
    		 userInput = (inputKey - 48);
    		 if (userInput > 3) {
    			 xil_printf("\n\r!!! You entered a number that was too big!");
    		 }
    		 else {
        		 xil_printf("\n\r    Hitting bunker #%d", userInput);
        		 parseKey('7', sillyTimer, userInput);
    		 }
    	 }
    	 else {
			 parseKey(inputKey, sillyTimer, userInput);
    	 }
    	 if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
        	 xil_printf("vdma parking failed\n\r");
         }
     }
     cleanup_platform();

    return 0;
}

