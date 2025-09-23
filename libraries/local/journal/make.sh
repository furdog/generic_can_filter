MISRA='../../../libraries/external/MISRA/misra.sh'

"$MISRA" check journal.h "$BITE"
#"$MISRA" check canary_log_reader.test.c "$BITE"

gcc journal.test.c -Wall -Wextra -g -std=c89 -pedantic
./a

rm a
