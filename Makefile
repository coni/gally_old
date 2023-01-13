# Makefile

CC			= gcc
CPPFLAGS	= -Igally/include/
CFLAGS 		= -Werror -Wextra -std=c11
LDFLAGS		= -ggdb3
LDLIBS		= -lcurl

EXE = main
DIR = gally

SRC	= $(wildcard $(DIR)/*.c) $(wildcard $(DIR)/src/*.c) $(wildcard $(DIR)/src/*/*.c)
OBJ	= ${SRC:.c=.o}
DEP	= ${SRC:.c=.d}

-include $(DEP)

all: $(EXE)

debug: CFLAGS += -ggdb3 -g -DDEBUG
debug: main

$(EXE): $(DIR)/main
	mv $(DIR)/main $(EXE)

$(DIR)/main: $(OBJ)

clean:
	rm -f $(OBJ) $(DEP) $(EXE)

# END
