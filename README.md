# ESP32/ESP8266 Telegram Bot for LED Control

## Overview
This project enables remote control of LEDs using Telegram Bot commands via an **ESP32** or **ESP8266** microcontroller. The system supports:
- Turning LEDs **ON/OFF**
- Checking LED **state**
- Applying **custom LED patterns**

## Features
- Secure authentication using **Telegram Bot API**
- LED control using Telegram commands
- Custom binary LED patterns with delay
- Real-time feedback on LED states

## Components Required
- **ESP32 or ESP8266**
- **4 LEDs**
- **Resistors** (220Ω recommended)
- **Jumper wires**
- **WiFi connection**

## Libraries Used
Ensure you have the following libraries installed in the **Arduino IDE**:
- `WiFi` (for ESP32) / `ESP8266WiFi` (for ESP8266)
- `WiFiClientSecure`
- `UniversalTelegramBot`
- `ArduinoJson`

## Circuit Diagram
Connect the LEDs to the following GPIO pins:
| LED | GPIO Pin (ESP32) | GPIO Pin (ESP8266) |
|-----|---------------|----------------|
| LED1 | 2 | 2 |
| LED2 | 15 | 15 |
| LED3 | 4 | 4 |
| LED4 | 5 | 5 |

## Setup Instructions
1. **Create a Telegram Bot**
   - Open Telegram and search for `BotFather`.
   - Use `/newbot` command and follow the instructions.
   - Copy the **Bot Token**.

2. **Update WiFi Credentials**
   - Replace `ssid` and `password` with your WiFi network details.

3. **Update Telegram Bot Token and Chat ID**
   - Replace `BOTtoken` with your bot's token.
   - Replace `CHAT_ID` with your Telegram ID.

4. **Upload Code**
   - Connect the ESP32/ESP8266 to your PC.
   - Select the correct board and COM port in Arduino IDE.
   - Compile and upload the code.

5. **Monitor Serial Output**
   - Open the Serial Monitor (115200 baud rate) to check WiFi and bot connection.

## Telegram Commands
| Command | Description |
|---------|-------------|
| `/start` | Display welcome message and command list |
| `/led_on` | Turn all LEDs ON |
| `/led_off` | Turn all LEDs OFF |
| `/state` | Get current LED states |
| `/pattern <binary> <delay>` | Apply custom LED pattern |

## Example Commands
- `/pattern 1010 500` → Turns ON LED1 & LED3 for 500ms.
- `/pattern 1111 1000` → Turns ON all LEDs for 1 second.

## Troubleshooting
- **Bot not responding?** Check WiFi connection and Telegram API token.
- **Invalid command format?** Ensure correct usage: `/pattern 1010 500`.

## Future Improvements
- Integrating more LED patterns
- Adding a web-based control panel
- Storing LED states in EEPROM for persistence

## License
This project is open-source under the **MIT License**.
