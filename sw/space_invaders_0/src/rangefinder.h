/*****************************************************************************
* Filename:          C:\RH\Lab\hw/drivers/rangefinder_v1_00_a/src/rangefinder.h
* Version:           1.00.a
* Description:       rangefinder Driver Header File
* Date:              Sat Nov 15 15:14:12 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef RANGEFINDER_H
#define RANGEFINDER_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 */
#define RANGEFINDER_USER_SLV_SPACE_OFFSET (0x00000000)
#define RANGEFINDER_SLV_REG0_OFFSET (RANGEFINDER_USER_SLV_SPACE_OFFSET + 0x00000000)
#define RANGEFINDER_SLV_REG1_OFFSET (RANGEFINDER_USER_SLV_SPACE_OFFSET + 0x00000004)
#define RANGEFINDER_SLV_REG2_OFFSET (RANGEFINDER_USER_SLV_SPACE_OFFSET + 0x00000008)
#define RANGEFINDER_SLV_REG3_OFFSET (RANGEFINDER_USER_SLV_SPACE_OFFSET + 0x0000000C)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define RANGEFINDER_SOFT_RST_SPACE_OFFSET (0x00000100)
#define RANGEFINDER_RST_REG_OFFSET (RANGEFINDER_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a RANGEFINDER register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the RANGEFINDER device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void RANGEFINDER_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define RANGEFINDER_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a RANGEFINDER register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the RANGEFINDER device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 RANGEFINDER_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define RANGEFINDER_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from RANGEFINDER user logic slave registers.
 *
 * @param   BaseAddress is the base address of the RANGEFINDER device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void RANGEFINDER_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 RANGEFINDER_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define RANGEFINDER_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (RANGEFINDER_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define RANGEFINDER_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (RANGEFINDER_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define RANGEFINDER_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (RANGEFINDER_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
#define RANGEFINDER_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (RANGEFINDER_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))

#define RANGEFINDER_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RANGEFINDER_SLV_REG0_OFFSET) + (RegOffset))
#define RANGEFINDER_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RANGEFINDER_SLV_REG1_OFFSET) + (RegOffset))
#define RANGEFINDER_mReadSlaveReg2(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RANGEFINDER_SLV_REG2_OFFSET) + (RegOffset))
#define RANGEFINDER_mReadSlaveReg3(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RANGEFINDER_SLV_REG3_OFFSET) + (RegOffset))

/**
 *
 * Reset RANGEFINDER via software.
 *
 * @param   BaseAddress is the base address of the RANGEFINDER device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void RANGEFINDER_mReset(Xuint32 BaseAddress)
 *
 */
#define RANGEFINDER_mReset(BaseAddress) \
 	Xil_Out32((BaseAddress)+(RANGEFINDER_RST_REG_OFFSET), SOFT_RESET)


// MACROS for use with communicating with the RangeFinder IP
#define RANGEFINDER_readDistance(BaseAddress) RANGEFINDER_mReadSlaveReg0((BaseAddress), 0)
#define RANGEFINDER_readPhoto(BaseAddress) RANGEFINDER_mReadSlaveReg1((BaseAddress), 0)

#define RANGEFINDER_writeDebug(BaseAddress, Value) RANGEFINDER_mWriteSlaveReg1((BaseAddress), 0, (Value))
#define RANGEFINDER_readDebug(BaseAddress) RANGEFINDER_mReadSlaveReg1((BaseAddress), 0)

#define RANGEFINDER_readState(BaseAddress) RANGEFINDER_mReadSlaveReg2((BaseAddress), 0)

#define RANGEFINDER_readSW(BaseAddress) RANGEFINDER_mReadSlaveReg3((BaseAddress), 0)

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the RANGEFINDER instance to be worked on.
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
XStatus RANGEFINDER_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
#define TEST_AXI_LITE_USER_NUM_REG 4


#endif /** RANGEFINDER_H */
