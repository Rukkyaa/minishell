# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/19 10:54:33 by gabrielduha       #+#    #+#              #
#    Updated: 2022/12/22 14:41:57 by gduhau           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT	= all

SRCS	= minishell.c libft_utils.c get_path.c split_path.c gen_command.c redirection.c ft_epur.c
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