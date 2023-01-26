/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9-redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:32:08 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/26 12:07:03 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean_rest(t_tree *start, int i, int end)
{
	while (i <= end)
		start->cmd[i++] = ' ';
}

char	*get_filename(char *line, int i)
{
	char	*text;
	int		a;
	int		len;

	len = 0;
	while (is_whitespace(line[i]) == 1)
		i++;
	a = i;
	while (ft_isprint(line[a]) == 1)
	{
		if (line[a] == '\"' || line[a] == '\'')
			a = avoid_quotes(line, a);
		else
			a++;
	}
	text = malloc((a - i + 1) * sizeof(char));
	if (!text)
		return (NULL);
	while (i < a)
		text[len++] = line[i++];
	text[len] = '\0';
	return (text);
}

t_infile	*add_file_in(t_infile *lst, char *file)
{
	t_infile	*p;
	t_infile	*new_elem;
	t_infile	*p_bis;

	if (lst == NULL)
	{
		p = malloc(sizeof(t_infile));
		if (!p && (file == NULL || !file))
			return (NULL);
		else if (!p)
			return (free(file), NULL);
		return (p->file_in = file, p->next = NULL, p);
	}
	p = lst;
	p_bis = p;
	while (p->next != NULL)
		p = p->next;
	new_elem = malloc(sizeof(t_infile));
	if (!new_elem)
		return (free_files_in(lst), NULL);
	new_elem->file_in = file;
	return (new_elem->next = NULL, p->next = new_elem, p_bis);
}

static t_outfile	*null_case(char *file, int opt)
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

t_outfile	*add_file_out(t_outfile *lst, char *file, int opt)
{
	t_outfile	*p;
	t_outfile	*new_elem;
	t_outfile	*p_bis;

	if (lst == NULL)
		return (null_case(file, opt));
	p = lst;
	p_bis = p;
	while (p->next != NULL)
		p = p->next;
	new_elem = malloc(sizeof(t_outfile));
	if (!new_elem)
		return (free_files_out(lst, 1), free(file), NULL);
	new_elem->file_out = file;
	new_elem->next = NULL;
	if (opt == 1)
		new_elem->append = 1;
	else
		new_elem->append = 0;
	if (access(file, F_OK) != 0)
		new_elem->created = 1;
	else
		new_elem->created = 0;
	return (p->next = new_elem, p_bis);
}

static int	append_treat(char *cmd, t_minishell *maillon)
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

int	check_redirection(char *cmd, t_minishell *maillon)
{
	int	i;

	i = 0;
	while (i + 2 < ft_strlen(cmd))
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i) - 1;
		else if ((i == 0 && cmd[i] == '<' && cmd[i + 1] != '<')
			|| (i > 0 && cmd[i - 1] != '<'
				&& cmd[i] == '<' && cmd[i + 2] != '<'))
			maillon->file_in = add_file_in(maillon->file_in,
					ft_trimhard(get_filename(cmd, i + 1)));
		else if ((i == 0 && cmd[i] == '>' && cmd[i + 1] != '<')
			|| (cmd[i] != '>' && cmd[i + 1] == '>' && cmd[i + 2] != '>'))
			maillon->file_out = add_file_out(maillon->file_out,
					ft_trimhard(get_filename(cmd, i + 2)), 0);
		i++;
	}
	return (append_treat(cmd, maillon));
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
	while (cmd[i] != '\0' && cmd[i + 1] != '\0')
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i);
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			cmd[i] = ' ';
			if (cmd[i + 1] == '>')
				cmd[++i] = ' ';
			while (cmd[i] != '\0' && is_whitespace(cmd[i]) == 1)
				i++;
			while (cmd[i] != '\0' && is_whitespace(cmd[i]) == 0)
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
	while (cmd[i] != '\0' && cmd[i + 1] != '\0')
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i);
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			cmd[i] = ' ';
			if (cmd[i + 1] == '>' || cmd[i + 1] == '<')
				cmd[++i] = ' ';
			while (cmd[i] != '\0' && is_whitespace(cmd[i]) == 1)
				i++;
			while (cmd[i] != '\0' && is_whitespace(cmd[i]) == 0)
				cmd = erasing(cmd, &i);
		}
		if (cmd[i] == '\0')
			break ;
		i++;
	}
	return (cmd);
}
