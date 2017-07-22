# 人感センサ/距離センサによるスクリーンロック回避USBデバイス

会社のポリシー上10分でPCにロックをかけることになっているが、
別PCを使っている間にロックがかかると、
毎日何回もロック解除のためのパスワード入力をしていて無駄なので、
自席にいる間はロックしないようにするためのデバイスです。

人感センサ/距離センサにより、在席中の場合は9分おきにマウス操作をPCに送り付けます。

## [距離センサ版](VL53L0X)

## 人感センサ版

![写真](../img/avoidscreenlock.jpg)

```
人感センサ---Teensy------------[USB]---PC1
               |
               +---Digispark---[USB]---PC2
```

以下のArduino系デバイスを使って、PCにはマウスとして見える形にします。

+ ATmega32U4を使っているArduinoや互換機: Arduino Micro, Leonardo, ダ・ヴィンチ32U, Pro Micro, Pololu A-Star 32U4 Micro
+ Teensy
+ Digispark

## 部品
+ [Teensy LC](https://www.switch-science.com/catalog/2333/)
  または、[Digispark](http://www.elefine.jp/SHOP/Digispark.html)
+ [人感センサ NaPiOn AMN32112](http://www.sengoku.co.jp/mod/sgk_cart/detail.php?code=5A2C-MNGX)
+ 抵抗 10kΩ

回路は、
[『Prototyping Lab――「作りながら考える」ためのArduino実践レシピ』](http://www.oreilly.co.jp/books/9784873114538/)
にあるものそのままです。

## 関連
+ [PresenceStick](https://github.com/deton/presencestick)の画面ロック回避機能。
+ [感圧センサで離席検出してPCスクリーンロックするBLEデバイス](https://github.com/deton/ChairLeave)
+ BluetoothデバイスがPCから離れた時に自動ロック。
  Bluetooth 4.0のProximity Profile(PXP)。
  (Bluetoothデバイスを持ち歩く必要あり)
	+ [ぶるタグ](http://pc.nikkeibp.co.jp/article/column/20120912/1062902/?P=2)
	+ http://buffalo.jp/product/news/2012/09/05_04/
	+ [Ubuntuで同様に](http://gihyo.jp/admin/serial/01/ubuntu-recipe/0267)
+ [WinSensorシリーズ（SENSOR-HM/ECO）](http://www.iodata.jp/product/lcd/option/sensor-hmeco/)。人感センサーで離席時に省電力モードに切替

## 備考
Linux PCでも使いたかったのですが、
xinitから起動してxscreensaverを使っている環境では効果無し。
