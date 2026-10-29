/*
 * oledcontoroller.c
 *
 *  Created on: Nov 6, 2025
 *      Author: dhksw
 */

#include "oledcontoroller.h"
#include "run_man.h"

static uint8_t m_toggle = 0;
static uint8_t m_toggle_count = 0;

void printTemperNoUpdate(int temper);

void opening() {

	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("start!", &Font_11x18, 1);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts("  GO_CHU :)", &Font_11x18, 1);
	SSD1306_UpdateScreen(); //display
	HAL_Delay(1000);

	//SSD1306_InvertDisplay(1);

	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, runmanlog1, 128, 64, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(500);

	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, runmanlog2, 128, 64, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(500);

	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, runmanlog3, 128, 64, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(500);

	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, runmanlog4, 128, 64, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(500);

	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, runmanlog5, 128, 64, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(500);
}

void printDefault() {

	SSD1306_Clear();
	SSD1306_GotoXY(1, 0);
	SSD1306_Puts("Temp  Relay", &Font_11x18, 1);
	SSD1306_UpdateScreen();

	SSD1306_GotoXY(0, 15);
	SSD1306_Puts("-----------", &Font_11x18, 1);
	SSD1306_UpdateScreen();
	SSD1306_GotoXY(8, 38);

	char temper_str[100] = "";
	itoa(DEFAULT_TEMPER, temper_str, 10);
	strcat(temper_str,".0");
	SSD1306_Puts(temper_str, &Font_11x18, 1);
	SSD1306_UpdateScreen();

	SSD1306_GotoXY(81, 38);
	SSD1306_Puts("off", &Font_11x18, 1);
	SSD1306_UpdateScreen();

}

void printBackground() {

	SSD1306_Clear();
	SSD1306_GotoXY(1, 0);
	SSD1306_Puts("Temp  Relay", &Font_11x18, 1);
	SSD1306_UpdateScreen();

	SSD1306_GotoXY(0, 15);
	SSD1306_Puts("-----------", &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void printTemperNoUpdate(int temper) {

	SSD1306_GotoXY(8, 38);
	char temper_str[100] = "";
	sprintf(temper_str, "%2d.0", temper);
	SSD1306_Puts(temper_str, &Font_11x18, 1);
}


void printTemper(int temper) {

	SSD1306_GotoXY(8, 38);
	char temper_str[100] = "";
	sprintf(temper_str, "%2d.0", temper);
	SSD1306_Puts(temper_str, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void toggleScreen() {
	if (m_toggle_count > 0) {
		if (!m_toggle) {
			SSD1306_Clear();
			SSD1306_UpdateScreen();
			m_toggle = 1;
		} else {
			printBackground();
			printTemperNoUpdate(getFixedTemper());
			printHeaterState(OFF_t);
			m_toggle = 0;
		}
  	}
	if(m_toggle_count > 0){
		m_toggle_count--;
	}
}

void startToggle() {
	if(m_toggle_count == 0){
	m_toggle_count = 4;
	}
}

void printHeaterState(ON_OFF_t onoff){

	SSD1306_GotoXY(81, 38);
	if(onoff == ON_t){
		SSD1306_Puts(" On", &Font_11x18, 1);
	}else{
		SSD1306_Puts("off", &Font_11x18, 1);
	}
	SSD1306_UpdateScreen();
}
