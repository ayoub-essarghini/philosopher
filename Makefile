NAME = philo
INCLUDES = ./philo.h
SRCS = philo.c philo_helper.c ft_routines.c \
		ft_routines_helper.c utils.c utils2.c

CFLAGS= -Wall -Werror -Wextra -g3 -fsanitize=thread 

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(INCLUDES) $(OBJS)
	cc  $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "DONE ✅"

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
	
