#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif
#define _XTAL_FREQ 20000000 // 20MHz
#define PWM_FREQ 6520
    
    void configPWM()
    {
        // Configure the CCP1 module for PWM operation. 
        CCP1M2 = 1;
        CCP1M3 = 1;
        // Set CCP1 pin as output
        TRISC2 = 0;
        // Set the Timer2 prescaler 1:16
        T2CKPS0 = 1;
        T2CKPS1 = 1;
        TMR2ON = 1;     // enable Timer2

        // Set the PWM period
        PR2 = 49;      // 1/PWM_FREQ = (PR2 + 1)*4*1/_XTAL_FREQ*Prescaler
    }

#define PWM_MAX 200     // 1/PWM_FREQ = (PWM_MAX)*1/_XTAL_FREQ*Prescaler
    
    // set PWM to RC2 output
    void setPWM(unsigned char DC)
    {
        // Write to CCP1CON<5:4>
        CCP1Y = DC & (1<<0);
        CCP1X = DC & (1<<1);
        // Write to CCPR1L register
        CCPR1L = DC >> 2;
    }

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

