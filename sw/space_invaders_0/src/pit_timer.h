/*****************************************************************************
* Filename:          C:\RH\Lab\hw/drivers/pit_timer_v1_00_a/src/pit_timer.h
* Version:           1.00.a
* Description:       pit_timer Driver Header File
* Date:              Wed Nov 05 16:07:01 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef PIT_TIMER_H
#define PIT_TIMER_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/

#define DEFAULT_RELOAD 	0x000F4240	// Use this value to get 1 interrupt/10ms
#define INTR_ENABLE		0x00000002
#define	COUNTER_ENABLE	0x00000001
#define RELOAD_ENABLE	0x00000004
#define	FORCE_LOAD		0x00000008

/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 */
#define PIT_TIMER_USER_SLV_SPACE_OFFSET (0x00000000)
#define PIT_TIMER_SLV_REG0_OFFSET (PIT_TIMER_USER_SLV_SPACE_OFFSET + 0x00000000)
#define PIT_TIMER_SLV_REG1_OFFSET (PIT_TIMER_USER_SLV_SPACE_OFFSET + 0x00000004)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define PIT_TIMER_SOFT_RST_SPACE_OFFSET (0x00000100)
#define PIT_TIMER_RST_REG_OFFSET (PIT_TIMER_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a PIT_TIMER register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PIT_TIMER device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PIT_TIMER_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define PIT_TIMER_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a PIT_TIMER register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PIT_TIMER device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 PIT_TIMER_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PIT_TIMER_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from PIT_TIMER user logic slave registers.
 *
 * @param   BaseAddress is the base address of the PIT_TIMER device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void PIT_TIMER_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 PIT_TIMER_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PIT_TIMER_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (PIT_TIMER_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define PIT_TIMER_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (PIT_TIMER_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))

#define PIT_TIMER_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (PIT_TIMER_SLV_REG0_OFFSET) + (RegOffset))
#define PIT_TIMER_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (PIT_TIMER_SLV_REG1_OFFSET) + (RegOffset))

/**
 *
 * Reset PIT_TIMER via software.
 *
 * @param   BaseAddress is the base address of the PIT_TIMER device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PIT_TIMER_mReset(Xuint32 BaseAddress)
 *
 */
#define PIT_TIMER_mReset(BaseAddress) \
 	Xil_Out32((BaseAddress)+(PIT_TIMER_RST_REG_OFFSET), SOFT_RESET)



/********************** Custom Macros *******************************/

/**
 * Writes the specified delay value into the Delay Value Register
 * @param delayValue the new interval between interrupts
 */
#define PIT_TIMER_SET_DELAY(delayValue) \
	PIT_TIMER_mWriteSlaveReg0(XPAR_PIT_TIMER_0_BASEADDR, 0, delayValue)

/**
 * Writes the specified value into the Control Register
 * Bit 0 = Counter Enable
 * Bit 1 = Interrupt Enable
 * Bit 2 = Reload Enable
 * Bit 3 = Force Load
 * All other bits are ignored.
 * @param val The new value to set in Control Register
 */ 
#define PIT_TIMER_WRITE_CONTROL(val) \
	PIT_TIMER_mWriteSlaveReg1(XPAR_PIT_TIMER_0_BASEADDR, 0, val)

/**
 * Reads the value that is currently in the Control Register
 * @return Xuint32 value of the Control Register
 */
#define PIT_TIMER_READ_CONTROL \
    PIT_TIMER_mReadSlaveReg1(XPAR_PIT_TIMER_0_BASEADDR, 0)

/**
 * Reads the value that is currently in the Delay Value Register
 * @return Xuint32 value of the Delay Value Register
 */
#define PIT_TIMER_READ_DELAY \
    PIT_TIMER_mReadSlaveReg0(XPAR_PIT_TIMER_0_BASEADDR, 0)

/**
 * Send a software reset to the PIT
 */
#define PIT_TIMER_RESET	\
	PIT_TIMER_mReset(XPAR_PIT_TIMER_0_BASEADDR)
    
/****************** End Custom Macros *******************************/

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PIT_TIMER instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus PIT_TIMER_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
//#define TEST_AXI_LITE_USER_NUM_REG 2


#endif /** PIT_TIMER_H */
