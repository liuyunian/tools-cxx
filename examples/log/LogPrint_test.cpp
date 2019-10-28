#include <iostream>

#include <stdint.h>

#include <tools/log/LogPrint.h>

int main(){
    // test no %
    {
        LogPrint lp;
        lp.printf("hello world");   
        lp.printf_buffer();
    }

    // test %d -- 32bit
    {
        int a = 1;                  // 32bit
        LogPrint lp;
        lp.printf("%d", a);            
        lp.printf_buffer();
    }
    
    {
        short a = 1;                // 16bit -- type promotion
        LogPrint lp;
        lp.printf("%d", a);            
        lp.printf_buffer();
    }

    {
        int8_t a = 1;               // 8bit -- type promotion
        LogPrint lp;
        lp.printf("%d", a);            
        lp.printf_buffer();
    }

    {
        int a = -1;                 // negative
        LogPrint lp;
        lp.printf("%d", a);            
        lp.printf_buffer();
    }
    
    // test %ud -- 32bit
    {
        uint32_t a = 1;             // normal
        LogPrint lp;
        lp.printf("%ud", a);            
        lp.printf_buffer();
    }

    {
        uint32_t a = -1;            // negative
        LogPrint lp;
        lp.printf("%ud", a);            
        lp.printf_buffer();
    }

    // test %l -- 64bit
    {
        int64_t a = 1;
        LogPrint lp;
        lp.printf("%d", a);            
        lp.printf_buffer();
    }

    // test %ul -- 64bit
    {
        uint64_t a = 1;
        LogPrint lp;
        lp.printf("%d", a);            
        lp.printf_buffer(); 
    }

    // test %xd
    {
        int a = 31;                 // expect 0x1f
        LogPrint lp;
        lp.printf("%xd", a);            
        lp.printf_buffer(); 
    }

    // test %Xd
    {
        int a = 31;                 // expect 0X1F
        LogPrint lp;
        lp.printf("%Xd", a);            
        lp.printf_buffer(); 
    }

    return 0;
}