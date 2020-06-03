/*
 * File:   main.c
 * Author: BDN
 *
 * Created on June 3, 2020, 9:57 PM
 */

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

/* Configure the data bus and Control bus as per the hardware connection 
   Dtatus bus is connected to PB0:PB7 and control bus PD0:PD2*/
#define LcdDataBus PORTB
#define LcdCtrlBus PORTD

#define LcdDataBusDirnReg TRISB
#define LcdCtrlBusDirnReg TRISD

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

#define _XTAL_FREQ 4000000

/* Function to send the command to LCD*/
void Lcd_CmdWrite(char cmd)
{
    LcdDataBus = cmd;               //Send the Command nibble
    LcdCtrlBus &= ~(1<<LCD_RS);  // Send LOW pulse on RS pin for selecting Command register
    LcdCtrlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
    LcdCtrlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
    __delay_ms(100);
    LcdCtrlBus &= ~(1<<LCD_EN); 

    __delay_ms(100);
}

/* Function to send the Data to LCD */
void Lcd_DataWrite(char data)
{
    LcdDataBus = data;              //Send the data on DataBus nibble
    LcdCtrlBus |= (1<<LCD_RS);   // Send HIGH pulse on RS pin for selecting data register
    LcdCtrlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
    LcdCtrlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
    __delay_ms(100);
    LcdCtrlBus &= ~(1<<LCD_EN);

    __delay_ms(100);
}

void main(void)
{
    char i,a[]={"Good morning!"};

    LcdDataBusDirnReg = 0x00;  // Configure all the LCD pins as output
    LcdCtrlBusDirnReg = 0x00;  // Configure the Ctrl pins as output


    Lcd_CmdWrite(0x38);        // enable 5x7 mode for chars 
    Lcd_CmdWrite(0x0E);        // Display OFF, Cursor ON
    Lcd_CmdWrite(0x01);        // Clear Display
    Lcd_CmdWrite(0x80);        // Move the cursor to beginning of first line


    Lcd_DataWrite('H');
    Lcd_DataWrite('e');
    Lcd_DataWrite('l');
    Lcd_DataWrite('l');
    Lcd_DataWrite('o');
    Lcd_DataWrite(' ');
    Lcd_DataWrite('w');
    Lcd_DataWrite('o');
    Lcd_DataWrite('r');
    Lcd_DataWrite('l');
    Lcd_DataWrite('d');

    Lcd_CmdWrite(0xc0);        //Go to Next line and display Good Morning
    for(i = 0; a[i] != 0; i++)
    {
        Lcd_DataWrite(a[i]);
    }

    while(1);

    return;
}