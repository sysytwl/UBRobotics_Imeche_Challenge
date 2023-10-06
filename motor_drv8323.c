/**
  ******************************************************************************
  * @file    motor_drv8323.c
  * @date    2023-05-08
  * @brief   驱动芯片DRV8323相关 - STM32 F407
  ******************************************************************************
  */ 

#include "motor_drv8323.h"

	uint16_t TIMP01, TIMP02, TIMP03, TIMP04, TIMP05;
	uint16_t TIMP1, TIMP2, TIMP3, TIMP4, TIMP5;

 /**
  * @brief    【MOTOR-DRV8323 SPI通信】
  *             配置 DRV8323工作状态
  * @function  0 - SPI读&写一体化函数 16位 - HAL库函数版本
  * @function  1 - SPI 读 & 写16bit函数 - 寄存器版本
  * @function  2 - DRV8323写入函数 16位 - 寄存器版本
  * @function  3 - DRV8323读取函数 16位 - 寄存器版本
  * @function  4 - SPI功能异常提示函数
  * @function  5 - 用SPI配置DRV8323的函数  (操作封装)
*/
// 0 - SPI读&写一体化函数 16位 - HAL库函数版本  【要么用后三个，要么就用这个】
uint16_t SPI_ReadWrite_DRV8323(uint16_t ReadAddr)
{
	uint16_t value; 
	
	SPI3_nSCS_LOW();   // SPI3-nSCS 拉低使能  //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
	
	//HAL_Delay(500);      // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
	HAL_SPI_TransmitReceive(&hspi3, (uint8_t*)&ReadAddr, (uint8_t*)&value, 1, 1000);  // 通过一个强制转换“骗过”库函数接口
	//HAL_Delay(500);      // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
	SPI3_nSCS_HIGH();  // SPI3-nSCS 拉高关断  //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
	
	HAL_Delay(500);
	
  return value;
}
// 1 - SPI读&写16bit函数 - 寄存器版本
uint16_t SPI_ReadWrite16bit(uint16_t ReadAddr)
{
	int SPITimeout = SPIT_FLAG_TIMEOUT;     // 等待超时时间 设为0x1000
    // Loop while DR register in not emplty 等待发送缓冲区为空，TXE 事件
    while (__HAL_SPI_GET_FLAG( &hspi3,  SPI_FLAG_TXE ) == RESET)
    {
      if((SPITimeout--) == 0)
			  {  SPI_ERROR_UserCallback();  return 0;    }
    }
    // Send Half Word through the SPIx peripheral 写入数据寄存器，把要写入的数据写入发送缓冲区
    WRITE_REG(hspi3.Instance->DR, ReadAddr);

    SPITimeout = SPIT_FLAG_TIMEOUT;
    // Wait to receive a Half Word 等待接收缓冲区非空，RXNE 事件
    while (__HAL_SPI_GET_FLAG( &hspi3, SPI_FLAG_RXNE ) == RESET)
    {
      if((SPITimeout--) == 0)
			  {    SPI_ERROR_UserCallback();  return 0;  }
    }
    // Return the Half Word read from the SPI bus 读取数据寄存器，获取接收缓冲区数据
    return READ_REG(hspi3.Instance->DR);
}

// 2 - DRV8323写入函数 16位 - 寄存器版本
uint16_t SPI_WRITE_DRV8323(uint16_t WriteData)
{
  uint16_t RxData; 
	
  SPI3_nSCS_LOW();   // SPI3-nSCS 拉低使能
  HAL_Delay(100);       // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
  SPI_ReadWrite16bit(WriteData);
  HAL_Delay(100);       // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
  SPI3_nSCS_HIGH();  // SPI3-nSCS 拉高关断
  HAL_Delay(100);       // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
	
  return RxData;  // 对于Write操作不用有返回值，这里有只是更方便调试观察，看看是否正确写入
}

// 3 - DRV8323读取函数 16位 - 寄存器版本
uint16_t SPI_Read_DRV8323(uint16_t ReadAddr)  
{
	uint16_t RxData;
	
	SPI3_nSCS_LOW();    // SPI3-nSCS 拉低使能
    HAL_Delay(100);        // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
	RxData = SPI_ReadWrite16bit(ReadAddr);
    HAL_Delay(100);        // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
	SPI3_nSCS_HIGH();   // SPI3-nSCS引脚拉高，因为不用它了
	HAL_Delay(100);        // 如果 LOW & HIGH 操作是使用的BSRR寄存器操作版本，需要加上延时，否则速度太快可能会出错！
	
    return RxData;
}

// 4 - SPI功能异常提示函数
void SPI_ERROR_UserCallback(void)
{
    /* 等待超时后的处理,输出错误信息 */
    printf("SPI 通信异常! \r\n");
}

