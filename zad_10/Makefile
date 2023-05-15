CXX = g++
CXXFLAGS = -I image.h -Wall -Wpedantic -Wextra -Og -g
SRCS = zad_7.c image.c
OUT = example.out

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
