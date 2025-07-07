#!/bin/bash



set -x
gcc 256.c -o 256
gcc 128.c -o 128

./256 addresses.txt >256.txt
./128 addresses.txt >128.txt

diff 256.txt correct.txt

cut -d ' ' -f 8 correct.txt > cutcorrect.txt
cut -d ' ' -f 8 128.txt > cut128.txt

diff cut128.txt cutcorrect.txt
