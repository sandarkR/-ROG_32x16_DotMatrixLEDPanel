# ROG_32x16_DotMatrixLEDPanel
これは、MicroSDソケット付きのESP32開発ボード用に作った、赤色と緑色の2色のドットマトリクスLEDのArduinoライブラリです。赤と緑の同時点灯で橙色も表示できるため、実質3色(Red/Orange/Green = ROG)で表現できます。

Adafruit GFX Libraryに基づいて、日本語表示を実装しました。複数枚並べたり、文字をスクロールしたり、GIMPを使って作った画像を表示したり、その他のグラフィック表示についても、動作を確認したメソッドのサンプルスケッチがあります。



# Get the Panel
大阪日本橋デジットさんで販売されています。ジャンク品扱いのため、いつ販売が終了してもおかしくありません。2021/4/25時点ではパネル本体はまだ販売されています。
- 3枚セットはこちら：[32×16ドットマトリクス3色LEDパネル(赤/橙/緑) 3枚+ケーブルセット ■限定特価品■](https://eleshop.jp/shop/g/gK86413/)
- 1枚のみならこちら：[32×16ドットマトリクスLEDパネル(赤/橙/緑)■限定特価品■](https://eleshop.jp/shop/g/gEB8411/)
- 接続用ケーブル3枚用(実店舗販売のみ)：[32×16ドットマトリクスLEDパネル接続用ケーブル3枚用](https://eleshop.jp/shop/g/gEBC412/)
- 接続用ケーブル1枚用(在庫なし)：[32×16ドットマトリクスLEDパネル接続用ケーブル1枚用](https://eleshop.jp/shop/g/gEBC411/)


デジットBlog 記事もあるようです。->[デジットイチオシ！特価32×16のドットマトリクスLEDパネル](http://blog.digit-parts.com/archives/51941371.html)


最初、店頭で見つけた時はこのブログ内にもある動画のようなデモがあり、表示がとてもきれいでしたが、1枚3,600円で手が出せなかったのを覚えています...(2016年8月くらい？) でも、ちょっと前から値下げされて、一気に1/3の値段になっています！（2021/4/25時点）



# Usage
- このライブラリは、MicroSDソケット付きのESP32開発ボード用に作っています。
  - 参考：[Bluetooth 無線 lan モジュール sma ESP32 LoRa32 V2.1 1.6 バージョン 433/868/915 mhz lora ESP-32 oled 0.96 インチ sd カード](https://ja.aliexpress.com/item/10000209791921.html?channel=twinner)
  - AliExpress内に他にも販売されていますが、それほど多くは無いようです。

- 基板も作り（キット化検討中）、余ったGPIOのなかで、入力専用の所にはスイッチの入力（4つ）を付けたり、入出力可能な2つのピン(16, 17)が残っています。良く売られているESP32開発ボードに、SDカードモジュールを接続しても使えると思いますが、動作は未保証です。

- スイッチを付けなくともマトリクスLEDのみを制御するサンプルスケッチもあります。

- このライブラリをダウンロードし、Arduino IDEの"スケッチ"->"ライブラリを管理"->「.ZIP形式のライブラリをインストール」からインストールできます。

- 日本語表示をしない限りはMicroSDカードは必要ありませんが、日本語表示を行うにはSDカードが必要です。ただし、どちらの場合でも、次のライブラリもインストールしてください。
  - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
  - [ESP32_SD_ShinonomeFNT](https://github.com/mgo-tec/ESP32_SD_ShinonomeFNT)
  - [ESP32_SD_UTF8toSJIS](https://github.com/mgo-tec/ESP32_SD_UTF8toSJIS)


## Pin Assign

| ESP32 | LEDマトリクスパネル |
| :--: | :--: |
| 05 | 1: SE_PIN |
| 18 | 2: ABB_PIN |
| 37 | 3: A3_PIN |
| 29 | 4: A2_PIN |
| 26 | 5: A1_PIN |
| 21 | 6: A0_PIN |
| -- | 7: (Controller  Vss) |
| 25 | 8: DG_PIN |
| 33 | 9: CLK_PIN |
| 32 | 10: WE_PIN |
| 22 | 11: DR_PIN |
| 23 | 12: ALE_PIN |

| ESP32 | SDカード |
| :--: | :--: |
| 13 | SD_CS    |
| 14 | SD_CLK  |
|  2 | SD_MISO |
| 15 | SD_MOSI |

| ESP32 | スイッチ |
| :--: | :--: |
| 39 | SW_1 |
| 34 | SW_2 |
| 35 | SW_3 |
| 36 | SW_4 |

※スイッチはすべてプルダウン。
 
 
 

## 使用例 on Twitter @sandarkR  
[Twitterモーメント：モバブで動くポータブル電光掲示板2](https://twitter.com/i/events/1379372812234727424?s=20)



# Library samples
- basicExample  
このライブラリで使える全てのメソッドの使用例です。

- methodsCompatibleAdafruitGFX  
Adafruit GFX Libraryにあるメソッドで動作確認済みのメソッドの使用例です。  
参考：[Adafruit GFX Library](http://adafruit.github.io/Adafruit-GFX-Library/html/index.html)

- PortableElectricBulletinBoard  
タクトスイッチ（プルダウン）やスライドスイッチ、スマホからの制御などいろいろ詰め込んだ例です。下に書く基板がないと、動作確認は少しめんどうかも...



# Reference
このライブライリは、いくつかのライブラリや公開されているプログラムを組み込みまたは使っています。

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [ESP32_SD_ShinonomeFNT](https://github.com/mgo-tec/ESP32_SD_ShinonomeFNT)
- [ESP32_SD_UTF8toSJIS](https://github.com/mgo-tec/ESP32_SD_UTF8toSJIS)
- [ESP32_SD_EasyWebSocket](https://github.com/mgo-tec/ESP32_SD_EasyWebSocket) ※一部のサンプルスケッチのみに使用。
- [M5Stack 電光掲示板をスマホでリアルタイムコントロールしてみた](https://www.mgo-tec.com/blog-entry-m5stack-websocket-message-board-esp32.html) ※一部のサンプルスケッチのみに使用。

# Breakout board
キット化を本格的に検討はじめました。まだ、準備中ですが、購入を希望される方は現時点ではTwitterなどでDMをください！

- [キットの詳細について（準備中）](https://t.co/D9qhNwetGZ?amp=1)

- [NT金沢2021での販売の検討について](https://twitter.com/sandarkR/status/1401406649265393666?s=20)

- [Twitter sandark(@sandarkR)](https://twitter.com/sandarkR)

基板Cadデータも今後公開するかも(?)

[Tweetリンク：モバブで動く電光掲示板第2弾！](https://twitter.com/sandarkR/status/1379369349589385220?s=20)

[Twitterモーメント：モバブで動くポータブル電光掲示板2：製作記(?)](https://twitter.com/i/events/1379414496079310849?s=20)



# History
- Ver. 1.0.0  ライブラリ公開。
