CC		= gcc
CFLAGS	= -g -Wall -std=c99
PROGRAM	= tinyCPU
OBJS	= tinyCPU.o tinyCPUcore.o tinyDebugger.o

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
tinyCPUcore.o: tinyCPUcore.h
tinyDebugger.o: tinyDebugger.h

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS) *.h.gch
