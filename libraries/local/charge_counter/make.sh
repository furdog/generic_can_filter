MISRA='../../../libraries/external/MISRA/misra.sh'
BITE='../../../libraries/external/bitE/'

"$MISRA" check charge_counter.h "$BITE"
#"$MISRA" check charge_counter.test.c "$BITE"

gcc -I"$BITE" charge_counter.test.c -Wall -Wextra -g -std=c89 -pedantic
./a

rm a
