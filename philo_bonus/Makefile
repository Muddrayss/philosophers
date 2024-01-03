# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/29 15:02:34 by egualand          #+#    #+#              #
#    Updated: 2024/01/03 14:36:37 by egualand         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INCLUDES_DIR = .
SRCS = 	check_args.c \
		death.c \
		ft_atoi.c \
		init.c \
		philosophers.c \
		utils.c \
		sem_check.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INCLUDES_DIR) -pthread
RM = rm -f

NAME = philo_bonus

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(NAME) compiled"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@echo "removed objects files"

fclean: clean
	@$(RM) $(NAME)
	@echo "removed $(NAME)"

re: fclean all

.PHONY: all clean fclean re