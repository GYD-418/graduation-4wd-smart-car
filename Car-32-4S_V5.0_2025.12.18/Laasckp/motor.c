#include "motor.h"//�������ͷ�ļ�

unsigned int speed_count=0;//ռ�ձȼ����� 100��һ����
unsigned char move_rec_flag=0;//�����־λ 1 �������� 0 û��
unsigned char stop_display_flag=0;//��ͣ��ʾ��־λ
unsigned char Time_10us_motor = 0;//10us����������Ϊ��������Ļ���������
int left_speed_duty=0;  //��ߵ��PWM
int right_speed_duty=0;//�ұߵ��PWM
 
//GPIO���ú���
void MotorGPIO_Configuration(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LEFT_F_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LEFT_F_GPIO, &GPIO_InitStructure);    
	
	GPIO_InitStructure.GPIO_Pin = LEFT_Z_PIN;	
	GPIO_Init(LEFT_Z_GPIO, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_F_PIN;	
	GPIO_Init(RIGHT_F_GPIO, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_Z_PIN;	
	GPIO_Init(RIGHT_Z_GPIO, &GPIO_InitStructure); 
	
}

//����ռ�ձ��������ת��
void CarMove(void)
{   
	
  //����
	if(left_speed_duty > 0)
	{
		if(speed_count < left_speed_duty)
		{
			LEFT_GO;//��ǰ
		}else
		{
			LEFT_STOP;//ֹͣ
		}
	}
	else if(left_speed_duty < 0)
	{
		if(speed_count < (-1)*left_speed_duty)
		{
			LEFT_BACK;//���
		}else
		{
			LEFT_STOP; //ֹͣ
		}
	}
	else                
	{
		 LEFT_STOP;//ֹͣ
	}
	if(left_speed_duty==1)
	{
		LEFT_BRAKE;//ɲ��
	}
	
	//����
	if(right_speed_duty > 0)
	{
		if(speed_count < right_speed_duty)
		{
			RIGHT_GO;//��ǰ
		}else                
		{
			RIGHT_STOP;//ֹͣ
		}
	}
	else if(right_speed_duty < 0)
	{
		if(speed_count < (-1)*right_speed_duty)
		{
			RIGHT_BACK;//���
		}else               
		{
			RIGHT_STOP; //ֹͣ
		}
	}
	else               
	{
		RIGHT_STOP; //ֹͣ
	}
	
	if(right_speed_duty==1)//ɲ��
	{
		RIGHT_BRAKE;//ɲ��
	}

}

//speed˵�����������������15-100 ��Ӧ15%-100%��PWMռ�ձȡ�
//���С�����ӡ�suduΪ��������˳ʱ��ת����suduΪ����������ʱ��ת����
//��ߵ�����������ұ����������ת������һ����

//��ǰ
void CarGo(void)
{
	left_speed_duty=-sudu;
	right_speed_duty=sudu;
	PutStr2("qian");
}

//����
void CarBack(void)
{
  left_speed_duty=sudu;
	right_speed_duty=-sudu;
	PutStr2("hou");
}

//����
void CarLeft(void)
{
	left_speed_duty=sudu;
	right_speed_duty=sudu;
	PutStr2("zuo");
}

//����
void CarRight(void)
{
	left_speed_duty=-sudu;
	right_speed_duty=-sudu;
	PutStr2("you");
}

//����С������ֹͣ
//�������������ĵ�ѹ������������Сֱ��Ϊ��
//����С���Ử��һ��ʱ���ֹͣ
//��ʱ�������оƬΪ����״̬
void CarStop(void) 
{
	left_speed_duty=0;
	right_speed_duty=0;
  
}

//����С��ɲ��ֹͣ
//�������������ĵ�ѹ����������Ϊ��
//����ɲ����С����˲��ֹͣ
void Car_Brake(void)
{
	left_speed_duty=1;
	right_speed_duty=1;
	
}

//����ѭ������
void CarBack_Trailing(void)
{
	left_speed_duty=sudu;
	right_speed_duty=-sudu;

}

//����ѭ������
void CarLeft_Trailing(void)
{
	left_speed_duty=70;
	right_speed_duty=60;

}

//����ѭ������
void CarRight_Trailing(void)
{
	left_speed_duty=-60;
	right_speed_duty=-70;

}


//������Ϻ���
void CarBack_obstacle_avoidance(void)
{
	left_speed_duty=100;
	right_speed_duty=-100;

}

//�����������
void CarLeft_obstacle_avoidance(void)
{
	left_speed_duty=100;
	right_speed_duty=80;

}

//�����������
void CarRight_obstacle_avoidance(void)
{
	left_speed_duty=-80;
	right_speed_duty=-100;

}

//���������ʼ��
void motor_init(void)
{
	MotorGPIO_Configuration(); //GPIO���ú���
	
	CarStop(); //С����ͣ 

	search_flag=0;
	obstacle_avoidance_flag=0;
	speed_measurement_flag=0; 
	ultrasonic_flag=0;  
	
}

		
 void stop_display_execute()//�����ͣ��ʾִ�к���
 {
	 
				if(stop_display_flag==1)//��Ļ��ʾ ��ͣ ��־λ ok��
		{
			          Car_Brake();//С��ɲ��
				        OLED_Clear();	//OLED��Ļ����
			          sudu=90;//�ٶ�����Ϊ90
                OLED_ShowCH(48,1,"��ͣ");		    //��ʾ ��ͣ ����
			          OLED_ShowCH(3,6,"�����ٶ�:");	   //��ʾ �����ٶ� ����
                OLED_ShowCH(110,6,"%");		      //��ʾ % ����
                OLED_ShowNum(80,6,sudu,3,0);    //��ʾ ��ǰ�����ٶ� ��ֵ
			          stop_display_flag=0;//��Ļ��ʾ ��ͣ ��־λΪ��
			          speed_stop=0;//�ٶȲ���������̲�������Ϊ��
			   	      IrDA=0;		        	         
			          CarStop(); //С����ͣ 
		}

 }	


