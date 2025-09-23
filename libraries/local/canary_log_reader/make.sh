MISRA='../../../libraries/external/MISRA/misra.sh'
BITE='../../../libraries/external/bitE/'

"$MISRA" check canary_log_reader.h "$BITE"
#"$MISRA" check canary_log_reader.test.c "$BITE"

gcc -I"$BITE" canary_log_reader.test.c -Wall -Wextra -g -std=c89 -pedantic
./a

rm a
