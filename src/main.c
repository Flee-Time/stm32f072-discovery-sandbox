#include "main.h"

void GPIO_init(void);
void SPI2_init(void);
void TSC_Init(void);
void SystemClock_Config(void);
float twosComplement(uint8_t byte);

TSC_HandleTypeDef TSC_Struct = {0};
SPI_HandleTypeDef SPI_Struct = {0};

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  GPIO_init();
  SPI2_init();

  // Set ncs of mems chip high for spi transfer
  HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
  i3g4250d_init(SPI_Struct);

  while (1)
  {
    uint8_t x_mems = i3g4250d_readreg(SPI_Struct, 0x29, 2);
    uint8_t y_mems = i3g4250d_readreg(SPI_Struct, 0x2B, 2);
    uint8_t z_mems = i3g4250d_readreg(SPI_Struct, 0x2D, 2);
    
    if (twosComplement(x_mems) * 10 > 50)
    {
      HAL_GPIO_WritePin(GPIOC, LD5_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, LD5_Pin, GPIO_PIN_RESET);
    }
        
    if (twosComplement(y_mems) * 10 > 50)
    {
      HAL_GPIO_WritePin(GPIOC, LD6_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, LD6_Pin, GPIO_PIN_RESET);
    }
        
    if (twosComplement(z_mems) * 10 > 50)
    {
      HAL_GPIO_WritePin(GPIOC, LD4_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, LD4_Pin, GPIO_PIN_RESET);
    }
  }
}

float twosComplement(uint8_t byte)
{
  int sign = (byte & 0x80) ? -1 : 1;
  int value = (sign == -1) ? (~byte + 1) & 0xFF : byte;
  float result = (float)value;
  return result;
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /*
  Initializes the RCC Oscillators 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /*
  Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

void GPIO_init(void) 
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* 
    USER button Configuration
    Configure GPIO pin : PA0 
  */

  /* Set GPIOA Pin#0 Parameters */
  GPIO_InitStruct.Pin     = B1_Pin;
  GPIO_InitStruct.Mode    = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  /* Init GPIOA Pin#0 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*
    Configure GPIOC
  */

  /* Reset GPIO State */
  HAL_GPIO_WritePin(GPIOC, NCS_MEMS_SPI_Pin | EXT_RESET_Pin | LD3_Pin | LD6_Pin | LD4_Pin | LD5_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = NCS_MEMS_SPI_Pin | EXT_RESET_Pin | LD3_Pin | LD6_Pin | LD4_Pin | LD5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIOC MEMS Interrupt */
  GPIO_InitStruct.Pin = MEMS_INT1_Pin | MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI2_SCK_Pin | SPI2_MISO_Pin | SPI2_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void TSC_Init(void)
{
  /*
  Configure and enable the TSC peripheral AKA Linear Touch Sensor
  */
  TSC_Struct.Instance = TSC;
  TSC_Struct.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  TSC_Struct.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  TSC_Struct.Init.SpreadSpectrum = DISABLE;
  TSC_Struct.Init.SpreadSpectrumDeviation = 1;
  TSC_Struct.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TSC_Struct.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  TSC_Struct.Init.MaxCountValue = TSC_MCV_8191;
  TSC_Struct.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  TSC_Struct.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  TSC_Struct.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  TSC_Struct.Init.MaxCountInterrupt = DISABLE;
  TSC_Struct.Init.ChannelIOs = TSC_GROUP1_IO3 | TSC_GROUP2_IO3 | TSC_GROUP3_IO2;
  TSC_Struct.Init.ShieldIOs = 0;
  TSC_Struct.Init.SamplingIOs = TSC_GROUP1_IO4 | TSC_GROUP2_IO4 | TSC_GROUP3_IO3;

  if (HAL_TSC_Init(&TSC_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  else
  {
    __HAL_TSC_ENABLE(&TSC_Struct);
  }

}

void SPI2_init(void)
{
  __HAL_RCC_SPI2_CLK_ENABLE();

  /*
  Configure and enable the Hardware SPI Bus
  */

  SPI_Struct.Instance = SPI2;
  SPI_Struct.Init.Mode = SPI_MODE_MASTER;
  SPI_Struct.Init.Direction = SPI_DIRECTION_2LINES;
  SPI_Struct.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI_Struct.Init.CLKPolarity = SPI_POLARITY_LOW;
  SPI_Struct.Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI_Struct.Init.NSS = SPI_NSS_SOFT;
  SPI_Struct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  SPI_Struct.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI_Struct.Init.TIMode = SPI_TIMODE_DISABLE;
  SPI_Struct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI_Struct.Init.CRCPolynomial = 7;
  SPI_Struct.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  SPI_Struct.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  
  if (HAL_SPI_Init(&SPI_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  else
  {
    __HAL_SPI_ENABLE(&SPI_Struct);
  }
}

/* 
 ----- Handlers for the stm32 hardware abstraction layer ------
 --------------- DO NOT TOUCH AFTER THIS POINT ----------------
 ---------------- I GOT NO IDEA TF THESE ARE ------------------
*/

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)
{
}

void MemManage_Handler(void)
{
  while (1) {}
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

void HardFault_Handler(void)
{
  __disable_irq();
  while (1) {}
}

void BusFault_Handler(void)
{
  __disable_irq();
  while (1) {}
}

void UsageFault_Handler(void)
{
  __disable_irq();
  while (1) {}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}