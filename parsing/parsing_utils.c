/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:36:57 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/30 12:38:07 by axlamber         ###   ########.fr       */
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
					printf("%c", line[i++]);
				return (free(line), free(var), 1);
			}
			free(var);
		}
	}
	return (free(line), 0);
}

int	empty_redir(char *line)
{
	int	i;

	i = 0;
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	while (i < ft_strlen(line) && (line[i] == '>' || line[i] == '<'))
		i++;
	if (i < ft_strlen(line) && line[i] != ' '
		&& line[i] != '<' && line[i] != '>')
		return (0);
	while (i < ft_strlen(line) && is_whitespace(line[i]) == 1)
		i++;
	if (i == ft_strlen(line))
		return (1);
	return (0);
}

int	first_check(char *line, t_env *env)
{
	char	*line_bis;
	char	**tab_cmd;

	if (ambiguous(ft_strdup(line), env) == 1)
		return (printf(": ambiguous redirect\n"), 1);
	if (ft_strlen(line) == 1 && line[0] == '.')
		return (printf(".: filename argument required\n"), 1);
	if (empty_redir(line) == 1)
		return (printf("syntax error near unexpected token `newline'\n"), 1);
	line_bis = erase_redir(ft_strdup(line));
	if (check_spaces(line_bis, ' ') == 1 || que_space(line_bis) == 1)
		return (free(line_bis), 0);
	tab_cmd = ft_split_spe(line_bis, '|');
	if (pipe_in(line_bis) == 1 && all_spaces(tab_cmd) == 1)
		return (printf("syntax error near unexpected token `|'\n"),
			free_tab(tab_cmd), free(line_bis), g_sig.cmd_stat = 2, 1);
	return (free_tab(tab_cmd), free(line_bis), 0);
}
