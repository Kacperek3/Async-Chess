CXX = g++
CXXFLAGS = -g -I./Game/include
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = $(shell find Game -name "*.cpp")
OBJ = $(patsubst Game/%.cpp,obj/%.o,$(SRC))
BIN = bin/project_name

all: $(BIN)

# Tworzenie pliku wykonywalnego
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Kompilowanie plików .cpp do .o w katalogu obj/
obj/%.o: Game/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Sprzątanie plików .o i pliku wykonywalnego
clean:
	rm -rf obj/ $(BIN)
