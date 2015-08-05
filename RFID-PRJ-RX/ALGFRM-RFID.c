//RX
//==============================================================================

//==============================================================================

#include <ALGFRM-RFID.h>
#include <ALGFRM-i2c_Flex_LCD.c>
//#include <ALGFRM-DS3231.c>
#include <ALGFRM-nrf2401.c>
#zero_ram
#fill_rom 0x00

#use fast_io(A)
#use fast_io(B)
#use fast_io(C)                       
#use fast_io(D)
#use fast_io(E)                   
#use fast_io(F)             
#use fast_io(G)

//void clear_usart_receiver();
/*                           
unsigned char s = 00;                    
unsigned char min =26;              
unsigned char hr = 9;    
unsigned char dy = 2;    
unsigned char dt = 8;              
unsigned char mt = 07;                    
unsigned char yr = 15;
short hr_format = _24_hour_format; 
short am_pm = 1;                                                      
*/
//==============================================================================

#org 0x1B02,0x1FFF
void set_CHID(void)
{
   channel=0x50; 
   idH=0xAA;//43605
   idL=0x55;

chXORcod=(idH^idL)^channel;
nrfaddress[0]=idH;
nrfaddress[1]=chXORcod;
nrfaddress[2]=idL;

}
//==============================================================================
#int_RDA
void  RDA_isr(void) 
{
static char data,cc=0;

data=getc();

RFIDmsg[cc]=data;
cc++;
 if(data==0x0D)
 {
    cc=0;
    msgrdy=1;
 }
}


#int_TIMER0
void  TIMER0_isr(void)
{
cpucounter++;
if(cpucounter<100)
IOpin.cpuLED=1;
else
IOpin.cpuLED=0;
if(cpucounter>=1000)
cpucounter=0;

set_timer0(5);
clear_interrupt(int_timer0);
}


#int_EXT2 
void EXT_isr(void)                                
{ 
unsigned int8 c,d;
char ShiftReg;
#bit ShiftRegLSB=ShiftReg.0
set_tris_c(0x82);// RX modul DATA için giriþ yap
IOpin.moduleCLK=0;
//resetcounter=0;
//reinitnrfcounter=0;
//reinitikeycounter=0;

for(d=0;d<PAYLOADSIZE;d++)
{
   for(c=0;c<8;c++)
   {
      ShiftReg=ShiftReg<<1;
      ShiftRegLSB=IOpin.moduleDATA;
      IOpin.moduleCLK=1;
      IOpin.moduleCLK=1;
      IOpin.moduleCLK=0;
      IOpin.moduleCLK=0;
   }

RXBuffer[d]=ShiftReg;
}

IOpin.comled^=1;

//set_CHID();
//init_nrf24();
//IOpin.antled1^=1;
//IOpin.antled2=0;
clear_interrupt(int_ext2);
}

//==============================================================================
/*
unsigned int16 calculate_crc16(unsigned int16 old_crc, unsigned int8 data)
{ 
unsigned int16 crc16bit;
unsigned int16 x; 

x = make8(old_crc,1) ^ data;  //x = ((old_crc>>8) ^ data) & 0xff; 
x ^= x>>4; 
crc16bit = (old_crc << 8) ^ (x << 12) ^ (x <<5) ^ x; 
return crc16bit; 
}
*/

//==============================================================================
void transmit_data(void)
{
int8 a,b;
unsigned char databit;
#bit tempdatabit=databit.7

iopin.modulePWRUP=1;

delay_ms(3);

IOpin.moduleCE=1;//
IOpin.moduleCS=0;//RX Aktif nrf24'e veri yüklenebilir

delay_us(5);

for(a=0;a<ADDRESSSIZE;a++){
databit=nrfaddress[a];

   for(b=0;b<8;b++)
   {
      IOpin.moduleDATA=tempdatabit;
         IOpin.moduleCLK=1;
         IOpin.moduleCLK=1;
           
         IOpin.moduleCLK=0;
         IOpin.moduleCLK=0;
      databit=databit<<1;
   }
}

for(a=0;a<PAYLOADSIZE;a++){
databit=TXBuffer[a];

   for(b=0;b<8;b++)
   {
      IOpin.moduleDATA=tempdatabit;
      IOpin.moduleCLK=1;
      IOpin.moduleCLK=1;
      
      IOpin.moduleCLK=0;
      IOpin.moduleCLK=0;
      databit=databit<<1;
   }
}

IOpin.modulepower=1; //Yükselteciyi aktive et
delay_us(5);
IOpin.moduleDATA=0;
IOpin.moduleCE=0; // Veri iletimine baþla
delay_us(1000);
IOpin.modulepower=0;// Yükselticiyi kapat
IOpin.modulePWRUP=0;
delay_us(13000);
}

