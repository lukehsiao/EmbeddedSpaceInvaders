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

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "globals.h"

void print(char *str);

int main()
{
    init_platform();

    xil_printf("\n\r\n\rTest...\n\r");

    // initialize
    int i;
    xil_printf("Initially:\n\r");
    for (i = 0; i <= 9; i++) {
    	setBunkerState(0, i, 0);
    	xil_printf("%x", getBunkerState(0,i));
    }
    xil_printf("\n\rAfter changing:\n\r");

    //test!
    setBunkerState(0, 5, 0x7);
    setBunkerState(0, 2, 0x2);
    for (i = 9; i >= 0; i--) {
    	xil_printf("%x", getBunkerState(0,i));
    }

    cleanup_platform();

    return 0;
}
