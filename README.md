Nordic BLE
----------

This project attempts to achieve a clear separation between the nRF8001 house keeping logic, that is repeated again and again across projects, and the code specific to the services being implemented for a particular project.

This code here is intended to work with the [Nordic nRF8001](http://www.nordicsemi.com/eng/Products/Bluetooth-R-low-energy/nRF8001]).

In particular the [nRF8001 breakout](https://www.adafruit.com/product/1697) from Adafruit.

Nordic have created an [Arduino SDK](https://github.com/NordicSemiconductor/ble-sdk-arduino/) for the nRF8001 that includes lots of [example code](https://github.com/NordicSemiconductor/ble-sdk-arduino/tree/master/libraries/BLE/examples).

And Adafruit have, as with most of their products, created an excellent [tutorial](https://learn.adafruit.com/getting-started-with-the-nrf8001-bluefruit-le-breakout) for their breakout.

However Adafruit go out of their way to hide the underlying interaction with the nRF8001, providing their own [library](https://github.com/adafruit/Adafruit_nRF8001) (which internally includes an old version of the Nordic SDK library code).

Even if you dig into the details of the Adafruit library or into the Nordic examples you'll find that neither make any effort to separate from the code specific to the particular project from standard nRF8001 house keeping logic.

The Nordic [UART example](https://github.com/NordicSemiconductor/ble-sdk-arduino/tree/master/libraries/BLE/examples/ble_uart_project_template) is very similar to the Adafruit code.

Both implement a custom UART service using BLE.

Here I've separated out the nRF8001 house keeping logic into a seperate class [BleCore](https://github.com/george-hawkins/nordic-ble/blob/master/nRF8001-core/BleCore.cpp) that has no dependencies on the UART specific aspects of the project.

What really distinguishes the simple kind of projects covered by the Nordic examples is the configuration created using [nRFgo Studio](https://www.nordicsemi.com/chi/node_176/2.4GHz-RF/nRFgo-Studio).

nRFgo Studio is a GUI application with which one specifies the BLE services, their characteristics etc., that make up a profile that will be uploaded to the nRF8001.

Such a configuration is saved as an XML file, e.g. like the [UART_over_BLE.xml](https://github.com/george-hawkins/nordic-ble/blob/master/ble-uart-peripheral/uart/UART_over_BLE.xml) file used here.

From this XML file one can generate a C header file called services.h that includes various #defines.

One of these #defines specifies the binary configuration data that your code will need to upload to the nRF8001 using the Nordic SDK library.

Most of the other #defines relate to what Nordic calls pipes, the entities with which your code interacts with the characteristics you've configured on the nRF8001.

So in the end it's the logic that depends on this services.h file that really distinguishes one project, at least in BLE terms, from another.

So here, in addition to separating out the nRF8001 house keeping logic, I've localized the services.h dependant logic as far as possible.

In the case of the UART implementation this comes down to a tiny amount of code contained in [BleUart.cpp](https://github.com/george-hawkins/nordic-ble/blob/master/ble-uart-peripheral/BleUart.cpp).

Even here much of the code is boilerplate, in particular the definition of `setup_msgs` and `setup_info` which needs to done for every project.

Notes
-----

This code depends on:
* The Nordic BLE SDK - the latest available distribution can be found on their GitHub [releases page](https://github.com/NordicSemiconductor/ble-sdk-arduino/releases).
* The Arduiniana [Streaming library](http://arduiniana.org/libraries/streaming/) - which is used for most of the Serial output in this project.

The nRFgo Studio generated XML file used here uses the same service and characteristic names, base UUID and short form UUIDs as those used in the already mentioned Adafruit and Nordic UART projects.

Installation
------------

You need to:
* Install the Arduino library from the Nordic SDK, i.e. the SDK subfolder libraries/BLE, into your Arduino IDE libraries folder.
* Install the Arduiniana Streaming library.
* Retrieve this project (either with the GitHub "Download ZIP" option or by cloning).
* Once it's unpacked or cloned you need to go into the ble-uart-peripheral/uart folder and use nRFgo Studio to generate the services.h file (see below).
* Then you need to copy or move the nRF8001-core and ble-uart-peripheral folders into your Arduino IDE libraries folder.

Now in the Arduino IDE you can open File --> Examples --> ble-uart-peripheral --> ble_stream_echo.

You can then upload this sketch to an Arduino with nRF8001 breakout, configured as described in the Adafruit product tutorial, and it should work just like the Adafruit UART sketch when used with the Android and iOS UART apps from Nordic.

You can generate services.h from the Windows command line with nRFgo Studio like so:
```bat
C:\...\nordic-ble> cd ble-uart-peripheral\uart
C:\...\nordic-bleble-uart-peripheral\uart> nrfgostudio -nrf8001 -g UART_over_BLE.xml -codeGenVersion 1 -o .
```

You'll need to replace `nrfgostudioa` with the location where you installed nRFgo Studio, typically `C:\Program Files\Nordic Semiconductor\nRFgo Studio\nRFgoStudio`.

You can also start nRFgo Studio normally, load the XML file and then select the menu item nRF8001 Setup --> Generate Source Files --> Generate only services.h.

If you're using Linux or Mac OS X the following commands show how to retrieve and install everything from scratch:
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

The above assumes you've already installed nRFgo Studio using [Wine](https://www.winehq.org/about/) with the `WINEPREFIX` `~/.wine32` and that you're using a 1.5.X release of the Arduino IDE (if using a 1.0.X release the libraries directory would be found under `~/sketchbook` on Linux).

Restarting Bluetooth
--------------------

You can completely quit out of the Nordic Android UART app by presseing "Disconnect" if currently connected and then pressing the system back button. The app will then ask if you really want to quit.

However I've often gotten the Android Bluetooth stack into a state where quiting and restarting the app isn't enough to reset things.

These situations could only be resolved by turning Bluetooth off (via the normal mechanism) outside the app and then turning it back on again (or restating the UART app and letting it request Bluetooth be turned back on).

The first time I saw this situation it was quite hard to diagnose as the app could establish a connection but could no longer receive any UART data.

I thought I'd introduced a bug in my code but in the end found that the Android UART app could no longer successfully trigger the actions that result in the opening of a pipe on the nRF8001.

TODO
----

The Arduino IDE implies that the contents of the .data and .bss sections end up in RAM and correspond to global variables.

Currently the two make up a noticeable amount, around 40%, of the available 2KB of an ATmega328P, find out what's taking up this space (perhaps the IDE is mis-counting PROGMEM data?).

Adafruit
--------

All this started with the Adafruit nRF8001 breakout and the related product tutorial.

As they usually note in their source:
```cpp
/*
 * Adafruit invests time and resources providing this open source code, please
 * support Adafruit and open-source hardware by purchasing products from Adafruit!
 */
```

So why not pickup an [nRF8001 breakout](https://www.adafruit.com/product/1697) now if you haven't already?

Nordic
------

There are other players in the hobbyist BLE market - Bluegiga and TI and particular.

However these other manufacturers (Jeff Rowberg's heroic efforts on the part of Bluegiga aside) don't seem to make much effort to cater to the hobbyist market - generally requiring expensive programmers, IDEs etc. to really use their products.

Nordic seem to make a much bigger effort to make their products useable by hobbyists.

I hope this pays off for them in terms of general good will and in the new environment where hobbyists can end up producing products with noticeable production runs, e.g. via Kickstarter.
