#include "ROG_32x16_DotMatrixLEDPanel.h"

/* Constructor */
ROG_32x16_DotMatrixLEDPanel::ROG_32x16_DotMatrixLEDPanel(
  uint8_t n_panel,
  uint8_t pin_se, uint8_t pin_abb,
  uint8_t pin_a3, uint8_t pin_a2,  uint8_t pin_a1, uint8_t pin_a0,
  uint8_t pin_dg, uint8_t pin_clk, uint8_t pin_we, uint8_t pin_dr, uint8_t pin_ale) :
  Adafruit_GFX(DOT_PANEL_WIDTH * n_panel, DOT_PANEL_HEIGHT),
  pinSE(pin_se), pinABB(pin_abb),
  pinA3(pin_a3), pinA2(pin_a2), pinA1(pin_a1), pinA0(pin_a0),
  pinDG(pin_dg), pinCLK(pin_clk), pinWE(pin_we), pinDR(pin_dr), pinALE(pin_ale),
  nPanel(n_panel), transactionCounter(0) {
  if (!bufferR) {
    bufferR = (uint32_t*) calloc(n_panel * DOT_PANEL_HEIGHT, sizeof(uint32_t));
  }
  if (!bufferG) {
    bufferG = (uint32_t*) calloc(n_panel * DOT_PANEL_HEIGHT, sizeof(uint32_t));
  }
}

/* Destructor */
ROG_32x16_DotMatrixLEDPanel::~ROG_32x16_DotMatrixLEDPanel(void) {
  if (bufferR) {
    free(bufferR);
  }
  if (bufferG) {
    free(bufferG);
  }
  if (sjCode) {
    free(sjCode);
  }
  if (ZorH) {
    free(ZorH);
  }
  if (fntBmp) {
    free(fntBmp);
  }
  if (fntBmpColor) {
    free(fntBmpColor);
  }
}

/* Initialize */
void ROG_32x16_DotMatrixLEDPanel::begin() {
  pinMode(pinSE,  OUTPUT);
  pinMode(pinABB, OUTPUT);
  pinMode(pinA3,  OUTPUT);
  pinMode(pinA2,  OUTPUT);
  pinMode(pinA1,  OUTPUT);
  pinMode(pinA0,  OUTPUT);
  pinMode(pinDG,  OUTPUT);
  pinMode(pinCLK, OUTPUT);
  pinMode(pinWE,  OUTPUT);
  pinMode(pinDR,  OUTPUT);
  pinMode(pinALE, OUTPUT);

  digitalWrite(pinSE,  LOW);
  digitalWrite(pinABB, LOW);
  digitalWrite(pinA3,  LOW);
  digitalWrite(pinA2,  LOW);
  digitalWrite(pinA1,  LOW);
  digitalWrite(pinA0,  LOW);
  digitalWrite(pinDG,  LOW);
  digitalWrite(pinCLK, LOW);
  digitalWrite(pinWE,  LOW);
  digitalWrite(pinDR,  LOW);
  digitalWrite(pinALE, LOW);

  fillScreen(DOT_BLACK);

  /* default */
  textcolor = DOT_RED;
  textbgcolor = DOT_BLACK;
}

/* Draw pixel */
void ROG_32x16_DotMatrixLEDPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
  startWrite();
  writePixel(x, y, color);
  endWrite();
}

/* Begin write data */
void ROG_32x16_DotMatrixLEDPanel::startWrite() {
  transactionCounter++;
}

/* Finish write data */
void ROG_32x16_DotMatrixLEDPanel::endWrite() {
  if (--transactionCounter) {
    return;
  }
  if (!bufferR || !bufferG) {
    return;
  }

  for (uint16_t y = 0; y < HEIGHT; y++) {
    /* Select RAM (Y) address */
    digitalWrite(pinA0, y & 1);
    digitalWrite(pinA1, y & 2);
    digitalWrite(pinA2, y & 4);
    digitalWrite(pinA3, y & 8);

    /* Write X dots to shift register */
    for (uint16_t x = 0; x < WIDTH; x++) {
      uint16_t i = (x >> 5) + (y * nPanel);
      uint8_t panelX = x & 31;

      digitalWrite(pinDR, (bufferR[i] >> panelX) & 1);
      digitalWrite(pinDG, (bufferG[i] >> panelX) & 1);
      digitalWrite(pinCLK, HIGH);
      digitalWrite(pinCLK, LOW);
    }

    /* Write data from shift register to RAM */
    digitalWrite(pinALE, HIGH);
    digitalWrite(pinWE, HIGH);
    digitalWrite(pinWE, LOW);
    digitalWrite(pinALE, LOW);
  }

  /* For auto RAM A/B changing */
  digitalWrite(pinA0, 0);
  digitalWrite(pinA1, 0);
  digitalWrite(pinA2, 0);
  digitalWrite(pinA3, 0);
}

