# CRITICAL NOTE
The project is NOT under active development now. I may get back to it sooner or later, but for now i fucused on other stuff.
So nothing to see here at the moment. The project is still mainly in concept stage. I'll soon open source other my project related with CAN FILTER.

# Generic CAN Filter
This project implements generic CAN2.0 filter software for various vehicles, with a focus on determinism, safety, and re-usability. It is currently a work in progress and not ready for production use.


---

## Features
* **Current:**
    * Highly precise, integer arithmetic charge counter (will override BMS counter).
* **Planned:**
    * BMS monitor that collects various battery variables in real-time.
    * Suppress various errors (low SOH, low SOC, Isolation errors, etc.).
    * BMS emulator.
    * User-friendly Web interface.
    * 100% MISRA compliance for platform-independent code.

---

## Supported Vehicles
* Renault Kangoo ZE
* Nissan Leaf (ZE 0)
* *More vehicles will be added in the future.*

---

## Project Structure
* `libraries`: Contains various (either git or local) libraries.
* `platform`: Platform-specific implementation of the CAN filter (e.g., arduino, test).
* `vehicles`: Vehicle-specific implementations, including CAN2.0 codecs.
* `setup.sh`: The main setup script to download and prepare necessary git libraries.

---

## Project Vision & Architecture
Safety and determinism are the main priorities. The project aims for 100% compliance with MISRA and is designed with safety-critical concerns in mind to ensure robustness and reliability.
The only bottlenecks are:
- Target platforms can't be properly tested for critical safety. (Arduino is a BIG NO, though is suitable for initial prototyping)
- No strict immutablity, nor version control policy.
- There's no restriction on hardware, yet.

For now only business logic and underliyng libraries are fully compliant and designed that way.

The `vehicles` directory will be renamed to a more generic name in the future, as this CAN filter will be targeted for more devices.

Each vehicle's CAN filter is planned to have the following subsystems:
* `Codec`: Low-level CAN message coder/encoder.
* `Monitor`: Monitors the vehicle, deduces its version, and tracks the general operational context.
* `Filter`: Uses monitored data to override CAN messages, considering the vehicle's context.
* `Emulators`: Various emulators that reproduce or implement vehicle module behavior (e.g., BMS, ECU, BCB).
* `Proxy`: Simplifies and unifies access to vehicle data by providing a high-level generic interface.

The main CAN filter itself will implement these core subsystems:
* `Filesystem`: Restores all saved settings on startup.
* `Transport`: An abstraction layer over specific transmission protocols (e.g., WebSockets).
* `Udev`: Deduces the target vehicle based on input data. It can also be pre-configured, loaded from the filesystem, or changed by the user.
* `User interface server`: A UI server that runs on the target MCU and uses the vehicle's `Proxy` to exchange data with a UI client.

---

## Dependencies
* `POSIX` utilities
* `bash`
* `git`

---

## Notes
This project aims to re-engineer previous workings to make them more usable and accessible to the public. There are two existing closed-source projects that perform similar functions. This project offers an open-source alternative.

* **Disclaimer:** This project is for educational and experimental purposes only. It has not been certified for safety-critical applications. Use at your own risk. The author is not liable for any damages or issues arising from its use.
