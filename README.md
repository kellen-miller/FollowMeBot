# FollowMeBot

An Arduino robot built to sense heat and follow people. This was a senior-year
elective project and my first experience with robotics and Arduino.

[Robot photos](https://photos.app.goo.gl/MfkJ4ecmNqUwXZY57)

## Project layout

| Directory | Contents |
| --- | --- |
| `firmware/` | Main `follow_me_bot` sketch (formerly `full_code.ino`) |
| `diagnostics/` | Standalone distance, temperature, motor, and head experiments |
| `examples/` | Original object-avoidance and object-following sketches |
| `hardware/` | Original parts spreadsheet and fabrication templates |
| `reference/` | Lessons, historical alternate firmware, and SparkFun library ZIP |
| `docs/original/` | Original class proposal and example Illustrator files |

Each `.ino` now lives in a directory with the same name. Open that file in the
Arduino IDE as a separate sketch. Arduino requires a matching primary sketch
filename and [combines sketch files in the same directory](https://docs.arduino.cc/arduino-cli/sketch-build-process/).

The maintained entry point is
[`follow_me_bot.ino`](firmware/follow_me_bot/follow_me_bot.ino), renamed from
`full_code.ino` without code changes. The historical `optimized_full_code`
variant lives under `reference/legacy/`; it is not the baseline for future work.
The diagnostics are historical experiments, not automated tests; `head_test`
also drives the motors.

## Build

Install Arduino CLI **1.5.1**, then run:

```sh
arduino-cli compile --profile uno --warnings all firmware/follow_me_bot
```

Each active sketch has a `sketch.yaml` profile with exact core/library versions.
The CLI downloads these automatically into its isolated dependency cache;
there is no manual library installation or ZIP extraction step.
Profiles use Arduino AVR Boards **1.8.8**, Servo **1.3.0** where needed, and
SparkFun Qwiic IR Thermometer MLX90614 Arduino Library **1.1.5** where needed.
`Wire` comes from the board core.

The `uno` target follows source comments; the physical board and wiring still
need confirmation. Compilation does not verify operation or fix existing
sensor return-path and retry defects.

## CI and updates

GitHub Actions compiles firmware, diagnostics, and examples on PRs, pushes to
`main`, and manual dispatch. It uses the same pinned profiles and CLI version.
Historical sketches under `reference/` are excluded. Compiler warnings remain
visible, including existing firmware and upstream core warnings.

Renovate tracks the core and library pins through annotated profile entries,
the CLI version through the workflow annotation, and GitHub Actions through
its built-in manager. Core/library updates use upstream three-part version tags;
confirm the proposed version is also available in Arduino's package/library
index. CI must compile successfully before accepting an update.

For another sketch, use its directory in the build command. For a new sketch,
add a `sketch.yaml` with an `uno` profile and only the libraries it includes;
CI discovers `.ino` files under the three active directories automatically.
Arduino IDE remains useful for interactive editing, but use the CLI command
for builds with the pinned profiles.

See [organization notes](docs/project-organization.md) for remaining hardware
and firmware work. No automated hardware tests or uploads run in CI.
