TARGET		:= easydns
BUILDDIR 	:= build

SRCCPP		:= $(shell find src -name '*.cpp')
SRCC		:= $(shell find src -name '*.c')
OBJ			:= $(SRCCPP:src/%.cpp=$(BUILDDIR)/%.o) $(SRCC:src/%.c=$(BUILDDIR)/%.o)

CC			:= clang
CXX			:= clang++
CPPFLAGS	:= -Iinclude -MMD -MP
CFLAGS		:= -std=c11 -O2 -Wall -Wextra
CXXFLAGS	:= -std=c++20 -O2 -Wall -Wextra
LDLIBS		:=

REQUIREMENTS 	:= clang

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ $(LDLIBS) -o $@

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

requirements:
	@echo $(REQUIREMENTS)

-include $(OBJ:.o=.d)