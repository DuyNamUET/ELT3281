#define _XTAL_FREQ 20000000

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include "lcd.h"
#include "dht11.h"
#include "pwm.h"
#include "adc.h"

// Set idea temp and humi
char idea_temp[] = "IDTemp = 20.0 C";
char idea_humi[] = "IDRH   = 85.0 %";
unsigned char itemp = 20;
unsigned char ihumi = 95;

unsigned char status = 0;           // status to show on LCD

void setPumpValue(unsigned char r_rh, unsigned char id_rh);

int main()
{
    /* Interrupt setup */
    // RB0 interrupt
    INTEDG = 1;                 // Interrupt edge config bit (HIGH value means interrupt occurs every rising edge)
    INTE = 1;                   // IRQ (Interrupt request pin RB0) Interrupt Enable bit
    GIE = 1;                    // Global Interrupt Enable bit
    /* Interrupt end setting*/

    // Lcd init
    TRISD = 0x00;
    RD2 = 0;
    Lcd_Start();
    // Configure PWM
    configPWM();
    // Init ADC module
    ADC_Init();
    while(1)
    {
        Lcd_Clear();
        if(status == 0)
        {
            if(DhtUpdateData())
            {
                Lcd_Set_Cursor(1,1);
                Lcd_Print_String(temp);
                Lcd_Set_Cursor(2,1);
                Lcd_Print_String(humi);
            }
            else
            {
                Lcd_Print_String("Error");
            }
        }
        else if(status == 1)
        {
            
            Lcd_Set_Cursor(1,1);
            Lcd_Print_String(idea_temp);
            Lcd_Set_Cursor(2,1);
            Lcd_Print_String(idea_humi);
        }
        else
        {
            Lcd_Set_Cursor(1,1);
            Lcd_Print_String("Handheld PWM");
            Lcd_Set_Cursor(2,1);
            setPWM(ADC_Read(0)/10);
        }
        if(status != 2) setPumpValue(rh_byte1, ihumi);
        __delay_ms(1000);
    }
    return 0;
}

// Function to do if interrupt
void __interrupt() ISR()
{
    if(INTF == 1)       // RB0 interrupt
    {
        status = (status+1)%3;
        INTF = 0;
    }
}

// Function to set value for pump
void setPumpValue(unsigned char r_rh, unsigned char id_rh)
{
    char delta = 10*(id_rh - r_rh);     // set delta = 10 * error
    if(delta > 0)
    {
        if(delta < PWM_MAX) setPWM(10*delta);    // set PWM pump
        else setPWM(PWM_MAX);
    }
    else setPWM(0);     // stop pump
}