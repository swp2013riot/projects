/*
 * Copyright (C) 2013 Ludwig Ortmann <ludwig.ortmann@fu-berlin.de>
 */

#include <stdio.h>
#include <board.h>
#include <stdint.h>
#include <hwtimer.h>
#include <malloc.h>
#include <stdlib.h>
#include <stk1160.h>

#define BUFFER_SIZE (1024*1024)
uint8_t *buffer;
uint8_t *buf_p;
int frame_counter;
int packet_counter;

void our_handler(uint8_t *data, uint16_t length);

int main(void)
{
    printf("\n\t\t\tWelcome to RIOT\n\n");

    uint8_t foo;

    stk1160_set_videosource(STK1160_VIDEO_SOURCE_COMPOSITE0);
    stk1160_read_reg(0x000, &foo);
    printf("foo = 0x%x\n", foo);

    buffer = malloc(BUFFER_SIZE);
    buf_p = buffer;
    frame_counter = 0;
    packet_counter = 0;

    int ret;
    ret = stk1160_start_streaming(our_handler);
    printf("ret = %d\n", ret);

    return 0;
}

void our_handler(uint8_t *data, uint16_t length)
{
    printf("our_handler(%p, %d)\n", data, length);

    if (data[0] == 0xc0)
    {
        printf(">>> START OF NEW FRAME (%d) %d\n", ++frame_counter, packet_counter);

        // DO SOME PROCESSING HERE FOR WE *SHOULD* HAVE A COMPLETE FRAME
        char* filename = malloc(32);
        snprintf(filename, 32, "/tmp/stk1160/frame-%04d", frame_counter);
        FILE* file = fopen(filename, "w");
        int written = fwrite(buffer, sizeof(uint8_t), buf_p - buffer, file);
        printf("wrote %d bytes\n", written);
        fclose(file);

        buf_p = buffer;
    }

    if (buf_p + length < buffer + BUFFER_SIZE)
    {
        memcpy(buf_p, data, length);
        buf_p += length;
        printf("got %d bytes of \"video\"-data\n", buf_p - buffer);
        packet_counter++;
    }

    else
    {
        buf_p = buffer;
        printf("our_handler: ran out of memory, skip this frame\n");
    }
}
