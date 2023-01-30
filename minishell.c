/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/30 10:36:41 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_sig	g_sig;

t_tree	*parsingator(char *line, t_all *p)
{
	t_tree	*start;
	char	**line_bis;

	if (line == NULL || !line)
		return (NULL);
	line_bis = malloc(sizeof(char *));
	if (!line_bis)
		return (NULL);
	*line_bis = ft_strdup(line);
	if (!(*line_bis) || *line_bis == NULL)
		return (free(line_bis), NULL);
	if (invalid_quote(*line_bis) == 1 || first_check(*line_bis, p->env) == 1)
		return (free(*line_bis), free(line_bis), NULL);
	p->here_docs = get_here_docs(line_bis, p);
	if (p->here_docs == NULL && heredoc_count(line, 0) != 0)
		return (free(*line_bis), free(line_bis), NULL);
	*line_bis = replace_var(*line_bis, p);
	if (*line_bis == NULL)
		return (free(line_bis), NULL);
	start = init_tree(line_bis);
	if (init_shell(start, p) == -1)
		return (free_start(start, 1), NULL);
	return (start);
}

//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
//--suppressions=./.readline.supp ./minishell

char	*do_minishell(t_all *p)
{
	init_signal(0);
	rl_event_hook = event;
	g_sig.line = readline("\033[1;033mMinishell> \033[m");
	if (ft_strcmp(g_sig.line, "end") == 0 && g_sig.sig_quit == 1)
		return (free(g_sig.line), free_all(p),
			rl_clear_history(), exit(0), NULL);
	if (check_whitespace(g_sig.line) == 1)
		g_sig.sig_int = 1;
	if (g_sig.sig_int == 0 && g_sig.sig_quit == 0)
	{
		add_history(g_sig.line);
		g_sig.p_status = 1;
		p->start = parsingator(g_sig.line, p);
		if (g_sig.sig_int == 1)
			free_start(p->start, 1);
		if (p->start != NULL && g_sig.sig_int == 0)
			executor(p->start, p, g_sig.line);
		free_here_docs(p->here_docs);
	}
	free(g_sig.line);
	g_sig.line = NULL;
	// if (rl_line_buffer != NULL)
	// 	rl_replace_line("", 0);
	return (g_sig.line);
}

int	main(int argc, char **argv, char **env)
{
	t_all	*p;

	(void)argc;
	(void) **argv;
	p = init_env(env);
	if (!p || p == NULL)
		return (EXIT_FAILURE);
	g_sig.line = NULL;
	while (g_sig.line == NULL)
	{
		do_minishell(p);
	}
	return (rl_clear_history(), free_all(p), EXIT_SUCCESS);
}