// 5 - 用SPI设置DRV8323的函数  (操作封装)
void Set_DRV8323(void)
{
	  //库函数版本测试代码
	  Drv8323_ENABLE();//使能Drv-Enable输入高电平，驱动芯片开始工作
      HAL_Delay(1000);
	//SPI_ReadWrite_DRV8323(Dummy_Byte);HAL_Delay(500);
//	TIMP1 = SPI_ReadWrite_DRV8323(0x9000);//取地址0x02
//  TIMP2 = SPI_ReadWrite_DRV8323(0x9800);//取地址0x03
//  TIMP3 = SPI_ReadWrite_DRV8323(0xA000);//取地址0x04
//  TIMP4 = SPI_ReadWrite_DRV8323(0xA800);//取地址0x05
//  TIMP5 = SPI_ReadWrite_DRV8323(0xB000);//取地址0x06
	TIMP01 = SPI_ReadWrite_DRV8323(0x1000); //address 02
	TIMP02 = SPI_ReadWrite_DRV8323(0x1bff); //address 03
	TIMP03 = SPI_ReadWrite_DRV8323(0x27ff); //address 04
	TIMP04 = SPI_ReadWrite_DRV8323(0x2a59); //address 05
	TIMP05 = SPI_ReadWrite_DRV8323(0x3283); //address 06
//	SPI_ReadWrite_DRV8323(0x1000); //address 02
//  SPI_ReadWrite_DRV8323(0x1B22); //address 03
//  SPI_ReadWrite_DRV8323(0x2722); //address 04
//  SPI_ReadWrite_DRV8323(0x2B66); //address 05
//  SPI_ReadWrite_DRV8323(0x3280); //address 06
    HAL_Delay(500);

    TIMP1 = SPI_ReadWrite_DRV8323(0x9000);//取地址0x02
    TIMP2 = SPI_ReadWrite_DRV8323(0x9800);//取地址0x03
    TIMP3 = SPI_ReadWrite_DRV8323(0xA000);//取地址0x04
    TIMP4 = SPI_ReadWrite_DRV8323(0xA800);//取地址0x05
    TIMP5 = SPI_ReadWrite_DRV8323(0xB000);//取地址0x06

	printf("add0x02 = %d \r\n", TIMP1);
	printf("add0x03 = %d \r\n", TIMP2);
	printf("add0x04 = %d \r\n", TIMP3);
	printf("add0x05 = %d \r\n", TIMP4);
	printf("add0x06 = %d \r\n", TIMP5);
	
	Drv8323_DISABLE();//使能Drv-Enable输入低电平，驱动芯片停止工作
	
	/*  //寄存器版本测试代码
	  Drv8323_ENABLE();//使能Drv-Enable输入高电平，驱动芯片开始工作
    HAL_Delay(1000);
	
//  TIMP01 = SPI_WRITE_DRV8323(0x1000); //address 02
//  TIMP02 = SPI_WRITE_DRV8323(0x1bff); //address 03
//  TIMP03 = SPI_WRITE_DRV8323(0x27ff); //address 04
//  TIMP04 = SPI_WRITE_DRV8323(0x2a59); //address 05
//  TIMP05 = SPI_WRITE_DRV8323(0x3283); //address 06
//    HAL_Delay(500);

  TIMP1 = SPI_Read_DRV8323(0x9000);//取地址0x02
  TIMP2 = SPI_Read_DRV8323(0x9800);//取地址0x03
  TIMP3 = SPI_Read_DRV8323(0xA000);//取地址0x04
  TIMP4 = SPI_Read_DRV8323(0xA800);//取地址0x05
  TIMP5 = SPI_Read_DRV8323(0xB000);//取地址0x06

	printf("add0x02 = %d \r\n", TIMP1);
	printf("add0x03 = %d \r\n", TIMP2);
	printf("add0x04 = %d \r\n", TIMP3);
	printf("add0x05 = %d \r\n", TIMP4);
	printf("add0x06 = %d \r\n", TIMP5);
	
	  Drv8323_DISABLE();//使能Drv-Enable输入低电平，驱动芯片停止工作
	*/
}

 /**
  * @brief    【MOTOR-DRV8323 芯片使能】
  * @function  1 - DRV8323的芯片使能
  * @function  2 - DRV8323的芯片关闭
*/
// 1 - DRV8323的芯片使能
void Drv8323_ENABLE(void)   //DRV8323 Enable
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	//printf("DRV8323S 芯片启用。\r\n");
}
// 2 - DRV8323的芯片关闭
void Drv8323_DISABLE(void)   //DRV8323 Disable
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	//printf("DRV8323S 芯片关闭。\r\n");
}

