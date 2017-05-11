cat table.s > tmp.s && gcc -c tmp.s && objdump -d tmp.o > tmp.d && cat tmp.d && rm tmp.s tmp.d tmp.o
