/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9-redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:32:08 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/29 23:46:48 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_outfile	*null_case(char *file, int opt)
{
	t_outfile	*p;

	p = malloc(sizeof(t_outfile));
	if (!p && (file == NULL || !file))
		return (NULL);
	else if (!p)
		return (free(file), NULL);
	p->file_out = file;
	p->next = NULL;
	if (opt == 1)
		p->append = 1;
	else
		p->append = 0;
	if (file != NULL && access(file, F_OK) != 0)
		p->created = 1;
	else
		p->created = 0;
	return (p);
}

int	append_treat(char *cmd, t_minishell *maillon)
{
	int	i;

	i = 0;
	while (i + 3 < ft_strlen(cmd))
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i) - 1;
		else if (cmd[i] != '>' && cmd[i + 1] == '>'
			&& cmd[i + 2] == '>' && cmd[i + 3] != '>')
		{
			maillon->file_out = add_file_out(maillon->file_out,
					ft_trimhard(get_filename(cmd, i + 3)), 1);
			if (maillon->file_out == NULL)
				return (-1);
		}
		i++;
	}
	return (1);
}

char	*erasing(char *cmd, int *i)
{
	int	end;

	if (cmd[*i] == '\"' || cmd[*i] == '\'')
	{
		end = avoid_quotes(cmd, *i);
		while (*i < end)
			cmd[(*i)++] = ' ';
	}
	else
		cmd[(*i)++] = ' ';
	return (cmd);
}

char	*erase_redir(char *cmd)
{
	int	i;

	i = 0;
	while (i + 1 < ft_strlen(cmd))
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i);
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			cmd[i] = ' ';
			if (cmd[i + 1] == '>')
				cmd[++i] = ' ';
			while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
				i++;
			while (i < ft_strlen(cmd) && potential_name(cmd[i] == 1))
				cmd = erasing(cmd, &i);
		}
		if (cmd[i] == '\0')
			break ;
		i++;
	}
	return (cmd);
}

char	*erase_redirbis(char *cmd)
{
	int	i;

	i = 0;
	if (cmd == NULL)
		return (NULL);
	while (i + 1 < ft_strlen(cmd))
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i);
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			cmd[i] = ' ';
			if (cmd[i + 1] == '>' || cmd[i + 1] == '<')
				cmd[++i] = ' ';
			while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
				i++;
			while (i < ft_strlen(cmd) && potential_name(cmd[i] == 1))
				cmd = erasing(cmd, &i);
		}
		if (cmd[i] == '\0')
			break ;
		i++;
	}
	return (cmd);
}
