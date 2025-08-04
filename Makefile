# ==== Toolchain ====
CXX      := clang++
CXXFLAGS := -std=c++17 -g -O2 -MMD -MP $(shell pkg-config --cflags raylib) -Wall -Werror -Wextra
LDFLAGS  := -rdynamic $(shell pkg-config --libs raylib) -L/usr/local/lib -lafcuda

# ==== Project structure ====
SRCDIR   := srcs
INCDIR   := -Iincludes/Snake -Iincludes/Learning -I/usr/local/lib

OBJDIR   := .objs
TARGET   := Learn2Slither

# ==== Source files ====
SRCFILES := $(wildcard $(SRCDIR)/*.cpp)
OBJFILES := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))
DEPFILES := $(OBJFILES:.o=.d)

# ==== Build rules ====
.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJFILES)
	@echo "Linking $@..."
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# ==== Include auto-generated dependency files ====
-include $(DEPFILES)

# ==== Cleaning ====
.PHONY: clean fclean re

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "Removing binary $(TARGET)..."
	@rm -f $(TARGET)

re: fclean all
