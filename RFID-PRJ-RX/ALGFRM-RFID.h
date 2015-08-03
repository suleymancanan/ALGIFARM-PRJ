#include <18F6310.h>
#device adc=8
#FUSES WDT64                    
#FUSES INTRC_IO                  //High speed osc with HW enabled 4X PLL
#FUSES NOBROWNOUT                //Reset when brownout detected
#FUSES PUT                       //No Power Up Timer
#FUSES NOSTVREN                  //Stack full/underflow will cause reset
#FUSES NODEBUG                   //No Debug mode for ICD
#FUSES NOIESO                    //Internal External Switch Over mode enabled
#FUSES NOFCMEN                   //Fail-safe clock monitor enabled
#FUSES NOXINST                   //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PROTECT                   //Code not protected from reading
#FUSES NOLPT1OSC                 //Timer1 configured for low-power operation
#FUSES NOMCLR                    //Master Clear pin disabled                  

#use delay(clock=32000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8, ERRORS)
#use i2c(Master,Fast=400000,sda=PIN_F3,scl=PIN_F2,force_sw)

struct IOpins {
           BOOLEAN five;       //A0  0          
           BOOLEAN six;        //A1  0            
           BOOLEAN CD_2;       //A2  0
           BOOLEAN antled2;    //A3  0
           BOOLEAN antled1;    //A4  0
           BOOLEAN E1;         //A5  0
           BOOLEAN F1;         //A6  0
           BOOLEAN unused1;    //A7  0
           BOOLEAN moduleCLK;     //B0  0
           BOOLEAN three;      //B1  0        
           BOOLEAN moduleDR1;      //B2  1    
           BOOLEAN four;       //B3  0 
           BOOLEAN AB_2;       //B4  0
           BOOLEAN tandem;     //B5  0
           BOOLEAN comLED;     //B6  0
           BOOLEAN moduleCTX;        //B7  0  
           BOOLEAN cpuLED;     //C0  0
           BOOLEAN moduleDATA;     //C1  0
           BOOLEAN moduleCS;       //C2  0 
           BOOLEAN moduleCE;       //C3  0
           BOOLEAN modulePWRUP;    //C4  0
           BOOLEAN moduleCPS;        //C5  0   
           BOOLEAN rs232Tx;    //C6  0
           BOOLEAN rs232Rx;    //C7  1
           BOOLEAN down1;      //D0  0
           BOOLEAN up1;        //D1  0
           BOOLEAN right1;     //D2  0
           BOOLEAN left1;      //D3  0
           BOOLEAN two;        //D4  0
           BOOLEAN one;        //D5  0
           BOOLEAN down2;      //D6  0
           BOOLEAN up2;        //D7  0  
           BOOLEAN antennaselect;   //E0  0      
           BOOLEAN LR_2;            //E1  0       
           BOOLEAN safetyfeedback2; //E2  0 
           BOOLEAN NS_2;            //E3  0
           BOOLEAN safetyfeedback1; //E4  0
           BOOLEAN errorLED;        //E5  0
           BOOLEAN runrelay;        //E6  0
           BOOLEAN horn;            //E7  0  
           BOOLEAN buzzer;        //F0  0      
           BOOLEAN AUXrelay;            //F1  0       
           BOOLEAN e2promSCL;       //F2  0 
           BOOLEAN e2promSDA;       //F3  1
           BOOLEAN iKeystatus;      //F4  1
           BOOLEAN EF_2;            //F5  0
           BOOLEAN teachbutton;     //F6  0
           BOOLEAN modulepower;     //F7 
           BOOLEAN emergencyLED;    //G0  0    
           BOOLEAN SW01      ;      //G1  0    
           BOOLEAN start2;          //G2  0 
           BOOLEAN start1;          //G3  0                     
           BOOLEAN ecomidselect;    //G4  0  
           //BOOLEAN unused1;       //G5
           //BOOLEAN unused2;       //G6
           //BOOLEAN unused3;       //G7
} IOpin;
#byte IOpin=0xF80

union {
         unsigned int8 buffer[4];
         struct {
                  int1 up1;
                  int1 down1;               
                  int1 left1;        
                  int1 right1;                         
                  int1 up2;
                  int1 down2; 
                  int1 LR_2;
                  int1 unused0;
                  int1 one;
                  int1 two;               
                  int1 three;        
                  int1 four;                         
                  int1 five;
                  int1 six; 
                  int1 seven;
                  int1 eight;
                  int1 p1;
                  int1 p2;
                  int1 p3;       
                  int1 p4;
                  int1 p5;
                  int1 p6;
                  int1 p7;
                  int1 p8;
                  int1 SW01; 
                  int1 SW02; 
                  int1 AUX1; 
                  int1 unused1; 
                  int1 unused2; 
                  int1 horn; 
                  int1 unused3; 
                  int1 buttonpressed;
         }bit;
} data;

struct timer {
                 unsigned int16 count;
                 unsigned int16 setvalue;
                 unsigned int16 time;
                 unsigned int16 timesetvalue;
                 unsigned int8 setONvalue;
                 unsigned int8 setOFFvalue;
                 int1 overflow;
                 int1 timeoverflow;
                 int1 enable;
                 int1 toggle;
             };
             
#define ADDRESSSIZE 3
#define PAYLOADSIZE 15           
unsigned char channel;
unsigned char idH,idL,chXORcod;
unsigned char RFIDmsg[17];
short msgrdy,txmode=1;
unsigned char databit;
#bit tempdatabit=databit.7
unsigned char TXBuffer[PAYLOADSIZE];
unsigned char RXBuffer[PAYLOADSIZE];
unsigned char nrfaddress[ADDRESSSIZE];
unsigned int8 config_setup[14];
//unsigned int16 emgcounter=0,signalledcounter=0,sleepcounter=0;
char ShiftReg;
#bit ShiftRegLSB=ShiftReg.0
#byte RCREG = 0x1A //6310 da dresse bak
unsigned int16 cpucounter;
unsigned char sec;

