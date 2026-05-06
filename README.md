# ATmega32 Vehicle Body Control ECU
 
A bare-metal embedded system simulating a vehicle body control module running on an ATmega32 microcontroller. Built entirely from scratch with minimal use of external libraries and no RTOS — custom scheduler, event queue, state machines, NVM manager, fault monitor, and hardware drivers.
 
---
 
## System Overview
 
The system is structured in 4 layers:
 
**MCAL (Hardware Layer)** — Direct register access. DIO, UART, Timer0, Timer1, EEPROM. No AVR library used except `pgmspace.h` isolated to `UART.c`.
 
**Service Layer** — Scheduler and EventQueue. The scheduler is tick-based (1ms Timer1 CTC ISR sets a flag, main loop processes it). The EventQueue is a circular buffer of 15 events shared across the whole system.
 
**Application Layer** — All state machines, output controllers, fault monitor, NVM manager, and input manager live here. Each state machine owns its state and only changes it in response to events. Outputs only read state — they never decide anything.
 
**Test Interface** — UART command handler that lets you inject inputs over serial, simulating hardware signals without physical switches.
 
---
 
## Wiring Guide
 
| Signal | Pin | Direction | Notes |
|---|---|---|---|
| Front Left Blinker | PA2 | OUTPUT | LED + resistor to GND |
| Front Right Blinker | PC0 | OUTPUT | LED + resistor to GND |
| Rear Left Blinker | PB7 | OUTPUT | LED + resistor to GND |
| Rear Right Blinker | PD7 | OUTPUT | LED + resistor to GND |
| Lock Actuator | PB1 | OUTPUT | Relay or motor driver |
| Unlock Actuator | PB2 | OUTPUT | Relay or motor driver |
| Wiper PWM Output | PB3 | OUTPUT | OC0 pin — connect to motor driver PWM input |
| UART TX | PD1 | OUTPUT | Connect to USB-Serial RX |
| UART RX | PD0 | INPUT | Connect to USB-Serial TX |
 
**UART Settings:** 9600 baud, 8N1, no flow control
 
**Power:** 5V regulated, common GND between MCU and all drivers
 
---
 
## How to Test via UART
 
Open any serial terminal (PuTTY, Arduino Serial Monitor, Tera Term) at 9600 baud. Send commands followed by Enter (`\r` or `\n`).
 
### Command Reference
 
| Command | Values | Effect |
|---|---|---|
| `IGN <n>` | 0=OFF, 1=ACC, 2=ON, 3=START | Set ignition position |
| `DOOR <n>` | 0=CLOSED, 1=OPEN | Set door state |
| `LOCK <n>` | 0=LOCK, 1=UNLOCK, 2=NONE | Press lock/unlock button |
| `BLINK <n>` | 0=OFF, 1=LEFT, 2=RIGHT, 3=HAZARD | Set blinker stalk |
| `WIP <n>` | 0=OFF, 1=INT, 2=LOW, 3=HIGH | Set wiper stalk |
| `SPD <n>` | 0=STOPPED, 1=MOVING | Set vehicle speed state |
 
Commands are debounced — the input manager requires 5 stable consecutive samples (50ms) before generating an event.
 
### Test Sequences
 
**Test 1 — Full ignition sequence:**
```
IGN 1        → expect: [INF][TRACE_IGNITION] Ignition to IGNITION_ACC
IGN 2        → expect: [INF][TRACE_IGNITION] Ignition to IGNITION_ON
IGN 3        → expect: [INF][TRACE_IGNITION] Ignition to IGNITION_START
             → after 3 seconds: [INF][TRACE_IGNITION] START auto-returned to ON
```
 
**Test 2 — Blinker ignition gate:**
```
IGN 0        → ignition off
BLINK 1      → expect: [ERR][TRACE_BLINKER] Blinker Request Rejected (IGNITION_OFF)
BLINK 3      → expect: [INF][TRACE_BLINKER] HAZARD on  (HAZARD works regardless)
```
 
**Test 3 — Lock safety (door open):**
```
DOOR 1       → door open
LOCK 0       → expect: [ERR][TRACE_LOCK] Cannot lock - Door Open
DOOR 0       → door closed
LOCK 0       → expect: [INF][TRACE_LOCK] Door locked
```
 
**Test 4 — Auto unlock on movement:**
```
LOCK 0       → lock the door first
SPD 1        → vehicle moving
             → expect: [INF][TRACE_LOCK] Auto unlocked for safety
```
 
**Test 5 — Auto lock timeout:**
```
SPD 0        → vehicle stopped, door unlocked
             → wait ~10 seconds
             → expect: [INF][TRACE_LOCK] Door locked  (auto-lock fired)
```
 
