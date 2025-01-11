all:
	cc -I /usr/include/postgresql/server -fPIC -c *.c
	cc -shared -o pg_hw.so *.o
	rm *.o
	sudo rm /lib/pg_hw.so
	chmod 755 pg_hw.so
	sudo mv pg_hw.so /lib

local:
