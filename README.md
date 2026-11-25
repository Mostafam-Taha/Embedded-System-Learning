# ☀️ Sun Alarm – ESP32 Light Alarm with Phone Notifications

A beginner-friendly ESP32 project that detects when the sun comes up (or when a room gets dark) using a light sensor (LDR). When triggered it:

- turns on an **LED** and a **buzzer**
- shows the status on a small **web page** hosted by the ESP32
- sends a **push notification** to your phone using [ntfy](https://ntfy.sh)
- can be silenced with a **physical button** or from the web page

> Built step by step as a first embedded-systems project: LED → buzzer → button → light sensor → Wi-Fi web page → phone notifications.

---

## 🧰 Parts

| Part | Qty |
|---|---|
| ESP32 Dev Board (ESP32-WROOM) | 1 |
| Breadboard | 1 |
| Red LED | 1 |
| Resistor ~220Ω–330Ω (for the LED) | 1 |
| Active buzzer | 1 |
| Push button (2-pin tactile) | 1 |
| LDR (light-dependent resistor) | 1 |
| Resistor 10kΩ (brown-black-orange) | 1 |
| Jumper wires (male-male and male-female) | several |

A potentiometer is also in the kit but is not used in this version (see *Ideas*).

---

## 🔌 Wiring

All grounds go to the breadboard's blue (−) rail, which is connected to one **GND** pin on the ESP32.

| Component | Connection |
|---|---|
| LED (+, long leg) → resistor | resistor other end → **GPIO 26** |
| LED (−, short leg) | **GND** |
| Buzzer (+) | **GPIO 27** |
| Buzzer (−) | **GND** |
| Button leg 1 | **GPIO 25** |
| Button leg 2 | **GND** |
| LDR leg 1 | **3V3** |
| LDR leg 2 | **GPIO 34** *(also joined to the 10kΩ resistor)* |
| 10kΩ resistor other end | **GND** |

The LDR and the 10kΩ resistor form a *voltage divider*: GPIO 34 reads a number between 0 (dark) and 4095 (very bright).

### Breadboard layout used in this build

Columns A–E on a row number are connected together.

| Part | Position |
|---|---|
| LED + / resistor | A10 / B10 |
| Resistor other leg → wire to GPIO 26 | B14 / A14 |
| LED − → wire to GND | A18 / B18 |
| Buzzer + → wire to GPIO 27 | A22 / B22 |
| Buzzer − → wire to GND rail | A25 / B25 |
| Button legs | E30, E32 (wires A30 → GPIO 25, A32 → GND rail) |
| LDR | A36, A40 |
| 10kΩ resistor | B40, B44 |
| LDR wires | C36 → 3V3, C40 → GPIO 34, C44 → GND rail |

---

## 💻 Setup

1. Install the **Arduino IDE**.
2. Add ESP32 support: *Boards Manager → search "esp32" → install*.
3. Select **Tools → Board → ESP32 Dev Module** and the right **Port**.
4. Install the **ntfy** app on your phone, tap **+**, and subscribe to a long, hard-to-guess topic name (for example `my-sun-alarm-48271`).
   - Enable *Instant delivery in Doze mode* (Android).
   - Leave *Use another server* unchecked.
5. Open [`sun_alarm.ino`](sun_alarm.ino) and edit:
   ```cpp
   const char* ssid = "YOUR_WIFI_NAME";
   const char* password = "YOUR_WIFI_PASSWORD";
   String topic = "my-sun-alarm-48271";
   int limit = 2500;
   ```
6. Upload. If it stops at `Connecting...`, hold the **BOOT** button on the board.
7. Open **Serial Monitor** at **115200** baud and read the IP address, e.g. `http://192.168.1.25`.
8. Open that address in your phone browser (same Wi-Fi network).

> ⚠️ The ESP32 only works with **2.4 GHz** Wi-Fi.

---

## 🎚️ Calibrating the threshold (`limit`)

1. Look at the **Light** number on the web page (or Serial Monitor) at night, e.g. `200`.
2. Look at it again in daylight, e.g. `3500`.
3. Set `limit` to a value **between** them and re-upload.

To make it alarm in the **dark** instead of in the light, change this line in `loop()`:

```cpp
bool bright = light > limit;   // sun alarm
// bool dark = light < limit;  // dark alarm (also swap the names below)
```

---

## 🧠 How it works

- `loop()` reads the LDR continuously.
- When the reading goes above `limit`, the LED and buzzer turn on and a notification is sent **once**.
- Pressing the button (or the *Silence* link on the web page) turns the alarm off.
- When the light drops below `limit` again, the alarm re-arms for the next sunrise.

---

## 🌦️ Using it outdoors

- Put the ESP32 and breadboard in a **closed plastic box**, out of rain and direct sun.
- Leave only the LDR looking out through a small clear window.
- Power it with a USB charger or a power bank.
- Make sure the Wi-Fi signal reaches the spot.

---

## 🚀 Ideas for next steps

- Use the **potentiometer** to adjust the threshold by hand (`analogRead(35)`).
- Add **temperature / humidity** (DHT22) and **rain** sensors.
- Send readings to a home server (e.g. Raspberry Pi) and plot them.
- Garage monitoring with cameras and plate recognition (the ESP32 handles sensors, a server does the AI).

---

## 🔒 Notes

- Don't commit your real Wi-Fi password to GitHub.
- Anyone who knows your ntfy topic can read its messages, so keep the name private.

## 📄 License

MIT – free to use and modify.
