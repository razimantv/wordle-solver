CXX ?= g++
CXXFLAGS = -W -Wall -Wno-unused-result -Wno-sign-compare -O3 \
					 -I src -std=c++20

all: wordle nerdle crosswordle dordle quordle octordle

wordle: src/gamebase.o src/wordle.o src/wordle_main.o
	@echo "Building" $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

nerdle: src/gamebase.o src/wordle.o src/nerdle.o src/nerdle_main.o
	@echo "Building" $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

crosswordle: src/gamebase.o src/crosswordle.o src/crosswordle_main.o
	@echo "Building" $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

dordle: wordle
	@echo "Creating" $@
	@echo "#/bin/bash" > $@
	@echo "./wordle -b 2 -a 7" > $@
	@chmod +x $@

quordle: wordle
	@echo "Creating" $@
	@echo "#/bin/bash" > $@
	@echo "./wordle -b 4 -a 9" > $@
	@chmod +x $@

octordle: wordle
	@echo "Creating" $@
	@echo "#/bin/bash" > $@
	@echo "./wordle -b 8 -a 13" > $@
	@chmod +x $@

%.o: %.cpp
	@echo "Compiling" $<
	@$(CXX) -o $@ -c $< $(CXXFLAGS)

.phony: clean
clean:
	@echo "Cleaning"
	@rm -f src/*.o wordle nerdle crosswordle dordle quordle octordle
