OBJ = pmms.o readfile.o matrix_ops.o
EXEC = pmms
LINK = -lrt -lpthread -lm

$(EXEC): $(OBJ)
	gcc $(OBJ) -o $(EXEC)  $(LINK)

pmms.o : pmms.c readfile.h matrix_ops.h
	gcc -c pmms.c

readfile.o : readfile.c readfile.h
	gcc -c readfile.c

matrix_ops.o : matrix_ops.c matrix_ops.h
	gcc -c matrix_ops.c

clean: 
	rm -f $(EXEC) $(OBJ)
