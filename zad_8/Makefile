CXX = g++
CXXFLAGS = -I graph_lib/ -I header_files/ -Wall -Wpedantic -Wextra -Og
SRCS = zad_5.cpp $(shell find graph_lib/ -name "*.cpp") $(shell find source_files/ -name "*.cpp")
OUT = example.out
LIBS = -lfltk -lfltk_images -lX11 -lXext

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)