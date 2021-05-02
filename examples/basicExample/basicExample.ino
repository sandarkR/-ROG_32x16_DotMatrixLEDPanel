#include <ROG_32x16_DotMatrixLEDPanel.h>
#include "kaomoji_64x16_01_Csource.c"


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
const uint16_t led_width = 32 * num_panel;
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
const uint8_t SD_CS   = 13;  // CS(SS)
const uint8_t SD_CLK  = 14;
const uint8_t SD_MISO =  2;
const uint8_t SD_MOSI = 15;

const uint32_t sd_freq = 80000000;
/*********************************************************/

/*** Shinonome Font **************************************************************************/
ESP32_SD_ShinonomeFNT SFR(SD_CS, sd_freq);

const char* UTF8SJIS_file = "/font/Utf8Sjis.tbl";  // UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
const char* Shino_Zen_Font_file = "/font/shnmk16.bdf";  // 全角フォントファイル名を定義
const char* Shino_Half_Font_file = "/font/shnm8x16.bdf";  // 半角フォントファイル名を定義
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

  matrix.ShinonomeFNT_SD_init(SD_CLK, SD_MISO, SD_MOSI, SD_CS, sd_freq,
                              UTF8SJIS_file, Shino_Zen_Font_file, Shino_Half_Font_file, &SFR);
  matrix.shnmFntBegin(MAX_WORD);  // max number of characters
  matrix.setTextColor(DOT_RED, DOT_BLACK);  // text color, background color
  /********************************************************************************************/

  /*** Switches *********/
  pinMode(SW_1, INPUT);
  pinMode(SW_2, INPUT);
  pinMode(SW_3, INPUT);
  pinMode(SW_4, INPUT);
  /**********************/
  
  delay(1000);



  /***** Test of library methods *****/
  /* origin of drawing coordinates */
  /*  supported only some methods  */
  matrix.setCursor(0, 0);

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

  /* draw bitmap with color */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.drawGIMP_CSourceImageDump(0,  // x axis
                                   0,  // y axis
                                   KAOMOJI_64X16_01_pixel_data,
                                   KAOMOJI_64X16_01_WIDTH,
                                   KAOMOJI_64X16_01_HEIGHT
                                  );
  delay(3000);

  /* display Japanese characters by convert and write with same time */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.setTextColor(DOT_RED, DOT_BLACK);  // text color, background color
  matrix.shnmFntPrint("単色表示");
  delay(3000);

  /* display Japanese characters by write after convert */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.setTextColor(DOT_GREEN, DOT_BLACK);  // text color, background color
  matrix.shnmFntStrConv("単色表示");
  matrix.startWrite();
  matrix.shnmFntWrite();  // write
  matrix.endWrite();  // display
  delay(3000);

  /* display Japanese characters with color by convert and write with same time */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.setTextColor(DOT_RED, DOT_BLACK);  // text color, background color
  matrix.shnmFntPrintWithColor("赤[r赤][g緑][o橙]");
  delay(3000);
  
  /* display Japanese characters with color by write after convert */
  matrix.fillScreen(DOT_BLACK);
  delay(500);
  matrix.setTextColor(DOT_GREEN, DOT_BLACK);  // text color, background color
  matrix.shnmFntStrConvWithColor("緑[r赤][g緑][o橙]");
  matrix.startWrite();
  matrix.shnmFntWriteWithColor();  // write
  matrix.endWrite();  // display
  delay(3000);

  /* scroll Japanese characters with color by convert and write with same time */
  matrix.fillScreen(DOT_BLACK);
  matrix.setTextColor(DOT_ORANGE, DOT_BLACK);  // text color, background color
  uint8_t delay_scroll_1 = 20;  // 20msec/dot
  matrix.shnmFntScroll("デフォルトカラーで表示されます。", delay_scroll_1);

  /* scroll Japanese characters by write after convert */  
  matrix.fillScreen(DOT_BLACK);
  matrix.setTextColor(DOT_RED, DOT_BLACK);  // text color, background color
  String displayText = "それぞれ[r赤][g緑][o橙]デフォルトカラーで表示されます。";
  uint8_t delay_scroll_2 = 20;  // 20msec/dot
  matrix.shnmFntScrollWithColor(displayText, delay_scroll_2);

  /* debug for using shinonome font */
  /*     output Serial monitor      */
  String debugText = "abcABCＡＢＣひらがなカタカナﾊﾝｶｸ漢字";
  matrix.shnmFntDbg_FontDisp(debugText);
}
  

void loop() {

}
