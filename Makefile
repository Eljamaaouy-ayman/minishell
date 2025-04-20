CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = minishell
FILES = exection_part/minishell.c exection_part/ft_split.c exection_part/additional.c exection_part/additional1.c exection_part/export_and_unset.c exection_part/ft_execvp.c exection_part/ft_strjoin.c \
		parsing-part/execute.c parsing-part/minishell.c
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
