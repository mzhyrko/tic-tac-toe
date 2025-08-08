# Makefile для Tic Tac Toe Ultimate

# Компилятор и флаги
CXX := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -Ilib/spdlog/include -Isrc -I/usr/local/include
LDFLAGS := -pthread
DEBUG_FLAGS := -g -DDEBUG
RELEASE_FLAGS := -O3 -DNDEBUG

# Директории
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/tictactoe

# Исходные файлы
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Основная цель
all: $(TARGET)

# Цель для отладочной сборки
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Цель для релизной сборки
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Правило для сборки объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка основной программы
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

# Создание директорий
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Запуск программы
run: $(TARGET)
	@echo "Запуск сервера..."
	@./$(TARGET)

# Запуск сервера
run-server: $(TARGET)
	@echo "Запуск сервера..."
	@./$(TARGET)

# Очистка
clean:
	@echo "Очистка проекта..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Принудительная пересборка
rebuild: clean all

.PHONY: all debug release run run-server clean rebuild