CXX = g++
CXXFLAGS = -Wall -Wpedantic -Wextra -Og -g
SRCS = test.cpp
OUT = a.out

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
