# 🔐 Arduino Vault Security System

A PIN-code protected vault system built with **Arduino**, an **I2C LCD**, push buttons, LEDs, and a buzzer. Users enter a 4-digit PIN using push buttons, and if the wrong code is entered three times, the system triggers a lockout alarm — complete with a *Super Mario Bros. "Game Over"* jingle. 🍄🎮

---

## ✨ Features

- 🔢 **4-digit PIN entry** via individual increment buttons (0–9 per digit)
- ⏱️ **Hold-to-increment** support — hold a button down to cycle digits automatically
- 📟 **16x2 I2C LCD display** showing live PIN entry and system status
- ✅ **Correct PIN** → Green LED lights up, vault "opens"
- ❌ **Wrong PIN** → Red LED blinks, PIN resets, attempt counter increases
- 🚨 **3 failed attempts** → System locks, red LED stays on, and a Mario-style "death" tune plays on the buzzer

---

## 🛠️ Hardware Required

| Component                  | Quantity |
|-----------------------------|----------|
| Arduino (Uno/Nano/etc.)     | 1        |
| 16x2 I2C LCD (address 0x27) | 1        |
| Push buttons                | 5 (4 digit + 1 enter) |
| Green LED                   | 1        |
| Red LED                     | 1        |
| Buzzer                      | 1        |
| Resistors / jumper wires    | as needed |

### Pin Connections

| Component     | Arduino Pin |
|----------------|-------------|
| Digit Button 1 | 2 |
| Digit Button 2 | 3 |
| Digit Button 3 | 4 |
| Digit Button 4 | 5 |
| Enter Button   | 6 |
| Buzzer         | 10 |
| Red LED        | 11 |
| Green LED      | 12 |
| LCD (SDA/SCL)  | I2C pins (A4/A5 on Uno) |

> All buttons use `INPUT_PULLUP`, so wire them between the pin and GND — no external resistors needed.

---

## 📦 Libraries Used

- [`Wire.h`](https://www.arduino.cc/en/reference/wire) — I2C communication
- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) — LCD control over I2C

Install `LiquidCrystal_I2C` via the Arduino IDE Library Manager before uploading.

---

## 🚀 How It Works

1. On startup, the LCD displays a welcome message, then shows the PIN entry screen (`0 0 0 0`).
2. Press a digit button to increase that digit (0–9, wraps back to 0). Hold it down to auto-increment every 400ms.
3. Press **Enter** to submit the PIN.
4. If correct:
   - LCD shows `PIN Accepted / Vault Opened`
   - Green LED turns on, system halts.
5. If incorrect:
   - LCD shows `Wrong PIN / Try Again`
   - Red LED blinks 3 times, PIN resets to `0000`
   - After **3 wrong attempts**, the system locks:
     - LCD shows `!!! ALARM !!! / System Locked`
     - Red LED stays on
     - Buzzer plays the Mario "Game Over" jingle 🎵
     - System halts until reset

---

## ⚙️ Setup

1. Wire the components according to the pin table above.
2. Open the `.ino` file in the Arduino IDE.
3. Install the `LiquidCrystal_I2C` library if you haven't already.
4. Change `SECRET_PIN[]` in the code to set your own 4-digit code.
5. Upload the sketch to your Arduino.
6. Power up and start entering PINs! 🔓

```cpp
const int SECRET_PIN[4] = {7, 1, 5, 9}; // 🔑 change this to your own PIN
```

---

## 🎯 Future Improvements

- Add EEPROM storage to persist wrong-attempt count across power cycles
- Add a reset/reboot button instead of requiring a full power cycle
- Support changing the PIN via a "master code"
- Add a servo-actuated physical lock mechanism

---

## 📄 License

Feel free to use, modify, and build on this project for personal or educational purposes.
