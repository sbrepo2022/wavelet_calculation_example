CPP=g++
COMPILE_FLAGS=-std=c++20 -g -Wall -fopenmp
LINKER_FLAGS=-lm -fopenmp -lpthread
BUILD_DIR=build

APP_SRC=main.cpp integrate.cpp wavelet.cpp graphics.cpp
TESTS_SRC=tests.cpp integrate.cpp wavelet.cpp

APP_OBJ=$(APP_SRC:%.cpp=$(BUILD_DIR)/%.o)
TESTS_OBJ=$(TESTS_SRC:%.cpp=$(BUILD_DIR)/%.o)

APP_DEP=$(APP_SRC:%.cpp=$(BUILD_DIR)/%.d)
TESTS_DEP=$(TESTS_SRC:%.cpp=$(BUILD_DIR)/%.d)


$(shell mkdir -p $(BUILD_DIR))

all: app tests
	build/tests

$(BUILD_DIR)/%.d: %.cpp
	$(CPP) -M $<  | sed 's,\($*\)\.o[ :]*,$(BUILD_DIR)/\1.o $@ : ,g' > $@

$(BUILD_DIR)/%.o: %.cpp
	$(CPP) -c $< -o $@ $(COMPILE_FLAGS)

$(BUILD_DIR)/app: $(APP_OBJ)
	$(CPP) -o $@ $^ $(LINKER_FLAGS)

$(BUILD_DIR)/tests: $(TESTS_OBJ)
	$(CPP) -o $@ $^ $(LINKER_FLAGS)

app: $(BUILD_DIR)/app

tests: $(BUILD_DIR)/tests

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean app tests

-include $(APP_DEP)
-include $(TESTS_DEP)