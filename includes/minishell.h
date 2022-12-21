/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:15:05 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/19 18:15:36 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>


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


char **check_redirection(char *line);

//ft_slit.c
char	**ft_split(char *s, char c);
void	free_tab(char **tab);
int	count_words(char const *str, char c);
int	words_length(char const *str, int i, char c);

//get_path.c
char	**get_path(char **env);
char	**split_path(char *s, char c);

//libft_utils.c
int is_whitespace(char c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_isprint(int c);
char	*ft_strdup(const char *s1);
int	ft_strlen(const char *s);
char	*ft_strjoin(char *s1, char *s2);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putchar_fd(char c, int fd);

//gen_command.c
char *get_filename(char *line, int i);
char *extrac_cmd(char *line, char **redir);
char	*clean_up(char *line, int dep, int length);
t_list	*gen_maillon(char *line);


void    get_redirection(char *str);
#endif
