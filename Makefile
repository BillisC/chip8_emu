all: exec

LINKER_LIBS := -lSDL2main -lSDL2
OBJECTFILES := build/chip8.o build/chip8Handler.o build/chip8DBG.o build/Graphics/GraphicsCore.o

exec: linker
	@./chip8.exe

linker: src/chip8.o src/chip8Handler.o src/chip8DBG.o src/Graphics/GraphicsCore.o
	@echo "Linking objects.."
	@g++ -o chip8.exe ${OBJECTFILES} ${LINKER_LIBS}

src/chip8.o: src/chip8.cpp
	@echo "Compiling main.."
	@g++ -c src/chip8.cpp -o build/chip8.o

src/chip8Handler.o: src/chip8Handler.cpp
	@echo "Compiling handler.."
	@g++ -c src/chip8Handler.cpp -o build/chip8Handler.o

src/chip8DBG.o: src/chip8DBG.cpp
	@echo "Compiling debugger.."
	@g++ -c src/chip8DBG.cpp -o build/chip8DBG.o

src/Graphics/GraphicsCore.o: src/Graphics/GraphicsCore.cpp 
	@echo "Compiling gfx core.."
	@g++ -c src/Graphics/GraphicsCore.cpp -o build/Graphics/GraphicsCore.o

clean:
	@rm -r build/*