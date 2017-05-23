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
	//全局中断使能
    sei(); 
	
 
	while(1){

		CMD = getch();//
 
		 /* 
		     响应包              frame[0]   frame[1]     frame[2]                            frame[3]
		                           head     sensorID     数据位1                              校验位

	温度		#define TEM         F8       0x31    温度=frame[2]*0.5                  frame[0]异或frame[1]异或frame[2]异或frame[3]
	温湿度		#define DHT         F8       0x32    湿度=frame[2]  
                             
	震动		#define VIB         F8       0x33     有：FF  无：11         
	红外光源	#define INF         F8       0x34     有：FF  无：11 
	光敏		#define LIGHT       F8       0x35     有：FF  无：11    
	     
	响铃		#define BELL               
	电机顺转	#define MOTOR_SHUN          
	电机逆转	#define MOTOR_NI              
    LED         #define LED    	
	    
         */



		if(TEM == CMD){   //温度       #define TEM                0x31          成功 十六进制显示 需要乘以 0.5

	        uchar temp=0;			
            temp=temperature_lm75();
 
            frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = TEM;
			frame[FRAME_DATA_1] = temp;
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);

		}

		else if(DHT  == CMD){     //温湿度——湿度      #define DHT1_TEM           0x32          //似乎准确的
		
           
            get_DHT11_value();
			
			frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = DHT;
			frame[FRAME_DATA_1] = hum_h;
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
		
		}

		else if(VIB == CMD){  //震动传感器      #define VIB                0x33         成功

            frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = VIB;
			frame[FRAME_DATA_1] = get_vib();
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
 
		}

		else if(INF == CMD){   //热源红外        #define INF                0x34       灵敏度有点拿捏不准 弃用

            frame[FRAME_HEAD] = 0xF8;
			frame[FRAME_SENSORID] = INF;
			frame[FRAME_DATA_1] = get_inf();
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
		
		}

		else if(LIGHT == CMD){  //光敏         #define LIGHT              0x35      灵敏度已调好

            frame[FRAME_HEAD] = 0xF8;
            frame[FRAME_SENSORID] = LIGHT;
			frame[FRAME_DATA_1] = get_light();
			frame[FRAME_CHECK] = check(frame);

			putframe(frame);
		
		}
		else if(BELL == CMD){   //蜂鸣器        #define BELL               0x36                       //成功

				bell(1);
		
		}

		else if(MOTOR_SHUN == CMD){  //电机顺时针转      #define MOTOR_SHUN         0x37                 //成功
			
	
			motor_shun(MOTOR_SHUN);
		        		
		}

		else if(MOTOR_NI == CMD){    //电机逆时针转       #define MOTOR_NI           0x38              //成功
			
			motor_ni(MOTOR_NI);

		}

        else if(LED == CMD){       //流水灯闪烁          #define LED                0x39            //成功
		
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
