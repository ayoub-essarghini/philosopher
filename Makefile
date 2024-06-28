NAME = philosopher
INCLUDES = ./philo.h
SRCS = philo.c philo_helper.c ft_routines.c utils.c
SRCS_BONUS= philo.c philo_helper.c ft_routines.c utils.c

CFLAGS= -Wall -Werror -Wextra -g3 #-fsanitize=thread

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(INCLUDES) $(OBJS)
	cc  $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "DONE ✅"

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
	
