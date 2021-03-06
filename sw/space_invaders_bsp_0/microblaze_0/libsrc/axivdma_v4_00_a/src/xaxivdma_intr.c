/******************************************************************************
*
* (c) Copyright 2010-11 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xaxivdma_intr.c
*
* Implementation of interrupt related functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a jz   08/16/10 First release
* 2.00a jz   12/10/10 Added support for direct register access mode, v3 core
* 2.01a jz   01/19/11 Added ability to re-assign BD addresses
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xaxivdma.h"
#include "xaxivdma_i.h"

/*****************************************************************************/
/**
 * Enable specific interrupts for a channel
 *
 * Interrupts not specified by the mask will not be affected
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 * @param Direction is the DMA channel, use XAXIVDMA_READ or XAXIVDMA_WRITE
 * @param IntrType is the bit mask for the interrups to be enabled
 *
 * @return
 *  None
 *
 * @note
 * If channel is invalid, then nothing is done
 *****************************************************************************/
void XAxiVdma_IntrEnable(XAxiVdma *InstancePtr, u32 IntrType, u16 Direction)
{
	XAxiVdma_Channel *Channel;

	Channel = XAxiVdma_GetChannel(InstancePtr, Direction);

	if (Channel->IsValid) {
		XAxiVdma_ChannelEnableIntr(Channel, IntrType);
	}

	return;
}

/*****************************************************************************/
/**
 * Disable specific interrupts for a channel
 *
 * Interrupts not specified by the mask will not be affected
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 * @param IntrType is the bit mask for the interrups to be disabled
 * @param Direction is the DMA channel, use XAXIVDMA_READ or XAXIVDMA_WRITE
 *
 * @return
 *  None
 *
 * @note
 * If channel is invalid, then nothing is done
 *****************************************************************************/
void XAxiVdma_IntrDisable(XAxiVdma *InstancePtr, u32 IntrType, u16 Direction)
{
	XAxiVdma_Channel *Channel;

	Channel = XAxiVdma_GetChannel(InstancePtr, Direction);

	if (Channel->IsValid) {
		XAxiVdma_ChannelDisableIntr(Channel, IntrType);
	}

	return;
}

/*****************************************************************************/
/**
 * Get the pending interrupts of a channel
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 * @param Direction is the DMA channel, use XAXIVDMA_READ or XAXIVDMA_WRITE
 *
 * @return
 * The bit mask for the currently pending interrupts
 *
 * @note
 * If Direction is invalid, return 0
 *****************************************************************************/
u32 XAxiVdma_IntrGetPending(XAxiVdma *InstancePtr, u16 Direction)
{
	XAxiVdma_Channel *Channel;

	Channel = XAxiVdma_GetChannel(InstancePtr, Direction);

	if (!Channel) {
		xdbg_printf(XDBG_DEBUG_ERROR,
		    "IntrGetPending: invalid direction %d\n\r", Direction);

		return 0;
	}

	if (Channel->IsValid) {
		return XAxiVdma_ChannelGetPendingIntr(Channel);
	}
	else {
		/* An invalid channel has no intr
		 */
		return 0;
	}
}

/*****************************************************************************/
/**
 * Clear the pending interrupts specified by the bit mask
 *
 * Interrupts not specified by the mask will not be affected
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 * @param Direction is the DMA channel, use XAXIVDMA_READ or XAXIVDMA_WRITE
 * @param IntrType is the bit mask for the interrups to be cleared
 *
 * @return
 *  None
 *
 * @note
 * If channel is invalid, then nothing is done
 *****************************************************************************/
void XAxiVdma_IntrClear(XAxiVdma *InstancePtr, u32 IntrType, u16 Direction)
{
	XAxiVdma_Channel *Channel;

	Channel = XAxiVdma_GetChannel(InstancePtr, Direction);

	if (Channel->IsValid) {
		XAxiVdma_ChannelIntrClear(Channel, IntrType);
	}
	return;
}

/*****************************************************************************/
/**
 * Interrupt handler for the read channel
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 *
 * @return
 *  None
 *
 * @note
 * If the channel is invalid, then no interrupt handling
 *****************************************************************************/
