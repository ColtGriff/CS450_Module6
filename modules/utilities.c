#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include <core/io.h>
#include <mem/heap.h>
#include <system.h>

char *reverseStr(char *str)
{
    int size = strlen(str);
    char temp[size];
    

    int i = 0;
    while (size >= 0)
    {
        temp[i] = str[size - 1];
        size--;
        i++;
    }
    char* test= temp;
    
    return test;
}

char *itoa(int num, char *buffer)
{
    int i = 0;
    int neg = FALSE;

    if (num == 0)
    {
        buffer[i] = '0';
        buffer[++i] = '\0';
    }

    if (num < 0)
    {
        neg = TRUE;
        num = -num;
    }

    do
    {
        buffer[i++] = (num % 10) + '0';
    } while ((num /= 10) > 0);

    if (neg == TRUE)
    {
        buffer[i++] = '-';
    }

    buffer = reverseStr(buffer);
    buffer[i] = '\0';

    return buffer;
}

void printMessage(char *str)
{
    //klogv("Entered printMessage function.");
    char Desc[137];

    size_t length = strlen(str);
    if (length > (sizeof(Desc) - 2))
    {
        length = sizeof(Desc) - 2;
        Desc[sizeof(Desc) - 1] = '\0';
    }
    strcpy(Desc, str);
    int tempBuffer = strlen(Desc);
    sys_req(WRITE, DEFAULT_DEVICE, (char *)Desc, &tempBuffer);
}