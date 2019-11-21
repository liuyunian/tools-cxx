#include <iostream>

#include <stdint.h> // uint32_t uint64_t
#include <stdarg.h> // va_list va_arg
#include <math.h>   // pow

#include "tools/log/LogPrint.h"

const int LogPrint::SMALL_BUFFER = 4000;    // 4k

LogPrint::LogPrint() : 
    m_buffer(SMALL_BUFFER){}


void LogPrint::printf(const char* fmt, ...){
    va_list args;

    va_start(args, fmt);

    vprintf(fmt, args);
    va_end(args);
}

void LogPrint::vprintf(const char* fmt, va_list args){
    // 放在循环外定义的原因是：减少重复定义
    int64_t     int64;      // 保存有符号整数
    uint64_t    uint64;     // 保存无符号整数
    char        ch;         // 保存字符
    char*       string;     // 保存字符串
    double      fn;         // 保存浮点数
    uint64_t    frac;       // 保存小数部分

    bool    sign;           // 是否有符号
    HexFlag hex;            // 是否以16进制形式显示，No: 10进制, Lower: 0xef小写形式, Upper: 0XEF大写形式
    uint8_t width;          // 用于保存%5d中的数字5 -- 最长255
    uint8_t frac_width;     // 用于保存%7.2中小数点后的数字2 -- 最长255 
    char    padding;        // 记录填充的字符，空格" " or 空字符

    while(*fmt && m_buffer.available()){           // 格式字符串不为空字符并且缓冲区还有空间存储
        if(*fmt != '%'){
            m_buffer.append_char(*fmt);
            ++ fmt;
            continue;
        }

        // 每处理一个%都要进行置空
        int64 = 0;
        uint64 = 0; 
        ch = 0;
        string = nullptr;
        fn = 0;
        frac = 0;

        sign = true;                                // 默认按有符号处理数字
        hex = No;                                   // 默认不以16进制方式显示
        width = 0;
        frac_width = 6;                             // 默认浮点数显示小数点后6位，如果实际的数字不够，则填充0(空字符)

        ++ fmt;                                     // 指向%后面的内容

        // 处理%5d, %05d前面的数字
        padding = (*fmt == '0' ? '0' : ' ');
        while(*fmt >= '0' && *fmt <= '9'){
            width = width * 10 + (*fmt - '0');
            ++ fmt;
        }

        // 处理%后字符组合的情况，比如%xd, %Xd, %7.2f
        // 使用循环处理的原因是：该格式化字符串还没有处理完成，不能进行下一次的循环
        for(;;){
            switch(*fmt){
            // %ud %ul -- unsigned
            case 'u': {
                sign = false;
                ++ fmt;
                continue;
            }

            // %xd %xl -- 小写16进制
            case 'x': {
                sign = false;
                hex = Lower;
                m_buffer.append("0x", 2);

                ++ fmt;
                continue;
            }

            // %Xd %Xd -- 大写16进制
            case 'X': {
                sign = false;
                hex = Upper;
                m_buffer.append("0X", 2);

                ++ fmt;
                continue;
            }

            // %x.y -- 小数
            case '.': {
                ++ fmt;
                frac_width = 0;
                while(*fmt >= '0' && *fmt <= '9'){
                    frac_width = frac_width * 10 + (*fmt - '0');
                    ++ fmt;
                }
                continue;
            }

            default:
                break;                              // leave switch
            }

            break;                                  // leave for
        }

        switch(*fmt){
        // %c -- 字符型
        case 'c': {
            ch = static_cast<char>(va_arg(args, int));
            m_buffer.append_char(ch);

            ++ fmt;
            continue;                               // continue while
        }

        // %d -- unsigned int/int 32位无/有符号整型
        case 'd': {
            if(sign){
                int64 = static_cast<int64_t>(va_arg(args, int));
            }
            else{
                uint64 = static_cast<uint64_t>(va_arg(args, unsigned int));
            }

            break;                                  // leave switch
        }

        // %f -- float/doble 浮点数
        case 'f': {
            fn = va_arg(args, double);
            if(fn < 0){
                m_buffer.append_char('-');
                fn = -fn;
            }

            uint64 = static_cast<uint64_t>(fn);     // 浮点数的整数部分
            frac = static_cast<uint64_t>((fn - uint64) * pow(10, frac_width) + 0.5);   // 浮点数的小数部分，四舍五入
            if(frac == pow(10, frac_width)){
                ++ uint64;
                frac = 0;
            }

            printf_num(uint64, hex, padding, width);
            m_buffer.append_char('.');
            printf_num(frac, No, '0', frac_width);

            ++ fmt;
            continue;
        }

        // %l -- unsigned long long/long long -- 64位无/有符号整数
        case 'l': {
            if(sign){
                int64 = static_cast<int64_t>(va_arg(args, int64_t));
            }
            else{
                uint64 = static_cast<uint64_t>(va_arg(args, uint64_t));
            }

            break;
        }

        // %p -- 指针类型
        case 'p': {
            uint64 = reinterpret_cast<uintptr_t>(va_arg(args, void*));
            sign = false;
            hex = Upper;
            padding = '0';
            width = 2 * sizeof(void*);
            break;
        }

        // %s -- 字符串型
        case 's': {
            string = va_arg(args, char*);
            m_buffer.append(string, strlen(string));

            ++ fmt;
            continue;
        }

        // %% -- 转义成一个%
        case '%': {
            m_buffer.append_char('%');

            ++ fmt;
            continue;
        }

        default: {
            m_buffer.append_char(*fmt);
            
            ++ fmt;
            continue;
        }
        }

        if(sign){
            if(int64 < 0){
                m_buffer.append_char('-');
                int64 = -int64;
            }

            uint64 = static_cast<uint64_t>(int64);     
        }
        printf_num(uint64, hex, padding, width);

        ++ fmt;
    }
}

void LogPrint::printf_num(uint64_t uint64, HexFlag hex, char padding, uint8_t width){
    char numString[32] = {0};                                       // 用于存放64位整数的字符串形式，转换之后的字符串长度必然是小于32的
    char* last = numString + 31;                                    // last指向数组最后

    static char hexArray[] = "0123456789abcdef";
    static char HEXArray[] = "0123456789ABCDEF";
    if(hex == Lower){
        do{
            -- last;
            *last = hexArray[static_cast<size_t>(uint64 & 0xf)];    // 十六进制数字转换成字符
        }
        while(uint64 >>= 4);
    }
    else if(hex == Upper){
        do{
            -- last;
            *last = HEXArray[static_cast<size_t>(uint64 & 0xf)];    // 十进制数字转换成字符
        }
        while(uint64 >>= 4);
    }
    else{
        do{
            -- last;
            *last = static_cast<char>(uint64 % 10 + '0');           // 十进制数字转换成字符
        }
        while(uint64 /= 10);
    }

    uint8_t len = static_cast<uint8_t>((numString+31) - last);
    while(len < width){
        m_buffer.append_char(padding);
        ++ len;
    }

    size_t numLen = static_cast<size_t>((numString+31) - last);
    m_buffer.append(last, numLen);
}

void LogPrint::printf_buffer(){
    std::cout << "Buffer-length = " << m_buffer.length() << ", Buffer-content = " << m_buffer.to_string() << std::endl;
}