# Makefile

CC			= gcc
CPPFLAGS 	= 
CFLAGS 		= -Wall -Werror -Wextra
LDFLAGS		=
LDLIBS		= -lcurl

EXE = main
DIR = gally

SRC	= $(wildcard $(DIR)/*.c) $(wildcard $(DIR)/src/*.c) $(wildcard $(DIR)/src/*/*.c)
OBJ	= ${SRC:.c=.o}
DEP	= ${SRC:.c=.d}

-include $(DEP)

all: $(EXE)

$(EXE): $(DIR)/main
	mv $(DIR)/main $(EXE)

$(DIR)/main: $(OBJ)

clean:
	rm -f $(OBJ) $(DEP) $(EXE)

# END
