# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/19 10:54:33 by gabrielduha       #+#    #+#              #
#    Updated: 2022/12/29 10:17:15 by rukkyaa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT	= all

SRCS	= minishell.c libft_utils.c get_path.c split_path.c gen_command.c redirection.c ft_epur.c \
			parsing/1-init.c parsing/2-free.c \
			parsing/3-segmentation.c parsing/4-operations.c parsing/5-heredocs.c \
			parsing/get_next_line/get_next_line.c parsing/get_next_line/get_next_line_utils.c \
			parsing/6-count.c parsing/7-print.c parsing/8-init2.c parsing/9-redir.c \
			$(wildcard my_builtins/*.c) $(wildcard exec/*.c)
OBJS	= ${SRCS:.c=.o}
NAME	= minishell
CC		= cc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror
HEADERS = -I includes/ -I libft/

LIBFT = -L libft/ -lft

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
			make -C libft/
			${CC} ${CFLAGS} ${OBJS} -lreadline $(LIBFT) ${HEADERS} -o ${NAME}

all: ${NAME}

clean:
			make clean -C libft/
			${RM} ${OBJS}

fclean:	clean
			make fclean -C libft/
			${RM} ${NAME}

re:		fclean all

.PHONY: all clean fclean re