#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <SPI.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


void setup()
{
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	display.clearDisplay();
}


int zOff = 150;//z axis shift on display
int xOff = 0;//x axis shift on display
int yOff = 0;//y axis shift on display
int dots = 8;//how many dots rotate calculating
int cSize = 39;//size of 1 side
int view_plane = 64;
float angle = PI / 60;


int hres = 128;
int vres = 64;

float cube3d[8][3] = {
	{ xOff - cSize, yOff + cSize, zOff - cSize },
	{ xOff + cSize, yOff + cSize, zOff - cSize },
	{ xOff - cSize, yOff - cSize, zOff - cSize },
	{ xOff + cSize, yOff - cSize, zOff - cSize },
	{ xOff - cSize, yOff + cSize, zOff + cSize },
	{ xOff + cSize, yOff + cSize, zOff + cSize },
	{ xOff - cSize, yOff - cSize, zOff + cSize },
	{ xOff + cSize, yOff - cSize, zOff + cSize }
};

unsigned char cube2d[8][2];

void draw_cube()
{
	display.drawLine(cube2d[0][0], cube2d[0][1], cube2d[1][0], cube2d[1][1], WHITE);
	display.drawLine(cube2d[0][0], cube2d[0][1], cube2d[2][0], cube2d[2][1], WHITE);
	display.drawLine(cube2d[0][0], cube2d[0][1], cube2d[4][0], cube2d[4][1], WHITE);
	display.drawLine(cube2d[1][0], cube2d[1][1], cube2d[5][0], cube2d[5][1], WHITE);
	display.drawLine(cube2d[1][0], cube2d[1][1], cube2d[3][0], cube2d[3][1], WHITE);
	display.drawLine(cube2d[2][0], cube2d[2][1], cube2d[6][0], cube2d[6][1], WHITE);
	display.drawLine(cube2d[2][0], cube2d[2][1], cube2d[3][0], cube2d[3][1], WHITE);
	display.drawLine(cube2d[4][0], cube2d[4][1], cube2d[6][0], cube2d[6][1], WHITE);
	display.drawLine(cube2d[4][0], cube2d[4][1], cube2d[5][0], cube2d[5][1], WHITE);
	display.drawLine(cube2d[7][0], cube2d[7][1], cube2d[6][0], cube2d[6][1], WHITE);
	display.drawLine(cube2d[7][0], cube2d[7][1], cube2d[3][0], cube2d[3][1], WHITE);
	display.drawLine(cube2d[7][0], cube2d[7][1], cube2d[5][0], cube2d[5][1], WHITE);

}

void printcube()
{
	//calculate 2d points
	for (byte i = 0; i < dots; i++)
	{
		cube2d[i][0] = (unsigned char)((cube3d[i][0] * view_plane / cube3d[i][2]) + (hres / 2));
		cube2d[i][1] = (unsigned char)((cube3d[i][1] * view_plane / cube3d[i][2]) + (vres / 2));
	}

	draw_cube();
	display.display();

	delay(20); //�������� � ����� ��������� ����� "���������" ���

	display.clearDisplay();
}

void zrotate(float q)
{
	float tx, ty, temp;
	for (byte i = 0; i < dots; i++)
	{
		tx = cube3d[i][0] - xOff;
		ty = cube3d[i][1] - yOff;
		temp = tx * cos(q) - ty * sin(q);
		ty = tx * sin(q) + ty * cos(q);
		tx = temp;
		cube3d[i][0] = tx + xOff;
		cube3d[i][1] = ty + yOff;
	}
}

void yrotate(float q)
{
	float tx, tz, temp;
	for (byte i = 0; i < dots; i++)
	{
		tx = cube3d[i][0] - xOff;
		tz = cube3d[i][2] - zOff;
		temp = tz * cos(q) - tx * sin(q);
		tx = tz * sin(q) + tx * cos(q);
		tz = temp;
		cube3d[i][0] = tx + xOff;
		cube3d[i][2] = tz + zOff;
	}
}

void xrotate(float q)
{
	float ty, tz, temp;
	for (byte i = 0; i < dots; i++)
	{
		ty = cube3d[i][1] - yOff;
		tz = cube3d[i][2] - zOff;
		temp = ty * cos(q) - tz * sin(q);
		tz = ty * sin(q) + tz * cos(q);
		ty = temp;
		cube3d[i][1] = ty + yOff;
		cube3d[i][2] = tz + zOff;
	}
}

void loop()
{
	int rsteps = random(10, 60); //how long rotate
	switch (random(6)) //randomize rotate direction
	{
	case 0:
		for (int i = 0; i < rsteps; i++)
		{
			zrotate(angle);
			printcube();
		}
		break;

	case 1:
		for (int i = 0; i < rsteps; i++)
		{
			zrotate(2 * PI - angle);
			printcube();
		}
		break;

	case 2:
		for (int i = 0; i < rsteps; i++)
		{
			xrotate(angle);
			printcube();
		}
		break;

	case 3:
		for (int i = 0; i < rsteps; i++)
		{
			xrotate(2 * PI - angle);
			printcube();
		}
		break;

	case 4:
		for (int i = 0; i < rsteps; i++)
		{
			yrotate(angle);
			printcube();
		}
		break;

	case 5:
		for (int i = 0; i < rsteps; i++)
		{
			yrotate(2 * PI - angle);
			printcube();
		}
		break;
	}
}