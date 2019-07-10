#include <msp430x16x.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define DOUTDATA (P2IN&BIT0)

#define channel1        P3OUT &= ~(BIT0+BIT1)
#define channel2        (P3OUT |= BIT1)&&(P3OUT &= ~BIT0)
#define channel3        (P3OUT |= BIT0)&&(P3OUT &= ~BIT1)
#define channel4        P3OUT |= BIT0+BIT1

#define SCK_CLR	        P2OUT &= ~BIT1            //SCK�õ�
#define SCK_SET	        P2OUT |= BIT1             //SCK�ø�

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))

 uint adc1,adc2,adc3,adc4;
uint con_data[15];
uchar integerl1,integerh1,decimalh1,decimall1;
uchar integerl2,integerh2,decimalh2,decimall2;
uchar integerl3,integerh3,decimalh3,decimall3;
uchar integerl4,integerh4,decimalh4,decimall4;
void Port_Init()
{
  //P3SEL=0x00;
 // P3DIR=0x0F;
  P3SEL&= ~(BIT0 + BIT1);
  P3DIR|=BIT0 + BIT1;
  P2SEL=0x00;
  P2DIR|=BIT1;
  P2DIR&=~BIT0;
 SCK_CLR;
}


void delay2055()
{
  unsigned long i=4011;
  while(i!=0)
  {
    i--;
  }
}

void delay368()
{
  unsigned long i=736;
  while(i!=0)
  {
    i--;
  }
}

void delay12()
{
  unsigned int i=24;
  while(i!=0)
  {
    i--;
  }
}

void delay()
{
  uchar i=1;
    i--;
}

uint ADC()
{ 
 // channel1;
   SCK_CLR;
  uchar i,j,p,q;
  uint adc,temp;
  delay2055();
  for(j=0;j<15;j++)
  {
   delay();
  for(i=0;i<=15;i++)
  {
    SCK_SET;
      delay();
       if(DOUTDATA)   
    {con_data[j]++;}
      SCK_CLR;
     delay();
     con_data[j]<<=1; 
  }
  for(i=16;i<=23;i++)
  {
     SCK_SET;
     SCK_CLR;
  }
  delay368();
  while(DOUTDATA);
  delay12();
  }
  for(p=0;p<14;p++)
  {
    for(q=0;q<14-p;q++) {
      if(con_data[q] > con_data[q+1])
      {
        temp=con_data[q];
        con_data[q]=con_data[q+1];
        con_data[q+1]=temp;
      }
    }
  }
  adc=con_data[7];
  return adc;

}


void WDT_Init()
{
   WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
}

void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}

void UART_Init()
{
  U0CTL|=SWRST;               //��λSWRST
  U0CTL|=CHAR;                //8λ����ģʽ 
  U0CTL&=~SPB;
  U0TCTL|=SSEL1;              //SMCLKΪ����ʱ��
  U0BR1= baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0= baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                //΢���Ĵ���Ϊ0��������9600bps
  /*U0BR0=0x68;
  U0BR1=0x00;
  U0MCTL=0x02;*/
  ME1|=UTXE0;                 //UART0����ʹ��
  ME1|=URXE0;                 //UART0����ʹ��
  
  U0CTL&=~SWRST;
  IE1|=URXIE0;
  P3SEL|= BIT4;               
 // P3DIR|= BIT4;               //����IO�ڷ���Ϊ���
  P3SEL|= BIT5;
//  P3DIR&= ~BIT5; 
  
   P3SEL|= BIT6;               
 // P3DIR|= BIT6;               //����IO�ڷ���Ϊ���
  P3SEL|= BIT7;
 // P3DIR&= ~BIT7; 
  U1CTL|=SWRST;               //��λSWRST
  U1CTL|=CHAR;                //8λ����ģʽ 
  U1CTL&=~SPB;
  U1TCTL|=SSEL1;              //SMCLKΪ����ʱ��
  U1BR1= baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U1BR0= baud_l;               //N=UBR+(UxMCTL)/8
  U1MCTL=0x00;                //΢���Ĵ���Ϊ0��������9600bps
  ME2|=UTXE1;                 //UART1����ʹ��
  ME2|=URXE1;                 //UART1����ʹ��
  
  U1CTL&=~SWRST;
  IE2|=URXIE1;
 
}

void Send_com(uchar data)
{
  while((IFG1&UTXIFG0)==0);          //���ͼĴ����յ�ʱ��������
    U0TXBUF=data;
}


void Send_Byte(uchar data)
{
  while((IFG2&UTXIFG1)==0);          //���ͼĴ����յ�ʱ��������
    U1TXBUF=data;
}

