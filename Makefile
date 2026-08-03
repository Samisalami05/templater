CC := gcc
NAME := templater
SOURCEDIR := src
BUILDDIR := build

SRCS := $(shell find $(SOURCEDIR) -name '*.c')
OBJS := $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

CFLAGS := -Wall

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MMD -c $(CFLAGS) $< -o $@

.PHONY: clean install uninstall

install: $(NAME)
	sudo cp templater /usr/local/bin/
	mkdir -p ~/.local/share/templater
	cp -r presets/ ~/.local/share/templater
	
	@echo ""
	@echo "Templater successfully installed at '/usr/local/bin'"

uninstall:
	sudo rm -f /usr/local/bin/templater
	rm -rf ~/.local/share/templater

	@echo ""
	@echo "Templater successfully uninstalled from '/usr/local/bin'"

clean:
	rm -rf $(BUILDDIR)/*.d
	rm -rf $(BUILDDIR)/*.o
	rm $(NAME)

-include $(OBJS:.o=.d)
