# Makefile

CC			= gcc
CPPFLAGS	= -Igally/include/
CFLAGS 		= -Werror -Wextra -std=c11
LDFLAGS		=
LDLIBS		= -lcurl

EXE = main
DIR = gally

SRC	= $(wildcard $(DIR)/*.c) $(wildcard $(DIR)/src/*.c) $(wildcard $(DIR)/src/*/*.c)
OBJ	= ${SRC:.c=.o}
DEP	= ${SRC:.c=.d}

-include $(DEP)

release: $(EXE)

debug: CFLAGS += -g -DDEBUG
debug: LDFLAGS += -ggdb3
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ) -o $@

clean:
	$(RM) $(OBJ) 
	$(RM) $(DEP)
	$(RM) $(EXE)
	$(RM) -r saves/
	$(RM) -r stats/
	$(RM) -r resourcepacks
	$(RM) -r logs/
	$(RM) options.txt
	$(RM) output-client.log
	$(RM) *.png

# END
