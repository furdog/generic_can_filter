#!/bin/bash

# Setup verbose output and fail on errors
export PS4="\e[34m>> \e[37m"; set -x; set -e

# Cleanup
function cleanup_and_exit() {
	local exit_code=$?

	set +e;
	rm a.* 2>/dev/null

	if [ "$exit_code" -eq 0 ]; then
		echo -e "\e[32m\nSUCCESS!\e[0m"
	else
		echo -e "\e[31m\nERROR!\e[0m"
	fi
}

# Trap exit
trap cleanup_and_exit EXIT

###############################################################################
# CONFIGURATION:
###############################################################################
COMPILER="$PWD"/tools/arduino-cli

SERIAL_PORT=COM10
MONITOR_BAUD=115200
export OTA_IP="10.10.10.10"

export TARGET_NAME="CanBOXuni"
export TARGET=generic_can_filter_esp32c6_hw1

#EXTRA_FLAGS="-v"

###############################################################################
# TARGETS AVAILABLE
###############################################################################

if [ "$TARGET" == "generic_can_filter_esp32c6_hw1" ]; then
	VARIANT_PATH="variant/esp32c6/"
	BOARD=esp32:esp32:esp32c6
	FQBN=:CDCOnBoot=cdc
else
	echo "Bad target!"
	exit 1
fi

###############################################################################
# MAIN
###############################################################################
# Set environment variables
if [ -z "${GIT_REPO_VERSION+x}" ]; then
	export GIT_REPO_VERSION=$(git describe --tags)
fi

mk_base64_updater()
{
	local BOARD_PATH="${BOARD//:/\.}"  # Replace ':' with '.'
	local FILE="build/${BOARD_PATH}/$(basename "$PWD").ino.bin"

	# Generate BASE64 firmware
	export BASE64_ENCODED_FIRMWARE=$(base64 -w 0 "$FILE")

	# Generate updater html file (with embedded base64 firmware)
	../awk/ENV.awk ../web/updater.html > \
		     build/"$BOARD_PATH"/"$TARGET"_"$GIT_REPO_VERSION".html
}

compile() {
	# enumerate (and empty) all generated files
	mkdir -p build

	rm build/* 2> /dev/null || true # Clean build
	

	echo "Initial setup..."

	# Setup tools and libraries
	./setup.sh
	if [[ $? -ne 0 ]]; then
	    echo "FATAL ERROR: Setup failed."
	    exit 1
	fi


	echo "Copying..."

	# Copy all necessary files into build/
	cp "$VARIANT_PATH"*.ino build/build.ino
	cp "$VARIANT_PATH"*.h   build/
	#cp *.h build/

	echo "Compiling..."

	# Goto build directory with all generated files
	cd build

	echo "PROPS: " ${PROPS}
	echo "FQBN: " ${FQBN}
	while ! ${COMPILER} compile -b ${BOARD}${FQBN} --warnings "all" \
			   ${PROPS} -e --libraries "../libraries/" \
			   ${EXTRA_FLAGS}; do
		read -p "Press any key to continue "
		exit
	done

	#mk_base64_updater
}

# Function to monitor
monitor() {
	if [ -n "${SERIAL_PORT+x}" ]; then
		while true; do
			${COMPILER} monitor -p ${SERIAL_PORT} \
			      --config baudrate=${MONITOR_BAUD} ${EXTRA_FLAGS};
			sleep 1
		done
	fi
}

upload() {
	if [ -n "${SERIAL_PORT+x}" ]; then
		while ! ${COMPILER} upload -b ${BOARD}${FQBN} \
				   -p ${SERIAL_PORT} ${EXTRA_FLAGS}; do
			sleep 1
		done
	fi
}

web_upload() {
	local BOARD_PATH="${BOARD//:/\.}"  # Replace ':' with '.'
	local FILE="build/${BOARD_PATH}/$(basename "$PWD").ino.bin"

	if [ ! -f "$FILE" ]; then
		echo "Firmware file not found: $FILE"
		return 1
	fi

	while true; do
		echo "Uploading $FILE to http://${OTA_IP}/update..."
		curl -F "file=@$FILE" http://${OTA_IP}/update && break
		echo "Upload failed, retrying in 1 second..."
		sleep 1
	done

	echo "Upload complete."
}

# Check if the argument is "monitor"
if [ "$1" == "monitor" ]; then
	monitor
elif [ "$1" == "upload" ]; then
	cd build
	upload
elif [ "$1" == "flash" ]; then
	cd build
	upload
elif [[ "$1" == "web" && ( "$2" == "upload" || "$2" == "flash" ) ]]; then
	cd build
	web_upload
elif [ "$1" == "web" ]; then
	compile
	web_upload
else
	compile
	upload
	monitor
fi

read -p "Press any key to continue "
