TARGET = libsha256.a

CC = clang
CFLAGS = -Wall -Wextra -Werror -fPIC -g

SRCDIR = src
OBJDIR = obj
INCDIR = -I include

SRCS = $(shell find $(SRCDIR) -name '*.c')
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

OBJDIRS = $(sort $(dir $(OBJS)))

all: $(OBJDIRS) $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

$(OBJDIRS):
	mkdir -p $(OBJDIRS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re

