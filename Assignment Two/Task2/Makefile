# Compiler
MPICC = mpicc

# Source and binary names
SRC = Matrix.c
BIN = Matrix

# Default number of processes
NP = 4

# Default target
all: $(BIN)

# Compile the MPI program
$(BIN): $(SRC)
	$(MPICC) -o $(BIN) $(SRC)

# Run the program with mpirun
run: $(BIN)
	mpirun -np $(NP) ./$(BIN)

# Clean up generated files
clean:
	rm -f $(BIN)

.PHONY: all run clean
