# compilation flags
CCX_FLAGS=-std=c++17 -O3 -fstrict-aliasing -march=native -funroll-loops
CCX=g++

# directory containing executables
OUT_DIR = exec

# makedir command
MKDIR_P = mkdir -p

# main executables 
EXECS = $(OUT_DIR)/gen-dfa.x $(OUT_DIR)/gen-dfa-file.x $(OUT_DIR)/gen-dfa-bin.x $(OUT_DIR)/gen-dfa-bin-64.x

# targets not producing a file declared phony
.PHONY: build clean

build: ${OUT_DIR} $(EXECS)

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

$(OUT_DIR)/gen-dfa.x: internal/cons-spa-WDFA-gen.cpp
	$(CCX) $(CCX_FLAGS) -o $@ internal/cons-spa-WDFA-gen.cpp

$(OUT_DIR)/gen-dfa-file.x: internal/cons-spa-WDFA-gen.cpp
	$(CCX) $(CCX_FLAGS) -o $@ internal/cons-spa-WDFA-gen.cpp -DOFILE

$(OUT_DIR)/gen-dfa-bin.x: internal/cons-spa-WDFA-gen-bin.cpp
	$(CCX) $(CCX_FLAGS) -o $@ internal/cons-spa-WDFA-gen-bin.cpp

$(OUT_DIR)/gen-dfa-bin-64.x: internal/cons-spa-WDFA-gen-bin.cpp
	$(CCX) $(CCX_FLAGS) -o $@ internal/cons-spa-WDFA-gen-bin.cpp -DM64

clean:
	rm -f $(EXECS) #lib/*.o  
	rmdir $(OUT_DIR)
