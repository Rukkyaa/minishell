/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:15:05 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/30 12:48:29 by axlamber         ###   ########.fr       */
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
# include <signal.h>
# include <curses.h>
# include <term.h>
# include <termios.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <errno.h>
# include <poll.h>
# include <sys/msg.h>
# include "../libft/libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	int				code;
	struct s_env	*next;
}	t_env;

typedef struct s_infile
{
	char			*file_in;
	struct s_infile	*next;
}	t_infile;

typedef struct s_outfile
{
	char				*file_out;
	struct s_outfile	*next;
	int					created;
	int					append;
}	t_outfile;

typedef struct s_minishell
{
	pid_t				pid;
	char				**cmd;
	int					fd[2];
	t_infile			*file_in;
	t_outfile			*file_out;
	int					append;
	t_env				*env;
	struct s_minishell	*next;
}	t_minishell;

typedef struct s_tree
{
	t_minishell		*first_elem;
	char			*cmd;
	struct s_tree	*and;
	struct s_tree	*or;
	int				nb_cmd;
}	t_tree;

typedef struct s_all
{
	t_env	*env;
	char	**here_docs;
	t_tree	*start;
}	t_all;

typedef struct s_sig
{
	int		sig_quit;
	int		sig_int;
	int		p_status;
	int		cmd_stat;
	char	*line;
}	t_sig;

extern t_sig	g_sig;

# define BUFFER_SIZE 42
//# define PATH_MAX 100

int			var_empt(char *var);
t_env		*env_to_struct(char **env);
int			sig_exit(int nb);
int			potential_name(char c);
char		*trimator(char *str);
int			pipe_in(char *str);
int			var_empt(char *var);
char		*get_var(char *line, t_env *envp, int i, int *leng);

int			check_minishell(char **tabl);
int			count_words2(char *str, char c);
char		*erase_redirbis(char *cmd);
int			all_spaces(char **tabl);
int			stop_signals(void);
char		**trim_tab(char **tabl);
int			create_signal_spe(void);
int			create_signal_here(void);
int			check_redirection(char *cmd, t_minishell *maillon);
char		*ft_strcpy(char *dst, const char *src);
void		clean_rest(t_tree *start, int i, int end);
char		**maj_path(t_env *env);
void		print_env(t_env *env);
int			check_spaces(char *s, char c);
int			check_spaces_spe(char *str, char c);
int			que_space(char *str);
void		abort_pipe2(t_minishell *elem, t_all *p);
void		abort_pipe3(t_minishell *elem, t_all *p);
void		abort_pipe(t_minishell *elem, t_all *p);

//wildcard.c
char		*wildcard(char *extension);

//signal/signal.c
int			event(void);
int			create_signal(void);
void		init_signal(int nb);
void		sig_eof(int code);
void		sighandler(int code);

//parsing/1-init.c
t_all		*init_env(char **env);
t_tree		*init_tree(char **line);
t_tree		*init_m_tree(char *str);
t_tree		*fill_branch(char *reserve, int i);

//parsing/1-initbis.c
int			check_interligne(char *line);
int			check_extraligne(char *line);
char		*clean_first(char *line);
int			ambiguous(char *line, t_env *env);
int			fill_file(t_all *p, char **line, int max, int nb);

//parsing/2-free.c
void		free_start(t_tree *start, int mode);
void		free_files_in(t_infile *lst);
void		free_files_out(t_outfile *lst, int mode);
void		free_minishell(t_minishell *elem, int mode);
void		free_env(t_env *envp);

//parsing/2-freebis.c
void		free_here_docs(char **here_docs);
void		free_all(t_all *p);

//parsing/3-segmentation.c
int			first_segmentation(t_tree *start, t_tree *init);
int			scnd_segmentation(t_tree *start, t_tree *init);
int			op_scd_treat_and(t_tree *start, int i, char *reserve);

//parsing/3-segmentationbis.c
int			op_segmentation(t_tree *start, int i, int end, char *reserve);
int			op_scd(t_tree *start, int i, char *reserve);

//parsing/4-operations.c
int			op_found(char *line);
char		*clean_rest_op(t_tree *start, int index);
char		*clean_res(char *reserve, int index);
int			ft_strcmp(char *s1, char *s2);
int			avoid_quotes_spe(char *line, int i);

//parsing/5-heredocs.c
char		**get_here_docs(char **line, t_all *p);
int			heredoc_count(char *line, int index);
int			prev_valo(char *lect);
void		warning(char *str, int nb);
char		*generate_name(char *limiter);
char		*find_lim(char *line, int *alert);
char		*gen_new_limiter(char *limiter);
char		*clean_heredoc_line(char *line, char *filename,
				char *LIM, int *alert);
int			signals_hdoc(int opt);
int			pipex(t_minishell *first_elem, t_all *p, t_tree *start);
bool		check_line(char *line, int *i, int flag);
void		alert_case(char *str);
char		*find_lim2(char *line, int i, int compt, int d);
int			increment_amb(char *line, int i);
void		emergency(t_minishell *e, t_all *p);
void		clean_p(char **paths, int flag, t_all *p);
void		emergency(t_minishell *e, t_all *p);

