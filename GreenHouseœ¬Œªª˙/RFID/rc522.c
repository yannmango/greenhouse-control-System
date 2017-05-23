/********************************************************************
 * RFID����ʵ���ļ�
 * ���ܣ�	 ʵ��RFID��ز����ӿ�
 * Ŀ��������ATmega162v						                          
 * ����:     RC 8MHZ								                  
 * ���뻷����ICCAVR 7						                    
 * �汾��    V1.0 (2012/08/09)
 * ��վ��    www.briup.com 
 * ���䣺    iot@briup.com 
 * ʱ�䣺    2012��08��09��
 * ��Դ��    ��������Ƽ����޹�˾
 *����Ȩ��   Copyright(C) ������� www.briup.com  All Rights Reserved
 *��������   �˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��
 *******************************************************************/

#include "rc522.h"
/********************************************************************
 * �������ܣ�΢����ʱ�ӳ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺����n����ʾ��ʱʱ��ĳ���
 * ��    �أ�
 * *****************************************************************/
void nop()
{
	_delay_us(10);
}

/********************************************************************
 * �������ܣ���SPI���� 
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
uint8 SPIReadByte(void)
{
	uint8 SPICount;// Counter used to clock out the data
	uint8 SPIData; 

	SPIData = 0;
	for (SPICount = 0; SPICount < 8; SPICount++)
	{// Prepare to clock in the data to be read

		SPIData <<=1;// Rotate the data
		CLR_SPI_CK; 
		nop();nop();// Raise the clock to clock the data out of the MAX7456
		if(STU_SPI_MISO)
		{
			SPIData|=0x01;
		}
		SET_SPI_CK;   nop();nop();// Drop the clock ready for the next bit
	}// and loop back
	return (SPIData);// Finally return the read data
} 

/********************************************************************
 * �������ܣ�дSPI���� 
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
void SPIWriteByte(uint8 SPIData)
{
	uint8 SPICount;// Counter used to clock out the data
	for (SPICount = 0; SPICount < 8; SPICount++)
	{
		if (SPIData & 0x80) {
			SET_SPI_MOSI;
		} else {
			CLR_SPI_MOSI;
		} 
		nop();nop();
		CLR_SPI_CK;nop();nop();
		SET_SPI_CK;nop();nop();
		SPIData <<= 1;
	}         	
}

/********************************************************************
 * �������ܣ���RC632�Ĵ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺Address[IN]:�Ĵ�����ַ
 * ��    �أ�������ֵ
 * *****************************************************************/
uint8 ReadRawRC(uint8 Address)
{
	uint8 ucAddr;
	uint8 ucResult=0;
	CLR_SPI_CS;
	ucAddr = ((Address<<1)&0x7E)|0x80;
	SPIWriteByte(ucAddr);
	ucResult=SPIReadByte();
	SET_SPI_CS;
	return ucResult;
}

/********************************************************************
 * �������ܣ�дRC632�Ĵ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺Address[IN]:�Ĵ�����ַ
 * ��    �أ�д���ֵ
 * *****************************************************************/
void WriteRawRC(uint8 Address, uint8 value)
{  
	uint8 ucAddr;

	CLR_SPI_CS;
	ucAddr = ((Address<<1)&0x7E);
	SPIWriteByte(ucAddr);
	SPIWriteByte(value);
	SET_SPI_CS;
}   

/********************************************************************
 * �������ܣ����RC632�Ĵ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺reg[IN]:�Ĵ�����ַ mask[IN]:��λ��
 * ��    �أ�
 * *****************************************************************/
void ClearBitMask(uint8 reg,uint8 mask)  
{
	char tmp = 0x00;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 


/********************************************************************
 * �������ܣ�����RC632�Ĵ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺reg[IN]:�Ĵ�����ַ mask[IN]:����λ��
 * ��    �أ�
 * *****************************************************************/
void SetBitMask(uint8 reg,uint8 mask)  
{
	char tmp = 0x00;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg,tmp | mask);  // set bit mask
}

/********************************************************************
 * �������ܣ���MF522����CRC16����
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
void CalulateCRC(uint8 *pIndata,uint8 len,uint8 *pOutData)
{
	uint8 i,n;
	ClearBitMask(DivIrqReg,0x04);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);
	for (i=0; i<len; i++)
	{   WriteRawRC(FIFODataReg, *(pIndata+i));   }
	WriteRawRC(CommandReg, PCD_CALCCRC);
	i = 0xFF;
	do 
	{
		n = ReadRawRC(DivIrqReg);
		i--;
	} while ((i!=0) && !(n&0x04));
	pOutData[0] = ReadRawRC(CRCResultRegL);
	pOutData[1] = ReadRawRC(CRCResultRegM);
}

/********************************************************************
 * �������ܣ�ͨ��RC522��ISO14443��ͨѶ
 * ע�����
 * ��ʾ˵����
 * ��    �룺Command[IN]:RC522������ 
 * 			 pInData[IN]:ͨ��RC522���͵���Ƭ������ 
 * 			 InLenByte[IN]:�������ݵ��ֽڳ��� 
 * 			 pOutData[OUT]:���յ��Ŀ�Ƭ�������� 
 * 			 *pOutLenBit[OUT]:�������ݵ�λ����
 * ��    �أ�
 * *****************************************************************/
