MISRA='../../../libraries/external/MISRA/misra.sh'
BITE='../../../libraries/external/bitE/'

"$MISRA" check leaf_can_codec.h "$BITE"
#"$MISRA" check leaf_can_codec.test.c "$BITE"

gcc -I"$BITE" leaf_can_codec.test.c -Wall -Wextra -g -std=c89 -pedantic
./a

rm a
