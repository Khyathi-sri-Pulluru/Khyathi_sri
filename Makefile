SRC:= $(wildcard *.c)

OBJ:= $(patsubst *.c, *.o, $(SRC))

LSB.exe: $(OBJ)
	gcc -o $@ $(OBJ)

clean:
	rm *.o *.exe 
