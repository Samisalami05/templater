CC := gcc
NAME := main
SOURCEDIR := src
BUILDDIR := build
DEPSDIR := deps

CPPFLAGS := -Ideps/glad/include
CFLAGS := -Wall -MMD -MP
LDFLAGS := 
LDLIBS := -lm -lglfw -lGL

SRCS := $(shell find $(SOURCEDIR) -name '*.c')

# Glad
SRCS += $(DEPSDIR)/glad/glad.c

OBJS := $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))


$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $(CFLAGS) $< -o $@


.PHONY: clean run help

help: # Show this help
	@echo "Usage: make [TARGET]..."
	@echo ""
	@echo "Available targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*#' Makefile | sort | \
		awk 'BEGIN { FS = ":.*# " } { printf " - \x1b[1m%-10s\x1b[0m %s\n", $$1, $$2 }'

run: $(NAME) # Run the application
	./$(NAME)

clean: # Remove all build files
	rm -rf $(BUILDDIR)
	rm -f $(NAME)

-include $(OBJS:.o=.d)
