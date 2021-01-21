# VARIABLES

NAME = ft_ping
CC = gcc -g -Wall -Wextra -Werror -fsanitize=address
RM = rm -rf

# DIRECTORIES

DHEADERS = ./inc/
DSRCS	= ./srcs/
DOBJS	= ./comp/


HEADERS = ft_ping.h
# SOURCES

SRCS = ft_ping.c network.c data.c annexes.c
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

test: all
		sudo ./ft_ping "8.8.8.8"

# CLEAR

clean:
	$(RM) $(DOBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean re fclean