//==============================================================================
/*
void show_parameters()
{ 
            
} 
*/
//------------------------------------------------------------------------------
void init_prog(void)
{
setup_wdt(WDT_OFF);
setup_adc_ports(NO_ANALOGS|VSS_VDD);
setup_adc(ADC_OFF);
setup_psp(PSP_DISABLED);                                                
setup_spi(SPI_SS_DISABLED);
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_16|RTCC_8_BIT);// TIMER0
setup_timer_1(T1_DISABLED);
setup_timer_2(T2_DISABLED,0,1);
setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
setup_comparator(NC_NC_NC_NC);                                           
setup_vref(FALSE);
setup_low_volt_detect(FALSE);              
setup_oscillator(OSC_32MHZ);                                        

set_tris_a(0x00);
set_tris_b(0x24); 
set_tris_c(0x80);
set_tris_d(0x00);                                      
set_tris_e(0x15);     
set_tris_f(0x58);
set_tris_g(0x10);
output_a(0x00);
output_b(0x00);
output_c(0x00);
output_d(0x00);
output_e(0x00);
output_f(0x00);
output_g(0x00);
// RF Modul and PA/LNA activation
   IOpin.modulepower=0;
   if(txmode)
   {
   IOpin.moduleCTX=1;
   IOpin.moduleCPS=0;
   }
   else
   {
   IOpin.moduleCTX=0;
   IOpin.moduleCPS=1;
   
   }
   IOpin.modulePWRUP=1;
   
}

//------------------------------------------------------------------------------
void main(void)
{
   unsigned int16 i;
   unsigned char tmpbcd;
   txmode=0;
   init_prog();
   delay_ms (1000);
   lcd_init (); 

EXT_INT_EDGE(2,L_TO_H);
enable_interrupts(INT_EXT2);
enable_interrupts(INT_TIMER0);
enable_interrupts(INT_RDA);

IOpin.buzzer=1;
for(i=0;i<16;i++)
{
   RFIDmsg[i]=' ';
   RXBuffer[i]=0;
}
   lcd_gotoxy(4,1);
   printf(lcd_putc,"ELFATEK");
   for(i=0;i<100;i++)
         {
            IOpin.buzzer=0;
            delay_us(500);
            IOpin.buzzer=1;
            delay_us(500);
         }
   //printf(lcd_putc,"ALGIFarm");
   delay_ms(1000);
   lcd_putc('\f');
   
   
   //show_parameters();
   set_CHID();
   init_nrf24();
   enable_interrupts(GLOBAL);
  
  for(;;)
   {
      //lcd_backlight=ON;
      for(i=1;i<6;i++)
         printf("%d",RXBuffer[i]);
         printf("\n\r");
      for(i=6;i<16;i++)
      if(i==9) printf("_%d",RXBuffer[i]);
         else
         printf("%d",RXBuffer[i]);
         
         printf("\n\r");
      /*
      if(msgrdy)
      {
         lcd_backlight=ON;
         IOpin.errorLED=1;
         set_CHID();
         init_nrf24();
         for(i=0;i<100;i++)
         {
            IOpin.buzzer=0;
            delay_us(500);
            IOpin.buzzer=1;
            delay_us(500);
         }
         
          lcd_putc('\f');
          lcd_gotoxy(1,1);
          printf(lcd_putc,"ID:");
         for(i=0;i<16;i++)
         {
            lcd_gotoxy(1+i,2);
            printf(lcd_putc,"%c",RFIDmsg[i]);
         }
         IOpin.errorLED=0;
         delay_ms(1000);
         lcd_gotoxy(1,2);
         lcd_putc('\f');
          
               
               for(i=6;i<16;i++) RFIDmsg[i]-=48;//nein
               
               TXBuffer[0]=0;
               TXBuffer[1]=hr;
               TXBuffer[2]=min;
               TXBuffer[3]=dt;
               TXBuffer[4]=mt;
               TXBuffer[5]=yr;
               TXBuffer[6]=RFIDmsg[0];// country code
               TXBuffer[7]=RFIDmsg[1];// country code
               TXBuffer[8]=RFIDmsg[2];// country code
               
               tmpbcd=RFIDmsg[4]<<4||RFIDmsg[5];
               TXBuffer[9]=tmpbcd;//ID
               
               tmpbcd=RFIDmsg[6]<<4||RFIDmsg[7];
               TXBuffer[10]=tmpbcd;
               
               tmpbcd=RFIDmsg[8]<<4||RFIDmsg[9];
               TXBuffer[11]=tmpbcd;
               
               tmpbcd=RFIDmsg[10]<<4||RFIDmsg[11];
               TXBuffer[12]=tmpbcd;
               
               tmpbcd=RFIDmsg[12]<<4||RFIDmsg[13];
               TXBuffer[13]=tmpbcd;
               
               tmpbcd=RFIDmsg[14]<<4||RFIDmsg[15];
               TXBuffer[14]=tmpbcd;
               transmit_data();
               msgrdy=0;
               
               
}*/
              
               /*
               if(s!=sec)
               show_parameters();
               delay_ms(2000);
               */
               lcd_backlight=OFF;
               //sleep();
         
   }
   
}   
