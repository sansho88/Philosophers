# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/13 10:32:33 by tgriffit          #+#    #+#              #
#    Updated: 2022/04/14 13:41:31 by tgriffit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo
CC := gcc
FLAGS := -Wall -Wextra -Werror
SANI := -fsanitize=address -g3

DIR_UTILS := utils/

SRCS := main.c \
		$(DIR_UTILS)ft_atoi.c \
		 $(DIR_UTILS)ft_isdigit.c \
		 $(DIR_UTILS)ft_str_isdigit.c

OBJS := $(SRCS:.c=.o)

all: $(NAME)

%.o:%.c philo.h Makefile
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
		$(CC) $(FLAGS) $(OBJS) -o $(NAME)
		@echo "philo:\t\033[1;32m[READY]\033[0m"
		@echo "You can use it like:"
		@echo "\033[7m./philo [number_of_philosophers] [time_to_die]" \
        "[time_to_eat] [time_to_sleep]" \
        "(number_of_times_each_philosopher_must_eat)\n"

clean:
		@rm $(OBJS)

fclean:	clean
		rm $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SILENT: