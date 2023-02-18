SRC_DIR := ./src
SRC := $(wildcard $(SRC_DIR)/*.cpp)

INC_DIR := ./include
DEPS := $(wildcard $(INC_DIR)/*.hpp)

OBJ_DIR := ./obj
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXXFLAGS := -Wall -std=c++1z -I$(INC_DIR)

SDL_FLAGS := `pkg-config --cflags sdl2`
SDL_LIBS := `pkg-config --libs sdl2`

BIN := nn_viz

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(SDL_FLAGS) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CXX) $(SDL_FLAGS) $(CXXFLAGS) -o $@ $^ $(SDL_LIBS)

.PHONY: clean
clean:
	$(RM) $(OBJ) $(BIN)
