obj= func.o main.o

compress: $(obj)
	gcc $(obj) -o compress
	./compress
func.o:
	gcc -c func.c
main.o:
	gcc -c main.c
send: compress
	mv mafile rectest/
	mv compfile rectest/
clean:
	rm *.o
	rm compress
	rm rectest/mafile
	rm rectest/compfile