//parsing/6-count.c
int			countof_spe(char **line, char c, int compt, int i);
int			avoid_quotes(char *line, int i);
int			last_char_spe(char *line, char c);
int			first_char_spe(char *line, char c, int index);
int			count_pipe(char *cmd);
int			increment_d(int d, int *compt, char *line);

//parsing/7-print.c
void		print_tree(t_tree *start, int i);
void		print_pipe(t_minishell *elem, int i);
void		print_cmd(t_tree *start, int i);
void		print_here_doc(char **here_docs);
void		print_all(t_all *p);

//parsing/8-init2.c
int			init_shell(t_tree *start, t_all *p);
int			init_cmd(t_tree *start, t_all *p);
int			recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p);
int			length_tab(char **tabl);
char		*treat_str(char *s1, int *i, char c);
char		**tabone(char *s);

//parsing/9-redir.c
char		*erase_redir(char *cmd);
int			check_redirection(char *cmd, t_minishell *maillon);
char		*get_filename(char *line, int i);
void		clean_rest(t_tree *start, int i, int end);
t_outfile	*add_file_out(t_outfile *lst, char *file, int opt);
t_infile	*add_file_in(t_infile *lst, char *file);
t_outfile	*null_case(char *file, int opt);
int			append_treat(char *cmd, t_minishell *maillon);
void		clean_rest(t_tree *start, int i, int end);
int			increment_i(char *line, int i);
int			init_shell(t_tree *start, t_all *p);

//parsing/10-complement.c
int			countbis(int i, char d, char *line);
int			countofquotes(char *line, char c, int compt);
int			check_whitespace(char *line);
int			invalid_quote(char *line);
int			first_check(char *line, t_env *env);
int			empty_redir(char *line);

//parsing/11-wildparsing.c
char		**w_finder(char **tabl);

//parsing/12-taboperations.c
char		*tab_to_str(char **tabl, int opt);
char		*ft_trim(char *s1);
char		**trim_tab(char **tabl);
int			length_tab(char **tabl);
char		*ft_trimhard(char *s1);

//parsing/spe_split.c
char		**ft_split_spe(char *s, char c);

//parsing/var_dealer.c
char		*replace_var(char *line, t_all *p);

//parsing//var_dealer2.c
int			find_other(char *line, int init, int *i, int opt);
char		*tilde(t_env *env);
char		*change_line(char *line, char *var, int i, int *leng);
char		*get_var(char *line, t_env *envp, int i, int *leng);

//pipex/command.c
int			exec_command(char **paths, char **cmd, t_all *p, t_tree *start);
void		end_process(t_all *p, int nb);

//pipex/executor.c
int			executor(t_tree *start, t_all *p, char *line);
int			opening(char *file, int port, int append, int mode);
char		**env_to_char(t_env *env);
int			cond_redir(t_minishell *elem, t_all *p);

//pipex/executor2.c
int			opening_in(t_infile *file_org, int port, char **cmd, t_all *p);
int			opening_out(t_outfile *file_org, int port, char **cmd, t_all *p);
int			find_cat(t_minishell *elem);

//pipex/exec_builtins.c
int			path_comp_builtins(char **paths);
int			exec_builtin(int nb, char **cmd, t_all *p, t_tree *start);

//pipex/pipe.c
int			first_pipe(t_minishell *elem, t_all *p, t_tree *start);
int			mid_pipe(t_minishell *elem, t_all *p, t_tree *start);
int			last_pipe(t_minishell *elem, t_all *p, t_tree *start);
int			close_all(int fd1, int fd2, int *status1, pid_t pid);
int			end_pipe2(t_minishell *elem, t_all *p, t_tree *start);
int			end_pipe3(t_minishell *elem);

//pipex/pipe_spe.c
int			first_pipe_cat(t_minishell *elem, t_all *p, t_tree *start);
int			mid_pipe_cat(t_minishell *elem, t_all *p, t_tree *start);
int			last_pipe_cat(t_minishell *elem, t_all *p, t_tree *start);

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

//get_path.c
char		*get_env_var(t_env *envp, char	*var);
int			is_whitespace(char c);
int			ft_isprint(int c);

//split_path.c
void		free_tab(char **tabl);
int			count_words(char const *str, char c);
int			words_length(char const *str, int i, char c);
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

void		remove_sub(char **str, int start, int end);
int			get_redirection(char *str, t_minishell *minishell);

// BUILTINS
int			ft_pwd(void);
int			ft_echo(char **split);
int			ft_env(t_env *env);
int			ft_unset(t_env *env, char **to_unset);
int			ft_export(t_env *env, char **split);
int			ft_cd(t_env *env, char **split);
void		ft_exit(t_all *p, t_tree *start, char **cmd);
void		error_cd(char *str);

// ENV STRUCT
t_env		*ft_envlast(t_env *lst);
void		ft_env_add_back(t_env **lst, t_env *new);
t_env		*env_to_struct(char **env);
t_env		*ft_envnew(char *key, char *value);
bool		ft_is_in_env(t_env *env, char *str);
char		**env_to_char_export(t_env *env);
char		**sort_env(char **env);
char		*ft_strndup(char *str, int n);
void		change_value(t_env *env, char *key, char *value);
void		change_value_equal(t_env *env, char *key);

#endif
