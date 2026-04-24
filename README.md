# 🔐 Smart Locker – Bare-Metal Embedded System (STM32F411)

## 📌 Overview
Smart Locker is a bare-metal embedded system designed to simulate a real-world **access control system** using an STM32F411 microcontroller.

The project demonstrates low-level driver development, peripheral integration, and system design without relying on external frameworks such as HAL.

---

## 🎯 Features

- 🔢 Keypad-based PIN authentication
- 📡 Bluetooth (HC-06) remote control
- 📟 LCD user interface
- 🔊 Buzzer feedback (success / error patterns)
- 🕒 RTC (DS1307) for timestamped events
- 💾 SD Card logging system
- 🔐 Relay-controlled lock mechanism
- ⚡ Interrupt-driven input handling (EXTI + NVIC)

---
## 🧠 System Architecture
Application Layer
↓
Services (Auth, Logger, Input)
↓
Peripheral Drivers (GPIO, SPI, I2C, UART)
↓
MCU Register Layer (stm32f411re.h)

---

## 🧩 Project Structure
├── inc/
│ ├── stm32f411re.h
│ ├── stm32f411re_gpio_driver.h
│ ├── platform.h
│
├── src/
│ ├── stm32f411re_gpio_driver.c
│ ├── main.c
│
├── .vscode/
├── README.md

---

## 🔧 Technologies Used

- Microcontroller: STM32F411RE (ARM Cortex-M4)
- Language: C (bare-metal)
- Toolchain: arm-none-eabi-gcc
- IDE: VS Code / STM32CubeIDE
- Debug: ST-Link

---

## ⚙️ Core Concepts Implemented

- Register-level programming (no HAL)
- Bit manipulation & masking
- NVIC interrupt configuration
- EXTI (external interrupt controller)
- Peripheral clock control (RCC)
- Memory-mapped I/O
- Modular driver architecture

---

## 🔄 Example Flow (Unlock Sequence)

1. User enters PIN via keypad or Bluetooth  
2. System validates input  
3. If correct:
   - Relay activates (unlock)
   - LCD displays "Access Granted"
   - Buzzer emits success tone
   - Event logged with timestamp
4. If incorrect:
   - LCD shows error
   - Buzzer alert
   - Attempt logged

---

## 🚀 Getting Started

### Requirements
- STM32F411RE board (NUCLEO recommended)
- ST-Link debugger
- ARM GCC toolchain
- Serial terminal

🧪 Testing
Verify LED (PA5)
Test keypad input
Trigger EXTI interrupts
Validate Bluetooth commands
Check SD logging

⚠️ Known Limitations
No RTOS (cooperative only)
Basic error handling
No encryption for Bluetooth

🔮 Future Improvements
FreeRTOS integration
Secure authentication
Mobile app interface
Power optimization
Multi-user support

📚 References
STM32F411 Reference Manual (RM0383)
ARM Cortex-M4 NVIC documentation

👨‍💻 Author

Octavio Piña
