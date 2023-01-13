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

release: $(EXE)

debug: CFLAGS += -ggdb3 -g -DDEBUG
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
	$(RM) -r options.txt
	$(RM) -r output-client.log

# END
