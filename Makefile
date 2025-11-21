# compiler we are using
CC = gcc

EXECUTABLE_NAME = main

#default target
all: $(EXECUTABLE_NAME)

#link object files to create the exec
$(EXECUTABLE_NAME): main.o BillLib.o
	@$(CC) $^ -o $@

# main -> object file
main.o: main.c
	@$(CC) -c $<

# bill library files -> object
BillLib.o: BillLib.c BillLib.h
	@$(CC) -c $<

# clean up
clean:
	@-rm -f $(EXECUTABLE_NAME) $(EXECUTABLE_NAME).exe *.o
