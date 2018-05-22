
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>

#define RECT(x, y, w, h) ((h) | ((w) << 8) | ((y) << 16) | ((x) << 24))

unsigned int frame;
u16 bg_color = RGB5(12, 18, 30); //cornflower blue
u16 player_color = RGB5(0, 0, 0); //black
u32 player_rect = RECT(20, 20, 30, 10);

void vsync()
{
	frame += 1;
}

u32 moverect(u32 rect, int8_t x_offset, int8_t y_offset)
{
	u8 r_x = (rect >> 24) + x_offset;
	u8 r_y = (rect >> 16) + y_offset;
	u8 r_w = rect >> 8;
	u8 r_h = rect;

	return RECT(r_x, r_y, r_w, r_h);
}

void clearscrn(u16 color)
{
	for (u8 x = 0, i = 0; x < SCREEN_WIDTH; ++x)
	{
		for (u8 y = 0; y < SCREEN_HEIGHT; ++y, ++i)
		{
			MODE3_FB[y][x] = color;
		}
	}
}

void drawrect(u32 rect, u16 color)
{
	u8 r_x = rect >> 24;
	u8 r_y = rect >> 16;
	u8 r_w = rect >> 8;
	u8 r_h = rect;

	for (u8 x = 0, i = 0; x < SCREEN_WIDTH; ++x)
	{
		for (u8 y = 0; y < SCREEN_HEIGHT; ++y, ++i)
		{
			if (x > r_x && y > r_y && x < r_x + r_w && y < r_y + r_h)
			{
				MODE3_FB[y][x] = color;
			}
		}
	}
}

int main(void) {
	irqInit();
	irqSet(IRQ_VBLANK, vsync);
	irqEnable(IRQ_VBLANK);

	SetMode(MODE_3 | BG2_ON);

	clearscrn(bg_color);
	

	while (1) {

		VBlankIntrWait();
		clearscrn(bg_color);
		player_rect = moverect(player_rect, 1, 0);

		drawrect(player_rect, player_color);
	}
}


