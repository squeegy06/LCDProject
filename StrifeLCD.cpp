// StrifeLCD.cpp : Defines the entry point for the console application.
//

#include "StrifeLCD.h"
#include <stdio.h>

TCHAR monoImageFileName[] = _T("LogiLogoMono.bmp");

ULONG_PTR g_gdiplusToken;

StrifeLCD::StrifeLCD(){}

bool StrifeLCD::initStrifeLCD()
{
	gold = STARTING_GOLD;
	gpm = STARTING_GPM;
	kills = STARTING_KILLS;
	deaths = STARTING_DEATHS;
	assists = STARTING_ASSISTS;
	level = STARTING_LEVEL;

	// Start Gdiplus 
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	currentScreen = 0;
	printf("Initialized\n");
	if (true)
	{
		if (!LogiLcdInit(STRIFELCD_APP_NAME, LOGI_LCD_TYPE_MONO))
		{
			return false;
		}
		printf("Connected To Keyboard\n");
		displaySplashScreen();
		return true;
	}
	printf("Not Connected To Keyboard\n");
	return false;
}

void StrifeLCD::clearDisplay()
{
	BYTE pixels[LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT];
	for (int i = 0; i < (LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT); i++)
	{
		pixels[i] = (BYTE)0;
	}
	LogiLcdMonoSetBackground(pixels);
	LogiLcdMonoSetText(0, L"");
	LogiLcdMonoSetText(1, L"");
	LogiLcdMonoSetText(2, L"");
	LogiLcdMonoSetText(3, L"");
}

void StrifeLCD::displaySplashScreen()
{
	clearDisplay();
	printf("I displayed the splash screen\n");
	setMonoBackgroundFromFile(monoImageFileName);
	LogiLcdUpdate();
}

void StrifeLCD::displayStatsScreen1()
{
	clearDisplay();

	LogiLcdMonoSetText(0, L"Stats about GPM , Kills");
	LogiLcdMonoSetText(1, L"Stats about Gold , Deaths");
	LogiLcdMonoSetText(2, L"Stats about Level , Assists");
	LogiLcdMonoSetText(3, L"More stats.");
}

void StrifeLCD::displayStatsScreen2()
{
	clearDisplay();

	int progressBars[4];
	progressBars[0] = -1;
	progressBars[1] = 75;
	progressBars[2] = -1;
	progressBars[3] = 50;

	LogiLcdMonoSetText(0, L"Health");
	LogiLcdMonoSetText(2, L"Mana");
	drawProgressBar(progressBars);
}

void StrifeLCD::updateScreen()
{
	if (LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_0))
	{
		currentScreen = 0;
	}

	if (LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1))
	{
		currentScreen = 1;
	}

	switch (currentScreen)
	{
	case 0:
		displayStatsScreen1();
		break;
	case 1:
		displayStatsScreen2();
		break;
	default:
		break;
	}

	LogiLcdUpdate();
}

bool StrifeLCD::drawProgressBar(int lineNumbers[4])
{
	BYTE pixels[LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT];
	char lines[6];

	for (int p = 0; p < sizeof(lineNumbers); p++)
	{
		if (lineNumbers[p] > -1)
		{
			switch (p)
			{
			case 0:
				lines[0] = 1;
				lines[1] = 2;
				lines[2] = 3;
				lines[3] = 4;
				lines[4] = 5;
				lines[5] = 6;
				break;
			case 1:
				lines[0] = 11;
				lines[1] = 12;
				lines[2] = 13;
				lines[3] = 14;
				lines[4] = 15;
				lines[5] = 16;
				break;
			case 2:
				lines[0] = 22;
				lines[1] = 23;
				lines[2] = 24;
				lines[3] = 25;
				lines[4] = 26;
				lines[5] = 27;
				break;
			case 3:
				lines[0] = 33;
				lines[1] = 34;
				lines[2] = 35;
				lines[3] = 36;
				lines[4] = 37;
				lines[5] = 38;
				break;
			default:
				return false;
			}

			//Create the top of the line.
			for (int i = ((160 * lines[0]) + 9); i <= ((160 * (lines[0] + 1)) - 10); i++)
			{
				pixels[i] = (BYTE)128;
			}
			//Create the bottom line.
			for (int i = ((160 * lines[5]) + 9); i <= ((160 * (lines[5] + 1)) - 10); i++)
			{
				pixels[i] = (BYTE)128;
			}
			//Create the ends.
			pixels[((160 * lines[1]) + 9)] = (BYTE)128;
			pixels[((160 * lines[2]) + 9)] = (BYTE)128;
			pixels[((160 * lines[3]) + 9)] = (BYTE)128;
			pixels[((160 * lines[4]) + 9)] = (BYTE)128;
			pixels[((160 * (lines[1] + 1)) - 10)] = (BYTE)128;
			pixels[((160 * (lines[2] + 1)) - 10)] = (BYTE)128;
			pixels[((160 * (lines[3] + 1)) - 10)] = (BYTE)128;
			pixels[((160 * (lines[4] + 1)) - 10)] = (BYTE)128;
			//Fill the bars with progress!
			int progress = (140 * lineNumbers[p]) / 100;
			for (int i = 0; i <= progress; i++)
			{
				pixels[((160 * lines[1]) + 9 + i)] = 128;
				pixels[((160 * lines[2]) + 9 + i)] = 128;
				pixels[((160 * lines[3]) + 9 + i)] = 128;
				pixels[((160 * lines[4]) + 9 + i)] = 128;
			}
		}
	}

	//Fill all the empty bits with 0.
	for (int i = 0; i < (LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT); i++)
	{
		if (pixels[i] != 128)
			pixels[i] = (BYTE)0;
	}

	LogiLcdMonoSetBackground(pixels);

	return true;
}

