/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:36:57 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/30 23:54:34 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_here_docs(char **line, t_all *p)
{
	if (heredoc_count(*line, 0) == 0)
		return (NULL);
	p->here_docs = malloc((heredoc_count(*line, 0) + 1) * sizeof(char *));
	if (!p->here_docs)
		return (NULL);
	if (fill_file(p, line, heredoc_count(*line, 0), 0) == -1)
		return (free_here_docs(p->here_docs), NULL);
	return (p->here_docs);
}

char	*find_lim(char *line, int *alert)
{
	int		i;
	int		compt;
	int		d;

	i = 0;
	compt = 0;
	while (line[i] != '\0' && line[i + 1] != '\0'
		&& !(line[i] == '<' && line[i + 1] == '<'))
		i = increment_i(line, i);
	i += 2;
	while (line[i] != '\0' && is_whitespace(line[i]) == 1)
		i++;
	d = i;
	*alert = i;
	while (d < ft_strlen(line) && potential_name(line[d]) == 1
		&& is_whitespace(line[d]) == 0)
		d = increment_d(d, &compt, line);
	return (find_lim2(line, i, compt, d));
}

int	ambiguous(char *line, t_env *env)
{
	int		i;
	int		leng;
	char	*var;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i + 1 < ft_strlen(line) && (line[i] == '>' || line[i] == '<'))
	{
		i = increment_amb(line, i);
		if (i < ft_strlen(line) && line[i] == '$')
		{
			var = get_var(line, env, i, &leng);
			if (var_empt(var) == 1)
			{
				while (i < ft_strlen(line) && is_whitespace(line[i]) == 0)
					ft_putchar_fd(line[i++], 2);
				return (free(line), free(var), 1);
			}
			free(var);
		}
	}
	return (free(line), 0);
}

int	all_rest(char *line, int i)
{
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i == ft_strlen(line))
		return (1);
	return (0);
}

int	empty_redir(char *line)
{
	int	i;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if ((line[i] == '>' || line[i] == '<') && all_rest(line, i + 1) == 1)
		return (ft_putstr_fd("syntax error near unexpected token `newline'\n", 2), 1);
	else if (line[i] == '>' && line[i + 1] == '>' && all_rest(line, i + 2) == 1)
		return (ft_putstr_fd("syntax error near unexpected token `newline'\n", 2), 1);
	return (0);
}

int	first_check(char *line, t_env *env)
{
	if (ambiguous(ft_strdup(line), env) == 1)
		return (ft_putstr_fd(": ambiguous redirect\n", 2), g_sig.cmd_stat = 1, 1);
	if (ft_strlen(line) == 1 && line[0] == '.')
		return (ft_putstr_fd(".: filename argument required\n", 2), g_sig.cmd_stat = 2, 1);
	if (empty_redir(line) == 1)
		return (g_sig.cmd_stat = 2, 1);
	return (0);
}
