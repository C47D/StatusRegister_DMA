#include <project.h>

/* Defines for DMA_ClearSPITxInterrupt */
#define DMA_ClearSPITxInterrupt_BYTES_PER_BURST 1
#define DMA_ClearSPITxInterrupt_REQUEST_PER_BURST 1
#define DMA_ClearSPITxInterrupt_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_ClearSPITxInterrupt_DST_BASE (CYDEV_SRAM_BASE)

/* Variable declarations for DMA_ClearSPITxInterrupt */
/* Move these variable declarations to the top of the function */
uint8 DMA_ClearSPITxInterrupt_Chan;
uint8 DMA_ClearSPITxInterrupt_TD[1];

volatile uint8_t Interrupt_Status;

void DMA_Config(void);

int main()
{
    CyGlobalIntEnable;

    SPI_Start();
    DMA_Config();
    
    for(;;)
    {
        
        CyDelayUs(50);
        SPI_WriteTxData(0xDEUL);
        SPI_WriteTxData(0xADUL);
        SPI_WriteTxData(0xC0UL);
        SPI_WriteTxData(0xDEUL);
        SPI_ClearRxBuffer();

    }
}

void DMA_Config(void){
    /* DMA Configuration for DMA_ClearSPITxInterrupt */
    DMA_ClearSPITxInterrupt_Chan = DMA_ClearSPITxInterrupt_DmaInitialize(
                                    DMA_ClearSPITxInterrupt_BYTES_PER_BURST,
                                    DMA_ClearSPITxInterrupt_REQUEST_PER_BURST, 
                                    HI16(DMA_ClearSPITxInterrupt_SRC_BASE),
                                    HI16(DMA_ClearSPITxInterrupt_DST_BASE));
    
    DMA_ClearSPITxInterrupt_TD[0] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_ClearSPITxInterrupt_TD[0], 1, DMA_ClearSPITxInterrupt_TD[0], 0);
    
    /* Leemos el Tx STATUS Register para 'limpiar' el flag de la interrupcion */
    CyDmaTdSetAddress(DMA_ClearSPITxInterrupt_TD[0],
                        LO16((uint32)&SPI_TX_STATUS_REG), LO16((uint32)&Interrupt_Status));
    
    CyDmaChSetInitialTd(DMA_ClearSPITxInterrupt_Chan, DMA_ClearSPITxInterrupt_TD[0]);
    
    CyDmaChEnable(DMA_ClearSPITxInterrupt_Chan, 1);
}

/* [] END OF FILE */
