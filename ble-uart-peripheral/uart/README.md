    $ alias nrfgostudio="WINEPREFIX=~/.wine32 wine 'C:/Program Files/Nordic Semiconductor/nRFgo Studio/nRFgoStudio'"
    $ nrfgostudio -nrf8001 -codeGenVersion 1 -o . -g UART_over_BLE.xml

Note: when generating output nRFgo Studio will overwrite any existing services.h, services_lock.h and ublue_setup.gen.out.txt files without warning.