/* Write pixel data */
void ROG_32x16_DotMatrixLEDPanel::writePixel(int16_t x, int16_t y, uint16_t color) {
  if (!bufferR || !bufferG || x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
    return;
  }

  uint16_t i = (x >> 5) + (y * nPanel);
  uint8_t panelX = x & 31;

  switch (color) {
    case DOT_RED:
      bufferR[i] |= 1 << panelX;
      break;
    case DOT_GREEN:
      bufferG[i] |= 1 << panelX;
      break;   
    case DOT_ORANGE:
      bufferR[i] |= 1 << panelX;
      bufferG[i] |= 1 << panelX;
      break;
    default:
      bufferR[i] &= ~(1 << panelX);
      bufferG[i] &= ~(1 << panelX);
      break;
  }
}

/* Use for shinonome font */
void ROG_32x16_DotMatrixLEDPanel::ShinonomeFNT_SD_init(
    uint8_t   pin_SD_CLK,
    uint8_t   pin_SD_MISO,
    uint8_t   pin_SD_MOSI,
    uint8_t   pin_SD_CS,
    const uint32_t  SD_Frep,
    const char* UTF8SJIS_filePath,
    const char* Shino_Zen_Font_filePath,
    const char* Shino_Half_Font_filePath,
    ESP32_SD_ShinonomeFNT* Shino
  ) {
    
    SF = Shino;

    /* SD card Initialization */
    Serial.print("Initializing SD card...");

    SPI.begin(pin_SD_CLK, pin_SD_MISO, pin_SD_MOSI, pin_SD_CS);  // CLK, MISO, MOSI, SS

    if (!SD.begin(pin_SD_CS, SPI)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");

    /* Open font file in SD card */
    SF->SD_Shinonome_Init3F(UTF8SJIS_filePath, Shino_Half_Font_filePath, Shino_Zen_Font_filePath);
}

/* Setup for use shinonome font */
void ROG_32x16_DotMatrixLEDPanel::shnmFntBegin(uint8_t max_word) {
  if (max_word < 4) {
    /* Minimum number of character is 4 */
    MAX_WORD = 4;
  } else {
    MAX_WORD = max_word;
  }

  /* Buffers of shinonome font */
  if (!sjCode) {
    sjCode = (uint8_t*) calloc(MAX_WORD * 2, sizeof(uint8_t));
  }
  if (!ZorH) {
    ZorH = (boolean*) calloc(MAX_WORD, sizeof(boolean));
  }
  if (!fntBmp) {
    /* Number of character is MAX_WORD when Zenkaku, MAX_WORD * 2 when Hankaku */
    fntBmp = (uint8_t*) calloc(MAX_WORD * 2 * 16, sizeof(uint8_t));
  }
  if(!fntBmpColor) {
    fntBmpColor = (uint16_t*) calloc(MAX_WORD * 8 * 2 * 16, sizeof(uint16_t));
  }
}

/* Serial print Hankaku font bitmap line 8bit */
void ROG_32x16_DotMatrixLEDPanel::shnmFntDbg_FontDispLine8(uint8_t buf) {
  for(int i=0; i<8; i++) {
    if(((128>>i)&buf)>>(7-i)) {
      Serial.print("#");
    }else{
      Serial.print(" ");
    }
  }
}

/* Serial print Zenkaku font bitmap line 16bit */
void ROG_32x16_DotMatrixLEDPanel::shnmFntDbg_FontDispLine16(uint8_t buf_l, uint8_t buf_r) {
  shnmFntDbg_FontDispLine8(buf_l);
  shnmFntDbg_FontDispLine8(buf_r);
}

// http://dencha.ojaru.jp/programs_sub/zenkaku.html
// Shift-JIS コードから全角・半角を判断する関数
// 引数： Shift-JISコードの配列の長さ 配列のインデックス
uint8_t ROG_32x16_DotMatrixLEDPanel::shnmFntZenOrHan(uint16_t len) {
  
  uint8_t cnt = 0;   // Number of characters

  for(int i=0; i<len; i++) {
    if ( (sjCode[i] >=0x81 && sjCode[i]<=0x9f) || (sjCode[i]>=0xe0 && sjCode[i]<=0xfc)){    // 全角の第１バイト
        if (sjCode[i+1]>=0x40 && sjCode[i+1]<=0xfc) {    // 全角の第２バイト
          ZorH[cnt] = 1;  // Zenkaku
          cnt++;
          i++;
        }
    }else{
      ZorH[cnt] = 0;  // Hankaku
      cnt++;
    }
  }
  
  return cnt;
}

/* Serial print str font */
void ROG_32x16_DotMatrixLEDPanel::shnmFntDbg_FontDisp(String str) {
  
  /* String(UTF-8)->Shift-JIS code. and get sj_code array length */
  uint16_t sj_length = SF->UTF8toSJIS_convert(str, sjCode);
  /* String(UTF-8)->BitMap(8bitx16bit) */
  uint8_t fntBmpTmp[MAX_WORD][16] ={0};
  SF->StrDirect_ShinoFNT_readALL(str, fntBmpTmp);
  for(int i=0; i<MAX_WORD; i++) {
    for(int line=0; line<16; line++) {
      fntBmp[i*16 + line] = fntBmpTmp[i][line];
    }
  }

  /* Zenkaku or Hankaku by Shift-JIS code, and get nubmer of character */
  wordCnt = shnmFntZenOrHan(sj_length);

  Serial.println("\n ### Font Display Test ###");
  // Serial.printf("index = %d\n", index);
  Serial.print("str: "); Serial.println(str); 
  Serial.printf("sj_length = %d\n", sj_length);
  Serial.printf("Word count = %d\n\n", wordCnt);

  /* font bitmap index */
  int font_index = 0;
  
  for(uint8_t i=0; i<wordCnt; i++) {
    Serial.printf("%d=>\n", i);
    
    if(ZorH[i]) { // Zenkaku
      
      for(int j=0; j<16; j++) {
        shnmFntDbg_FontDispLine16(fntBmp[font_index*16 + j], fntBmp[(font_index+1)*16 + j]);
        Serial.println();
      }

      font_index = font_index + 2;
      
    }else{  // Hankaku
      
      for(int j=0; j<16; j++) {
        shnmFntDbg_FontDispLine8(fntBmp[font_index*16 + j]);
        Serial.println();
      }
      
      font_index++;
      
    }
  }
}

// /* Get str bitmap width  */
// uint16_t ROG_32x16_DotMatrixLEDPanel::shnmFntGetTxtWidth(String str) {
//   // String(UTF-8)->Shift-JIS code. and get sj_code array length
//   uint16_t sjLen = SF->UTF8toSJIS_convert(str, sjCode);
//   // Zenkaku or Hankaku by Shift-JIS code. and get nubmer of character
//   wordCnt = shnmFntZenOrHan(sjLen);
//   // Text length limit
//   if (wordCnt > MAX_WORD) {
//     Serial.println("str ->");
//     Serial.println(str);
//     Serial.print("MAX_WORD ->");
//     Serial.print(MAX_WORD);
//     Serial.print("  wordCnt ->");
//     Serial.println(wordCnt);
//     Serial.println("エラー：文字数が設定された表示可能文字数を超えています。");

//     str = "ｴﾗｰ";  // Display error message

//     // String(UTF-8)->Shift-JIS code. and get sj_code array length
//     sjLen = SF->UTF8toSJIS_convert(str, sjCode);
//     // Zenkaku or Hankaku by Shift-JIS code. and get nubmer of character
//     wordCnt = shnmFntZenOrHan(sjLen);
//   }
  
//   uint16_t width = 0;  // width of shinonome font bitmap data

//   // Culculate text width
//   for(int i=0; i<wordCnt; i++) {
//     width += ZorH[i] + 1;  // 全角1 半角0
//   }

//   return width;
// }

/* Text font convert to shinonome font */
uint16_t ROG_32x16_DotMatrixLEDPanel::shnmFntStrConv(String str) {

  /* Initialize */
  for(int i=0; i < MAX_WORD * 2 * 16 ;i++) {
    fntBmp[i] = 0;
  }

  /* String(UTF-8)->Shift-JIS code. and get sj_code array length */
  uint16_t sjLen = SF->UTF8toSJIS_convert(str, sjCode);
  /* Zenkaku or Hankaku by Shift-JIS code. and get nubmer of character */
  wordCnt = shnmFntZenOrHan(sjLen);

  /* Text length limit */
  if (wordCnt > MAX_WORD) {
    Serial.println("str ->");
    Serial.println(str);
    Serial.print("MAX_WORD ->");
    Serial.print(MAX_WORD);
    Serial.print("  wordCnt ->");
    Serial.println(wordCnt);
    Serial.println("エラー：文字数が設定された表示可能文字数を超えています。");

    str = "ｴﾗｰ";  // Display error message

    /* String(UTF-8)->Shift-JIS code. and get sj_code array length */
    sjLen = SF->UTF8toSJIS_convert(str, sjCode);
    /* Zenkaku or Hankaku by Shift-JIS code. and get nubmer of character */
    wordCnt = shnmFntZenOrHan(sjLen);
  }
  
  uint16_t numOf8x16 = 0;  // number of 8x16 bitmap data

  /* Culculate text width */
  for(int i=0; i<wordCnt; i++) {
    numOf8x16 += ZorH[i] + 1;
  }

  /* String(UTF-8)->BitMap(8bitx16bit) */
  uint8_t fntBmpTmp[str.length()][16] ={0};

  SF->StrDirect_ShinoFNT_readALL(str, fntBmpTmp);
  for(int i=0; i<numOf8x16; i++) {
    for(int line=0; line<16; line++) {
      fntBmp[i*16 + line] = fntBmpTmp[i][line];
    }
  }

  return numOf8x16 * 8;
}

/* Text font convert to shinonome font with color */
uint16_t ROG_32x16_DotMatrixLEDPanel::shnmFntStrConvWithColor(String str) {

  uint16_t charIdx = 0;       // coordinate of x axis
  uint16_t tColor = DOT_RED;  // default text color
  uint8_t fntBmpTmp[MAX_WORD][16] ={0};

  int16_t idx1, idx2;
  uint16_t wordTotalCnt = 0;
  String strTmp;

  uint16_t sjLen;

  /* Initialize */
  for (int i=0; i<MAX_WORD * 8 * 2 * 16; i++) {
    fntBmpColor[i] = DOT_BLACK;
  }

  idx1 = 0;  // begin of string processing
  idx2 = 0;  // end of string processing


  while(idx2 != str.length() && wordTotalCnt < MAX_WORD) {
    if (str.charAt(idx1) == '[') {
      idx2 = str.indexOf(']', idx1);

      if (idx2 > 0) {
        while (idx2 > 0 && str.charAt(idx2-1) == '\\') {
          if (str.charAt(idx2-1) == '\\') {
            idx2 = str.indexOf(']', idx2+1);
            
            while(str.charAt(idx2-1) == '\\') {
              idx2 = str.indexOf(']', idx2+1);
            }
          }
        }
        strTmp = str.substring(idx1, idx2+1);
        
        strTmp.replace("\\]", "]");
      }
    } else {
      idx2 = str.indexOf('[', idx1);

      if (idx2 == -1) {
        idx2 = str.length();
        strTmp = str.substring(idx1, idx2);
      } else if (idx2 > 0) {
        strTmp = str.substring(idx1, idx2);
      }
    }
    
    if (idx1 >= 0 && idx2 > idx1) {
      /* Text color setting */
      if(strTmp.charAt(0) == '[') {
        if (strTmp.charAt(1) == 'r') {
          tColor = DOT_RED;
        } else if (strTmp.charAt(1) == 'g') {
          tColor = DOT_GREEN;
        } else if (strTmp.charAt(1) == 'o') {
          tColor = DOT_ORANGE;
        }
        strTmp = strTmp.substring(2, strTmp.length() - 1);
      } else {
        tColor = textcolor;
      }

      /* String(UTF-8)->Shift-JIS code. and get sj_code array length */
      sjLen = SF->UTF8toSJIS_convert(strTmp, sjCode);
      /* Zenkaku or Hankaku by Shift-JIS code. and get nubmer of character */
      wordCnt = shnmFntZenOrHan(sjLen);

      /* Text length limit */
      if (wordCnt > MAX_WORD) {
        Serial.println("str -> ");
        Serial.println(str);
        Serial.print("MAX_WORD -> ");
        Serial.print(MAX_WORD);
        Serial.print("  wordCnt -> ");
        Serial.println(wordCnt);
        Serial.println("エラー：文字数が設定された表示可能文字数を超えています。");

        strTmp = "ｴﾗｰ";  // display error message

        /* String(UTF-8)->Shift-JIS code. and get sj_code array length */
        sjLen = SF->UTF8toSJIS_convert(strTmp, sjCode);
        /* Zenkaku or Hankaku by Shift-JIS code. and get nubmer of character */
        wordCnt = shnmFntZenOrHan(sjLen);
      }

      if (wordTotalCnt + wordCnt <= MAX_WORD) {
        /* Culculate total text length */
        wordTotalCnt += wordCnt;

        uint16_t numOf8x16 = 0;  // number of 8x16 bitmap data
        for(int i=0; i<wordCnt; i++) {
          numOf8x16 += ZorH[i] + 1;
        }

        /* String(UTF-8)->BitMap(8bitx16bit) */
        SF->StrDirect_ShinoFNT_readALL(strTmp, fntBmpTmp);
        
        uint16_t fntBmpColorIdx;

        for(int i=0; i<numOf8x16; i++) {
          for(int line=0; line<16; line++) {
            for(int bit=0; bit<8; bit++) {
              fntBmpColorIdx = charIdx*8*16 + i*8*16 + line*8 + bit;

              if((fntBmpTmp[i][line] & (0b10000000 >> bit)) > 0) {
                fntBmpColor[fntBmpColorIdx] = tColor;
              } else {
                fntBmpColor[fntBmpColorIdx] = DOT_BLACK;
              }
            }
          }
        }
        
        charIdx += numOf8x16;
        
      } else {
        Serial.print("word total count -> ");
        Serial.print(wordTotalCnt);
        Serial.println();
        Serial.print("word count -> ");
        Serial.print(wordCnt);
        Serial.println();
        Serial.print("str.length -> ");
        Serial.print(str.length());
        Serial.println();
        Serial.print("idx1 -> ");
        Serial.print(idx1);
        Serial.print("  idx2 -> ");
        Serial.println(idx2);
        Serial.print("MAX_WORD -> ");
        Serial.println(MAX_WORD);
        Serial.println("エラー1：文字数が設定された表示可能文字数を超えたため、一部の文字は表示されません。");
        Serial.print("表示されない文字列 -> ");
        Serial.println(str.substring(idx1, str.length()));
      }
      
      if (str.charAt(idx2) == '[' ) {
        idx1 = idx2;
      } else {
        idx1 = idx2 + 1;
      }
    }
  }

  if (idx2 != str.length()) {
    Serial.print("word total count -> ");
    Serial.print(wordTotalCnt);
    Serial.println();
    Serial.print("word count -> ");
    Serial.print(wordCnt);
    Serial.println();
    Serial.print("str.length -> ");
    Serial.print(str.length());
    Serial.print("idx1 -> ");
    Serial.print(idx1);
    Serial.print("  idx2 -> ");
    Serial.println(idx2);
    Serial.print("MAX_WORD -> ");
    Serial.println(MAX_WORD);
    Serial.println("エラー2：文字数が設定された表示可能文字数を超えたため、一部の文字は表示されません。");
  }

  return charIdx * 8;
}

/* Draw shinonome font text */
void ROG_32x16_DotMatrixLEDPanel::shnmFntWrite(void) {

  uint16_t fntBmpX0;
  uint16_t panelX0;
  uint16_t fntBmpIdx_l, fntBmpIdx_r;  // left and right 8bit address of fntBmp
  uint8_t bmp8bit;

  if (cursor_x >= 0) {
    fntBmpX0 = 0;
    panelX0 = cursor_x;
  } else {
    fntBmpX0 = -cursor_x;
    panelX0 = 0;
  }

  for(uint8_t panelIdx=0; panelIdx<nPanel; panelIdx++) {
    for(int line=0; line<16; line++) {
      for(int slot=0; slot<4; slot++) {
        fntBmpIdx_l = (panelIdx * 64) + (slot * 16) + (fntBmpX0 / 8)*16 + line;
        fntBmpIdx_r = (panelIdx * 64) + ((slot+1) * 16) + (fntBmpX0 / 8)*16 + line;

        bmp8bit = 0;
        if (fntBmpIdx_l >= 0 && fntBmpIdx_l < MAX_WORD * 2 * 16) bmp8bit |= fntBmp[fntBmpIdx_l] << (fntBmpX0 % 8);
        if (fntBmpIdx_r >= 0 && fntBmpIdx_r < MAX_WORD * 2 * 16) bmp8bit |= fntBmp[fntBmpIdx_r] >> (8-(fntBmpX0 % 8));
        
        for(int bit=0; bit<8; bit++) {
          int16_t x = (panelIdx * DOT_PANEL_WIDTH) + (slot * 8) + (7 - bit) + panelX0;
          if((bmp8bit & (0b00000001 << bit)) > 0) {
            writePixel(x, line + cursor_y, textcolor);
          } else {
            writePixel(x, line + cursor_y, textbgcolor);
          }
        }
      }
    }
  }
}

/* Draw shinonome font text with color */
void ROG_32x16_DotMatrixLEDPanel::shnmFntWriteWithColor(void) {
  
  uint16_t fntBmpX0;
  uint16_t panelX0;

  if (cursor_x > 0) {
    fntBmpX0 = 0;
    panelX0 = cursor_x;
  } else {
    fntBmpX0 = -cursor_x;
    panelX0 = 0;
  }

  
  for(uint8_t panelIdx=0; panelIdx<nPanel; panelIdx++) {
    for(uint8_t line=0; line<DOT_PANEL_HEIGHT; line++) {
      for(uint8_t slot=0; slot<4; slot++) {
        for(uint8_t bit=0; bit< (8 - fntBmpX0 % 8); bit++) {
          writePixel(panelIdx*DOT_PANEL_WIDTH + slot*8 + bit + panelX0,
                     line + cursor_y, 
                     fntBmpColor[
                       panelIdx * DOT_PANEL_WIDTH * DOT_PANEL_HEIGHT +
                       slot * 8 * DOT_PANEL_HEIGHT + 
                       (fntBmpX0 / 8) * 8 * DOT_PANEL_HEIGHT +
                       line * 8 + 
                       fntBmpX0 % 8 + bit
                      ]);
        }
        for (uint8_t bit=0; bit<fntBmpX0 % 8; bit++) {
          writePixel(panelIdx*DOT_PANEL_WIDTH + slot*8 + 7-bit + panelX0,
                     line + cursor_y, 
                     fntBmpColor[
                       panelIdx * DOT_PANEL_WIDTH * DOT_PANEL_HEIGHT +
                       (slot+1) * 8 * DOT_PANEL_HEIGHT + 
                       (fntBmpX0 / 8) * 8 * DOT_PANEL_HEIGHT +
                       line * 8 + 
                       (fntBmpX0 % 8 - (bit + 1))
                      ]);
        }
      }
    }
  }
}

/* Print shinonome font */
void ROG_32x16_DotMatrixLEDPanel::shnmFntPrint(String str) {
  shnmFntStrConv(str);

  startWrite();
  shnmFntWrite();
  endWrite();
}

/* Print shinonome font with color */
void ROG_32x16_DotMatrixLEDPanel::shnmFntPrintWithColor(String str) {
  shnmFntStrConvWithColor(str);

  startWrite();
  shnmFntWriteWithColor();
  endWrite();
}

/* Scroling shinonome text font */
void ROG_32x16_DotMatrixLEDPanel::shnmFntScroll(String str, uint8_t speed_ms) {
  
  uint16_t tWidth;
  tWidth = shnmFntStrConv(str);

  for(int x0 = WIDTH; x0 > -(tWidth + WIDTH); x0--) {
    startWrite();
    setCursor(x0, 0);
    shnmFntWrite();
    delay(speed_ms);
    endWrite();
  }
}

/* Scroling shinonome text font with color */
void ROG_32x16_DotMatrixLEDPanel::shnmFntScrollWithColor(String str, uint8_t speed_ms) {

  uint16_t tWidth;
  tWidth = shnmFntStrConvWithColor(str);

  for(int x0 = WIDTH; x0 > -(tWidth + WIDTH); x0--) {
    startWrite();
    setCursor(x0, 0);
    shnmFntWriteWithColor();
    delay(speed_ms);
    endWrite();
  }
}

/* Draw Bitmap Image for exported by GIMP C-Souce image dump */
/* Usage: 
   背景は白でレイヤーを統合して1つのレイヤになった状態でCソースコードのエクスポートを選び、
   適当な「前置名」を入力し、「struct ではなくマクロを使う」のみにチェックを入れてエクスポートする。 */
void ROG_32x16_DotMatrixLEDPanel::drawGIMP_CSourceImageDump(int16_t x, int16_t y,
                                  const uint8_t bitmap[], int16_t w, int16_t h) {
  
  startWrite();
  for (int16_t j=0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (bitmap[(j * 3) * w + i * 3 + 0] > 0) writePixel(x + i, y, DOT_RED);
      if (bitmap[(j * 3) * w + i * 3 + 1] > 0) writePixel(x + i, y, DOT_GREEN);
    }
  }
  endWrite();
}
