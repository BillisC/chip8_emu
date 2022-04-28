all: exec

TARGET := chip8
LFLAGS := -lSDL2main -lSDL2
CFLAGS := -O2
OBJECTFILES := build/chip8.o build/handler.o build/debugger.o build/window.o

exec: linker
	@./${TARGET} roms/bc_test.ch8

linker: ${OBJECTFILES}
	@echo "Linking objects.."
	@g++ -o ${TARGET} ${OBJECTFILES} ${LFLAGS}

build/chip8.o: src/chip8.cpp
	@echo "Compiling main.."
	@g++ ${CFLAGS} -c src/chip8.cpp -o build/chip8.o

build/handler.o: src/handler.cpp
	@echo "Compiling handler.."
	@g++ ${CFLAGS} -c src/handler.cpp -o build/handler.o

build/debugger.o: src/debugger.cpp
	@echo "Compiling debugger.."
	@g++ ${CFLAGS} -c src/debugger.cpp -o build/debugger.o

build/window.o: src/window.cpp 
	@echo "Compiling window core.."
	@g++ ${CFLAGS} -c src/window.cpp -o build/window.o

clean:
	@rm -r build/*