char PcdComMF522(uint8 Command, 
		uint8 *pInData, 
		uint8 InLenByte,
		uint8 *pOutData, 
		uint32 *pOutLenBit)
{
	char status = MI_ERR;
	uint8 irqEn   = 0x00;
	uint8 waitFor = 0x00;
	uint8 lastBits;
	uint8 n;
	uint32 i;
	switch (Command)
	{
		case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
	}

	WriteRawRC(ComIEnReg,irqEn|0x80);
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);

	for (i=0; i<InLenByte; i++) {   
		WriteRawRC(FIFODataReg, pInData[i]);    
	}
	WriteRawRC(CommandReg, Command);

	if (Command == PCD_TRANSCEIVE) {    
		SetBitMask(BitFramingReg,0x80);  
	}

	//i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
	i = 2000;
	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	} while ((i!=0) && !(n&0x01) && !(n&waitFor));
	ClearBitMask(BitFramingReg,0x80);

	if (i!=0) {    
		if(!(ReadRawRC(ErrorReg)&0x1B)) {
			status = MI_OK;
			if (n & irqEn & 0x01) {   status = MI_NOTAGERR;   }
			if (Command == PCD_TRANSCEIVE) {
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if (lastBits) {   
					*pOutLenBit = (n-1)*8 + lastBits;   
				} else {   
					*pOutLenBit = n*8;   
				}
				if (n == 0) {   
					n = 1;    
				}
				if (n > MAXRLEN) {   
					n = MAXRLEN;   
				}
				for (i=0; i<n; i++) {   
					pOutData[i] = ReadRawRC(FIFODataReg);    
				}
			}
		} else {   
			status = MI_ERR;   
		}

	}

	SetBitMask(ControlReg,0x80);           // stop timer now
	WriteRawRC(CommandReg,PCD_IDLE); 
	return status;
}

/********************************************************************
 * �������ܣ�Ѱ��
 * ע�����
 * ��ʾ˵����
 * ��    �룺����˵��: req_code[IN]:Ѱ����ʽ 
 * 			 0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ� 
 * 			 0x26 = Ѱδ��������״̬�Ŀ� 
 * 			 pTagType[OUT]����Ƭ���ʹ��� 
 * 			 0x4400 = Mifare_UltraLight 
 * 			 0x0400 = Mifare_One(S50) 
 * 			 0x0200 = Mifare_One(S70) 
 * 			 0x0800 = Mifare_Pro(X) 
 * 			 0x4403 = Mifare_DESFire
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdRequest(uint8 req_code,uint8 *pTagType)
{
	char status;  
	uint32 unLen;
	uint8 ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);

	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10)) {    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	} else {   
		status = MI_ERR;   
	}

	return status;
}


/********************************************************************
 * �������ܣ�����ײ
 * ע�����
 * ��ʾ˵����
 * ��    �룺pSnr[OUT]:��Ƭ���кţ�4�ֽ�
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdAnticoll(uint8 *pSnr)
{
	char status;
	uint8 i,snr_check=0;
	uint32 unLen;
	uint8 ucComMF522Buf[MAXRLEN]; 


	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x00);
	ClearBitMask(CollReg,0x80);

	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

	if (status == MI_OK) {
		for (i=0; i<4; i++) {   
			*(pSnr+i)  = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i]) {   
			status = MI_ERR;    
		}
	}

	SetBitMask(CollReg,0x80);
	return status;
}

/********************************************************************
 * �������ܣ�ѡ����Ƭ
 * ע�����
 * ��ʾ˵����
 * ��    �룺pSnr[IN]:��Ƭ���кţ�4�ֽ�
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdSelect(uint8 *pSnr)
{
	char status;
	uint8 i;
	uint32 unLen;
	uint8 ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x70;
	ucComMF522Buf[6] = 0;
	for (i=0; i<4; i++)
	{
		ucComMF522Buf[i+2] = *(pSnr+i);
		ucComMF522Buf[6]  ^= *(pSnr+i);
	}
	CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);

	ClearBitMask(Status2Reg,0x08);

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x18)) {   
		status = MI_OK;  
	} else {   
		status = MI_ERR;    
	}

	return status;
}

/********************************************************************
 * �������ܣ���֤��Ƭ����
 * ע�����
 * ��ʾ˵����
 * ��    �룺auth_mode[IN]: ������֤ģʽ 
 * 				0x60 = ��֤A��Կ 
 * 				0x61 = ��֤B��Կ 
 * 			 addr[IN]�����ַ 
 * 			 pKey[IN]������ 
 * 			 pSnr[IN]����Ƭ���кţ�4�ֽ�
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdAuthState(uint8 auth_mode,uint8 addr,uint8 *pKey,uint8 *pSnr)
{
	char status;
	uint32 unLen;
	uint8 ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = auth_mode;
	ucComMF522Buf[1] = addr;
	memcpy(&ucComMF522Buf[2], pKey, 6); 
	memcpy(&ucComMF522Buf[8], pSnr, 6); 

	status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
	if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
	{   
		status = MI_ERR;   
	}

	return status;
}

/********************************************************************
 * �������ܣ���ȡM1��һ������
 * ע�����
 * ��ʾ˵����
 * ��    �룺addr[IN]�����ַ 
 * 			 pData[OUT]�����������ݣ�16�ֽ�
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdRead(uint8 addr,uint8 *pData)
{
	char status;
	uint32 unLen;
	uint8 ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_READ;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
	if ((status == MI_OK) && (unLen == 0x90)) {   
		memcpy(pData, ucComMF522Buf, 16);   
	} else {   
		status = MI_ERR;   
	}
	return status;
}

/********************************************************************
 * �������ܣ�дһ�����ݵ�M1��
 * ע�����
 * ��ʾ˵����
 * ��    �룺addr[IN]�����ַ 
 * 			 pData[IN]��д������ݣ�16�ֽ�
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdWrite(uint8 addr,uint8 *pData)
{
	char status;
	uint32 unLen;
	uint8 ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_WRITE;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
	{   
		status = MI_ERR;   
	}

	if (status == MI_OK)
	{
		memcpy(ucComMF522Buf, pData, 16);
		CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

		status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
		if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
		{   
			status = MI_ERR;   
		}
	}

	return status;
}

/********************************************************************
 * �������ܣ����Ƭ��������״̬
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
/*
   char PcdHalt(void)
   {
   char status;
   uint32 unLen;
   uint8 ucComMF522Buf[MAXRLEN]; 

   ucComMF522Buf[0] = PICC_HALT;
   ucComMF522Buf[1] = 0;
   CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

   return MI_OK;
   }
   */

