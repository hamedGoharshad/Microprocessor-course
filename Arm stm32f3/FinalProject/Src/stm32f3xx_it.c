/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2021 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "stm32f3xx_it.h"

/* USER CODE BEGIN 0 */
extern int currentPage;
extern void enterPassword(char input[1]);
extern void navigateTo(int page);
extern void navigateUp();
extern void logIt(char msg[]);
extern _Bool lamp1,lamp2,lamp3,lamp4;
extern unsigned char inputUart[30];
extern void updateRTC(char rtc[]);
extern float light;
extern float temp;
extern int passIndex;
extern void setPass();
extern _Bool passChangeMode;
extern void EnterNewPass();
extern void printWrong();
extern _Bool alert;
extern int checkPass();
int sleepMS = 10;


void doAction(int number){
	if(number == -1){
	navigateUp();
		return;
	}
	char snum[1];
	sprintf(snum, "%d", number);
	// enterPassword(snum);
	if(currentPage == 0 ) navigateTo(number); // Main page 
	else if (currentPage == 3 ) {
	enterPassword(snum);  
			if(passIndex == 4 ) {
				if(passChangeMode){
				passIndex = 0;
		    setPass();
				passChangeMode = 0;
				logIt("Password changed");
				}else {
				if(checkPass()){
				passChangeMode = 1;
					EnterNewPass();
        }else{
				printWrong();
				}
				}
				passIndex = 0;
		}
	}// Password page
	else if (currentPage == 2){
	enterPassword(snum);  
			if(passIndex == 4 ) {
							if(checkPass()){
								if(alert){
								alert = 0;
									logIt("alert disabled");
								}else{
									alert = 1;
									logIt("alert Enabled");
								}
								navigateUp();
							}else{
								printWrong();
							}
			passIndex =0;
			}
	}
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */
	
for (int i = 0; i <4;i++){

HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

	if(i==0){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
		
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)){
				HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);
	doAction(1);
	}
	}
	
		if(i==1){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);
	doAction(4);
	}
	}
		
		if(i==2){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_10);
	doAction(7);
	}
	}
		
		if(i==3){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
	}
	}
		//HAL_Delay(sleepMS);
}


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line1 interrupt.
*/
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */
	
for (int i = 0; i <4;i++){

HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

	if(i==0){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);
			doAction(2);
	}
	}
	
		if(i==1){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);
			doAction(5);
	}
	}
		
		if(i==2){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_10);
			doAction(8);
	}
	}
		
		if(i==3){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
			doAction(0);

	}
	}
		HAL_Delay(sleepMS);
}


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI1_IRQn 1 */
}

/**
* @brief This function handles EXTI line2 and Touch Sense controller.
*/
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */

  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */
for (int i = 0; i <4;i++){

HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

	if(i==0){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);
			doAction(3);

	}
	}
	
		if(i==1){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);
			doAction(6);

	}
	}
		
		if(i==2){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_10);
			doAction(9);

	}
	}
		
		if(i==3){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
	}
	}
		HAL_Delay(sleepMS);
}


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
for (int i = 0; i <4;i++){

HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

	if(i==0){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);
			doAction(-1);

	}
	}
	
		if(i==1){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);
	}
	}
		
		if(i==2){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_10);
	}
	}
		
		if(i==3){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
	}
	}
		HAL_Delay(sleepMS);
}


HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
//strncpy(inputUart);
	
	updateRTC((char*)inputUart);  // problem ?? 
	HAL_UART_Receive_IT(&huart2,inputUart,sizeof(inputUart));
	
  /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) != lamp1){
		if(!lamp1)
		logIt("Lamp 1 turned on");
		else
		logIt("Lamp 1 turned off");
		lamp1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
	} else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13) != lamp2){
			if(!lamp2)
		logIt("Lamp 2 turned on");
		else
		logIt("Lamp 2 turned off");
	lamp2 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
	} else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14) != lamp3){
			if(!lamp3)
		logIt("Lamp 3 turned on");
		else
		logIt("Lamp 3 turned off");
		lamp3 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
	} else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)!= lamp4){
		if(!lamp4)
		logIt("Lamp 4 turned on");
		else
		logIt("Lamp 4 turned off");
	lamp4 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
	}
  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */



/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
