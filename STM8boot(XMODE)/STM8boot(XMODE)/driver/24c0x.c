/*******************************************************************************
****����Ƕ��ʽϵͳ STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****�汾:V1.0
****����:14-2-2014
****˵��:��оƬSTM8S105C4T6
********************************************************************************/
#include "include.h"
void WaiteI2C_SDA_Idle( )
{
  unsigned int nCount=0xfff;
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
/*******************************************************************************
****��������:��EEPROMд��һ���ֽ�����
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_WriteByte(unsigned char addr,unsigned char data)
{
     while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));//����æ
     
     I2C_GenerateSTART(ENABLE);//������ʼλ
     while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
     /* ����I2C��������ַ��I2C���豸Ϊдģʽ*/
	I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
	
	/* ����EV6�������־λ��������������һ��Ӧ���ź� */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	 
	/* ������������д�����ڲ��ĵ�ַ */
	I2C_SendData(addr);
	/* ����EV8�������־λ ��������������һ��Ӧ���ź�*/
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	
	/* �������дһ���ֽ� */
	I2C_SendData(data); 
	
	/* ����EV8�������־λ��������������һ��Ӧ���ź� */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING)); 
	
	/* ������λ */
	I2C_GenerateSTOP(ENABLE);
}
/*******************************************************************************
****��������:��EEPROM����һ���ֽ�����
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
unsigned char EEPROM_ReadByte(unsigned char addr)
{
        unsigned char data;
	/* �ȴ����� */
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
	
	/* ����ʼλ */
	I2C_GenerateSTART(ENABLE);
	
	/* ����EV5 ��������������һ��Ӧ���ź�*/
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	
	/* ����I2C��������ַ��I2C���豸Ϊдģʽ*/ 
	I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
	
	/* ����EV6�������־λ */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
        
	I2C_Cmd(ENABLE); 

	I2C_SendData(addr);

	/* ����EV8�������־λ */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	/* ����ʼλ */ 
	I2C_GenerateSTART(ENABLE);
	
	/* ����EV5 ��������������һ��Ӧ���ź�*/
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	
	/* ����I2C��������ַ��I2C���豸Ϊ��ģʽ*/
	I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
	
	/* ����EV6 ��������������һ��Ӧ���ź�*/
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	/* ����EV7��������������һ��Ӧ���ź� */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
	data=I2C_ReceiveData();
	I2C_AcknowledgeConfig(I2C_ACK_NONE);
	
	/* ������λ*/
	I2C_GenerateSTOP(ENABLE);
        return data;
}
/*******************************************************************************
****��������:��EEPROMд��һҳ����
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:buffer-д�����ݻ�����addr-��ַnum-Ҫд���������Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_WriteOnePage(unsigned char *buffer,unsigned char addr,unsigned char num)
{
  /* �ȴ����� */
    while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
    
    /* ����ʼλ */
    I2C_GenerateSTART(ENABLE);
    
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* ����I2C��������ַ��I2C���豸Ϊдģʽ*/
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
  
    /* ����EV6 ��������������һ��Ӧ���ź�*/
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  
    I2C_SendData((u8)(addr)); 
    /* ����EV8 ��������������һ��Ӧ���ź�*/
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
  
    /* ���������豸д����*/
    while(num--)  
    {
      
      I2C_SendData(*buffer); 
  
      /*ָ��ָ����һ����ַ*/
      buffer++; 
    
      /* ����EV6 ��������������һ��Ӧ���ź�*/
     while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      I2C_AcknowledgeConfig(I2C_ACK_CURR);
    }
  
    /* ������λ*/
    I2C_GenerateSTOP(ENABLE);
}
/*******************************************************************************
****��������:��EEPROMд������
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:buffer-д�����ݻ�����addr-��ַnum-Ҫд���������Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_WriteData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = addr % EEPROM_Page_Byte_Size;/*����һҳ�Ŀ�ʼд�ĵ�ַ*/
  count = EEPROM_Page_Byte_Size - Addr;/*����һҳ�ĵ�ַʣ������*/
  NumOfPage =  num / EEPROM_Page_Byte_Size;/*д��������ҳ��*/
  NumOfSingle = num % EEPROM_Page_Byte_Size;/*д������ҳʣ�������*/
 
  /* д���ĵ�ַ����ҳ���׵�ַ  */
  if(Addr == 0){ 
    /*д�����ֽ�������һҳ*/
    if(NumOfPage == 0) 
      EEPROM_WriteOnePage(buffer, addr, NumOfSingle);
     
    /*д�����ֽ�������һҳ*/
    else{
      while(NumOfPage--){
        EEPROM_WriteOnePage(buffer, addr, EEPROM_Page_Byte_Size ); /*дһҳ*/
        addr +=  EEPROM_Page_Byte_Size ;
        buffer+= EEPROM_Page_Byte_Size ;
        WaiteI2C_SDA_Idle();
      }/*д����ҳ*/
      if(NumOfSingle!=0){/*дβ��*/
        EEPROM_WriteOnePage(buffer, addr, NumOfSingle);
      }
    }
  }
  /* ����д���ĵ�ַ����ҳ���׵�ַ*/
  else { 
  
    if (NumOfPage == 0) {/*д�����ֽ�������һҳ */
    
      if (NumOfSingle > count){
       /*Ҫд������ҳʣ����������ڲ���һҳ�ĵ�ַʣ������*/
        temp = NumOfSingle - count;
        EEPROM_WriteOnePage(buffer, addr, count);/*�ѵ�ǰҳ�ĵ�ַд��*/
        WaiteI2C_SDA_Idle();
        addr +=  count;
        buffer += count;
        EEPROM_WriteOnePage(buffer, addr, temp);/*���µ�һҳдʣ����ֽ�*/
      }
      else
      {
        EEPROM_WriteOnePage(buffer, addr, num);
      }
    }
    else{ /*д�����ֽ�������һҳ*/
    
      num -= count;
      NumOfPage =  num / EEPROM_Page_Byte_Size;
      NumOfSingle = num % EEPROM_Page_Byte_Size;

      EEPROM_WriteOnePage(buffer, addr, count);/*�ѵ�ǰҳ�ĵ�ַд��*/
      WaiteI2C_SDA_Idle();
      addr +=  count;
      buffer += count;

      while (NumOfPage--)
      {
        EEPROM_WriteOnePage(buffer, addr, EEPROM_Page_Byte_Size);
        WaiteI2C_SDA_Idle();
        addr +=  EEPROM_Page_Byte_Size;
        buffer += EEPROM_Page_Byte_Size;
      }

      if (NumOfSingle != 0)
      {
        EEPROM_WriteOnePage(buffer, addr, NumOfSingle);
      }
    }
  }
  WaiteI2C_SDA_Idle();
}
/*******************************************************************************
****��������:��EEPROM��������
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:buffer-�������ݻ�����addr-��ַnum-Ҫ������������Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_ReadData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
   /* �ȴ����� */
    while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
    /* ����ʼλ */
    I2C_GenerateSTART(ENABLE);
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* ����I2C��������ַ��I2C���豸Ϊдģʽ*/
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
    /* ����EV6 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
   
    I2C_SendData((u8)(addr)); 
    /* ����EV8 ��������������һ��Ӧ���ź�*/
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
    /* ����ʼλ */ 
    I2C_GenerateSTART(ENABLE);
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* ����I2C��������ַ��I2C���豸Ϊ��ģʽ*/
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
    /* ����EV6 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    /*�����ڴӴ��豸��ȡ���� */
  while(num)  
  {

      /* ����EV6 */
    if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED)){      
      /*�� EEPROM ��ȡһ���ֽ�*/
      *buffer = I2C_ReceiveData();
      /* ָ��ָ���¸�����ֽڵĵ�ַ*/
      buffer++;  
      /* �������һ���ֽ�*/
      if(num == 1){
      /* ����ҪӦ��*/
        I2C_AcknowledgeConfig(I2C_ACK_NONE);
      /* ������λ*/
        I2C_GenerateSTOP(ENABLE);  
      }
      else      
        /* �������һ���ֽ�����豸����Ӧ���ź�*/
        I2C_AcknowledgeConfig(I2C_ACK_CURR);
      num--;
    }   
  }
}