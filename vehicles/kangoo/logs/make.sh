CODEC='../codec/'
BITE='../../../libraries/external/bitE/'
CANARY='../../../libraries/local/canary_log_reader/'

gcc -I"$CANARY" -I"$CODEC" -I"$BITE" kangoo_log_decoder.c -Wall -Wextra -g -std=c89 -pedantic
./a

rm a
