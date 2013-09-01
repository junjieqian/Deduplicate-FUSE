bbfs : bbfs.o log.o fp_table.o chunk_store.o metafile.o sha1.o
	gcc -g -o bbfs bbfs.o log.o chunk_store.o fp_table.o metafile.o sha1.o `pkg-config fuse --libs`

bbfs.o : bbfs.c log.h params.h
	gcc -g -Wall `pkg-config fuse --cflags` -c bbfs.c

log.o : log.c log.h params.h
	gcc -g -Wall `pkg-config fuse --cflags` -c log.c

chunk_store.o: chunk_store.h chunk_store.c
	gcc -g -Wall `pkg-config fuse --cflags` -c chunk_store.c

fp_table.o: fp_table.h fp_table.c
	gcc -g -Wall `pkg-config fuse --cflags` -c fp_table.c

metafile.o: metafile.h metafile.c
	gcc -g -Wall `pkg-config fuse --cflags` -c metafile.c

sha1.o: sha1.h sha1.o
	gcc -g -Wall -c sha1.c
clean:
	rm -f bbfs *.o

dist:
	rm -rf fuse-tutorial/
	mkdir fuse-tutorial/
	cp ../*.html fuse-tutorial/
	mkdir fuse-tutorial/example/
	mkdir fuse-tutorial/example/mountdir/
	mkdir fuse-tutorial/example/rootdir/
	echo "a bogus file" > fuse-tutorial/example/rootdir/bogus.txt
	mkdir fuse-tutorial/src
	cp Makefile COPYING COPYING.LIB *.c *.h fuse-tutorial/src/
	tar cvzf ../../fuse-tutorial.tgz fuse-tutorial/
	rm -rf fuse-tutorial/
