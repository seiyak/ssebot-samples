CC=gcc
CFLAGS=-g -Wall
LIBS=-lpthread
EXEC=sample-array2

$(EXEC): sample-array2.c
	$(CC) $(CFLAGS) -MMD -MF $(EXEC).d -o $@ $< $(LIBS)
-include $(EXEC).d

#sample-array2.o: sample-array2.c
#	$(CC) $(CFLAGS) -MMD -MF sample-array2.d -c -o sample-array2.o sample-array2.c
#-include sample2-array2.d

.PHONY: clean

clean:
	rm -f $(EXEC).d $(EXEC)
