# VARIABLES

GREEN = \033[38;5;40m
RESET = \033[0m

NAME = ft_ping
CC = gcc -Wall -Wextra -Werror
RM = rm -rf

# DIRECTORIES

DSRCS	= ./srcs/
DOBJS	= ./comp/


HEADERS = ./inc/ping.h
# SOURCES

SRCS = ping.c network.c data.c annexes.c display.c error.c args.c ip.c signal.c icmp.c stats.c io_return.c time.c
OBJS = $(SRCS:%.c=$(DOBJS)%.o)



# COMPILATION

all: $(NAME)
	echo "$(GREEN)DONE ✔$(RESET)"

make_libft:
	cd libft && make

$(NAME): make_libft $(OBJS) $(HEADERS)
	$(CC) $(OBJS) -o $(NAME) libft/libft.a

$(OBJS): | $(DOBJS) # https://www.gnu.org/software/make/manual/make.html

$(DOBJS):
	mkdir $(DOBJS)

$(DOBJS)%.o: $(DSRCS)%.c
	$(CC) -c $< -o $@

test: all
		sudo ./ft_ping "8.8.8.8"

# CLEAR

clean:
	cd libft && make clean
	$(RM) $(DOBJS)

fclean: clean
	cd libft && make fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean re fclean make_libft
.SILENT: all