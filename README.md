Notes
-----

This code uses the Nordic BLE SDK available from https://github.com/NordicSemiconductor/ble-sdk-arduino/releases

The nRFgo Studio generated XML file was originally taken from https://github.com/adafruit/Adafruit_nRF8001/blob/master/utility/uart/UART_over_BLE.xml

However the .h files etc. were regenerated using the latest version nRFgo Studio. See uart/README.md for more details.

The usability of Serial was improved using the Streaming library from http://arduiniana.org/libraries/streaming/

Installation
------------

```bash
$ curl --location https://github.com/NordicSemiconductor/ble-sdk-arduino/archive/0.9.4.beta.tar.gz | tar -xzf -
$ ln -s $PWD/ble-sdk-arduino-0.9.4.beta/libraries/BLE ~/Arduino/libraries/ble-sdk-arduino
$ curl --remote-name http://arduiniana.org/Streaming/Streaming5.zip
$ unzip Streaming5.zip -d ~/Arduino/libraries
$ git clone git@github.com:george-hawkins/nordic-ble.git
$ cd nordic-ble
$ ln -s $PWD/nRF8001-core ~/Arduino/libraries
$ ln -s $PWD/ble-uart-peripheral ~/Arduino/libraries
$ cd ble-uart-peripheral/uart
$ alias nrfgostudio="WINEPREFIX=~/.wine32 wine 'C:/Program Files/Nordic Semiconductor/nRFgo Studio/nRFgoStudio'"
$ nrfgostudio -nrf8001 -g UART_over_BLE.xml -codeGenVersion 1 -o .
```

Now in the Arduino IDE you can open File --> Examples --> ble-uart-peripheral --> ble_stream_echo.

Restarting Bluetooth
--------------------

You can completely quit out of the Android UART app by presseing "Disconnect" if currently connected and then pressing the system back button. The app will then ask if you really want to quit.

However I have gotten the Android Bluetooth stack into a state where quiting and restarting the app isn't enough to reset things.

This situation could only be resolved by turning Bluetooth off (via the normal mechanism) outside the app and then turning it back on again (or restating the UART app and letting it request Bluetooth be turned back on).

This situation was quite hard to diagnose as the app could establish a connection but could no longer received any of the UART data.

I thought I'd introduced a bug in my code but in the end found that the UART app could no longer successfully trigger the actions that result in the opening of a pipe on the nRF8001.

TODO
----

Rework this page once the logic has all been moved to mirror the logic of Nordic's examples/ble_uart_project_template.

But keep the Adafruit comment below as their board is used.

Add license headers to source files.

The Arduino IDE implies that the contents of the .data and .bss sections end up in RAM and correspond to global variables.

Currently the two make up 802 bytes, i.e. 39% of the available 2K, so find out what's taking up this space (perhaps the IDE is mis-counting PROGMEM data?).

Adafruit
--------

Some code, in particular BleCore::pollAci(), is derived from the Adafruit code at:

    https://github.com/adafruit/Adafruit_nRF8001/

Here is the lead comment for that code - why not take their advice and buy something from Adafruit.

```cpp
/*********************************************************************
This is a library for our nRF8001 Bluetooth Low Energy Breakout

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1697

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Kevin Townsend/KTOWN  for Adafruit Industries.  
MIT license, check LICENSE for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/
```
