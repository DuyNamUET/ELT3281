// BEGIN CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program M
//END CONFIG

#include <pic16f877a.h>
#include <xc.h>
#include "lcd.h"
#include "dht11.h"
#include "pwm.h"

#define _XTAL_FREQ 20000000 // 20MHz

// Set idea temp and humi
char idea_temp[] = "IDTemp = 20.0 C";
char idea_humi[] = "IDRH   = 85.0 %";
unsigned char itemp = 20;
unsigned char ihumi = 95;

__bit status = 0;           // status to show on LCD

// Function to set value for pump
void setPumpValue(unsigned char r_rh, unsigned char id_rh);

void main(void)
{
    /* Interrupt setup */
    // RB0 interrupt
    INTEDG = 1;                 // Interrupt edge config bit (HIGH value means interrupt occurs every rising edge)
    INTE = 1;                   // IRQ (Interrupt request pin RB0) Interrupt Enable bit
    GIE = 1;                    // Global Interrupt Enable bit
    
    /* Interrupt end setting*/
    
    // Initialize hardware
    LcdInit();      // LCD initialization
    configPWM();    // Configure PWM
    
    while(1)
    {
        if(!status)
        {
            if(DhtUpdateData())
            {
                LcdCmdWrite(ROW1);
                LcdMsgPrint(temp);
                LcdCmdWrite(ROW2);
                LcdMsgPrint(humi);
            }
            else
            {
                LcdCmdWrite(ROW1);
                LcdMsgPrint("Error");
            }
        }
        else
        {
            LcdCmdWrite(CLEAR);
            LcdCmdWrite(ROW1);
            LcdMsgPrint(idea_temp);
            LcdCmdWrite(ROW2);
            LcdMsgPrint(idea_humi);
        }
        
        setPumpValue(rh_byte1, ihumi);
//        setPWM(100);
        __delay_ms(1000);
    }
    return;
}

void __interrupt() ISR()
{
    if(INTF == 1)
    {
        status = ~status;
        LcdCmdWrite(CLEAR);
        INTF = 0;
    }
}

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