#pragma vector=UART1RX_VECTOR
__interrupt void UART1_RX_ISR(void)
{
  
       Send_Byte(integerh1);
       Send_Byte(integerl1);
       Send_Byte(decimalh1);
       Send_Byte(decimall1);
       Send_Byte(integerh2);
       Send_Byte(integerl2);
       Send_Byte(decimalh2);
       Send_Byte(decimall2);
       Send_Byte(integerh3);
       Send_Byte(integerl3);
       Send_Byte(decimalh3);
       Send_Byte(decimall3);
       Send_Byte(integerh4);
       Send_Byte(integerl4);
       Send_Byte(decimalh4);
       Send_Byte(decimall4);
    
}

       
       
      
    


void main(void)
{
  WDT_Init();                                   //���Ź�����
  Clock_Init();                                 //ϵͳʱ������
  Port_Init();                                  //ϵͳ��ʼ��������IO������   
  UART_Init();
  _EINT();                                       //���ж�
 float dianzu,temp;
 uint integer,decimal;
 
  while(1)
  {  
         _DINT();
        channel1;                                      //����ͨ��
       adc1=ADC();                                         //��ʼ����15���㣬��ȡ��λֵ
        _EINT();
       dianzu=(32767.5/adc1)-1;                        //�����ֵ
       dianzu=dianzu*5000;
       integer=(uint)dianzu;                         //ȡ����
       temp=(dianzu-integer)*10000;
       decimal=(uint)temp;                          //ȡС��
       integerh1=BYTE1(integer);                     //���������С�����ֵĸߵ��ֽ�
       integerl1=BYTE0(integer);
       decimalh1=BYTE1(decimal);
       decimall1=BYTE0(decimal);
     
       Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x00);
       Send_com(integerh1);
       Send_com(integerl1);
  
       Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x04);
       Send_com(decimalh1);
       Send_com(decimall1);
       
       
        _DINT();
       channel2;                                      //����ͨ��
       adc2=ADC();                                         //��ʼ����15���㣬��ȡ��λֵ
        _EINT();
       dianzu=(32767.5/adc2)-1;                        //�����ֵ
       dianzu=dianzu*2000;
       integer=(uint)dianzu;                         //ȡ����
       temp=(dianzu-integer)*10000;
       decimal=(uint)temp;                          //ȡС��
       integerh2=BYTE1(integer);                     //���������С�����ֵĸߵ��ֽ�
       integerl2=BYTE0(integer);
       decimalh2=BYTE1(decimal);
       decimall2=BYTE0(decimal);

           Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x08);
       Send_com(integerh2);
       Send_com(integerl2);
  
       Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x0C);
       Send_com(decimalh2);
       Send_com(decimall2);
       
       
       
        _DINT();
       channel3;                                      //����ͨ��
       adc3=ADC();                                         //��ʼ����15���㣬��ȡ��λֵ
        _EINT();
       dianzu=(32767.5/adc3)-1;                        //�����ֵ
       dianzu=dianzu*15000;
       integer=(uint)dianzu;                         //ȡ����
       temp=(dianzu-integer)*10000;
       decimal=(uint)temp;                          //ȡС��
       integerh3=BYTE1(integer);                     //���������С�����ֵĸߵ��ֽ�
       integerl3=BYTE0(integer);
       decimalh3=BYTE1(decimal);
       decimall3=BYTE0(decimal);
       
        Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x10);
       Send_com(integerh3);
       Send_com(integerl3);
  
       Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x14);
       Send_com(decimalh3);
       Send_com(decimall3);
       
       
        _DINT();
       channel4;                                      //����ͨ��
       adc4=ADC();                                         //��ʼ����15���㣬��ȡ��λֵ
        _EINT();
       dianzu=(32767.5/adc4)-1;                        //�����ֵ
       dianzu=dianzu*5000;
       integer=(uint)dianzu;                         //ȡ����
       temp=(dianzu-integer)*10000;
       decimal=(uint)temp;                          //ȡС��
       integerh4=BYTE1(integer);                     //���������С�����ֵĸߵ��ֽ�
       integerl4=BYTE0(integer);
       decimalh4=BYTE1(decimal);
       decimall4=BYTE0(decimal);
 
        Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x18);
       Send_com(integerh4);
       Send_com(integerl4);
  
       Send_com(0xA5);
       Send_com(0x5A);
       Send_com(0x05);
       Send_com(0x82);
       Send_com(0x00);
       Send_com(0x1C);
     Send_com(decimalh4);
       Send_com(decimall4);
       
  }
  
  
}

