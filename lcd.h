#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#define _XTAL_FREQ 20000000 // 20MHz
 
    // Data bus is connected to PB4:PB7 
    // Control bus PB0:PB2
    
#define LcdDataBus      PORTD
#define LcdControlBus   PORTD
#define LcdDataBusDirnReg   TRISD

#define LCD_RS     0
#define LCD_RW     1
#define LCD_EN     2

    // LCD command
#define _4BIT   0x02    // Initialize Lcd in 4-bit mode
#define ENABLE  0x28    // enable 5x7 mode for chars 
#define CURSOR  0x0E    // Display OFF, Cursor ON
#define CLEAR   0x01    // Clear Display
#define ROW1    0x80    // First line
#define ROW2    0xC0    // Second line
    
    // Function to send the command to LCD. 
    // As it is 4bit mode, a byte of data is sent in two 4-bit nibbles
    void LcdCmdWrite(char cmd)
    {
        LcdDataBus = (cmd & 0xF0);      //Send higher nibble
        LcdControlBus &= ~(1<<LCD_RS);  // Send LOW pulse on RS pin for selecting Command register
        LcdControlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
        LcdControlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
        __delay_ms(10);
        LcdControlBus &= ~(1<<LCD_EN);

        LcdDataBus = ((cmd<<4) & 0xF0); // Send Lower nibble
        LcdControlBus &= ~(1<<LCD_RS);  // Send LOW pulse on RS pin for selecting Command register
        LcdControlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
        LcdControlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
        __delay_ms(10);
        LcdControlBus &= ~(1<<LCD_EN); 
    }

    // Function to initialize LCD
    void LcdInit()
    {
        LcdDataBusDirnReg = 0x00;   // Configure all the LCD pins as output
        LcdCmdWrite(_4BIT);         // Initialize LCD in 4-bit mode
        LcdCmdWrite(ENABLE);        // enable 5x7 mode for chars 
        LcdCmdWrite(CURSOR);        // Display OFF, Cursor ON
        LcdCmdWrite(CLEAR);         // Clear Display
    }
    
    // Function to send the Data to LCD. 
    //   As it is 4bit mode, a byte of data is sent in two 4-bit nibbles
    void LcdDataWrite(char data)
    {
        LcdDataBus = (data & 0xF0);      //Send higher nibble
        LcdControlBus |= (1<<LCD_RS);   // Send HIGH pulse on RS pin for selecting data register
        LcdControlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
        LcdControlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
        __delay_ms(10);
        LcdControlBus &= ~(1<<LCD_EN);

        LcdDataBus = ((data<<4) & 0xF0);  //Send Lower nibble
        LcdControlBus |= (1<<LCD_RS);    // Send HIGH pulse on RS pin for selecting data register
        LcdControlBus &= ~(1<<LCD_RW);   // Send LOW pulse on RW pin for Write operation
        LcdControlBus |= (1<<LCD_EN);    // Generate a High-to-low pulse on EN pin
        __delay_ms(10);
        LcdControlBus &= ~(1<<LCD_EN); 
    }

    // Function to write message
    void LcdMsgPrint(char* msg)
    {
        for(char i = 0; msg[i] != '\0'; i++)
        {
            LcdDataWrite(msg[i]);
        }
    }

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */