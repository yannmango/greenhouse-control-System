#include <avr/io.h>
#include <util/delay.h>
#include <avr/iom162.h>
#include <avr/interrupt.h>

#include "myDHT11.h"

#include "common.h"

FRAME frame[4];
uchar CMD;

uchar led_count = 0;
extern uint8	hum_h;

int main(void){
	
	CLKPR = (1<<CLKPCE);
	CLKPR = 0x00;
	DDRA  = 0xFF;
 
	usart_init(  ); 
	//ȫ���ж�ʹ��
    sei(); 
	
 
	while(1){

		CMD = getch();//
 
		 /* 
		     ��Ӧ��              frame[0]   frame[1]     frame[2]                            frame[3]
		                           head     sensorID     ����λ1                              У��λ

	�¶�		#define TEM         F8       0x31    �¶�=frame[2]*0.5                  frame[0]���frame[1]���frame[2]���frame[3]
	��ʪ��		#define DHT         F8       0x32    ʪ��=frame[2]  
                             
	��		#define VIB         F8       0x33     �У�FF  �ޣ�11         
	�����Դ	#define INF         F8       0x34     �У�FF  �ޣ�11 
	����		#define LIGHT       F8       0x35     �У�FF  �ޣ�11    
	     
	����		#define BELL               
	���˳ת	#define MOTOR_SHUN          
	�����ת	#define MOTOR_NI              
    LED         #define LED    	
	    
         */



		if(TEM == CMD){   //�¶�       #define TEM                0x31          �ɹ� ʮ��������ʾ ��Ҫ���� 0.5

	        uchar temp=0;			
            temp=temperature_lm75();
 
            frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = TEM;
			frame[FRAME_DATA_1] = temp;
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);

		}

		else if(DHT  == CMD){     //��ʪ�ȡ���ʪ��      #define DHT1_TEM           0x32          //�ƺ�׼ȷ��
		
           
            get_DHT11_value();
			
			frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = DHT;
			frame[FRAME_DATA_1] = hum_h;
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
		
		}

		else if(VIB == CMD){  //�𶯴�����      #define VIB                0x33         �ɹ�

            frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = VIB;
			frame[FRAME_DATA_1] = get_vib();
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
 
		}

		else if(INF == CMD){   //��Դ����        #define INF                0x34       �������е�����׼ ����

            frame[FRAME_HEAD] = 0xF8;
			frame[FRAME_SENSORID] = INF;
			frame[FRAME_DATA_1] = get_inf();
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
		
		}

		else if(LIGHT == CMD){  //����         #define LIGHT              0x35      �������ѵ���

            frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = LIGHT;
			frame[FRAME_DATA_1] = get_light();
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
		
		}
		else if(BELL == CMD){   //������        #define BELL               0x36                       //�ɹ�

				bell(1);
		
		}

		else if(MOTOR_SHUN == CMD){  //���˳ʱ��ת      #define MOTOR_SHUN         0x37                 //�ɹ�
			
	
			motor_shun(MOTOR_SHUN);
		        		
		}

		else if(MOTOR_NI == CMD){    //�����ʱ��ת       #define MOTOR_NI           0x38              //�ɹ�
			
			motor_ni(MOTOR_NI);

		}

        else if(LED == CMD){       //��ˮ����˸          #define LED                0x39            //�ɹ�
		
		    /* 
		     led_test(led_count);
			 led_count++;
			 if(led_count==8)
			 {
			    led_count=0;
			 } */

             led_test(led_count);


		}

	    else{
		
		    continue;
        }
	}

}
