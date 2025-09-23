LWDASH='../libraries/external/lwDASH'

"$LWDASH/make.sh" *.js

rm -rf build
cp -r "$LWDASH/build" ./
