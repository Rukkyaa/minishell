/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:15:05 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/19 13:29:48 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>


// < == 0 input
// > == 1 output
// >> == 2 output

typedef struct s_list
{
	pid_t			pid;
	char			**cmd;
	int				fd[2];
	char			**redirections;
	char			**env;
	struct s_list	*next;
}	t_list;

//ft_slit.c
char	**ft_split(char *s, char c);
void	free_tab(char **tab);
int	count_words(char const *str, char c);
int	words_length(char const *str, int i, char c);

//get_path.c
char	**path_generator(int path, char **env);
char	**split_path(char *s, char c);
int	find_path(char **env);

//libft_utils.c
int is_whitespace(char c);
int	ft_isprint(int c);

#endif
