#include <16f1824.h>
#use delay(clock=16000000)
#use RS232(baud=9600, parity=N, xmit=PIN_A0, rcv=PIN_A1)

//set for internal osc
#byte OSCCON=0x099
#byte OSCSTAT=0x09A

//set for tmr1
#byte TMR1L=0x016
#byte TMR1H=0x017
#byte T1CON=0x018
#byte T1GCON=0x019

//set global interrupt
#byte INTCON=0x00B
#byte PIE1=0x091
#byte PIR1=0x011
#bit PEIE=INTCON.6
#bit GIE=INTCON.7
//set for tmr1 interrupt
#bit TMR1IE=PIE1.0
#bit TMR1IF=PIR1.0
//set for UART interrupt
#byte APFCON0=0x11D
#byte TXREG=0x19A
#byte TXSTA=0x19E
#byte RCREG=0x199
#byte RCSTA=0x19D
#byte BAUDCON=0x19F
#byte SPBRGH=0x19C
#byte SPBRGL=0x19B
#bit RCIE=PIE1.5

//set for using INT Pin
#byte OPTION_REG=0x095
#bit INTE=INTCON.4
#bit INTF=INTCON.1
#bit INTEDG=OPTION_REG.6

//set for port control
#byte PORTA=0x00C
#byte TRISA=0x08C
#byte PORTC=0x00E
#byte TRISC=0x08E
#bit colorOff=PORTC.5

long int count;
unsigned long color_val;
unsigned char rcv_key=0;
char rcv_data;
int flag=0;

#int_rda
void rda_isr(){
   rcv_key=1;
   rcv_data=RCREG;
}
#int_timer1
void timer1_isr()
{
   TMR1H=0x3C;
   TMR1L=0xAF;

   count++;
   if(count==20) 
   {
      count=0;
      flag=1;
   }
}
#int_ext
void EXT0()
{
   color_val++;
}
void osc_init(void)
{
   OSCCON=0x7A;
//0 1111-16Mhz 0- 1x-internal
   OSCSTAT=0x19;
// b4->1 = HFINTOSC is ready
// b3->1 = HFINTOSC is at least 2% accurate
// b0->1 = HFINTOSC is at least 0.5% accurate
}
void tmr1_init(void)
{
   T1CON=0x21;
//00 =Timer1 clock source is instruction clock (FOSC/4)
//10 = 1:4 Prescale value
//0 = Dedicated Timer1 oscillator circuit disabled
//00 This bit is ignored.
//1 = Enables Timer1
   T1GCON=0x00;
//dont use gate
   TMR1H=0x3C;
   TMR1L=0xAF;
//<TMR1H:TMR1L>=0000h
//16,000,000/(4*4)=1Mhz, 1000000/20=50000,50ms

   TMR1IE=1;
   TMR1IF=0;
   PEIE=1;
   GIE=1;
}
void int_pin_init(void)
{
   INTEDG=1;         //1 = Interrupt on rising edge of RB0/INT pin
   INTF=0;
   INTE=1;           //1 = Enables the INT external interrupt

//   PEIE=1;
//   GIE=1;
}
void usart_init(void)
{
   TXSTA=0xa0;
//1 = Master mode (clock generated internally from BRG)
//0 = Selects 8-bit transmission
//1 = Transmit enabled
//0 = Asynchronous mode
//0 = Sync Break transmission completed
//0 = Low speed/High Baud Rate Select bit
//00 = 0 = TSR full/9th Bit of Transmit Data
   RCSTA=0x90;
//1 = Serial port enabled (configures RX/DT and TX/CK pins as serial port pins)
//00
//1 = Enables receiver/Continuous Receive Enable bit
//0000
   APFCON0=APFCON0|0x84;         //use pin A0,A1
   BAUDCON=0x00;
   SPBRGH=0x00;
   SPBRGL=0x19;

   RCIE=1;
//   PEIE=1;
//   GIE=1;
}
unsigned long search_color(int color)
{
   unsigned long temp;
   PORTA=color;
   TMR1IE=1;
   INTE=1;
   
   while(!(flag==1));
   
   INTE=0;
   TMR1IE=0;
   flag=0;
   temp=color_val;
   color_val=0;

   return temp;
}
void main()
{
   unsigned long R,G,B;
   unsigned int RED=0x00, GREEN=0x30, BLUE=0x10;
   osc_init();

   TRISA=0x0E;
   PORTA=0x00;          // RED=0x00, GREEN=0x30, BLUE=0x10
   TRISC=0x00;
   PORTC=0x20;

   count=0;
   color_val=0;
   
   usart_init();
   int_pin_init();
   tmr1_init();

   TMR1IE=0;
   INTE=0;
   
   while(TRUE)
   {
      if(rcv_key){
         if(rcv_data=='s')
         {
            rcv_key=0;
            RCIE=0;
            colorOff=0;
            PORTC=PORTC|0x04;
            R=search_color(RED);
            PORTC=PORTC|0x08;
            G=search_color(GREEN);
            PORTC=PORTC|0x10;
            B=search_color(BLUE);
            printf("Color Val: R = %lu, G = %lu, B = %lu\n\r",R,G,B);
            RCIE=1;
            PORTC=0x20;
         }
         else rcv_key=0;
      }
   }
}

