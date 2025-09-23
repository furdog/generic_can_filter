MISRA='../../../libraries/external/MISRA/misra.sh'
BITE='../../../libraries/external/bitE/'

"$MISRA" check kangoo_can_codec.h "$BITE"
#"$MISRA" check kangoo_can_codec.test.c "$BITE"

gcc -I"$BITE" kangoo_can_codec.test.c -Wall -Wextra -g -std=c89 -pedantic
./a

rm a
