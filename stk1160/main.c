/**
 * Demonstration project for the easycap compatible USB-Video-Grabber
 * driver stk1160 (native port only).
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

/**
 * In order to use this driver three functions have
 * to be called in the following order:
 * 1. stk1160_init() (might be optional if you use RIOT's auto_init module)
 * 2. stk1160_set_videosource
 * 3. stk1160_start_streaming
 */ 
int main(void)
{
    printf("\n\t\t\tWelcome to RIOT\n\n");
  
    buffer = malloc(BUFFER_SIZE); // allocate a buffer video data
    buf_p = buffer;
    frame_counter = 0;
    packet_counter = 0;
    
    // set video source to composite0
    stk1160_set_videosource(STK1160_VIDEO_SOURCE_COMPOSITE0);

    // stk1160_start_streaming calls a provided handler
    // every time video data arrives from the grabber.
    int ret;
    ret = stk1160_start_streaming(our_handler); 
    printf("ret = %d\n", ret);

    return 0;
}

/*
 * This simple handler dumps received 
 * data in files on the host system.
 * Each file represents (hopefully) one 
 * captured frame.
 *
 * data points to the start of the received
 * video data
 *
 * length denotes the length of this buffer 
 * in Bytes 
 */
void our_handler(uint8_t *data, uint16_t length)
{
    char filename[32];
  
    printf("our_handler(%p, %d)\n", data, length);
    
    // If the video data starts with 0xc0 (end of frame marker) everything 
    // we received before should be one frame.
    if (data[0] == 0xc0) 
    {
	// we have a complete frame in our buffer
        printf(">>> START OF NEW FRAME (%d) %d\n", ++frame_counter, packet_counter);

        // dump this frame into a file
        snprintf(filename, 32, "/tmp/stk1160/frame-%04d", frame_counter);
        FILE* file = fopen(filename, "w");
        int written = fwrite(buffer, sizeof(uint8_t), buf_p - buffer, file);
        printf("wrote %d bytes\n", written);
        fclose(file);

        buf_p = buffer; // reset buffer pointer to start of buffer
    }
    
    // checks whether the received data fits into 
    // our buffer.
    if (buf_p + length < buffer + BUFFER_SIZE)
    {
        memcpy(buf_p, data, length); // if so copy data into buffer
        buf_p += length;
        printf("got %d bytes of \"video\"-data\n", buf_p - buffer);
        packet_counter++; 
    }
    else
    {
        buf_p = buffer; // otherwise reset buffer pointer (we might have missed an
			// end of frame marker)
        printf("our_handler: ran out of memory, frame corrupted\n");
    }
}
