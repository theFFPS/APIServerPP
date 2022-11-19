CXX            ?= g++
CXX_OPTS       ?= -Wall -Werror -Wextra -g -fPIC -rdynamic -lm -ldl -I./inc -levent -Wno-cast-function-type -Wno-format-security
LD_OPTS        ?= -shared
SRC             = $(wildcard src/*.cc)
OBJ             = $(patsubst src/%.cc, bin/%.o, $(SRC))
OUT             = target/libAPIServer++.so
PREFIX         ?= /usr

all: build
bin/%.o: src/%.cc
	@echo "Building [ $@ ]"
	@$(CXX) $(CXX_OPTS) -c -o $@ $^
build: $(OBJ)
	@echo "Linking [ $(OBJ) ] into [ $(OUT) ]"
	@$(CXX) $(CXX_OPTS) -o $(OUT) $(OBJ) $(LD_OPTS)
install: build
	cp $(OUT) $(PREFIX)/lib
	rm -rf $(PREFIX)/include/APIServer++
	cp inc -r $(PREFIX)/include/APIServer++
clean:
	rm -rf $(OBJ) $(OUT)
