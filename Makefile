foolfs : foolfs.o log.o
	gcc -g -o foolfs foolfs.o log.o `pkg-config fuse --libs`

foolfs.o : foolfs.c log.h params.h
	gcc -g -Wall `pkg-config fuse --cflags` -c foolfs.c

log.o : log.c log.h params.h
	gcc -g -Wall `pkg-config fuse --cflags` -c log.c

clean:
	rm -f foolfs foolfs.o log.o
cleanlogs:
	rm -r foolfs.log

cleanall: clean cleanlogs
	echo "cleaned all"
	
