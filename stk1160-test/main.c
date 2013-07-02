/*
 * Copyright (C) 2013 Ludwig Ortmann <ludwig.ortmann@fu-berlin.de>
 */

#include <stdio.h>
#include <board.h>
#include <stdint.h>
#include <hwtimer.h>

#include <stk1160.h>

/*int main(void)
{
    while (1)
    {
        puts("Hallo!\n");
        hwtimer_wait(HWTIMER_TICKS(1000*1000));
    }
}*/

#if 1
int main(void)
{
    printf("\n\t\t\tWelcome to RIOT\n\n");

    uint8_t foo;

    stk1160_set_videosource(STK1160_VIDEO_SOURCE_COMPOSITE0);
    stk1160_read_reg(0x000, &foo);
    printf("foo = 0x%x\n", foo);

    /*stk1160_set_videosource(STK1160_VIDEO_SOURCE_COMPOSITE1);
    stk1160_read_reg(0x000, &foo);
    printf("foo = 0x%x\n", foo);

    stk1160_set_videosource(STK1160_VIDEO_SOURCE_COMPOSITE0);
    stk1160_read_reg(0x000, &foo);
    printf("foo = 0x%x\n", foo);

    stk1160_set_videosource(STK1160_VIDEO_SOURCE_COMPOSITE2);
    stk1160_read_reg(0x000, &foo);
    printf("foo = 0x%x\n", foo);*/

    int ret;
    ret = stk1160_start_streaming();
    printf("ret = %d\n", ret);

    return 0;
}
#endif
