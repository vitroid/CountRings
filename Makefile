#include ../Makefile.settings
BINTARGET=countrings2
TARGET=$(BINTARGET)

all: $(BINTARGET)
countrings2: countrings2.o SparseMatrix.o Int64Hash.o CountRings.o SparseMatrix_CountRings.o
	$(CC) $^ -o countrings2 $(LDFLAGS)
