obj = receiver.o

decompress: $(obj)
	gcc $(obj) -o decompress

receiver.o:
	gcc -c receiver.c

getfile: decompress
	./decompress || /bin/true


clean:
	rm decompress
	rm decompfile
	rm *.o
