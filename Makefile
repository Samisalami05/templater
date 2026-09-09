CC := gcc
NAME := templater
SOURCEDIR := src
BUILDDIR := build

ifeq ($(OS),Windows_NT)
	INSTALL_DIR := C:\msys64\ucrt64\bin
	PRESETS_DIR := ${LOCALAPPDATA}\templater
else
	INSTALL_DIR := /usr/local/bin
	PRESETS_DIR := ~/.local/share/templater
endif

SRCS := $(wildcard $(SOURCEDIR)/*.c)
OBJS := $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

CFLAGS := -Wall

ifeq ($(OS),Windows_NT)
	MKDIR = powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(1)' > $$null"
else
	MKDIR = mkdir -p "$(1)"
endif

$(NAME): builddir $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) -MMD -c $(CFLAGS) $< -o $@

builddir:
	@$(call MKDIR,$(BUILDDIR))

.PHONY: clean install uninstall builddir

clean:
	rm -rf $(BUILDDIR)/*.d
	rm -rf $(BUILDDIR)/*.o
	rm $(NAME)

install: $(NAME)
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "if (!(Test-Path $(INSTALL_DIR)\$(NAME).exe)) { Copy-Item .\$(NAME).exe $(INSTALL_DIR) }"
	@$(call MKDIR,${LOCALAPPDATA}\templater)
	powershell -NoProfile -Command "if (!(Test-Path ${LOCALAPPDATA}\templater\presets)) { Copy-Item -Recurse presets ${LOCALAPPDATA}\templater }"
else
	mkdir -p $(PRESETS_DIR)
	sudo cp $(NAME) $(INSTALL_DIR)/
	cp -r presets/ $(PRESETS_DIR)/
endif
	@echo ""
	@echo "Templater successfully installed at '${INSTALL_DIR}'"

uninstall:
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "if (Test-Path $(INSTALL_DIR)\$(NAME).exe) { Remove-Item -Force $(INSTALL_DIR)\$(NAME).exe }"
	powershell -NoProfile -Command "if (Test-Path ${PRESETS_DIR}) { Remove-Item -Recurse ${PRESETS_DIR} }"
else
	rm -f $(INSTALL_DIR)/$(NAME)
	rm -rf $(PRESETS_DIR)
endif
	@echo ""
	@echo "Templater successfully uninstalled from '${INSTALL_DIR}'"

-include $(OBJS:.o=.d)
