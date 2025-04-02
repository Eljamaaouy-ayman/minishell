CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = minishell
FILES = minishell.c ft_split.c additional.c additional1.c export_and_unset.c
OBJECT = $(FILES:.c=.o)

all : $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJECT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECT) -lreadline

clean :
	rm -f $(OBJECT)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all re clean fclean $(NAME)
.SECONDARY: $(OBJECT)
