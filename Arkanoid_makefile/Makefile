# ==== CONFIGURAÇÕES DO PROJETO ====
SRC_DIR    = src
INC_DIR    = includes
RAYLIB_DIR = raylib
BUILD_DIR  = build
OBJ_DIR    = $(BUILD_DIR)/obj

TARGET    = $(BUILD_DIR)/arkanoid.exe

CXX       = g++
CXXFLAGS  = -std=c++17 -O2 -Wall -I$(INC_DIR) -I$(RAYLIB_DIR)/include
LDFLAGS   = -L$(RAYLIB_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm

# ==== FONTES E OBJETOS ====
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS     = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))

# ==== REGRAS ====
all: dirs $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BUILD_DIR)

run: all
	@echo "Executando Arkanoid..."
	$(TARGET)

clean:
	@echo "Limpando build..."
	rm -rf $(BUILD_DIR)

.PHONY: all run clean dirs