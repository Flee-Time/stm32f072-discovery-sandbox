#include "mems.h"

uint8_t i3g4250d_readreg(SPI_HandleTypeDef SPI_Struct, uint8_t addr, uint8_t bytes)
{
    uint8_t tx_data[2];
    uint8_t rx_data[2];
    
    tx_data[0] = addr | 0x80;  // read operation
    tx_data[1] = 0;            // dummy byte for response
    
    HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&SPI_Struct, tx_data, rx_data, bytes, 10);
    HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
    
    return rx_data[1];
}

void i3g4250d_writereg(SPI_HandleTypeDef SPI_Struct, uint8_t addr, uint8_t data)
{
    uint8_t tx_data[2];
    
    tx_data[0] = addr;          // write operation
    tx_data[1] = data;          // data byte for writing
    
    HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&SPI_Struct, tx_data, 2, 10);
    HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
}

void i3g4250d_init(SPI_HandleTypeDef SPI_Struct)
{
    uint8_t rxbuffer = i3g4250d_readreg(SPI_Struct, I3G4250D_WHO_AM_I_ADDR, 2);

    if (rxbuffer != 0xD4)
    {
        return;
    }

    i3g4250d_writereg(SPI_Struct, I3G4250D_CTRL_REG1_ADDR, 0x5F); // Data Rate : 200Hz Cutoff : 25 Power Mode : normal mode, Z X Y : enable
    i3g4250d_writereg(SPI_Struct, I3G4250D_CTRL_REG2_ADDR, 0x02);
    i3g4250d_writereg(SPI_Struct, I3G4250D_CTRL_REG4_ADDR, 0x00);
    i3g4250d_writereg(SPI_Struct, I3G4250D_CTRL_REG5_ADDR, 0x00);

}