#include <xc.h>
#include <pic16f877a.h>
#include <math.h>


#define _XTAL_FREQ 8000000

void ADC_Init()
{
  ADCON0 = 0x81;               //Turn ON ADC and Clock Selection
  ADCON1 = 0x00;               //All pins as Analog Input and setting Reference Voltages
}

unsigned int ADC_Read(unsigned char channel)
{
  if(channel > 7)              //Channel range is 0 ~ 7
    return 0;

  ADCON0 &= 0xC5;              //Clearing channel selection bits
  ADCON0 |= channel<<3;        //Setting channel selection bits
  __delay_ms(2);               //Acquisition time to charge hold capacitor
  GO_nDONE = 1;                //Initializes A/D conversion
  while(GO_nDONE);             //Waiting for conversion to complete
  return ((ADRESH<<8)+ADRESL); //Return result
}

void main()
{
  unsigned int a;
  unsigned int d;
  TRISA = 0xFF;                 //Analog pins as Input
  TRISDbits.TRISD0 = 0;               //Port B as Output
  TRISC = 0x00;                 //Port C as Output
  TRISB = 0x00;
  ADC_Init();                   //Initialize ADC
  unsigned int i=0;
  do
  {
    a = ADC_Read(0);
    //Read Analog Channel 0
    PORTB = a;                  //Write Lower bits to PORTB
    PORTC = a>>8;
    d=a;
    unsigned int j=1;
    for(i=0;i<10;i++)
    { 
        PORTDbits.RD0 = a & j;
        
        j=j>>1;
        __delay_ms(100);
      
    }
    //Write Higher 2 bits to PORTC
    __delay_ms(100);
    //Delay
  }while(1);                    //Infinite Loop
}
