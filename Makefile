# VARIABLES

NAME = ft_ping
CC = clang++ -g -Wall -Wextra -Werror -fsanitize=address
RM = rm -rf

# DIRECTORIES

DHEADERS = ./inc/
DSRCS	= ./srcs/
DOBJS	= ./comp/


HEADERS = ./inc/*
# SOURCES

SRCS = ft_ping.c ft_ping.h network.c data.c annexes.c
OBJS = $(SRCS:%.cpp=$(DOBJS)%.o)



# COMPILATION

all: $(NAME)

run: all


$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS): | $(DOBJS) # https://www.gnu.org/software/make/manual/make.html

$(DOBJS):
	mkdir $(DOBJS)

$(DOBJS)%.o: $(DSRCS)%.cpp
	$(CC) -c $< -o $@

# CLEAR

clean:
	$(RM) $(DOBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean re fclean