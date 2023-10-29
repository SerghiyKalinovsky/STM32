#include <stdint.h>

#include "stm32f4xx.h"

void SysClockConfig (void)
{
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	SYS clock set to 84 Mhz
	********************************************************/
	
	
	#define PLL_M 	4
	#define PLL_N 	84
	#define PLL_P 	0  // PLLP = 4

	// 1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON;  // RCC->CR |= 1<<16; 
	while (!(RCC->CR & RCC_CR_HSERDY));  // while (!(RCC->CR & (1<<17)));
	
	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // RCC->APB1ENR |= 1<<28;
	PWR->CR |= PWR_CR_VOS;  // PWR->CR |= 3<<14; 
	
	
	// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_1WS;  // FLASH->ACR = (1<<8) | (1<<9)| (1<<10)| (1<<0);
	
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	
	// MCO prescaller
	
	RCC->CFGR |= 7U<<24;
	RCC->CFGR |= 3<<21;
	RCC->CFGR |=7U<<27;
	
	// AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // RCC->CFGR &= ~(1<<4);
	
	// APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  // RCC->CFGR |= (5<<10);
	
	// APB2 PR
	//RCC->CFGR |= RCC_CFGR_PPRE2_0;  // RCC->CFGR |= (4<<13);
	
	
	// 5. Configure the MAIN PLL
	RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (RCC_PLLCFGR_PLLSRC_HSE);  // (1<<22);

	// 6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;  // RCC->CR |= (1<<24);
	while (!(RCC->CR & RCC_CR_PLLRDY));  // while (!(RCC->CR & (1<<25)));
	
	// 7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;  // RCC->CFGR |= (2<<0);
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // while (!(RCC->CFGR & (2<<2)));
}



void mcoclc(void){
	
	RCC->AHB1ENR |= 0xFF;
	// PA8 MCO1 out
	/*RCC->AHB1ENR |= 0xFF;
	
	GPIOA->MODER |= 1U<<17;
	GPIOA->MODER &= ~(1U)<<16;
	
	GPIOA->AFR [1] &= ~(0xFU);*/
	
	// PC9 MCO2_out
	GPIOC->MODER |= 1U<<19;
	GPIOC->MODER &= ~(1U)<<18;
	GPIOC->AFR [1] &= ~(0xF0U);
	
}