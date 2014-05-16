#include "stdafx.h"
#pragma comment(lib , "LogitechLcd.lib")
#pragma comment(lib,"gdiplus.lib")
#include "afxdialogex.h"
#include "gdiplus.h"
#include "LogitechLcd.h"
#include <time.h>

#define STRIFELCD_STAT_GPM				1
#define STRIFELCD_STAT_GOLD				2
#define STRIFELCD_STAT_KILLS			3
#define STRIFELCD_STAT_DEATHS			4
#define STRIFELCD_STAT_ASSISTS			5
#define STRIFELCD_STAT_LEVEL			6
#define STRIFELCD_STAT_CURRENT_HEALTH	7
#define STRIFELCD_STAT_MAX_HEALTH		8
#define STRIFELCD_STAT_CURRENT_MANA		9
#define STRIFELCD_STAT_MAX_MANA			10
#define STRIFELCD_APP_NAME				L"Strife Profile"


class StrifeLCD
{
private:
	const int STARTING_GPM = 0;
	const int STARTING_GOLD = 0;
	const int STARTING_KILLS = 0;
	const int STARTING_DEATHS = 0;
	const int STARTING_ASSISTS = 0;
	const int STARTING_LEVEL = 1;

	unsigned long int currentHealth;
	unsigned long int maxHealth;
	unsigned long int currentMana;
	unsigned long int maxMana;
	unsigned int gold;
	unsigned int gpm;
	unsigned int kills;
	unsigned int deaths;
	unsigned int assists;
	unsigned int level;

	unsigned char currentScreen;

	void displayStatsScreen1();
	void displayStatsScreen2();
	bool setMonoBackgroundFromFile(TCHAR* fileName);
	bool drawProgressBar(int lineNumbers[4]);

	void clearDisplay();

public:
	StrifeLCD();

	//Start the connection to the logitech LCD.
	bool initStrifeLCD();

	//Disconnect from the logitech LCD screen.
	bool shutdownStrifeLCD();

	//Show the intro screen.
	void displaySplashScreen();

	//Update a stat with the given value.
	void updateStat(unsigned char statType, unsigned long int statValue);

	//Ends the display of the Strife Splash Screen and updates the display screen to display the appropriate stats.
	void updateScreen();
};