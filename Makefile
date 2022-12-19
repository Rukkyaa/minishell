# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/19 10:54:33 by gabrielduha       #+#    #+#              #
#    Updated: 2022/12/19 11:14:23 by gabrielduha      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT	= all

SRCS	= minishell.c
OBJS	= ${SRCS:.c=.o}
NAME	= minishell
CC		= cc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS} minishell.h
			${CC} ${CFLAGS} -lreadline -o minishell ${OBJS}

all: ${NAME}

clean:
			${RM} ${OBJS}

fclean:	clean
			${RM} ${NAME}

re:		fclean all

.PHONY: all clean fclean re