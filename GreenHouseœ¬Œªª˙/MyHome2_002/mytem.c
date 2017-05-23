
#include <avr/io.h>
#include <util/delay.h>
#include  "common.h"
#include  "mytem.h"

  
int T3;

//΢�뼶��ʱ
void delayus(uchar del)
{
	while(del--);
}
 

/********************************************************************
 * �������ܣ������¶ȴ�����LM75
 * ע�����
 * ��ʾ˵�������ڲ���TWI(IIC)���ߵ���ʼ�ź�
 * *****************************************************************/
void	init_lm75(void)
{
	//�������ŷ���
	SETBIT(DDRC,LM75_SDA);	  //����IO λ���
	SETBIT(DDRC,LM75_SCL);
	//����״̬ SDA��SCL��Ϊ�ߵ�ƽ
	SETBIT(PORTC,LM75_SDA);	  //���  ��1
	_delay_us(10);
	SETBIT(PORTC,LM75_SCL);
	_delay_us(10);
	//������ʼ�źţ�ʱ�Ӹߵ�ƽʱ�����½���
	CLRBIT(PORTC,LM75_SDA); //���  ��0
	_delay_us(20);
	CLRBIT(PORTC,LM75_SCL);
	_delay_us(20);
}


/********************************************************************
 * �������ܣ���Ƭ������һ�ֽ����ݸ�LM75
 * *****************************************************************/
uchar sendbyte_lm75(uchar buf)
{
	uchar i = 0;
	SETBIT(DDRC,LM75_SDA);//����IOΪ���
	SETBIT(DDRC,LM75_SCL);				
	//�����ݴ������
	for(i = 8;i > 0;i--){
		CLRBIT(PORTC,LM75_SCL);
		_delay_us(20);
		if(buf & 0x80)		 //bit7 == 1
			SETBIT(PORTC,LM75_SDA);  		  
		else					 //bit7 == 0
			CLRBIT(PORTC,LM75_SDA);
		buf <<= 1;
		SETBIT(PORTC,LM75_SCL);
		_delay_us(20);
	}
	//�ȴ�lm75����Ӧ���ź�
	CLRBIT(PORTC,LM75_SCL);
	_delay_us(20);//wait for ack
	//�ͷ�������
	SETBIT(PORTC,LM75_SDA);
	_delay_us(5);
	//�޸�SDA����Ϊ���룬׼������Ӧ���ź�
	CLRBIT(DDRC,LM75_SDA);	//set SDA  input	
	SETBIT(PORTC,LM75_SCL);		
	_delay_us(20);

	if(GETBIT(PINC,LM75_SDA)){//fail
		SETBIT(DDRC,LM75_SCL);
		CLRBIT(PORTC,LM75_SCL);
		return 0xEE;
	}else{//�ɹ����� ��ַ
		SETBIT(DDRC,LM75_SCL);
		CLRBIT(PORTC,LM75_SCL);
		return LM75_RD_ADDRESS;
	}

}


/****************************************************************
 * �������ܣ���Ƭ����LM75��ȡһ�ֽ����ݺ���
 * **************************************************************/
uchar getbyte_lm75(void)
{
	uchar i = 0,buf = 0;
	CLRBIT(DDRC,LM75_SDA);//��������
	SETBIT(DDRC,LM75_SCL);//ʱ�����
			 
	for(i = 0;i < 8;i++){

		/*��ʱ����Ϊ�ͣ�׼����������λ*/
		CLRBIT(PORTC,LM75_SCL);
		delayus(20);

		/*��ʱ����Ϊ��ʹ��������������Ч*/
		SETBIT(PORTC,LM75_SCL);
		delayus(10);
		buf <<= 1;

		/*������λ,���յ�����λ����buf��*/
		if(GETBIT(PINC,LM75_SDA)){
			buf += 1;
			delayus(10);
		}
		CLRBIT(PORTC,LM75_SCL);		
	}
	return buf;
}



/*****************************************************************
 * �������ܣ���Ƭ������ֹͣ�ź�
 * **************************************************************/
void stop_lm75(void)
{
	//����IO����
	SETBIT(DDRC,LM75_SDA);
	SETBIT(DDRC,LM75_SCL);
	//��ʼ�ź���Ϊ��
	CLRBIT(PORTC,LM75_SDA);
	delayus(20);
	//����ֹͣ�ź�
	SETBIT(PORTC,LM75_SCL);
	delayus(20);
	SETBIT(PORTC,LM75_SDA);
	delayus(20);
}


/****************************************************************
 * �������ܣ���Ƭ������Ӧ���ź� 
 * **************************************************************/
void ack_lm75(uchar ack)
{
	SETBIT(DDRC,LM75_SDA);
	SETBIT(DDRC,LM75_SCL);

	if(ack)
		CLRBIT(PORTC,LM75_SDA);
	else
		SETBIT(PORTC,LM75_SDA);

	delayus(8);
	SETBIT(PORTC,LM75_SCL);
	delayus(20);
	CLRBIT(PORTC,LM75_SCL);
	delayus(10);
}


/****************************************************************
 * �������ܣ���Ƭ����ȡ�¶�ֵ
 * **************************************************************/
uchar temperature_lm75(void)
{
 	uchar temp[2];
	uchar i;

	//������ʼ�ź�
	init_lm75();

	//����LM75�ĵ�ַ:��дλ 1001 A2 A1 A
	T3=sendbyte_lm75(0x95) ;

    //�����ֽ�
	temp[1] = getbyte_lm75();
	ack_lm75(1);

    //С���ֽ�
	temp[0] = getbyte_lm75();
	ack_lm75(0);
  
    //ֹͣ
	stop_lm75();


	i = 0;
	i = temp[1]&0x80;    //1000 0000  i�õ�temp[1]�ĵ�8λ
	temp[1] <<= 1;      //����һλ
	temp[1] |= (temp[0]>>7); //��temp[0]�����λ ��Ϊ temp[1]�����λ

	if(i){//����ת��
		temp[1] = ~temp[1]+1;
	}
   
    //�����������������ϲ㴦��
    //�����ֽڳ��� 0.5 ��������ʧ����
   // temp[1]=temp[1]>>1;

    //װ����10������
	//temp[1] = Hex_Decimal(temp[1]);

	return temp[1];
 
}