void XAxiVdma_ReadIntrHandler(void * InstancePtr)
{
	XAxiVdma *DmaPtr;
	XAxiVdma_Channel *Channel;
	XAxiVdma_ChannelCallBack *CallBack;
	u32 PendingIntr;

	DmaPtr = (XAxiVdma *)InstancePtr;

	CallBack = &(DmaPtr->ReadCallBack);

	if (!CallBack->CompletionCallBack) {

		return;
	}

	Channel = XAxiVdma_GetChannel(DmaPtr, XAXIVDMA_READ);

	if (!Channel->IsValid) {
		xdbg_printf(XDBG_DEBUG_ERROR,
		    "Read channel is invalid, no intr handling\n\r");

		return;
	}

	PendingIntr = XAxiVdma_ChannelGetPendingIntr(Channel);

	XAxiVdma_ChannelIntrClear(Channel, PendingIntr);

	if (!PendingIntr || (PendingIntr & XAXIVDMA_IXR_ERROR_MASK)) {

		CallBack->ErrCallBack(CallBack->ErrRef,
		    PendingIntr & XAXIVDMA_IXR_ERROR_MASK);

		/* The channel's error callback should reset the channel
		 * There is no need to handle other interrupts
		 */
		return;
	}

	if (PendingIntr & XAXIVDMA_IXR_COMPLETION_MASK) {

		CallBack->CompletionCallBack(CallBack->CompletionRef,
		    PendingIntr);
	}

	return;
}

/*****************************************************************************/
/**
 * Interrupt handler for the write channel
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 *
 * @return
 *  None
 *
 * @note
 * If the channel is invalid, then no interrupt handling
 *****************************************************************************/
void XAxiVdma_WriteIntrHandler(void * InstancePtr)
{
	XAxiVdma *DmaPtr;
	XAxiVdma_Channel *Channel;
	XAxiVdma_ChannelCallBack *CallBack;
	u32 PendingIntr;

	DmaPtr = (XAxiVdma *)InstancePtr;

	Channel = XAxiVdma_GetChannel(DmaPtr, XAXIVDMA_WRITE);

	if (!Channel->IsValid) {
		xdbg_printf(XDBG_DEBUG_ERROR,
		    "Write channel is invalid, no intr handling\n\r");

		return;
	}

	PendingIntr = XAxiVdma_ChannelGetPendingIntr(Channel);

	XAxiVdma_ChannelIntrClear(Channel, PendingIntr);

	CallBack = &(DmaPtr->WriteCallBack);

	if (!CallBack->CompletionCallBack) {

		return;
	}

	if (!PendingIntr || (PendingIntr & XAXIVDMA_IXR_ERROR_MASK)) {

		CallBack->ErrCallBack(CallBack->ErrRef,
		    PendingIntr & XAXIVDMA_IXR_ERROR_MASK);

		/* The channel's error callback should reset the channel
		 * There is no need to handle other interrupts
		 */
		return;
	}

	if (PendingIntr & XAXIVDMA_IXR_COMPLETION_MASK) {

		CallBack->CompletionCallBack(CallBack->CompletionRef,
		    PendingIntr);
	}

	return;
}

/*****************************************************************************/
/**
 * Set call back function and call back reference pointer for one channel
 *
 * @param InstancePtr is the pointer to the DMA engine to work on
 * @param HandlerType is the interrupt type that this callback handles
 * @param CallBackFunc is the call back function pointer
 * @param CallBackRef is the call back reference pointer
 * @param Direction is the DMA channel, use XAXIVDMA_READ or XAXIVDMA_WRITE
 *
 * @return
 * - XST_SUCCESS if everything is fine
 * - XST_INVALID_PARAM if the handler type or direction invalid
 *
 * @note
 * This function overwrites the existing interrupt handler and its reference
 * pointer. The function sets the handlers even if the channels are invalid.
 *****************************************************************************/
int XAxiVdma_SetCallBack(XAxiVdma * InstancePtr, u32 HandlerType,
        void *CallBackFunc, void *CallBackRef, u16 Direction)
{
	XAxiVdma_ChannelCallBack *CallBack;
	int Status;

	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XAXIVDMA_DEVICE_READY);

	if ((HandlerType != XAXIVDMA_HANDLER_GENERAL) &&
	    (HandlerType != XAXIVDMA_HANDLER_ERROR)) {

		return XST_INVALID_PARAM;
	}

	if (Direction == XAXIVDMA_READ) {
		CallBack = &(InstancePtr->ReadCallBack);
	}
	else {
		CallBack = &(InstancePtr->WriteCallBack);
	}

	switch (HandlerType) {
	case XAXIVDMA_HANDLER_GENERAL:
		CallBack->CompletionCallBack = CallBackFunc;
		CallBack->CompletionRef = CallBackRef;
		break;

	case XAXIVDMA_HANDLER_ERROR:
		CallBack->ErrCallBack = CallBackFunc;
		CallBack->ErrRef = CallBackRef;
		break;

	default:
		Status = XST_INVALID_PARAM;
	}

	return XST_SUCCESS;
}


