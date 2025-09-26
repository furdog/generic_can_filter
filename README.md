# WIP
Ongoing heavyly work in progress. Project is not ready at all.

# Generic can filter
This project implements generic CAN2.0 filter software for various vehicles:
	- Renault Kangoo ZE
	- Leaf (ZE 0)
	(more will be added in the future)

## Features (Work in progress)
- ~BMS monitor that collects various battery variables in real time~ (Not yet present)
- ~Suppress various errors (low SOH, low SOC, Isolation errors, etc.)~ (Not yet present)
- Highly precise, integer arithmetic charge counter (overrides BMS counter)
- ~BMS emulator~ (planned in future / hardcore testing) (Not yet present)
- User friendly Web interface
- 100% MISRA compliance (only for platform independent code)

## TODO
- ~Convert various can loggers data to RAW can data~ (CANARY logs are currently supported)
- Decode RAW can data and print all variables (using codecs) (WIP)
- Implement simple journaling system to monitor any events and changes in data (WIP)
- Enhance network dialog, so websocket host could be changed

## Project structure
`libraries` contains various (either git or local libraries).
`platform` platform specific implementation of can filter (arduino, test, etc).
`vehicles` vehicle specific implementation of can filter.
`setup.sh` main setup script that downloads and prepare neccessary git libs.

the `vehicles` contains subdirectories with codecs (CAN2.0 coders-encoders for specific vehicle)

## Future considerations and project vision in general
Safety and determinism are main priority. 100% compliance with MISRA.

There's NO guarantee that the project is safe per se, as the author, furdog, is not competent at creating such software.
The author assumes that code designed with safety critical concerns will be more easily accepted and processed in the future.
It's also convenient that the code written with consideration of high standards will be much more bug prone than ordinary software.

The `vehicles` directory MUST be renamed to something more generic, since this CAN filter will be targeted for more devices in the future.

CAN filter for each vehicle is planned to have the next subsystems:
- `Codec` - low level CAN message coder/encoder which is accessed by all LAYER1 modules
- `Monitor` - monitors vehicle, deduces its version and tracks operational context in general
- `Filter` - uses monitored data to override CAN messages, should consider context
- `Emulators` - Various emulators that reproduce or even implement vehicle modules behaviour, like BMS, ECU, BCB, etc..
- `Proxy` (or `Telemetry`) - simplifies and unifies acces to vehicle data, by making high level generic interface

The can filter itself MUST implement next subsystems:
- `Filesystem` - Restores all the settings saved on startup.
- `Transport` - Abstraction over specific transmission protocol (for example websockets)
- `Udev` - Based on input data deduces target vehicle, it also may be preconfigured, loaded from filesystem, or changed by user
- `User interface server` - UI server which runs directly on target MCU and uses `Proxy` of specific vehicle to exchange data with UI client

The remote (controlling) device must implement such modules:
- `User interface client` - UI client which runs on remote device (Currently only WEB ui is planned)

The user interface should be as generic as possible. UI Data MUST be generated automatically based on proxy output.

## Dependencies
 - POSIX utils, bash
 - git

## Notes
Active work in progress.

There's already two CAN filters that do all of that (though they're closed source).
In general the codebase was a total mess (due to tight timelines and solo support).
This project aims to reingineer all my workings and make them usable by broad public.

I don't guarantee that i finish this project at all.
