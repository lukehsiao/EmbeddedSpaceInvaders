///*
// * AC97test.c
// *
// *  Created on: Oct 25, 2014
// *      Author: Jeff Ravert
// *      Author: Luke Hsiao
// */
//
//
//
//#include <stdio.h>
//#include "platform.h"
//#include "xparameters.h"
//#include "xac97_l.h"
//#include "spaceInvadersSounds.h"
//
//int main()
//{
//	init_platform();
//	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
//	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
//	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
//	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);
//	int i = 0;
//	while(1){
//		while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){
//			i++;
//			if(i > alienKilled_numberOfSamples)
//				i = 0;
//			int sample = shoot_soundData[i];
//			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample|sample<<16);
//
//		}
//	}
//
//	cleanup_platform();
//
//	return 0;
//}


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "spaceInvadersSounds.h"
#include "xac97_l.h"

int main()
{
	init_platform();
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);

	int i = 0;
	int j = 0;
	while(1){
		while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){
			int sample = tankShot_soundData[i];
			int numberOfSamples = tankShot_numberOfSamples;
			if(i >= numberOfSamples){
				sample = 0;
			}
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample | (sample<<16));
			j++;
//			if(j == 4){
				j=0;
				i++;

				if(i > numberOfSamples*4){
					i = 0;
				}
//			}
		}
	}
	cleanup_platform();
	return 0;
}
