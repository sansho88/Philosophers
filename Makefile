# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tgriffit <tgriffit@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/13 10:32:33 by tgriffit          #+#    #+#              #
#    Updated: 2022/09/07 12:18:11 by tgriffit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo
CC := gcc
FLAGS := -Wall -Wextra -Werror
#FLAGS += -fsanitize=thread -g3
THREAD := -pthread
DIR_UTILS := utils/

SRCS := main.c \
		amanuensis.c \
        anihiliation.c \
        chronos.c \
        dayAndNight.c \
        nursery.c \
		$(DIR_UTILS)ft_atoi.c \
		 $(DIR_UTILS)ft_isdigit.c \
		 $(DIR_UTILS)ft_str_isdigit.c \
		 $(DIR_UTILS)ft_strchr.c \
		 $(DIR_UTILS)ft_strlen.c

OBJS := $(SRCS:.c=.o)

all: $(NAME)

%.o:%.c philo.h Makefile
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
		$(CC) $(FLAGS) $(OBJS) $(THREAD) -o $(NAME)
		@echo "philo:\t\033[1;32m[READY]\033[0m"
		@echo "You can use it like:"
		@echo "\033[7m./philo [number_of_philosophers] [time_to_die]" \
        "[time_to_eat] [time_to_sleep]" \
        "(number_of_times_each_philosopher_must_eat)\033[0m\n"

clean:
		@rm $(OBJS)

fclean:	clean
		rm $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SILENT: