#include <stdio.h>
#include "dma_controller.h"
#include "platform.h"
#include "xparameters.h"


void print(char *str);

int main()
{
	int source_word = 0xDEADBEEF;
	int destination_word = 0x0;

    init_platform();

    print("Hello World\n\r");
    cleanup_platform();
    printf("Printing value before DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    DMA_CONTROLLER_MasterRecvWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word);
    DMA_CONTROLLER_MasterSendWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &destination_word);

    printf("Printing value after DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

//    cleanup_platform();

    return 0;
}
