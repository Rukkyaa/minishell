# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/19 10:54:33 by gabrielduha       #+#    #+#              #
#    Updated: 2023/01/30 12:47:44 by axlamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT	= all

SRCS	= minishell.c libft_utils.c get_path.c split_path.c pipex/command.c\
			my_builtins/ft_pwd.c my_builtins/ft_echo.c parsing/1-init.c parsing/2-free.c \
			parsing/3-segmentation.c parsing/4-operations.c parsing/5-heredocs.c \
			parsing/get_next_line/get_next_line.c parsing/get_next_line/get_next_line_utils.c \
			parsing/6-count.c parsing/7-print.c parsing/8-init2.c parsing/9-redir.c \
			pipex/executor.c pipex/pipe.c parsing/spe_split.c parsing/var_dealer.c \
			pipex/exec_builtins.c my_builtins/envtostruct.c my_builtins/ft_cd.c my_builtins/ft_env.c \
			my_builtins/ft_export.c my_builtins/ft_unset.c my_builtins/ft_exit.c signal/signal.c \
			my_builtins/sort_env.c wildcard.c pipex/pipe_spe.c parsing/10-complement.c \
			pipex/executor2.c parsing/1-initbis.c parsing/2-freebis.c parsing/3-segmentationbis.c \
			parsing/11-wildparsing.c parsing/12-taboperations.c parsing/var_dealer2.c \
			my_builtins/export_utils.c parsing/9-redirebis.c parsing/8-initbis2.c \
			pipex/pipe2.c signal/signal2.c parsing/5-heredocs2.c parsing/5-heredocs3.c \
			parsing/utils.c pipex/pipex_utils.c parsing/9-redirebisbis.c parsing/parsing_utils.c \
			pipex/utils.c
			
OBJS	= $(SRCS:.c=.o)
NAME	= minishell
CC		= cc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror -g
HEADERS = -I includes/ -I libft/

LIBFT = -L libft/ -lft

flag:=1
.c.o:
		@setterm -cursor off
		@if [ $(flag) = "1" ]; then\
			clear ;\
			printf "\033[1;35m========================================\n";\
			printf "|           MINISHELL LOADING          |\n";\
			printf "========================================\n";\
		fi
		@printf "\033[C\033[32mCompiling $@... \033[K\033[m\r"
		@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@
		$(eval flag=$(shell echo $$(($(flag)+1))))

$(NAME):	$(OBJS)
			@echo "\033[K\033[1;32m Minishell : compiled\033[m"
			@make --no-print-directory -C libft/
			@$(CC) $(CFLAGS) $(OBJS) -lreadline $(LIBFT) $(HEADERS) -o $(NAME)
			@printf "\033[1;32m========================================\n"
			@printf "|         COMPILATION FINISHED !       |\n"
			@printf "========================================\n\033[m"
			@setterm -cursor on

all: $(NAME)

clean:
			@printf "\033[1;31m========================================\n"
			@printf "|           MINISHELL CLEANING         |\n"
			@printf "========================================\n\033[m"
			@printf "\033[K\033[1;33m Destroying objects\n\033[m"
			@make --no-print-directory clean -C libft/
			@$(RM) $(OBJS)

fclean:	clean
			@printf "\033[K\033[1;31m Destroying all\n\033[m"
			@make --no-print-directory fclean -C libft/
			@$(RM) $(NAME)

re:		fclean all

.PHONY: all clean fclean re
