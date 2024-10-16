#ifndef __MAX30102_H
#define __MAX30102_H

#include "stdbool.h"
#include "sys/sys.h"

/*****************���絥Ƭ�����******************
											STM32
 * �ļ�			:	MAX30102������c�ļ�                   
 * �汾			: V1.0
 * ����			: 2024.8.18
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							
 * BILIBILI	:	���絥Ƭ�����
 * CSDN			:	���絥Ƭ�����
 * ����			:	����

**********************BEGIN***********************/


//==============================================MAX30102Ӳ���ӿ�==================================================
#define		MAX30102_CLK				RCC_APB2Periph_GPIOB
#define		MAX30102_PORT				GPIOB
#define		MAX30102_SCL_PIN			GPIO_PIN_15
#define		MAX30102_SDA_PIN			GPIO_PIN_14

#define 	MAX30102_SCL_W(PinState)	HAL_GPIO_WritePin(MAX30102_PORT, MAX30102_SCL_PIN, PinState)
#define 	MAX30102_SDA_W(PinState)	HAL_GPIO_WritePin(MAX30102_PORT, MAX30102_SDA_PIN, PinState)
#define 	MAX30102_SDA_R()   			HAL_GPIO_ReadPin(MAX30102_PORT, MAX30102_SDA_PIN)

#define		MAX30102_INT_CLK			RCC_APB2Periph_GPIOA
#define		MAX30102_INT_PORT			GPIOA
#define		MAX30102_INT_PIN		    GPIO_PIN_8
#define		MAX30102_INT_R()    		HAL_GPIO_ReadPin(MAX30102_INT_PORT, MAX30102_INT_PIN)


//=============================================================================================================

//////////////////////////////////////////////////////////////////////////////// 	  

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF


#define true 1
#define false 0
#define FS 100
#define BUFFER_SIZE  (FS* 5) 
#define HR_FIFO_SIZE 7
#define MA4_SIZE  4 // DO NOT CHANGE
#define HAMMING_SIZE  5// DO NOT CHANGE
#define min(x,y) ((x) < (y) ? (x) : (y))

#define MAX_BRIGHTNESS 255
#define INTERRUPT_REG 0X00

#define max30102_WR_address 0xAE

#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF


//IIC���в�������
void MAX30102_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MAX30102_IIC_Start(void);				//����IIC��ʼ�ź�
void MAX30102_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MAX30102_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t MAX30102_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MAX30102_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MAX30102_IIC_Ack(void);					//IIC����ACK�ź�
void MAX30102_IIC_NAck(void);				//IIC������ACK�ź�

void MAX30102_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
void MAX30102_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr,uint8_t* data);

void MAX30102_IIC_WriteBytes(uint8_t WriteAddr,uint8_t* data,uint8_t dataLength);
void MAX30102_IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr,uint8_t* data,uint8_t dataLength);

//MAX30102���в�������
void MAX30102_Init(void);  
void MAX30102_Reset(void);
uint8_t M30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data);
uint8_t max30102_Bus_Read(uint8_t Register_Address);
void max30102_FIFO_ReadWords(uint8_t Register_Address,uint16_t  Word_Data[][2],uint8_t count);
void max30102_FIFO_ReadBytes(uint8_t Register_Address,uint8_t* Data);

void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);

//����Ѫ���㷨���к���
void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer ,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer ,   int32_t *pn_spo2, int8_t *pch_spo2_valid ,  int32_t *pn_heart_rate , int8_t  *pch_hr_valid);
void maxim_find_peaks( int32_t *pn_locs, int32_t *pn_npks,  int32_t *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num );
void maxim_peaks_above_min_height( int32_t *pn_locs, int32_t *pn_npks,  int32_t *pn_x, int32_t n_size, int32_t n_min_height );
void maxim_remove_close_peaks( int32_t *pn_locs, int32_t *pn_npks,   int32_t  *pn_x, int32_t n_min_distance );
void maxim_sort_ascend( int32_t *pn_x, int32_t n_size );
void maxim_sort_indices_descend(  int32_t  *pn_x, int32_t *pn_indx, int32_t n_size);

void getHeartAndSpo2(uint32_t *pun_ir_buffer,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer, uint8_t *dis_spo2,  
                     uint8_t *dis_heart_rate, uint8_t *isFirst);

#endif















