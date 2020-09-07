#include <16F1824.h>
#include <stdlib.h>
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=16000000)
#use I2C(master, sda=PIN_C1, scl=PIN_C0,Fast=400000)
#use RS232(baud=9600, parity=N, xmit=PIN_C4, rcv=PIN_C5)
#byte PORTA = 0x00c
#byte TRISA = 0x08c 
#byte PORTC = 0x00E
#byte TRISC = 0x08E
#bit SCL = TRISC.0
#bit SDA = TRISC.1
#bit TLED = TRISC.2
#bit TSW = TRISC.3
#bit LED = PORTC.2
#bit SW = PORTC.3

//set for oscillator
#byte OSCTUNE = 0x098
#byte OSCCON = 0x099
#byte OSCSTAT = 0x09A

//set for tmr1
#byte TMR1L=0x016
#byte TMR1H=0x017
#byte T1CON=0x018
#byte T1GCON=0x019

//set for tmr1 interrupt
#byte INTCON=0x00B
#byte PIE1=0x091
#byte PIR1=0x011

#bit TMR1IE=PIE1.0
#bit PEIE=INTCON.6
#bit GIE=INTCON.7
#bit TMR1IF=PIR1.0

#define MassageSize 4
char key;
int id=0,st=0,i=0;
int C[3]={0};
int R[3]={0};
int G[3]={0};
int B[3]={0};
char SR=0,SG=0,SB=0;
char SendMassage[MassageSize+1]={0};

long Red=0;
long Green=0;
long Blue=0;
long Clear=0;
long int count=0;
int flag=0,start=0;
char debug=0;
#int_timer1
void timer1_isr()
{
   TMR1H=0x3C;
   TMR1L=0xAF;
   count++;
   if(count==20) 
 start=1;
   else if(count==40){
		count=0;
		LED=0;
		flag=1;
	}
}	
void write_tcs(int address, int data) {
   //SDA=0;
   i2c_start();                           // 스타트 비트를 보냄
   i2c_write(0x52);                    // 컨트롤 바이트 0xA0는 쓰기 모드
   i2c_write(address|0x80);               // 주소를 설정
   i2c_write(data);                    // 데이터를 쓴다.
   i2c_stop();                           // 데이터 전송을 마침
}

int read_tcs(int address){
	int data;
	//SDA=0;
   i2c_start();                           // 스타트 비트 보냄
   i2c_write(0x52);                    // 컨트롤 바이트 쓰기 모드 0xa0
   i2c_write(address|0x80);              //  읽어들일 주소를 보냄
   i2c_start();                          //  다시 스타트 비트 보냄
   i2c_write(0x53);
   //SDA=1;                   //  컨트롤 바이트 읽기 모드 0xa1
   data=i2c_read(0);                 //  데이터를 읽어들임 ACK 확인 하지 않음
   i2c_stop();                          //  스톱 비트 보냄
   return(data);
}

void osc_init()
{
      OSCCON   = 0x7a;  //0111 1010;
      OSCSTAT  = 0x19;  //1001 1001;
      //OSCTUNE  = 0x1f;  //0001 1111;
	  
	  //SCL=0;
	  //SDA=0;
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
   //TMR1IF=0;
   PEIE=1;
   GIE=1;
}


void color_sensor(){
	if(debug=='1')
	printf("start\n\r");
	
	write_tcs(0x00,0x03); //enable
	write_tcs(0x01,0xff);
	write_tcs(0x03,0xff);
	write_tcs(0x0C,0x00);
	write_tcs(0x0D,0x00);
	write_tcs(0x0F,0x01);

	id=read_tcs(0x12);
	st=read_tcs(0x13);
	
	C[0]=read_tcs(0x14);
	C[1]=read_tcs(0x15);
	
	R[0]=read_tcs(0x16);
	R[1]=read_tcs(0x17);

	G[0]=read_tcs(0x18);
	G[1]=read_tcs(0x19);

	B[0]=read_tcs(0x1A);
	B[1]=read_tcs(0x1B);
	i2c_stop();
	//Clear= ((long)(C[1])<<8)+C[0];
	Red= ((long)(R[1])<<8)+R[0];
	Green = ((long)(G[1])<<8)+G[0];
	Blue = ((long)(B[1])<<8)+B[0];
	start=0;
	SR=(char)((Red<<6)/2048); //5bit Red color
	SG=(char)((Green<<6)/1024); //6bit Green color
	SB=(char)((Blue<<6)/2048); //5bit Blue color
	if(debug=='1')
	printf("color sensor finished\n\r");		
}
void color_clear(){
	C[0]=0;
	R[0]=0;
	G[0]=0;
	B[0]=0;
	C[1]=0;
	R[1]=0;
	G[1]=0;
	B[1]=0;
	Red=0;
	Green=0;
	Blue=0;
	id=0;
	st=0;
}

void main(){

	
	osc_init();
	TLED=0;
	TSW=1;
	flag=0;
	printf("do you want to use debuging ? 1.yes 0.no\n\r ");
	debug=getch();
	LED=0;
	//if(debug=='1')
	//printf("hello world\n\r");	
	
	while(1){
				if(SW==1){
					SW=0;
					LED=1;
					//printf("LED ON\n\r");
					tmr1_init();
					if(start==1){
					color_sensor();
					color_clear();
					color_sensor();
	SendMassage[0]='s';
	SendMassage[1]=(SR<<3)+((SG&0x38)>>3;		SendMassage[2]=(SG&0x07<<5)+SB;
	SendMassage[3]='p';
for(i=0; i<MassageSize; i++)					printf("%c ",SendMassage[i]);
					if(debug=='1'){		printf("==========================\n\r");		printf("id=%x st=%x ",id,st);
printf("Red=0x%lxGreen=0x%lxBlue=0x%lx\n\r",
Red,Green,Blue);		
}				color_clear();
	}
					delay_ms(500);
	}	 
	
if(flag==1)
	{
	TMR1IE=0;
  	//TMR1IF=0;
  	PEIE=0;
  	GIE=0;
	flag=0;
	}
}	
}

