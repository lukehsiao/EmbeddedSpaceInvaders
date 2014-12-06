#include <stdio.h>
#include "dma_controller.h"
#include "platform.h"
#include "xparameters.h"


void print(char *str);

int main()
{
	int source_word = 0xDEADBEEF;
	int destination_word = 0x0;

	int src_array[20] = {33, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	int dest_array[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    init_platform();
    cleanup_platform();

    xil_printf("Hello World\n\r");
    xil_printf("Printing value before DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    //DMA_CONTROLLER_MasterRecvWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word);
    //DMA_CONTROLLER_MasterSendWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &destination_word);

    DMA_CONTROLLER_CopyData(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &src_array, (Xuint32) &dest_array, 5);

    xil_printf("Printing value after DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    int i;
    xil_printf("Destination Array:\n\r[");
    for (i = 0; i < 19; i++) {
    	xil_printf("%d, ", dest_array[i]);
    }
    xil_printf("%d]\n\r", dest_array[19]);

//    cleanup_platform();

    return 0;
}
