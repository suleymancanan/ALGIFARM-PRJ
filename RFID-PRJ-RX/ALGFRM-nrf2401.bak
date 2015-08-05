/******************************************************************************/
void init_nrf24(void)
{
unsigned int i, j, temp;
unsigned int config_setup[14];
IOpin.modulepower=0;
delay_ms(10);
#bit tempbit=temp.7

//RXDATA pinini çýkýþ olarak tanýmla
set_tris_c (0x80);  
//portCpin.RXPWRUP=1;
//delay_ms(3);
IOpin.moduleCE=0;
IOpin.moduleCS=1;
   
    // nrf24 konfigürasyon dizisinin kurulmasý
    //===================================================================
    //111-104 data bitleri 1 hatta en fazla CRC ve adres hariç 232 bittir
    config_setup[0] = PAYLOADSIZE*8;

    // nrf24 çipte baðýmsýz 2 adet alýcý kanal vardýr
    //103-64  data bitleri  Alýcý 2. kanal adresi
    config_setup[1] = 0;
    config_setup[2] = 0;
    config_setup[3] = 0;
    config_setup[4] = 0;
    config_setup[5] = 0; 

    //nrf 63-24   data bitleri Alýcý 1. kanal adresi 
    config_setup[6] = 0;
    config_setup[7] = 0;

    config_setup[8] = idH;
    config_setup[9] = chXORcod; // 1. kanal hat adresi
    config_setup[10]= idL;     // 1. kanal hat adresi
    
  //****************************************************************************  
  //*Data bits(23:16) Address width and CRC
    
    config_setup[11] = 0b01100011; //0b011000=24 bit adres(COD), 0b11 CRC 16 bit CRC enable 0x63
  
  
  //****************************************************************************
  //*nrf Data bits(15:8)          **config_setup[12] ***************************
  //*bit:15                       **bit7=0 one rcv channel,bit7=1 2 rcv channel
  //*bit:14                       **bit6=0 Direct Mode, bit6=1 ShockBurst Mode
  //*bit:13                       **bit5=0 256 Kps, bit5=1 1Mps
  
  //*bit:12 bit:11 bit:10 XTAL Mhz**bit4 bit3 bit2
  //*   0      0      0     4     **
  //*   0      0      1     8     **
  //*   0      1      0    12     **
  //*   0      1      1    16     **
  //*   0      0      0    20     **
  //****************************************************************************
  
  //*bit:9 bit:8  RF Power dBm    **bit1 bit0
  //*   0     0     -20           **
  //*   0     1     -10           **
  //*   1     0      -5           **
  //*   1     1       0           **
  // 0b01001111  0x4F 250kps  0b011011110x6F 1Mps
  config_setup[12]=0x4F;
  //****************************************************************************
  
  //****************************************************************************  
  //*nrf Data bits(7:0)
  //*bit(7:1) Channel Number
  //*bit0=0 Transmit Mode, bit0=1 Receive Mode  
  // kanal nosunu ata ve nrf yi alýcý moduna getir alýcý modu için Bit0=1 olmalý
  if(txmode)
  {
   config_setup[13] = channel<<1;
  }
  else
  {
   config_setup[13] = (channel<<1) | 0x01;
  }
  //****************************************************************************
  
  // nrf24 konfigurasyon verilerini yükle
    for(i = 0 ; i < 14 ; i++)
    {
        temp = config_setup[i];
        
        for(j = 0 ; j < 8 ; j++)
        {   
            IOpin.moduleDATA=tempbit;
            IOpin.moduleCLK=1;
            IOpin.moduleCLK=1;
            IOpin.moduleCLK=0;
            IOpin.moduleCLK=0;
        
            temp <<= 1;
        }
    }
    
    //Konfigurasyon ayarlarý CS nin düþen kenarýnda aktif hale gelir
    IOpin.moduleCE=0;
    IOpin.moduleCS=0;
 
    //Alýcýnýn Konfigürasyon ayarlarýndan sonra RX_DATA giriþ olarak tanýmla
    set_tris_c (0x82); 
    //Havayý dinlemeye baþla
    IOpin.moduleCE=1;
    IOpin.moduleCS=0;
    
}
//******************************************************************************


