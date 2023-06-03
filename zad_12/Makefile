CXX = gcc
CXXFLAGS = -I header_files/ -Wall -Wpedantic -Wextra -Og -g -Wshadow -Wundef -Wdouble-promotion -Wformat=2 -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-but-set-parameter -Wno-unused-result -Wno-unused-label -Wno-unused-value -Wno-unused-local-typedefs -Wno-unused-const-variable -Wno-unused-local-typedefs
SRCS = zad_9.c $(shell find source_files/ -name "*.c")
OUT = example.out

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
