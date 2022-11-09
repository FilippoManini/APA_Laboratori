CC = g++
CFLAGS = -std=c++17 

LVQuickSort = LVQuickSort
LVQuickSort_OBJS = LVQuickSort.o 

EXECS = $(LVQuickSort)

all: $(EXECS)
.PHONY: clean tgz

$(LVQuickSort): $(LVQuickSort_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(LVQuickSort_OBJS): LVQuickSort.cpp
	$(CC) $(CFLAGS) -o $@ -c LVQuickSort.cpp

#Utilities
clean:
	rm -rf $(EXECS) *.o

tgz: clean
	cd ..; tar cvzf LVQuickSort.tgz LVQuickSort 
