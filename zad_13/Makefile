CXX = gcc
CXXFLAGS = -I header_files/ -Wall -Wpedantic -Wextra -Og -g
SRCS = zad_x.c $(shell find source_files/ -name "*.c")
OUT = example.out

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
