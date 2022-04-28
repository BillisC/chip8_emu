all: exec

TARGET := chip8
LFLAGS := -lSDL2main -lSDL2
CFLAGS := -O2
OBJECTFILES := build/chip8.o build/handler.o build/debugger.o build/window.o

# Checking
exec: linker
	@./${TARGET} roms/bc_test.ch8

# Linking
linker: build ${OBJECTFILES}
	@echo "Linking objects.."
	@g++ -o ${TARGET} ${OBJECTFILES} ${LFLAGS}

# Compiling
build/chip8.o: src/chip8.cpp
	@echo "Compiling main.."
	@g++ ${CFLAGS} -c $< -o $@

build/handler.o: src/handler.cpp
	@echo "Compiling handler.."
	@g++ ${CFLAGS} -c $< -o $@

build/debugger.o: src/debugger.cpp debug
	@echo "Compiling debugger.."
	@g++ ${CFLAGS} -c $< -o $@

build/window.o: src/window.cpp 
	@echo "Compiling window core.."
	@g++ ${CFLAGS} -c $< -o $@

# Folder generation
build:
	@mkdir $@

debug:
	@mkdir $@

# Cleaning
clean:
	@rm -rf build