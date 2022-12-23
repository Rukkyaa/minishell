/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:15:05 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/23 08:47:41 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>

# include "../libft/libft.h"


// < == 0 input
// > == 1 output
// >> == 2 output

typedef struct s_minishell
{
	pid_t			pid;
	char			**cmd;
	int				fd[2];
	char			**redirections;
	int				infile;
	int				outfile;
	char			**env;
	struct s_list	*next;
}	t_minishell;


char **check_redirection(char *line);

//ft_slit.c
char	**ft_split(char *s, char c);
void	free_tab(char **tab);
int	count_words(char const *str, char c);
int	words_length(char const *str, int i, char c);

//get_path.c
char	*get_env_var(char **env, char *var);
char	**split_path(char *s, char c);

//libft_utils.c
int is_whitespace(char c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_isprint(int c);
char	*ft_strdup(const char *s1);
int	ft_strlen(const char *s);
char	*ft_strjoin_spe(char *s1, char *s2);

//gen_command.c
char *get_filename(char *line, int i);
char *extrac_cmd(char *line, char **redir);
char	*clean_up(char *line, int dep, int length);
t_minishell	*gen_maillon(char *line);

char	*ft_epur(char *str);
void	remove_sub(char **str, int start, int end);
int	get_redirection(char *str, t_minishell *minishell);

// BUILTINS
int	ft_pwd(void);
int	ft_echo(char *str);

#endif
