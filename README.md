# R30_RC

This project is an 802.15.4 based remote control meant to pair with my [C21_ASF_RC_CAR](https://github.com/briantate/C21_ASF_RC_Car) project

It uses the Atmel [SAMR30 Xplained Pro](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/UserGuides/50002612A.pdf) board which features the SAMR30 wireless microcontroller. This is a Cortex M0+ with subGHz 802.15.4 radio. 

The battery powered SAMR30 samples two joysticks (via ADC), each of which controls the speed of one side of the differential steering system in the C21_ASF_RC_CAR. The sampled ADC data is filtered/limited and sent over the 802.15.4 radio using the Microchip MiWi protocol.

![RC](Media/rc_open.jpg)