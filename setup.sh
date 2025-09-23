set -x -e

# cd to this script dir
cd "$(dirname "$0")" || exit 1

# Download necessary libraries
mkdir -p libraries
mkdir -p libraries/external
pushd    libraries/external

# Clone MISRA and setup
REPO='https://github.com/furdog/MISRA.git'
if cd `basename $REPO .git`; then git pull; cd ..; else git clone "$REPO"; fi
./MISRA/misra.sh setup

# Clone lwDASH
REPO='https://github.com/furdog/lwDASH.git'
if cd `basename $REPO .git`; then git pull; cd ..; else git clone "$REPO"; fi

# Clone bitE
REPO='https://github.com/furdog/bitE.git'
if cd `basename $REPO .git`; then git pull; cd ..; else git clone "$REPO"; fi

popd
