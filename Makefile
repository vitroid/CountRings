#include ../Makefile.settings
BINTARGET=countrings2
TARGET=$(BINTARGET)
OBJS=countrings2.o SparseMatrix.o Int64Hash.o CountRings.o SparseMatrix_CountRings.o
CFLAGS=-Werror

all: $(BINTARGET)
countrings2: $(OBJS)
	$(CC) $^ -o countrings2 $(LDFLAGS)
clean:
	-rm $(OBJS)
archive: clean
	cd ..; tar zcvf CountRings.tar.gz CountRings/*
