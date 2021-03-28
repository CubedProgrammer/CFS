# CFS
Count file sizes

Compile into executable with the following commands.

gcc -O3 -c get_sub_dir.c

gcc -O3 -c cfs.c

gcc -o cfs.out cfs.o get_sub_dir.o

Compile into shared library with the following commands.

gcc -O3 -c get_sub_dir.c -fPIC

gcc -O3 -c cfs.c -fPIC

gcc -shared -o libcfs.so cfs.o get_sub_dir.o

Use .exe and .dll extension on windows.