    $ alias nrfgostudio="WINEPREFIX=~/.wine32 wine 'C:/Program Files/Nordic Semiconductor/nRFgo Studio/nRFgoStudio'"
    $ INPUT=UART_over_BLE.xml
    $ nrfgostudio -nrf8001 -g $INPUT -codeGenVersion 1 -o .

Note: when generating output nRFgo Studio will overwrite any existing services.h, services_lock.h and ublue_setup.gen.out.txt files without warning.
