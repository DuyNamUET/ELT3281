#ifndef DHT11_H
#define	DHT11_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#define _XTAL_FREQ 20000000 //20Mhz
    
    // Configure the data pin and Ctrl pin
#define DhtDataPin RB4
#define DhtDataDirnReg TRISB4

    // Variance
    char temp[] = "Temp = 00.0 C";
    char humi[] = "RH   = 00.0 %";
    
    unsigned char t_byte1, t_byte2;     // temp data
    unsigned char rh_byte1, rh_byte2;   // humi data
    unsigned char check_sum;

    // Function to initialize sensor
    void DhtStartSignal()
    {
        DhtDataDirnReg = 0;     // Configure connection pin as output
        DhtDataPin = 0;         // Connection pin output is low

        __delay_ms(25);
        DhtDataPin = 1;         // Connection pin output is high
        __delay_us(30);
        DhtDataDirnReg = 1;     // Configure connection pin as input
    }

    // Function to check response of sensor
    void DhtCheckResponse()
    {
        while(DhtDataPin & 1);      /* wait till bus is high */     
        while(!(DhtDataPin & 1));   /* wait till bus is low */
        while(DhtDataPin & 1);      /* wait till bus is high */
    }
    
    // Function to read data from sensor
    unsigned char DhtReadData()
    {
        char i, data = 0;  
        for(i = 0; i < 8; i++)
        {
            while(!(DhtDataPin & 1));  /* wait till 0 pulse, this is start of data pulse */
            __delay_us(30);         
            if(DhtDataPin & 1)  /* check whether data is 1 or 0 */    
                data = ((data<<1) | 1); 
            else
                data = (data<<1);  
            while(DhtDataPin & 1);
        }
        return data;
    }
    
    // Function to update DHT11 Data
    __bit DhtUpdateData()
    {
        DhtStartSignal();
        DhtCheckResponse();
        
        rh_byte1 = DhtReadData();
        rh_byte2 = DhtReadData();
        t_byte1 = DhtReadData();
        t_byte2 = DhtReadData();
        check_sum = DhtReadData();
        
        if(check_sum == ((rh_byte1 + rh_byte2 + t_byte1 + t_byte2) & 0xFF))
        {
            temp[7]  = t_byte1 / 10  + 48;
            temp[8]  = t_byte1 % 10  + 48;
            temp[10] = t_byte2 / 10  + 48;

            humi[7]  = rh_byte1 / 10 + 48;
            humi[8]  = rh_byte1 % 10 + 48;
            humi[10] = rh_byte2 / 10 + 48;
            
            return 1;   // DHT11 was updated
        }
        return 0;       // Error
    }
    
#ifdef	__cplusplus
}
#endif

#endif	/* DHT11_H */

