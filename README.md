# WIP
Ongoing heavyly work in progress. Project is not ready at all.

# Generic can filter
This project implements generic CAN2.0 filter software for various vehicles:
	- Renault Kangoo ZE
	- Leaf (ZE 0)
	(more will be added in the future)

## Features (Work in progress)
- BMS monitor that collects various battery variables in real time
- Suppress various errors (low SOH, low SOC, Isolation errors, etc.)
- Highly precise, integer arithmetic charge counter (overrides BMS counter)
- BMS emulator (planned in future / hardcore testing)
- User friendly Web interface
- 100% MISRA compliance (only for platform independent code)

## TODO
- Convert various can loggers data to RAW can data
- Decode RAW can data and print all variables (using codecs)

## Project structure
`libraries` contains various (either git or local libraries)
`platform` platform specific implementation of can filter (arduino, test, etc)
`vehicles` vehicle specific implementation of can filter
`setup.sh` main setup script that downloads and prepare neccessary git libs

## Dependencies
 - POSIX utils, bash
 - git

## Notes
Active work in progress
