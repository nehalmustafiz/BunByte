# BunByte 🐰

A robotic bunny that flaps its ears and moves its body when you get close. Controlled by ultrasonic sensor + optional serial commands.

## How It Works

When someone gets within **10cm** of the ultrasonic sensor and sends a command via serial, the bunny performs:

| Step | Action |
|------|--------|
| 1 | Flaps ears **3 times** (1 second between flaps) |
| 2 | Moves body **300° CCW** |
| 3 | Pauses **2 seconds** |
| 4 | Moves body **300° CW** back to start |

## Hardware Used

| Component | Pins | Purpose |
|-----------|------|---------|
| Arduino | - | Main controller |
| Ultrasonic sensor | Trig=2, Echo=3 | Detects proximity (10cm trigger) |
| Ear motor | IN3=10, IN4=11, ENB=7 | Flaps ears up/down |
| Body motor | IN1=8, IN2=9, ENA=6 | Rotates body 300° CCW/CW |

## Serial Commands (Optional Override)

While within 10cm and serial monitor open, you can send:

| Command | Action |
|---------|--------|
| `W` or `w` | Run full sequence (ears + body) |
| `D` or `d` | Move body only |
| `A` or `a` | Flap ears only |
| `C` or `c` | Stop body motor |
| `Z` or `z` | Stop ear motor |
| `X` or `x` | Stop everything |

## How the Sequence Runs

1. Ultrasonic detects object < 10 cm
2. Serial command received (or auto-trigger)
3. Ears flap: UP 0.5s
4. Ears flap: DOWN 0.5s
5. Repeat 3x
6. Body rotates CW for ~8.33 seconds
7. Pause 2 seconds
8. Body rotates CW for ~8.33 seconds
9. Done!


## Motor Settings

| Motor | Speed | Duration |
|-------|-------|----------|
| Ear motor | 200 (PWM) | 0.5s per direction |
| Body motor | 200 (PWM) | ~10s for 300° at 3 RPM |

## What Makes This Bunny Special

- **Proximity-triggered** — walk up and it activates
- **Two degrees of motion** — ears flap + body rotates
- **Serial override** — manual control for testing
- **Automatic return** — body returns to start position

## Future Upgrades

- [ ] Add different ear-flap patterns for different distances
- [ ] Add sound effects (boing!)
- [ ] Make it wiggle its tail too
- [ ] Replace serial trigger with fully automatic (no typing needed)

## Author

Nehal — building bunnies before line followers.