**Test 6 — Wiper ignition gate:**
```
IGN 2        → ignition ON
WIP 2        → expect: [INF][TRACE_WIPER] Wipers Set to LOW, PB3 outputs PWM
IGN 1        → drop to ACC
             → wait 5 seconds
             → expect: [INF][TRACE_WIPER] Wipers Set to off (Auto-off)
```
 
**Test 7 — Diagnostic mode:**
```
SPD 0        → must be stopped to enter diag
Send TECHNICIAN_DIAG_REQUEST event (currently only triggerable in code)
             → expect: [INF][TRACE_ECU] Entered Diag mode
             → blinkers and locks now work without ignition restrictions
             → after 10 minutes: [INF][TRACE_ECU] Diag timeout
```
 
---
 
## What Works
 
- Full ignition sequencing with sequential transition enforcement
- Blinker state machine with HAZARD override and ignition gating
- Door open/close tracking
- Lock/unlock with door-closed safety check
- Auto-lock after 10 seconds when stopped and unlocked
- Auto-unlock when vehicle starts moving while locked
- Wiper speed control via PWM on PB3 (Timer0 Phase Correct)
- Wiper auto-off after 5 seconds if ignition drops below ON
- Wiper intermittent mode with 3 second period, 500ms wipe
- Fault monitor with VOLATILE and LATCHED fault types
- NVM persistence with magic number + checksum validation
- Async EEPROM write (one byte per 1ms tick, non-blocking)
- Diagnostic mode with configurable feature overrides
- Cooperative scheduler with 5 tasks at different periods
- Full UART trace system with strings stored in flash (PROGMEM)
- UART serial command interface for simulation
---
 
## Limitations
 
**Single door sensor** — system tracks one door only. Real vehicles have per-door sensors.
 
**Simulated inputs** — InputManager reads from a software `sim_inputs_t` struct driven by UART. Real hardware pin reading is wired in but the `ReadX_Hardware()` functions return sim values. Replacing them with actual `DIO_ReadPin()` calls is the only change needed for real hardware.
 
**No vehicle speed value** — speed is binary (STOPPED/MOVING). No actual speed measurement or CAN bus integration.
 
**Fault detection is mostly stubbed** — `CheckBlinkerOutputFault`, `CheckWiperOutputFault`, and `CheckSpeedSignalFault` all return FALSE. Real detection requires current sensing feedback or signal timeout monitoring.
 
**Single NVM block** — only fault data is persisted. Lock state, ignition state, and odometer-style counters are not saved across power cycles.
 
**No watchdog timer** — if the system hangs, it stays hung. Adding `wdt_enable(WDTO_250MS)` in init and `wdt_reset()` in the main loop would fix this.
 
**No CAN/LIN communication** — the ECU operates in isolation. Real body control modules receive commands and broadcast status over a bus.
 
**UART is blocking on TX** — `UART_SendData` polls the UDRE flag. Under heavy trace output, this can add latency to the main loop. A TX ring buffer with interrupt-driven transmission would eliminate this.
 
**Wiper direction is hardware-dependent** — the ECU only controls PWM speed. The motor's mechanical park switch and cam linkage handle sweep direction and return-to-park, which is correct automotive practice.
 
---
 
## Known TODOs in Code
 
- Implement `CheckSpeedSignalFault` with timeout-based signal-loss detection
- Implement `CheckBlinkerOutputFault` with current sensing if feedback pin available
- Load last lock state from NVM on startup (NVM block for lock state not yet added)
- Add ignition fault handling to lock/reject transitions when signal fault is active
- Add technician diagnostic request via UART command (currently only triggerable in code)
- Replace `resetFunc()` (calls address 0) with proper watchdog reset
- Implement UCSRC explicit frame configuration in `UART_Init` for robustness
---
 
## Future Improvements
 
**Watchdog timer** — 3 lines of code, should be added before any real deployment.
 
**TX interrupt-driven UART** — replace blocking `UART_SendData` with a ring buffer flushed from the TX complete ISR. Removes all UART blocking from the main loop.
 
**Real speed signal** — connect a hall effect sensor or wheel speed sensor to a GPIO with interrupt capture. Replace the binary STOPPED/MOVING with actual RPM/speed value and use it to gate features more precisely.
 
**CAN or LIN communication** — even a basic UART-framed protocol that broadcasts system state periodically would bring this much closer to a real ECU architecture.
 
**Per-door tracking** — extend `door_state_t` to a bitmask or array, one bit per door. The state machine and lock logic would need minor changes.
 
**Fault freeze frame** — when a fault activates, capture and store the full system state (ignition, lock, speed, operational mode) not just the lock and op state currently saved.