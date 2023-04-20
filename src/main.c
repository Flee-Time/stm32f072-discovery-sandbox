#include "main.h"

void GPIO_init(void);
void SPI2_init(void);
void TSC_Init(void);
void SystemClock_Config(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  GPIO_init();
  SPI2_init();

  while (1)
  {
    for (int i = 0; i < 4; i++)
    {
      uint16_t GPIO_Pins[] = { LD5_Pin, LD6_Pin, LD4_Pin, LD3_Pin };
      HAL_GPIO_TogglePin(GPIOC, GPIO_Pins[i]);
      HAL_Delay(100);
    }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /*
  Initializes the RCC Oscillators 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
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
  GPIO_InitStruct.Pin = MEMS_INT1_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void TSC_Init(void)
{
  TSC_HandleTypeDef TSC_InitStruct = {0};

  /*
  Configure and enable the TSC peripheral AKA Linear Touch Sensor
  */
  TSC_InitStruct.Instance = TSC;
  TSC_InitStruct.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  TSC_InitStruct.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  TSC_InitStruct.Init.SpreadSpectrum = DISABLE;
  TSC_InitStruct.Init.SpreadSpectrumDeviation = 1;
  TSC_InitStruct.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TSC_InitStruct.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  TSC_InitStruct.Init.MaxCountValue = TSC_MCV_8191;
  TSC_InitStruct.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  TSC_InitStruct.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  TSC_InitStruct.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  TSC_InitStruct.Init.MaxCountInterrupt = DISABLE;
  TSC_InitStruct.Init.ChannelIOs = TSC_GROUP1_IO3|TSC_GROUP2_IO3|TSC_GROUP3_IO2;
  TSC_InitStruct.Init.ShieldIOs = 0;
  TSC_InitStruct.Init.SamplingIOs = TSC_GROUP1_IO4|TSC_GROUP2_IO4|TSC_GROUP3_IO3;

  if (HAL_TSC_Init(&TSC_InitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  else
  {
    __HAL_TSC_ENABLE(&TSC_InitStruct);
  }

}

void SPI2_init(void)
{
  __HAL_RCC_SPI2_CLK_ENABLE();

  SPI_HandleTypeDef SPI_InitStruct = {0};

  /*
  Configure and enable the Hardware SPI Bus
  */

  SPI_InitStruct.Instance = SPI2;
  SPI_InitStruct.Init.Mode = SPI_MODE_MASTER;
  SPI_InitStruct.Init.Direction = SPI_DIRECTION_2LINES;
  SPI_InitStruct.Init.DataSize = SPI_DATASIZE_4BIT;
  SPI_InitStruct.Init.CLKPolarity = SPI_POLARITY_LOW;
  SPI_InitStruct.Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI_InitStruct.Init.NSS = SPI_NSS_SOFT;
  SPI_InitStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  SPI_InitStruct.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStruct.Init.TIMode = SPI_TIMODE_DISABLE;
  SPI_InitStruct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.Init.CRCPolynomial = 7;
  SPI_InitStruct.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  SPI_InitStruct.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  
  if (HAL_SPI_Init(&SPI_InitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  else
  {
    __HAL_SPI_ENABLE(&SPI_InitStruct);
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