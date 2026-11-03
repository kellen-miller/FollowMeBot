# Organization and dependency recommendations

## Current state

This is a small Arduino project: eight standalone sketches, seven Robot Makey
lesson files, fabrication PDFs, class documents, a parts spreadsheet, and a
SparkFun library archive. There is no recorded board configuration or build
command. Keep the structure small enough to browse without a framework.

The initial cleanup separates firmware, diagnostics, examples, hardware, and
historical references. All moved files retain their original contents and names.
The maintained program is `firmware/follow_me_bot/follow_me_bot.ino`, renamed
from `full_code.ino`. The historical alternate is under `reference/legacy/`.
They differ in behavior: obstacle recovery turns left in
`full_code` and right in `optimized_full_code`; their temperature failure paths
also differ. Do not choose a baseline solely from the word “optimized.”

## Build and dependencies

Use Arduino IDE for editing/uploading and Arduino CLI for repeatable builds.
Record exact dependencies in each maintained sketch's `sketch.yaml` build
profile. Profiles can specify the board FQBN, platform version, and library
versions and build with isolated dependencies. See the official
[sketch project file specification](https://docs.arduino.cc/arduino-cli/sketch-project-file/).

| Dependency | Evidence | Proposed tracking |
| --- | --- | --- |
| Arduino board core | Arduino APIs; Uno mentioned in motor comments | Confirm board, then pin platform and FQBN in build profiles |
| `Wire` | Included by both full programs and `temp_test` | Supplied by the selected board core |
| `Servo` | Included by both full programs and `head_test` | Pin a verified compatible library version in those profiles |
| SparkFun MLX90614 | `SparkFunMLX90614.h`, `IRTherm` | Pin a verified compatible version in full-program and temperature profiles |
| Arduino CLI | Not currently configured or installed on this workstation | Pin the validated CLI version in CI and document local installation |

The ZIP at `reference/vendor/SparkFun_MLX90614_Arduino_Library-master.zip`
declares `SparkFun MLX90614` version `1.0.0`. Its ZIP comment identifies commit
`101d16cde167884fe1d52800eeace9331855e640`. Retain it as historical evidence,
including its bundled license. It is not an installed dependency or proof that
a registry release with the same version has identical contents.
The [upstream library](https://github.com/sparkfun/SparkFun_MLX90614_Arduino_Library)
owns the implementation and release history.

After confirming hardware, compile against the archived library as a baseline,
then evaluate a released library version. Commit working profiles and document:

```sh
arduino-cli compile --profile <verified-profile> <sketch-directory>
```

This is a command template, not an executable project configuration yet.
Avoid introducing PlatformIO alongside Arduino CLI at this stage; one dependency
mechanism is enough for this project. Keep the existing Renovate configuration,
but verify support for the chosen manifest before relying on automated updates.

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
5. Pin the validated build profiles and add compile-only CI for maintained
   sketches. Keep hardware checks separate; compilation cannot prove motion or
   sensing works. Add behavior tests when control logic is changed.

Retain the original parts spreadsheet. A normalized BOM can follow once someone
checks it against the actual robot. Preserve attribution on kit material and
review ownership before selecting a project-wide license.
