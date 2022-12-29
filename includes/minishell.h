/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:15:05 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/29 10:37:50 by rukkyaa          ###   ########.fr       */
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
# include <string.h>

# include "../libft/libft.h"

// < == 0 input
// > == 1 output
// >> == 2 output

typedef struct s_minishell
{
	pid_t				pid;
	char				**cmd;
	int					fd[2];
	char				*infile;
	char				*outfile;
	int					append;
	char				**env;
	char				**paths;
	struct s_minishell	*next;
}	t_minishell;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_tree
{
	t_minishell		*first_elem;
	char			*cmd;
	struct s_tree	*and;
	struct s_tree	*or;
}	t_tree;

typedef struct s_all
{
	char	**paths;
	char	**env;
	char	**here_docs;
	t_tree	*start;
}	t_all;

# define BUFFER_SIZE 42

//parsing/1-init.c
t_all		*init_env(char **env);
t_tree		*init_tree(char **line);
t_tree		*init_m_tree(int length);
t_tree		*fill_branch(char *reserve, int i);

int			check_redirection(char *cmd, t_minishell *maillon);
char		*ft_strcpy(char *dst, const char *src);
void		clean_rest(t_tree *start, int i, int end);

//parsing/2-free.c
void		free_all(t_all *p);
void		free_start(t_tree *start);
void		free_cmd(t_all *p);

//parsing/3-segmentation.c
int			op_segmentation(t_tree *start, int i, int end, char *reserve);
int			first_segmentation(t_tree *start, t_tree *init);
int			scnd_segmentation(t_tree *start, t_tree *init);
int			op_scd(t_tree *start, int i, char *reserve);

//parsing/4-operations.c
int			op_found(char *line);
char		*clean_rest_op(t_tree *start, int index);
void		clean_res(char *reserve, int index);

//parsing/5-heredocs.c
char		**get_here_docs(char **line);
int			heredoc_count(char *line, int index);

//parsing/6-count.c
int			countof_spe(char **line, char c, int compt);
int			avoid_quotes(char *line, int i);
int			last_char_spe(char *line, char c);
int			first_char_spe(char *line, char c, int index);
int			count_pipe(char *cmd);

//parsing/7-print.c
void		print_tree(t_tree *start, int i);
void		print_pipe(t_minishell *elem, int i);
void		print_cmd(t_tree *start, int i);
void		print_here_doc(char **here_docs);

//parsing/8-init2.c
int			init_shell(t_tree *start, t_all *p);
int			init_cmd(t_tree *start, t_all *p);
int			recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p);

//parsing/9-redir.c
char		*erase_redir(char *cmd);
int			check_redirection(char *cmd, t_minishell *maillon);
char		*get_filename(char *line, int i);
void		clean_rest(t_tree *start, int i, int end);

//get_next_line
char		*get_next_line(int fd);
int			return_exist(char *s);
char		*gen_answer(char *new_reserve);
char		*ft_strcatspe(char *s1, char *s2);
char		*clean_reserve(int i, char *reserve);
char		*maj_reserve(int fd, char *reserve);
char		*error_case(char *buf, char *reserve, int p);

//ft_slit.c
char		**ft_split(char *s, char c);
void		free_tab(char **tab);
int			count_words(char const *str, char c);
int			words_length(char const *str, int i, char c);

//get_path.c
char		*get_env_var(char **env, char *var);
char		**split_path(char *s, char c);

//libft_utils.c
int			is_whitespace(char c);
char		*ft_strcat(char *s1, char *s2);
char		*ft_strjoin_spe(char *s1, char *s2);

int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_isprint(int c);
char		*ft_strdup(const char *s1);
int			ft_strlen(const char *s);

//gen_command.c
char		*get_filename(char *line, int i);
char		*extrac_cmd(char *line, char **redir);
char		*clean_up(char *line, int dep, int length);
t_minishell	*gen_maillon(char *line);

char		*ft_epur(char *str);
void		remove_sub(char **str, int start, int end);
int			get_redirection(char *str, t_minishell *minishell);

// ENV
void		ft_env_add_back(t_env **lst, t_env *new);
t_env		*ft_envlast(t_env *lst);
t_env		*ft_envnew(char *key, char *value);
char		*ft_strndup(char *str, int n);
t_env		*env_to_struct(char **env, t_env *env_struct);

// BUILTINS
int			ft_echo(char *str);
int			ft_env(t_env *env);
int			ft_pwd(void);
int			ft_unset(t_env *env, char *to_unset);

#endif
