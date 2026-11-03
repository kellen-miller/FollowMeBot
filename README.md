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

See [organization and dependency recommendations](docs/project-organization.md)
for the proposed build setup and next steps. Source and asset contents were
preserved during reorganization. Compilation and operation remain unverified.
