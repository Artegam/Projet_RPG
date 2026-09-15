CXX := g++
PROJECT := Projet_RPG
TESTS = tests/
INC = includes/
SRC = src/
OUT = o/
BIN = bin/
INSTALL_DIR = /usr/bin/
TEST_DIR = $(TESTS)$(SRC)

PROJECT_DIRS = $(SRC) $(INC) $(OUT) $(BIN) $(TEST_DIR)

INCLUDES = -I $(INC) # -I $(INC_RENDER) -I $(TESTS)

LIBS = -lGL -lglut -lGLU
OPT = -Wall -g
OPT_THREAD = -std=c++0x


SRC_FILES = $(shell if [ -d src/ ]; then find src/ -type f -name '*.cpp'; fi)
OBJ_FILES = $(patsubst src/%.cpp, o/%.o, $(SRC_FILES))

## Pour declarer des targets qui ne sont pas des fichiers
.PHONY: project clean install uninstall install-tools uninstall-tools
.SILENT: install uninstall

all: $(PROJECT)

project:
	mkdir --parents $(PROJECT_DIRS)

$(PROJECT): $(OBJ_FILES)
	g++ $(OPT) $(INCLUDES) -L$(LIB) $(OPT_THREAD) $^ -o $(BIN)$@ $(LIBS)

o/%.o: src/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(OPT) -c $(INCLUDES) $? -o $@

clean:
	rm -r o/;find . -name "*~" | xargs rm -f

install:
	sudo cp $(BIN)$(PROJECT) $(INSTALL_DIR)
	if [ ! -d ~/.$(PROJECT)/ ]; then mkdir ~/.$(PROJECT)/; fi
	chmod 755 ~/.$(PROJECT)/
	if [ ! -d /var/log/$(PROJECT)/ ]; then sudo mkdir /var/log/$(PROJECT)/; fi
	echo "$(PROJECT) installed successfully !"

install-tools:
	sudo apt-get install g++ vim dia dia2code doxygen

install-libs:
	sudo apt-get install freeglut3-dev

uninstall:
	sudo rm -f $(INSTALL_DIR)$(PROJECT)

uninstall-tools:
	sudo apt-get remove g++ vim dia dia2code doxygen
	rm -r o/

uninstall-libs:
	sudo apt-get remove freeglut3-dev


include $(DEP_FILES)
