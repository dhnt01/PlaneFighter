#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int r, int c, unsigned short color)
{
	videoBuffer[OFFSET(r, c, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		for(int c=0; c<width; c++)
		{
			setPixel(row+r, col+c, color);
		}
	}
}

void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*10000; i++)
	{
		x++;
	}
}

void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

// Draw one row of the screen with DMA
void drawScreen(int row, const unsigned short *image) {

	// TODO re-implement this loop using DMA
	// Hint: There shouldn't even be a loop if you've done it correctly
	// This loop only draws one row of a fullscreen image
	DMA[DMA_CHANNEL_3].src = &(image[row * 240]);
  DMA[DMA_CHANNEL_3].dst = videoBuffer + 240 * row;
  DMA[DMA_CHANNEL_3].cnt = (240 * 160) |
                             DMA_SOURCE_INCREMENT |
                             DMA_DESTINATION_INCREMENT |
                             DMA_ON;
}

void drawObject(int row, int col, int width,
	int height, const unsigned short *image) {
		for(int i = 0; i < height; i++) {
			DMA[DMA_CHANNEL_3].src = &(image[i * width]);
		  DMA[DMA_CHANNEL_3].dst = videoBuffer + 240 * (row + i) + col;
		  DMA[DMA_CHANNEL_3].cnt = width |
		                             DMA_SOURCE_INCREMENT |
		                             DMA_DESTINATION_INCREMENT |
		                             DMA_ON;
		}
	}

void drawChar(int row, int col, char ch, u16 color) {
	for(int r = 0; r < 8; r++) {
		for(int c = 0; c < 6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch * 48]) {
				setPixel(row + r, col + c, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, u16 color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

void fillScreen(volatile u16 color) {
	DMA[DMA_CHANNEL_3].src = &color;
	DMA[DMA_CHANNEL_3].dst = videoBuffer;
	DMA[DMA_CHANNEL_3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;
}

int collisionENEMY(ENEMY e, MISSLE m) {
		if ((m.row < e.row + 15 && m.row > e.row)
			&& (m.col > e.col && m.col < e.col + 15)) {
				return 1;
		}
		return 0;
}

int collisionSHIP(SHIP s, MISSLE m) {
		if ((m.row < s.row + 15 && m.row > s.row)
			&& (m.col > s.col && m.col < s.col + 15)) {
				return 1;
		}
		return 0;
}
