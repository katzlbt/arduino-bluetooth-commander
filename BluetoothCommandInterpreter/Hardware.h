
// ====== Bluefruit UART SPI (http://www.adafruit.com/products/1697) PIN SETUP ======
// https://learn.adafruit.com/getting-started-with-the-nrf8001-bluefruit-le-breakout/pinouts

// SCK  13 (SPI - fixed by hardware)
// MISO 12 (SPI - fixed by hardware)
// MOSI 11 (SPI - fixed by hardware)
// REQ (CHIP_SELECT - SEE BELOW)
// RDY (INTERRUPT_0 - SEE BELOW)
// ACT NC
// RST (SEE BELOW)
// 3V3 OUTPUT max 200mA
// GND GND
// VIN 5V

// ====== Bluefruit UART SPI (https://www.adafruit.com/product/2633) PIN SETUP ======
// https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/pinouts

// SCK  13 (SPI - fixed by hardware)
// MISO 12 (SPI - fixed by hardware)
// MOSI 11 (SPI - fixed by hardware)
// CS  (CHIP_SELECT - SEE BELOW)
// IRQ (INTERRUPT_0 - SEE BELOW)
// DFU NC
// RST (SEE BELOW)
// GND GND
// VIN 5V

#ifdef HW_CONTROLLINO

#define CONTROLLINO_INTERRUPT_0 2
#define CONTROLLINO_INTERRUPT_1 3
#define CONTROLLINO_D0 4
#define CONTROLLINO_D1 5
#define CONTROLLINO_D2 6
#define CONTROLLINO_D3 7
#define CONTROLLINO_D4 8
#define CONTROLLINO_D5 9

// HW Real Time Clock
#define CONTROLLINO_RTC_CHIP_SELECT 10
#define CONTROLLINO_MOSI 11
#define CONTROLLINO_MISO 12
#define CONTROLLINO_SCK  13

#define CONTROLLINO_D6 18
#define CONTROLLINO_D7 19

#define CONTROLLINO_A0 14
#define CONTROLLINO_A1 15
#define CONTROLLINO_A2 16
#define CONTROLLINO_A3 17
#define CONTROLLINO_A4 20
#define CONTROLLINO_A5 21

// WARNING: A4, A5 do not work as OUTPUT!
#define BLE_CHIP_SELECT CONTROLLINO_A2
#define BLE_INTERRUPT   CONTROLLINO_INTERRUPT_0
#define BLE_RESET       CONTROLLINO_A3

#define PIN_D0 CONTROLLINO_D0
#define PIN_D1 CONTROLLINO_D1
#define PIN_D2 CONTROLLINO_D2
#define PIN_D3 CONTROLLINO_D3
#define PIN_D4 CONTROLLINO_D4
#define PIN_D5 CONTROLLINO_D5

#define PIN_A0 CONTROLLINO_A0
#define PIN_A1 CONTROLLINO_A1
// used
// used
#define PIN_A4 CONTROLLINO_A4  // input only!
#define PIN_A5 CONTROLLINO_A5  // input only!

#endif



// ProMini and Uno
#ifdef HW_UNO

// ====== Bluefruit UART SPI (http://www.adafruit.com/products/1697) PIN SETUP ======
// SPI Pins fixed by Hardware:
// SCK  13
// MISO 12
// MOSI 11

#define BLE_CHIP_SELECT 10
#define BLE_INTERRUPT    2
#define BLE_RESET        9

#define PIN_D0 4
#define PIN_D1 5
#define PIN_D2 6
#define PIN_D3 7
#define PIN_D4 8
#define PIN_D5 3

#define PIN_A0 A0
#define PIN_A1 A1
#define PIN_A2 A2
#define PIN_A3 A3
#define PIN_A4 A4  // input only!
#define PIN_A5 A5  // input only!

#endif