bool StrifeLCD::setMonoBackgroundFromFile(TCHAR* fileName)
{
	HDC hdc = ::GetDC(NULL);

	Gdiplus::Bitmap* imageMono = Gdiplus::Bitmap::FromFile( fileName );

	if (NULL == imageMono)
		return false;

	// Get the bitmap handle
	HBITMAP hBitmap = NULL;
	Gdiplus::Status status = imageMono->GetHBITMAP(RGB(0, 0, 0), &hBitmap);

	if (status != Gdiplus::Ok)
		return false;

	BITMAPINFO bitmapInfoMono = { 0 };
	bitmapInfoMono.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	// Check what we got
	int ret = GetDIBits(hdc, hBitmap, 0,
		0,
		NULL,
		&bitmapInfoMono, DIB_RGB_COLORS);

	if (LOGI_LCD_MONO_WIDTH != bitmapInfoMono.bmiHeader.biWidth || LOGI_LCD_MONO_HEIGHT != bitmapInfoMono.bmiHeader.biHeight)
	{
		::MessageBoxW(NULL, _T("Oooops. Make sure to use a 160 by 43 image for mono background."), _T("LCDDemo"), MB_ICONEXCLAMATION);
		return false;;
	}

	bitmapInfoMono.bmiHeader.biCompression = BI_RGB;
	bitmapInfoMono.bmiHeader.biHeight = -bitmapInfoMono.bmiHeader.biHeight; // this value needs to be inverted, or else image will show up upside/down

	BYTE byteBitmapMono[LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT * 4];

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by byteBitmapMono.
	ret = GetDIBits(hdc, hBitmap, 0,
		-bitmapInfoMono.bmiHeader.biHeight, // height here needs to be positive. Since we made it negative previously, let's reverse it again to make it positive.
		&byteBitmapMono,
		(BITMAPINFO *)&bitmapInfoMono, DIB_RGB_COLORS);

	BYTE byteBitmapMono8bpp[LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT];

	// Transform 32bpp data into 8bpp. Let's just take the value of first of 4 bytes (Blue)
	for (int ii = 0; ii < (LOGI_LCD_MONO_WIDTH * LOGI_LCD_MONO_HEIGHT * 4); ii = ii + 4)
	{
		byteBitmapMono8bpp[ii / 4] = byteBitmapMono[ii];
	}

	LogiLcdMonoSetBackground(byteBitmapMono8bpp);

	// delete the image when done 
	if (imageMono)
	{
		delete imageMono;
		imageMono = NULL;
	}

	return true;
}

bool StrifeLCD::shutdownStrifeLCD()
{
	LogiLcdShutdown();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool done = false;
	time_t startTime = time(NULL);

	static StrifeLCD strife;

	strife.initStrifeLCD();

	while ((startTime + 2) > time(NULL)){

	}
	while (!done){
		strife.updateScreen();

		if ((startTime + 20) < time(NULL))
		{
			done = true;
		}
	}
	strife.shutdownStrifeLCD();
	return 0;
}

