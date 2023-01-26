/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9-redirebis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 15:33:05 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/26 15:35:32 by axlamber         ###   ########.fr       */
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
