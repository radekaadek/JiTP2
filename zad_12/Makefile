CXX = gcc
CXXFLAGS = -I graph_lib/ -I header_files/ -Wall -Wpedantic -Wextra -Og -g
SRCS = zad_9.c $(shell find source_files/ -name "*.c")
OUT = example.out
LIBS = -lfltk -lfltk_images -lX11 -lXext

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
