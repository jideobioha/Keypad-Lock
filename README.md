# Keypad-Controlled Servo Lock
EECE 458 Embedded Systems project.

A bare-metal smart lock system for Raspberry Pi Pico 2 implementing password authentication with physical servo control. Built without SDK—direct hardware register programming only.

## Overview

This embedded system demonstrates a fully functional 4-digit password-protected lock using:
- **4x3 Matrix Keypad** for password entry
- **Servo Motor** for physical lock mechanism
- **Passive Buzzer** for audio feedback
- **Asynchronous FSM** for event-driven control

**Platform:** Raspberry Pi Pico 2 (RP2350)  
**Course:** EECE 458 - Embedded System Design, Binghamton University

## Quick Start

### Build
```bash
LIBS="systick.o servo.o timer0.o passive_buzzer.o keypad.o watchdog.o led.o usbcdc.o" \
make keypad_servo_lock.uf2
```

### Flash
1. Hold BOOTSEL button on Pico
2. Connect via USB
3. Copy `keypad_servo_lock.uf2` to the drive
4. Device auto-reboots and runs

## Usage

**Unlock:** Enter 4-digit PIN → Press `*`  
**Change Password:** Press `##` → Enter new 4-digit PIN → Press `*`

✅ Correct: Confirmation tone, servo unlocks for 5 seconds  
❌ Incorrect: Error tone, servo stays locked

## Key Features

- **Asynchronous FSM** - Event-driven state transitions triggered by keypad input
- **Callback-Based Keypad Driver** - 1kHz scanning with debouncing in SysTick ISR
- **PWM Servo Control** - Pulse duration modulation for lock positioning
- **Critical Sections** - Atomic data exchange between ISRs and application
- **Timer-Based Auto-Lock** - Scheduled callback re-locks servo after timeout
- **Watchdog Protection** - Automatic system reset on software hang

## Technical Highlights

### Pin Configuration
| Component | GPIO Pins |
|-----------|-----------|
| Keypad Rows | 10-13 |
| Keypad Columns | 7-9 |
| Servo Motor | 20 (PWM) |
| LED | 25 |

### Architecture
```
Main Loop (Asynchronous)
    ↓
Keypad Driver (SysTick ISR @ 1kHz)
    ↓
FSM State Transitions
    ↓
Servo/Buzzer/LED Control
    ↓
Timer Callbacks (Auto-lock, Buzzer silence)
```

### Custom Bare-Metal Drivers
- `keypad.c` - Row-column scanning with debouncing
- `servo.c` - PWM-based position control
- `passive_buzzer.c` - Tone generation
- `systick.c` - 1ms interrupt timer
- `timer0.c` - Scheduled callbacks
- `watchdog.c` - System reliability
- `usbcdc.c` - USB serial debugging

## Design Decisions

**Why Asynchronous FSM?**  
Keypad presses are unpredictable—polling would waste CPU cycles. The FSM only executes when keypad events occur.

**Why SysTick Priority?**  
SysTick runs at highest priority to maintain consistent 1ms scanning intervals. Missing a scan could cause false key detections due to improper debouncing.

**Critical Sections**  
The keypad buffer is shared between SysTick ISR and main application. `__disable_irq()` ensures atomic reads/writes to prevent race conditions.

## Project Structure

```
├── src/
│   └── keypad_servo_lock.c      # Main FSM application
├── drivers/
│   ├── keypad.c/h                # Matrix keypad driver
│   ├── servo.c/h                 # Servo motor control
│   ├── passive_buzzer.c/h        # Audio feedback
│   ├── systick.c/h               # System timing
│   ├── timer0.c/h                # Alarm callbacks
│   └── watchdog.c/h              # Reset protection
├── baremetal/
│   └── include/rp2350/           # Hardware register definitions
└── Makefile
```

## Author

**Jidechukwu Obioha**  
Binghamton University | EECE 458  
December 2025

