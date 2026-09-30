# ======== variables ========
NAME      := libmx.a
SRC_DIR   := src
OBJ_DIR   := obj
BUILD_DIR := build
TEST_DIR  := tests
INC_DIR   := inc

SRC_FILES  := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES  := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_BIN   := $(BUILD_DIR)/test_runner
TEST_HDRS := $(wildcard $(TEST_DIR)/*.h)

# -- commands --
MKDIR   := mkdir -p
RM      := rm -rf
CC      := clang
AR      := ar
ARFLAGS := rcs
CPPFLAGS := -I$(INC_DIR)
CFLAGS   := -std=c11 -Wall -Wextra -Werror -Wpedantic

ifdef SANITIZE
CFLAGS += -g -O0 -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=undefined
endif

# ========== body =========
all: $(NAME)

install: $(NAME)

$(NAME): $(OBJ_FILES) | $(BUILD_DIR)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR) $(BUILD_DIR):
	$(MKDIR) $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/libmx.h | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(NAME) $(TEST_FILES) $(TEST_HDRS) $(INC_DIR)/libmx.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_FILES) $(NAME) -o $@

test-bin: $(TEST_BIN)

test:
	$(MAKE) test-bin SANITIZE=1 \
	    OBJ_DIR=$(OBJ_DIR)/san \
	    NAME=$(BUILD_DIR)/libmx_san.a \
	    TEST_BIN=$(BUILD_DIR)/test_san
	./$(BUILD_DIR)/test_san

clean:
	$(RM) $(OBJ_DIR) $(BUILD_DIR)

uninstall: clean
	$(RM) $(NAME)

reinstall:
	$(MAKE) uninstall
	$(MAKE) all

.PHONY: all install uninstall clean reinstall test test-bin