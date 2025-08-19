
# Lindas Gröna Sköna – Del 2 (VG): TMP102-drivrutin för ESP32-C3 via I²C

**Väl godkänt – Drivrutin för TMP102 på ESP32-C3 via I²C**  
Del 2 av kursen *Inbyggda system och arkitektur*, projekt **Lindas Gröna Sköna**.  
Implementerar en modulär temperatursensordrivrutin i C med abstrakt gränssnitt (`temp_driver.h`) och UART-loggning.

## Funktioner
- ESP-IDF-projekt för **ESP32-C3**.
- I²C Master, läser TMP102 @ `0x48` (register `0x00`) med repeated start.
- Konverterar **12-bit two's complement** till Celsius (0.0625°C/LSB).
- Felhantering: timeout, NACK, okänt fel.
- Appen loggar temperatur var **5:e sekund**.

## Hårdvara / koppling
- **VDD** 3.3V, **GND** GND
- **SDA** -> GPIO4 (med pull-up 4.7k–10k till 3.3V)
- **SCL** -> GPIO5 (med pull-up 4.7k–10k till 3.3V)
- I²C-adress: `0x48` (standard för TMP102-moduler)

![Wiring](assets/wiring.png)

> Behöver du andra pinnar? Ändra `I2C_SDA_GPIO` och `I2C_SCL_GPIO` i `main/tmp102.c`.

## Bygga & köra (ESP-IDF 5.x)
```bash
idf.py set-target esp32c3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```
> Justera portflaggan `-p` efter din miljö.

## Projektstruktur
```
.
├─ CMakeLists.txt
├─ assets/
│  └─ wiring.png
├─ index.html
└─ main/
   ├─ CMakeLists.txt
   ├─ main.c
   ├─ temp_driver.h
   └─ tmp102.c
```
## Demo website: https://notblonde.github.io/inbyggda-system-del2/

## Köra i webbläsaren (Wokwi – utan hårdvara)

Detta projekt kan demonstreras helt i webbläsaren med **Wokwi** (ESP32‑C3 Arduino).
Simuleringen efterliknar firmwareflödet: mäter var 5:e sekund, skriver UART‑loggar
och visar ibland simulerade I²C‑fel (timeout/NACK).

### Snabbstart (3 steg)
1. Öppna https://wokwi.com/ → **New Project** → **ESP32‑C3 Arduino**.
2. Ersätt innehållet i `sketch.ino` med koden nedan (se “Wokwi‑sketch”). 
[https://github.com/NotBlonde/inbyggda-system-del2/blob/main/sketch.ino](https://github.com/NotBlonde/inbyggda-system-del2/blob/main/Wokwi%E2%80%91sketch.cpp)
3. Skapa en fil `diagram.json` (Project files → Add file) och klistra in innehållet
   från “diagram.json” nedan. Klicka **Run** och öppna **Serial Monitor**.
https://github.com/NotBlonde/inbyggda-system-del2/blob/main/diagram.json

### Vad som visas
```text
TMP102 demo startar...
TMP102 init OK.
Temp: raw=328, 20.50 C
...
Fel: I2C timeout
Fel: I2C NACK/busfel


## Exempeloutput
```
TMP102 demo startar...
TMP102 init OK.
Temp: raw=328, 20.50 C
Temp: raw=329, 20.56 C
...
```

## Licens
MIT
