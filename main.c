#include <stdbool.h>
#include <stdarg.h>
#include "main.h"
#include <stddef.h>
#include <stdio.h>

#ifndef  PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE 32U
#endif

#define FLAGS_ZEROPAD (1U << 0U)
#define FLAGS_LEFT (1U << 1U)

typedef void (*prnt_fct_type)(char character);
typedef struct {
    void (*prnt)(char character);
    void *arg;
} prnt_type;

void prnt_char(char character)
{
     if (character)
    {
        _putchar(character);
    }
}

void prnt_fct(char character, void *buffer)
{
    if(character)
    {
        //buffer is the output pointer for this function
        ((prnt_type*)buffer)->prnt(character);
    }
}
void prnt_str(char *str)
{
    int index=0;
    while (str[index] != '\0')
        _putchar(str[index++]);
}

size_t _ntoa_format(prnt_fct_type prnt, char* buf, size_t len, unsigned long base, bool negative)
{
    //printf("this is len %d\n", len);

    if (len < PRINTF_NTOA_BUFFER_SIZE)
    {
        if(negative)
        {
            buf[len++] = '-';
        }
    }

    while(len)
        prnt(buf[--len]);
}
size_t _ntoa_long_long(prnt_fct_type prnt, unsigned long value, unsigned long base, bool negative)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    do
    {
        const char digit = (char)(value % base);
                                //  prnt(('a') + digit - 10);
                                  //prnt('\n');
        buf[len++] = digit < 10 ? '0' + digit :('a') + digit - 10;
        value /= base;
    } while(value && (len < PRINTF_NTOA_BUFFER_SIZE));


    return _ntoa_format(prnt, buf, len, (unsigned int)base, negative);
}

size_t _ntoa_long(prnt_fct_type prnt, unsigned long value, unsigned long base, bool negative, unsigned int flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    do
    {
        const char digit = (char)(value % base);
        buf[len++] = digit < 10 ? '0' + digit :('a') + digit - 10;
        value /= base;
    } while(value && (len < PRINTF_NTOA_BUFFER_SIZE));


    return _ntoa_format(prnt, buf, len, (unsigned int)base, negative);
}
int _printf(const char *format, ...);

int main()
{
    _printf("Lets try printing something\n ");

    _printf("Character: [%c]\n",'H');
    _printf("\nCharacter: [%s]\n", "string i am");

    _printf("\n Numbers base 10: [%d %i]\n", 23, 23);
    _printf("\n Negative Numbers base 10: [%d]\n", -232367);

      return 0;
}




//main printf function
int _printf(const char *format, ...)
{
    va_list va;
    va_start(va, format);
    int result = varprintf(prnt_char, format, va);
    va_end(va);
    return result;
}

int varprintf(prnt_fct_type prnt, const char *format, va_list va)
{
    size_t idx = 0U;
    unsigned int flags, n;

    while (*format)
    {
        //format specifier ?
        if (*format != '%')
        {
            prnt(*format);
            format++;
            continue;
        }
        else
        {
            format++;
        }

        //evaluate flags
        flags = 0U;
        do
        {
            switch (*format)
            {
                case '0': flags |= FLAGS_ZEROPAD; format++; n = 1U; break;
                case '-': flags |= FLAGS_LEFT; format++; n = 1U; break;
                default:    n = 0U; break;
            }

        } while (n);
        switch (*format)
        {
            case 'c' :
                {
                    char character = (char) va_arg(va, int);
                    prnt(character);
                     format++;
                     break;
                }
            case 's' :
                {
                    char *str = va_arg(va, char*);
                     prnt_str(str);
                     format++;
                     break;
                }
            case 'd' :
            case 'i' :
                {
                    unsigned int base;

                    base = 10U;
                    if((*format == 'i') || (*format == 'd'))
                    {
                        //signed
                        if (false)
                        {   //printf("I am here");
                            const long long val = va_arg(va, long long);
                             //printf("I am here %d", val);
                            idx = _ntoa_long_long(prnt, (unsigned long long)(val > 0 ? val : 0 - val), base, val < 0);
                        }
                        else
                        {
                            const int val = (flags) ? (char)va_arg(va, int) : va_arg(va, int);
                            idx = _ntoa_long(prnt, (unsigned int)(val > 0 ? val : 0 - val), base, val < 0, flags);
                        }

                    }

                     format++;
                     break;
                }

            default:
                prnt(format);
                format++;
                break;
        }
    }
    return (int)idx;
}

