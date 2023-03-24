CXX = g++
CXXFLAGS = -I graph_lib/ -Wall -Wpedantic -Wextra -O4
SRCS = main.cpp $(shell find graph_lib/ -name "*.cpp")
OUT = example.out
LIBS = -lfltk -lfltk_images -lX11 -lXext

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)

