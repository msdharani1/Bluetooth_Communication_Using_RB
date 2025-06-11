# 🚀 Distance Measurement and Bluetooth Communication Using Rugged Board

## 📖 Project Overview

This project demonstrates how to use a **Rugged Board (RB)** to measure distance using an **ultrasonic sensor** and transmit the measured data to a **mobile phone** via **Bluetooth (HC-05 module)**. The data is processed on the Rugged Board and sent wirelessly, providing a simple IoT application for real-time distance monitoring.

---

## 🧰 Components Used

- ✅ Rugged Board (e.g., A5D2x)
- ✅ Ultrasonic Sensor (HC-SR04)
- ✅ HC-05 Bluetooth Module
- ✅ Jumper Wires
- ✅ 1.5kΩ Resistor (for voltage level shifting on echo pin)
- ✅ Host PC (for programming and debugging)
- ✅ Android Mobile (with Bluetooth terminal app)

---

## ⚙️ Features

- Real-time distance measurement (in cm)
- Processed and formatted data sent to a mobile device via Bluetooth
- Demonstrates embedded systems + wireless communication

---

## 🔌 Circuit Connections

### Ultrasonic Sensor (HC-SR04)
| Sensor Pin | Rugged Board GPIO | Description                      |
|------------|-------------------|----------------------------------|
| VCC        | 5V                | Power supply                     |
| GND        | GND               | Ground                           |
| TRIG       | PC11 (Output)     | Trigger signal from RB           |
| ECHO       | PC25 (Input)      | Echo signal to RB (5V → 3.3V via 1.5kΩ resistor) |

⚠️ **Note:** The Echo pin outputs 5V which must be dropped to 3.3V using a 1.5kΩ resistor to protect the RB input pin.

---

### HC-05 Bluetooth Module
| HC-05 Pin | Rugged Board | Description                      |
|-----------|--------------|----------------------------------|
| VCC       | 5V           | Power supply                     |
| GND       | GND          | Ground                           |
| TXD       | RX (UART on RB) | Data sent to RB (from HC-05)  |
| RXD       | TX (UART on RB via voltage divider) | Data sent from RB (to HC-05) |

---

## 🖥️ How It Works

1. Rugged Board sends a trigger pulse via **PC11** to the ultrasonic sensor.
2. Sensor emits a sound wave and listens for the echo.
3. Echo is received on **PC25**, and the time delay is measured.
4. Distance is calculated and formatted by the Rugged Board.
5. Data is transmitted to the mobile device over Bluetooth using HC-05.

---

## 📲 Mobile Side

- Install a Bluetooth Terminal App (e.g., Serial Bluetooth Terminal).
- Pair with HC-05 (`PIN: 1234` or `0000`).
- Open the terminal app to receive live distance data from the sensor.

---

## 🧾 Sample Output

Distance: 15.4 %
Distance: 14.7 %
Distance: 16.1 %
