#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.

#include "stateMachines.h"


//--------------------------------------------------------------------------------
//	Luke Hsiao & Jeff Ravert
//	ECEn 427 | Lab 4: Space Invaders
//	10 Sept 2014
//--------------------------------------------------------------------------------

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

// This is invoked in response to a timer interrupt.
// It calls all the state machines for timing.
void timer_interrupt_handler() {
//	unsigned char tasksNum = 4;
//	unsigned long tasksPeriodGCD = 1;
//	u32 tempWcet = 0;
//	u8 i;
//	for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
//		if (tasks[i].elapsedTime >= tasks[i].period){
//			XTmrCtr_SetResetValue(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID, 0);
//			// Task is ready to tick, so call its tick function
//			XTmrCtr_Start(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID);
//			tasks[i].state = tasks[i].TickFct(tasks[i].state);
//			XTmrCtr_Stop(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID);
//			tempWcet = XTmrCtr_GetValue(&Timer1, XPAR_AXI_TIMER_1_DEVICE_ID);
//			tasks[i].elapsedTime = 0; // Reset the elapsed time
//		}
//		tasks[i].elapsedTime += 1;
//		if(tempWcet > tasks[i].wcet)
//		{
//			tasks[i].wcet = tempWcet;
//		}
//
//	}
//
//	timerFlag = 1;
//	// Clear interrupt status bit in control register
//	XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR, 0, XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR, 0));
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
	// Clear the GPIO interrupt.
	XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
	//currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
	XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
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
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
//		pb_interrupt_handler();
	}
}

int main (void) {
    init_platform();
    // Initialize the GPIO peripherals.
    int success;
    print("Real-Time Clock Program is running...\n\r");
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
    // Enable the global GPIO interrupt for push buttons.
    XGpio_InterruptGlobalEnable(&gpPB);
    // Enable all interrupts in the push button peripheral.
    XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
    		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();


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


    while(1);  // Program never ends.

    cleanup_platform();

    return 0;
}
