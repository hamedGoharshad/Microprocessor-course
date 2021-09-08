/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
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
#include "main.h"
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


typedef unsigned char byte;
char password[5] = "11111";
char tempPass[5] ;
int passIndex = 0;
int currentPage = 0;
RTC_DateTypeDef myDate;
RTC_TimeTypeDef myTime;

unsigned char inputUart[30];

_Bool passChangeMode=0;
_Bool lamp1= 0,lamp2= 0,lamp3= 0,lamp4 = 0;
float light;
float temp;
_Bool alert = 0;

byte family1[] = {
  0x05,
  0x0A,
  0x04,
  0x02,
  0x19,
  0x1F,
  0x08,
  0x18
}; 

byte family2[] = {
  0x00,
  0x08,
  0x0E,
  0x15,
  0x0F,
  0x1C,
  0x00,
  0x00
};

byte family3[] = {
 0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x07,
  0x04,
  0x1C
};

byte family4[] = {
   0x04,
  0x0A,
  0x00,
  0x15,
  0x15,
  0x0A,
  0x00,
  0x00
};

byte family5[] = {
  0x00,
  0x02,
  0x02,
  0x1A,
  0x0A,
  0x1B,
  0x00,
  0x00
};



void initRTC(int year,int month,int date,int hour,int min , int sec){
	myTime.Hours =hour;
	myTime.Minutes =min;
	myTime.Seconds = sec;
	HAL_RTC_SetTime(&hrtc,&myTime,RTC_FORMAT_BIN);

	myDate.Year =year;
	myDate.Month =month;
	myDate.Date = date;
	HAL_RTC_SetDate(&hrtc,&myDate,RTC_FORMAT_BIN);
		
}

void navigateToMenu(){
clear();
begin(20,4);
setCursor(0,0);
print("1-System Status");
setCursor(0,1);
print("2-Toggle Alerting");
setCursor(0,2);
print("3-Change Password");
setCursor(0,3);
print("4-About Us");

//sprintf(snum, "%d", score);
//print(snum);

//setCursor(0,manRow);
//write(1);

//setCursor(fireCol,fireRow);
//write(0);
}

void navigateToStatus(){
	HAL_RTC_GetTime(&hrtc,&myTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&myDate,RTC_FORMAT_BIN);
	
	clear();
	setCursor(0,0);
	char timeStr[20];
	char dateStr[20];
	sprintf(timeStr,"%2d:%2d:%2d",myTime.Hours,myTime.Minutes,myTime.Seconds);
	print(timeStr);
	setCursor(0,1);
	sprintf(dateStr," %d \\ %d \\ %d",myDate.Year,myDate.Month,myDate.Date);
	print(dateStr);
	setCursor(0,2);
	char lampStatu[20];
	sprintf(lampStatu,"L1:%d L2:%d L3:%d L4:%d",lamp1,lamp2,lamp3,lamp4);
	print(lampStatu);
	
}

void navigateToEnableAlert(){
clear();
	setCursor(0,0);
	print("enter password :");
}
	void EnterNewPass(){
	passChangeMode = 1;
	clear();
	setCursor(0,0);
		print("enter new pass :");
	}
	void printWrong(){
					setCursor(0,3);
		print("          ");
					setCursor(0,3);		
		print("wrong password");
	}
	
void navigateToChangePassword(){
clear();
setCursor(0,0);
	print("Old password :");
}

int checkPass(){
	 if(strcmp(password,tempPass)==0) return 1 ; else return 0;
}


void navigateToAboutUs(){
clear();
	
createChar(0,family1);
createChar(1,family2);
createChar(2,family3);
createChar(3,family4);
createChar(4,family5);

	setCursor(0,0);
print(" * Smart Home Controll *");
		setCursor(0,1);
print("written on arm");
setCursor(0,2);

write(4);
write(3);
write(2);
write(1);
write(0);
}


void navigateUp(){
	currentPage = 0;
			navigateToMenu();
	}

void logIt(char msg[]){
	// get date + msg and UART it
//				HAL_UART_Transmit(&huart2, msg ,size , 1000);
	HAL_RTC_GetTime(&hrtc,&myTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&myDate,RTC_FORMAT_BIN);
	char timeStr[20];
	char dateStr[20];
	sprintf(timeStr,"%2d:%2d:%2d - ",myTime.Hours,myTime.Minutes,myTime.Seconds);
	sprintf(dateStr," %d \\ %d \\ %d",myDate.Year,myDate.Month,myDate.Date);
  char log[100];
	int size = sprintf(log,"%s - %s - %s \n",dateStr,timeStr,msg);
//		strcat(log,timeStr);
//		strcat(log,dateStr);
//		strcat(log,msg);
//		strcat(log,"\n");
		

		HAL_UART_Transmit(&huart2, (unsigned char*)log ,size , 1000);
	}

void updateRTC(char msg[],int size){
		//cmd : SetDateTime 03/01/2021 08:50

   char * token = strtok(msg, " ");
        token = strtok(NULL, " ");
		char date[10];
		char time[5];
	 sprintf(date,"%s", token); 
	 token = strtok(NULL, " ");
	 sprintf(time,"%s", token); 
		token = strtok(date,"/");
		int day = atoi(token);
		 token = strtok(NULL, " ");
	int month = atoi(token);
	 token = strtok(NULL, " ");
		int year = atoi(token);
	token = strtok(time,":");
	int hour = atoi(token);
	 token = strtok(NULL, " ");
	 int min = atoi(token);
	initRTC(year,month,day,hour,min,0);
	logIt("Date and Time Changed");
}

	

void enterPassword(char input[1]){
		char a[] = "a"; 
		if(strcmp(input,a)==0){
		setCursor(--passIndex +7,2);
			print(" ");
		}else{
		setCursor(passIndex++ + 7,2);
		print(input);
			strcat(tempPass,input);
		}
	
	}
void setPass(){
sprintf(password ,"%s",tempPass);
		sprintf(tempPass,"");
}

void navigateTo(int page){
	switch(page){
		case 1 : currentPage = 1; navigateToStatus(); break;
		case 2 : currentPage = 2; navigateToEnableAlert(); break;
		case 3 : currentPage = 3; navigateToChangePassword(); break;
		case 4 : currentPage = 4; navigateToAboutUs(); break;
		default : break;
	}
	}
	 
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);

HAL_TIM_Base_Start_IT(&htim2);
HAL_UART_Receive_IT(&huart2,inputUart,sizeof(inputUart));
HAL_ADC_Start_IT(&hadc1);
initRTC(21,1,1,12,0,0);
navigateToMenu();

logIt("System Running...");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
