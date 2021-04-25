#include <ROG_32x16_DotMatrixLEDPanel.h>
#include "kaomoji_64x16_01.c"


/*** ROG_32x16_DotMatrixLEDPanel *************************/
/* pin assign ->      [ESP32] [PANEL]*/
const uint8_t SE_PIN  =  5;  // 1p
const uint8_t ABB_PIN = 18;  // 2p
const uint8_t A3_PIN  = 27;  // 3p
const uint8_t A2_PIN  = 19;  // 4p
const uint8_t A1_PIN  = 26;  // 5p
const uint8_t A0_PIN  = 21;  // 6p
/*      Controller  Vss         7p */
const uint8_t DG_PIN  = 25;  // 8p
const uint8_t CLK_PIN = 33;  // 9p
const uint8_t WE_PIN  = 32;  // 10p
const uint8_t DR_PIN  = 22;  // 11p
const uint8_t ALE_PIN = 23;  // 12p

const uint8_t num_panel = 2;  // number of panel
const uint8_t led_width  = 32 * num_panel;
const uint8_t led_height = 16;

/* Limit number of characters to display */
const uint8_t MAX_WORD = 200;  // Max word count (shoud to set 4 or more)

ROG_32x16_DotMatrixLEDPanel matrix(
  num_panel,
  SE_PIN,
  ABB_PIN,
  A3_PIN,
  A2_PIN,
  A1_PIN,
  A0_PIN,
  DG_PIN,
  CLK_PIN,
  WE_PIN,
  DR_PIN,
  ALE_PIN
);
/*********************************************************/

/*** MicroSD card ****************************************/
/* Usage on ESP32 Breakout board with MicroSD Socket */
//const uint8_t SD_CS   = 13;  // CS(SS)
//const uint8_t SD_CLK  = 14;
//const uint8_t SD_MISO =  2;
//const uint8_t SD_MOSI = 15;
//
//const uint32_t sd_freq = 80000000;
/*********************************************************/

/*** Shinonome Font **************************************************************************/
//ESP32_SD_ShinonomeFNT SFR(SD_CS, sd_freq);
//
//const char* UTF8SJIS_file = "/font/Utf8Sjis.tbl";  // UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
//const char* Shino_Zen_Font_file = "/font/shnmk16.bdf";  // 全角フォントファイル名を定義
//const char* Shino_Half_Font_file = "/font/shnm8x16.bdf";  // 半角フォントファイル名を定義
/*********************************************************************************************/

/*** Switches ********************************************************************************/
/* Switches pin assign */
const int8_t SW_1 = 39;
const int8_t SW_2 = 34;
const int8_t SW_3 = 35;
const int8_t SW_4 = 36;  // slide switch

//volatile uint8_t sw1_Read = 0x00;
//volatile uint8_t sw2_Read = 0x00;
//volatile uint8_t sw3_Read = 0x00;
//volatile uint8_t sw4_Read = 0x00;
//volatile boolean sw1_Pressed = 0;
//volatile boolean sw2_Pressed = 0;
//volatile boolean sw3_Pressed = 0;
//volatile boolean sw4_Changed = 0;
//
///* Hardware Timer */
///* ref: https://garretlab.web.fc2.com/arduino/esp32/examples/ESP32/Timer_RepeatTimer.html */
//hw_timer_t * timer = NULL;
//volatile SemaphoreHandle_t timerSemaphore;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
//
//void IRAM_ATTR onTimer() {
//  /* Increment the counter and set the time of ISR */
//  portENTER_CRITICAL_ISR(&timerMux);
//
//  /* Read the status of switches by use N times matching method */
//  /* SW_1 */
//  sw1_Read = (0xfe & (sw1_Read << 1)) | (0x01 & (uint8_t)digitalRead(SW_1));
//  if (sw1_Read == 0x7f) sw1_Pressed = 1; // 0x7f=0111_1111 : N=7
//  /* SW_2 */
//  sw2_Read = (0xfe & (sw2_Read << 1)) | (0x01 & (uint8_t)digitalRead(SW_2));
//  if (sw2_Read == 0x7f) sw2_Pressed = 1; // 0x7f=0111_1111 : N=7
//  /* SW_3 */
//  sw3_Read = (0xfe & (sw3_Read << 1)) | (0x01 & (uint8_t)digitalRead(SW_3));
//  if (sw3_Read == 0x7f) sw3_Pressed = 1; // 0x7f=0111_1111 : N=7
//  /* SW_4 */
//  sw4_Read = (0xfe & (sw4_Read << 1)) | (0x01 & (uint8_t)digitalRead(SW_4));
//  if (sw4_Read == 0x7f || sw4_Read == 0xfe) sw4_Changed = 1;
//
//  portEXIT_CRITICAL_ISR(&timerMux);
//  // Give a semaphore that we can check in the loop
//  xSemaphoreGiveFromISR(timerSemaphore, NULL);
//  // It is safe to use digitalRead/Write here if you want to toggle an output
//}
/*********************************************************************************************/





