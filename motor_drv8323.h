#ifndef __MOTOR_DRV8323_H
#define	__MOTOR_DRV8323_H
#include "stm32f4xx.h"

#include "spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**********  MOTOR_CONTROL 宏定义  **********/
#define Dummy_Byte   0xFFFF
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)  // 等待超时时间
#define SPI3_CLK_ENABLE()           __HAL_RCC_SPI3_CLK_ENABLE()
//SCK 引脚
#define SPI3_SCK_PIN   GPIO_PIN_10
#define SPI3_SCK_      GPIO_PORT GPIOC
//MISO 引脚
#define SPI3_MISO_PIN        GPIO_PIN_11
#define SPI3_MISO_GPIO_PORT  GPIOC
//MOSI 引脚
#define SPI3_MOSI_PIN        GPIO_PIN_12
#define SPI3_MOSI_GPIO_PORT  GPIOC
//CS(nSCS) 引脚
#define SPI3_nSCS_PIN         GPIO_PIN_1
#define SPI3_nSCS_GPIO_PORT   GPIOD

// 以下是两种方法给SPI3_nSCS 高、低电平        ??据说操作寄存器比调用HAL库函数快
// 寄存器版本 给SPI3_nSCS 高、低电平，用寄存器版本可能需要多一点点延时，以免软件速度太快硬件跟不上。
 BSRR寄存器：控制管脚的高、低电平。32位有效，低16位写1 高电平，高16位写1 低电平。
//#define digitalLow(p,i)  {p->BSRR=(uint32_t)i << 16;}  //输出低电平
//#define digitalHigh(p,i) {p->BSRR=i;}                  //设置为高电平
//#define SPI3_nSCS_LOW()  digitalLow( SPI3_nSCS_GPIO_PORT, SPI3_nSCS_PIN )
//#define SPI3_nSCS_HIGH() digitalHigh(SPI3_nSCS_GPIO_PORT, SPI3_nSCS_PIN )
// 库函数版本 给SPI3_nSCS 高、低电平
#define SPI3_nSCS_LOW()   HAL_GPIO_WritePin(SPI3_nSCS_GPIO_PORT, SPI3_nSCS_PIN, GPIO_PIN_RESET)
#define SPI3_nSCS_HIGH()  HAL_GPIO_WritePin(SPI3_nSCS_GPIO_PORT, SPI3_nSCS_PIN, GPIO_PIN_SET)
/********************************************/

/********** MOTOR_CONTROL 相关变量 **********/
extern SPI_HandleTypeDef hspi3;

/********************************************/

/********** MOTOR_CONTROL 函数声明 **********/
// 【MOTOR-DRV8323 SPI通信】
uint16_t SPI_ReadWrite_DRV8323(uint16_t ReadAddr);// 0 - SPI读&写一体化函数 16位 - HAL库函数版本
uint16_t SPI_ReadWrite16bit(uint16_t ReadAddr);// 1 - SPI 读 & 写16bit函数 - 寄存器版本
uint16_t SPI_WRITE_DRV8323(uint16_t WriteData);// 2 - DRV8323写入函数 16位 - 寄存器版本
uint16_t SPI_Read_DRV8323(uint16_t ReadAddr);  // 3 - DRV8323读取函数 16位 - 寄存器版本
void SPI_ERROR_UserCallback(void);  // 4 - SPI功能异常提示函数
void Set_DRV8323(void);    // 5 - 用SPI配置DRV8323的函数  (操作封装)

// 【MOTOR-DRV8323 芯片使能】
void Drv8323_ENABLE(void);
void Drv8323_DISABLE(void);
/********************************************/


/* USER CODE BEGIN Private defines */


#endif  /* __MOTOR_DRV8323_H */

