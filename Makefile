# ======== variables ========
NAME := libmx.a
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# -- commands --
MKDIR := mkdir -p
RM := rm -rf
CC := clang
AR := ar
ARFLAGS := rcs
CPPFLAGS := -I$(INC_DIR)
CFLAGS := -std=c11 -Wall -Wextra -Werror -Wpedantic

# ========== body =========
all: $(NAME)

install: $(NAME)

$(NAME): $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR):
	$(MKDIR) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/libmx.h | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR) $(NAME)

uninstall: clean

reinstall: clean all

.PHONY: all install uninstall clean reinstall