void setup() {
  Serial.begin(115200);
  while (!Serial);

  /*** ROG_32x16_DotMatrixLEDPanel ************************************************************/
  matrix.begin();
  Serial.println("Initialized LED panel successfully.");

//  matrix.ShinonomeFNT_SD_init(SD_CLK, SD_MISO, SD_MOSI, SD_CS, sd_freq,
//                              UTF8SJIS_file, Shino_Zen_Font_file, Shino_Half_Font_file, &SFR);
//  matrix.shnmFntBegin(MAX_WORD);  // max number of characters
  matrix.setTextColor(DOT_RED, DOT_BLACK);  // text color, background color
  /********************************************************************************************/

  /*** Switches *********/
  pinMode(SW_1, INPUT);
  pinMode(SW_2, INPUT);
  pinMode(SW_3, INPUT);
  pinMode(SW_4, INPUT);
  /**********************/
  
  delay(1000);



  /***** Test of compatible with Adafruit GFX library methods *****/
  /* origin of drawing coordinates */
  /*  supported only some methods  */
  matrix.setCursor(10, 0);

  /* draw pixel */  
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.drawPixel(0, 0, DOT_RED);  // x axis, y axis, color
  matrix.drawPixel(led_width - 1, 0, DOT_GREEN);
  matrix.drawPixel(0, led_height - 1, DOT_ORANGE);
  delay(1000);
  
  /* draw pixel after write to buffer */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.startWrite();
  matrix.writePixel(0, 0, DOT_RED);  // x axis, y axis, color
  matrix.writePixel(led_width - 1, 0, DOT_GREEN);
  matrix.writePixel(0, led_height - 1, DOT_ORANGE);
  matrix.endWrite();
  delay(1000);

  /* print characters */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.println("abc");
  matrix.print("def");
  delay(3000);

  /* draw filled circle */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.fillCircle(7, 7, 7, DOT_GREEN);
  delay(1000);
  matrix.fillScreen(DOT_BLACK);
  matrix.fillCircle(16 + 7, 7, 7, DOT_ORANGE);
  delay(1000);
  matrix.fillScreen(DOT_BLACK);
  matrix.fillCircle(16 * 2 + 7, 7, 7, DOT_RED);
  delay(3000);

  /* draw XBitMap Files (*.xbm)  */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.drawXBitmap(0, 0, kaomoji_64x16_01_bits, 64, 16, DOT_RED);
  matrix.drawXBitmap(0, 0, kaomoji_64x16_01_bits, 64, 16, DOT_GREEN);
  matrix.drawXBitmap(0, 0, kaomoji_64x16_01_bits, 64, 16, DOT_ORANGE);
  delay(3000);

  matrix.fillScreen(DOT_BLACK);
}
  

void loop() {

}
