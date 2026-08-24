#include "obstacle_avoidance.h" //�������ͷ�ļ�

unsigned char obstacle_avoidance_flag=0;        //������ϼ���־λ
unsigned char obstacle_avoidance_display_flag=0;//������ϼ����ʾ��־λ

//������ϳ�ʼ��
void obstacle_avoidance_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = VOID_R_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_Init(VOID_R_GPIO , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = VOID_L_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_Init(VOID_L_GPIO , &GPIO_InitStructure);

}


/********************************************************************************************

�������ܣ�������ϼ�ⱨ������
��ڲ�������
����ֵ  ����
˵��    ��1.������ϼ�ⱨ��������ͨ���ж������������ģ���״̬����������С���˶��ͱ�����
          2.���ұ��϶�û�м�⵽              ������С��ǰ�����������رգ���ɫled��  ��
          3.��߱��ϼ�⵽���ұ߱���û�м�⵽������С����ת����������������ɫled��˸��
          4.�ұ߱��ϼ�⵽����߱���û�м�⵽������С����ת����������������ɫled��˸��
          5.���ұ��϶���⵽                  ������С�����ˣ���������������ɫled��˸��

*********************************************************************************************/
void obstacle_avoidance_detector(void)//������ϼ�ⱨ������
{
	 if(VOID_L_IO == BARRIER_N && VOID_R_IO == BARRIER_N)//��·���϶�û�м�⵽
	 {
	   BUZZER_RESET;//�رշ�����
	   LED_RED_SET; //��ɫled��
		 CarGo();     //����С��ǰ��
		}

	 if(VOID_L_IO == BARRIER_Y&&VOID_R_IO== BARRIER_N)//��߱��ϼ�⵽���ұ߱���û�м�⵽��
		{
		  BUZZER_SET;          //����������
	    led_red_frequency(2);//��ɫled��˸Ƶ�ʺ���
	    LED_GREEN_SET;       //��ɫled��
			PutStr2("zhang");
		  CarRight_obstacle_avoidance();//����С����ת
		  delay_ms(100);//��ʱ100����
		}

	 if(VOID_R_IO == BARRIER_Y&&VOID_L_IO == BARRIER_N)//�ұ߱��ϼ�⵽����߱���û�м�⵽��
	 {
			BUZZER_SET;          //����������
			led_red_frequency(2);//��ɫled��˸Ƶ�ʺ���
	    LED_GREEN_SET;       //��ɫled��
			PutStr2("zhang");
			CarLeft_obstacle_avoidance();//����С����ת
			delay_ms(100);//��ʱ100����
		}

	 if(VOID_R_IO == BARRIER_Y&&VOID_L_IO == BARRIER_Y)//��·���϶���⵽
	 {
		 BUZZER_SET;          //����������
	   led_red_frequency(2);//��ɫled��˸Ƶ�ʺ���
	   LED_GREEN_SET;       //��ɫled��
		 CarBack_obstacle_avoidance();//����С������
		 delay_ms(50);//��ʱ50����
		}

	 }


	   void obstacle_avoidance_display_execute(void)//���������ʾ��ִ�к���
	  {

					if(obstacle_avoidance_display_flag==1)//������Ļ��ʾ��־λ
			{


				          sudu=25;//�������Ĭ���ٶ�Ϊ25
					        OLED_Clear();//OLED��ʾ��Ļ����
                    OLED_ShowCH(28,1,"IR");//
                    OLED_ShowCH(3,6,"Speed:");//
	                OLED_ShowCH(110,6,"%");//��ʾ % ����
	                OLED_ShowNum(80,6,sudu,3,0); //��ʾ ��ǰ�����ٶ� ��ֵ
				          obstacle_avoidance_display_flag=0;
				          IrDA=0; //������ֵ���㣬��ֹ����
			}



		if(obstacle_avoidance_flag==1)//ң�ذ���1

			{


			  obstacle_avoidance_detector();//������ϼ�⺯��

			}


	}
