PROGRAM = snake-net-server
CFLAGS = -std=c++11
LDFLAGS = 
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system -lpthread 

all: $(PROGRAM)

$(PROGRAM): src/main.o src/network/Network.o src/network/Server.o
	g++ $(LDFLAGS) $(LDLIBS) $^ -o $@

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	find src -name '*.o' -delete
	rm $(PROGRAM)