/********************************************************************
 * �������ܣ���λRC522
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ��ɹ�����MI_OK
 * *****************************************************************/
char PcdReset(void)
{
	SET_RC522RST;
	delay_ns(10);
	CLR_RC522RST;
	delay_ns(10);
	SET_RC522RST;
	delay_ns(10);
	WriteRawRC(CommandReg,PCD_RESETPHASE);
	delay_ns(10);

	WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);

	WriteRawRC(TxAutoReg,0x40);//����Ҫ

	return MI_OK;
}

/********************************************************************
 * �������ܣ�����RC522�Ĺ�����ʽ
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
char M500PcdConfigISOType(uint8 type)
{
	if (type == 'A')                     //ISO14443_A
	{ 
		ClearBitMask(Status2Reg,0x08);
		WriteRawRC(ModeReg,0x3D);//3F
		WriteRawRC(RxSelReg,0x86);//84
		WriteRawRC(RFCfgReg,0x7F);   //4F
		WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);
		delay_ns(1000);
		PcdAntennaOn();
	}
	else{ return -1; }

	return MI_OK;
}

/********************************************************************
 * �������ܣ���������
 * ע�����
 * ��ʾ˵����ÿ��������ر����߷���֮��Ӧ������1ms�ļ��
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
void PcdAntennaOn(void)
{
	uint8 i;
	i = ReadRawRC(TxControlReg);
	if (!(i & 0x03))
	{
		SetBitMask(TxControlReg, 0x03);
	}
}


/********************************************************************
 * �������ܣ��ر�����
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

/********************************************************************
 * �������ܣ���ʼ��RC522�˿�
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
void init_rc522_port( void )
{
	DDRC |= ( BIT0 | BIT1 | BIT2 );	
	DDRC |= BIT5;
	DDRC &= ~BIT3;
}

/********************************************************************
 * �������ܣ���ʼ��RC522ģ��
 * ע�����
 * ��ʾ˵����
 * ��    �룺
 * ��    �أ�
 * *****************************************************************/
void init_rc522(void)
{
	init_rc522_port();
	PcdReset();
	PcdAntennaOff();  
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
}

uint8 getRFID(uint8* str)
{
	if( PcdRequest( PICC_REQIDL, str ) != MI_OK ){
		//Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�
		if( PcdRequest( PICC_REQIDL,str ) != MI_OK ){
			//Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�
			return 0;
		}
	}
	if( PcdAnticoll( str ) != MI_OK ) {
		//����ײ�����ؿ������к� 4�ֽ�
		return 0;//break;
	}
	
	if( PcdSelect( str ) != MI_OK ){//ѡ��
		return 0;//break;
	}
	//...usart
	return 1;
}

