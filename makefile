# Nombre del ejecutable
TARGET = pol

# Carpeta codigo fuente
SRC_DIR = src

# Archivos fuente
BISON_FILE = parser.y
FLEX_FILE = lexer.l

# Archivos generados
BISON_SRC           = $(SRC_DIR)/parser.cpp
BISON_HEADER        = $(SRC_DIR)/parser.hpp
FLEX_SRC            = $(SRC_DIR)/lexer.cpp

# Otros archivos fuente
CPP_SRCS = $(SRC_DIR)/Symbol_base.cpp\
$(SRC_DIR)/Symbol_table.cpp \
$(SRC_DIR)/nodes.cpp\
$(SRC_DIR)/Func_table.cpp

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++20 -I$(SRC_DIR)

# Regla por defecto
all: $(TARGET)

# Generar parser.cpp y parser.hpp
$(BISON_SRC) $(BISON_HEADER): $(BISON_FILE)
	bison -d --defines=$(SRC_DIR)/parser.hpp -o $(BISON_SRC) $(BISON_FILE)

# Generar lexer.cpp
$(FLEX_SRC): $(FLEX_FILE)
	flex -o $(FLEX_SRC) $(FLEX_FILE)
    

# Compilar todo
$(TARGET): $(BISON_SRC) $(FLEX_SRC) $(CPP_SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(BISON_SRC) $(FLEX_SRC) $(CPP_SRCS) -lfl

# Limpiar archivos generados
clean:
	rm -f $(TARGET) $(BISON_SRC) $(BISON_HEADER) $(FLEX_SRC)
