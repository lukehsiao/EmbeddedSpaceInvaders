#include <stdio.h>
#include "dma_controller.h"
#include "platform.h"
#include "xparameters.h"


void print(char *str);

int main()
{
    int i;
	int src_array[20] = {33, 1, 2, 3, 4, 5, 6, 7, 123, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	int dest_array[20] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
    init_platform();
    cleanup_platform();  // Disable caches.

    xil_printf("\n\r\n\r\n\r=========\n\r");
    xil_printf("Printing value before DMA transfer.\n\r");
    xil_printf("Destination Array:\n\r[");
    for (i = 0; i < 19; i++) {
    	xil_printf("%d, ", dest_array[i]);
    }
    xil_printf("%d]\n\r", dest_array[19]);

    // Perform the DMA
    DMA_CONTROLLER_CopyData(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &src_array, (Xuint32) &dest_array, 1);

    xil_printf("Printing value after DMA transfer.\n\r");
    xil_printf("Destination Array:\n\r[");
    for (i = 0; i < 19; i++) {
    	xil_printf("%d, ", dest_array[i]);
    }
    xil_printf("%d]\n\r", dest_array[19]);
    xil_printf("=========\n\r");
    return 0;
}
