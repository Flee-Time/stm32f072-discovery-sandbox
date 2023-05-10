#ifndef MEMS_H_
#define MEMS_H_

#include "stm32f0xx_hal.h"

/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
#define I3G4250D_WHO_AM_I_ADDR          0x0F  /* device identification register */
#define I3G4250D_CTRL_REG1_ADDR         0x20  /* Control register 1 */
#define I3G4250D_CTRL_REG2_ADDR         0x21  /* Control register 2 */
#define I3G4250D_CTRL_REG3_ADDR         0x22  /* Control register 3 */
#define I3G4250D_CTRL_REG4_ADDR         0x23  /* Control register 4 */
#define I3G4250D_CTRL_REG5_ADDR         0x24  /* Control register 5 */
#define I3G4250D_REFERENCE_REG_ADDR     0x25  /* Reference register */
#define I3G4250D_OUT_TEMP_ADDR          0x26  /* Out temp register */
#define I3G4250D_STATUS_REG_ADDR        0x27  /* Status register */
#define I3G4250D_OUT_X_L_ADDR           0x28  /* Output Register X */
#define I3G4250D_OUT_X_H_ADDR           0x29  /* Output Register X */
#define I3G4250D_OUT_Y_L_ADDR           0x2A  /* Output Register Y */
#define I3G4250D_OUT_Y_H_ADDR           0x2B  /* Output Register Y */
#define I3G4250D_OUT_Z_L_ADDR           0x2C  /* Output Register Z */
#define I3G4250D_OUT_Z_H_ADDR           0x2D  /* Output Register Z */
#define I3G4250D_FIFO_CTRL_REG_ADDR     0x2E  /* Fifo control Register */
#define I3G4250D_FIFO_SRC_REG_ADDR      0x2F  /* Fifo src Register */

#define I3G4250D_INT1_CFG_ADDR          0x30  /* Interrupt 1 configuration Register */
#define I3G4250D_INT1_SRC_ADDR          0x31  /* Interrupt 1 source Register */
#define I3G4250D_INT1_TSH_XH_ADDR       0x32  /* Interrupt 1 Threshold X register */
#define I3G4250D_INT1_TSH_XL_ADDR       0x33  /* Interrupt 1 Threshold X register */
#define I3G4250D_INT1_TSH_YH_ADDR       0x34  /* Interrupt 1 Threshold Y register */
#define I3G4250D_INT1_TSH_YL_ADDR       0x35  /* Interrupt 1 Threshold Y register */
#define I3G4250D_INT1_TSH_ZH_ADDR       0x36  /* Interrupt 1 Threshold Z register */
#define I3G4250D_INT1_TSH_ZL_ADDR       0x37  /* Interrupt 1 Threshold Z register */
#define I3G4250D_INT1_DURATION_ADDR     0x38  /* Interrupt 1 DURATION register */

#define NCS_MEMS_SPI_Pin GPIO_PIN_0
#define NCS_MEMS_SPI_GPIO_Port GPIOC
#define MEMS_INT1_Pin GPIO_PIN_1
#define MEMS_INT1_GPIO_Port GPIOC
#define MEMS_INT2_Pin GPIO_PIN_2
#define MEMS_INT2_GPIO_Port GPIOC

uint8_t i3g4250d_readreg(SPI_HandleTypeDef SPI_Struct, uint8_t addr, uint8_t bytes);
void i3g4250d_writereg(SPI_HandleTypeDef SPI_Struct, uint8_t addr, uint8_t data);
void i3g4250d_init(SPI_HandleTypeDef SPI_Struct);

#endif