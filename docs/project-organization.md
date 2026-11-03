# Organization and dependency recommendations

## Current state

This is a small Arduino project: eight standalone sketches, seven Robot Makey
lesson files, fabrication PDFs, class documents, a parts spreadsheet, and a
SparkFun library archive. Build configuration now lives in each active sketch's
`sketch.yaml`; CI compiles those sketches for an Uno. Keep the structure small enough to browse without a
framework.

The initial cleanup separates firmware, diagnostics, examples, hardware, and
historical references. All moved files retain their original contents and names.
The maintained program is `firmware/follow_me_bot/follow_me_bot.ino`, renamed
from `full_code.ino`. The historical alternate is under `reference/legacy/`.
They differ in behavior: obstacle recovery turns left in
`full_code` and right in `optimized_full_code`; their temperature failure paths
also differ. Do not choose a baseline solely from the word “optimized.”

## Build and dependencies

Use Arduino CLI 1.5.1 with the checked-in `uno` build profiles. They pin Arduino
AVR Boards 1.8.8, Servo 1.3.0 where included, and SparkFun Qwiic IR Thermometer
MLX90614 Arduino Library 1.1.5 where included. `Wire` is supplied by the core.
These versions compile all seven active sketches. The Uno is a compile target
inferred from comments, pending physical hardware confirmation.

Profiles download dependencies into an isolated cache; globally installed
libraries do not override the selected versions. See the official
[sketch project file specification](https://docs.arduino.cc/arduino-cli/sketch-project-file/).
The workflow pins its action commits and CLI version, and builds the same profiles.
See the [README](../README.md#build) for the local command and update process.

The ZIP at `reference/vendor/SparkFun_MLX90614_Arduino_Library-master.zip`
declares `SparkFun MLX90614` version `1.0.0`. Its ZIP comment identifies commit
`101d16cde167884fe1d52800eeace9331855e640`. Retain it as historical evidence,
including its bundled license. It is not an installed dependency or proof that
a registry release with the same version has identical contents.
The [upstream library](https://github.com/sparkfun/SparkFun_MLX90614_Arduino_Library)
owns the implementation and release history.

The archived ZIP is retained for provenance; active builds use the indexed
1.1.5 release, which retains the header and API used by this firmware. Compile
success does not establish hardware compatibility. Renovate checks upstream
tags for profile pins; the workflow uses its built-in action manager plus an
annotated CLI version. Check Arduino index availability when reviewing updates.

## Firmware ownership

Use `firmware/follow_me_bot/follow_me_bot.ino` as the maintained entry point.
The alternate program remains under `reference/legacy/` for historical comparison.
Optimize the maintained program in a separate PR, preserving its behavior unless
a change is explicitly documented. Keep `setup()` and `loop()` readable there.

Start with one sketch. If complexity warrants it, put pin assignments and
calibration in `robot_config.h`, motor operations in `drive.cpp/.h`, and thermal
and distance readings in domain-named files beside the sketch. Extract code only
when it earns that boundary; no generic helper layer or interfaces for testing.
Keep diagnostics independently runnable and label their hardware assumptions.

## Next steps

1. Confirm the board, motor shield, sensor models, wiring, and historical working status. The source comments suggest an Uno but do not establish the target.
2. Record a wiring table and calibration values in `hardware/README.md` from
   verified hardware. Existing sketches disagree on motor assignments and use
   different ultrasonic sensor arrangements.
3. Fix missing return paths in both full programs' `distance_read()` and in
   `optimized_full_code`'s `temp_read()`. The recursive distance retry discards
   its result. Check timeout and failed-sensor handling before hardware trials.
4. Check servo timer compatibility with motor PWM pins 9/10 on the confirmed
   board, then validate each diagnostic and the maintained program.
5. Keep hardware checks separate from compile CI; compilation cannot prove
   motion or sensing works. Review build warnings and sensor failure handling.

Retain the original parts spreadsheet. A normalized BOM can follow once someone
checks it against the actual robot. Preserve attribution on kit material and
review ownership before selecting a project-wide license.
