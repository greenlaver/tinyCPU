CC		= gcc
CFLAGS	= -g -Wall
PROGRAM	= tinyCPU
OBJS	= tinyCPU.o

.SUFFIXES: .c .o

all: $(PROGRAM)

# primary target
$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $^

# suffix rule
.c.o:
	$(CC) $(CFLAGS) -c $^

# dependence of header files
tinyCPU.o: tinyCPU.h

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS) *.h.gch