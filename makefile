# Nombre del ejecutable
TARGET = pol

# Archivos fuente
BISON_FILE = parser.y
FLEX_FILE = lexer.l

# Archivos generados
BISON_SRC = parser.cpp
BISON_HEADER = parser.hpp
FLEX_SRC = lexer.cpp

# Otros archivos fuente
CPP_SRCS = Symbol_base.cpp Symbol_table.cpp

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17

# Regla por defecto
all: $(TARGET)

# Generar parser.cpp y parser.hpp
$(BISON_SRC) $(BISON_HEADER): $(BISON_FILE)
	bison -d -o $(BISON_SRC) $(BISON_FILE)

# Generar lexer.cpp
$(FLEX_SRC): $(FLEX_FILE)
	flex -o $(FLEX_SRC) $(FLEX_FILE)

# Compilar todo
$(TARGET): $(BISON_SRC) $(FLEX_SRC) $(CPP_SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(BISON_SRC) $(FLEX_SRC) $(CPP_SRCS)

# Limpiar archivos generados
clean:
	rm -f $(TARGET) $(BISON_SRC) $(BISON_HEADER) $(FLEX_SRC)
