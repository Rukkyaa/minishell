/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9-redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:32:08 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/10 13:35:51 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void clean_rest(t_tree *start, int i, int end)
{
	while (i <= end)
		start->cmd[i++] = ' ';
}


char *get_filename(char *line, int i) //attention a gerer les quotes
{
	char *text;
	int a;
	int len;

	len = 0;
	while(is_whitespace(line[i]) == 1)
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
	t_infile *p;
	t_infile *new_elem;
	t_infile *p_bis;

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
	new_elem->next = NULL;
	return (p->next = new_elem, p_bis);
}


t_outfile	*add_file_out(t_outfile *lst, char *file, int opt)
{
	t_outfile *p;
	t_outfile *new_elem;
	t_outfile *p_bis;

	if (lst == NULL)
	{
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
		if (access(file, F_OK) != 0)
			p->created = 1;
		else
			p->created = 0;
		return (p);
	}
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

int check_redirection(char *cmd, t_minishell *maillon) //fonction bancale (risque de invalid read)
{
	int i;

	i = 0;
	while (i + 2 < ft_strlen(cmd)) //ft_strlen(cmd) > 3 && cmd[i + 2] != '\0') //il y a sans doute une plus opti a faire
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i) - 1; //MEGA BANCAL
		else if ((i == 0 && cmd[i] == '<' && cmd[i + 1] != '<') || (i > 0 && cmd[i - 1] != '<' && cmd[i] == '<' && cmd[i + 2] != '<')) //NB: il peut y avoir plusieurs fois la meme redirection
		{
			maillon->file_in = add_file_in(maillon->file_in, ft_trim(get_filename(cmd, i + 1)));
			if (maillon->file_in == NULL)
				return (-1);
		}
		else if (cmd[i] != '>' && cmd[i + 1] == '>' && cmd[i + 2] != '>') //NB: il peut y avoir plusieurs fois la meme redirection
		{
			maillon->file_out = add_file_out(maillon->file_out, ft_trim(get_filename(cmd, i + 2)), 0);
			if (maillon->file_out == NULL)
				return (-1);
		}
		i++;
	}
	i = 0;
	while (i + 3 < ft_strlen(cmd)) //ft_strlen(cmd) > 4 && cmd[i + 3] != '\0')
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			i = avoid_quotes(cmd, i) - 1; //MEGA BANCAL
		else if (cmd[i] != '>' && cmd[i + 1] == '>' && cmd[i + 2] == '>' && cmd[i + 3] != '>') //NB: il peut y avoir plusieurs fois la meme redirection
		{
			maillon->file_out = add_file_out(maillon->file_out, ft_trim(get_filename(cmd, i + 3)), 1);
			if (maillon->file_out == NULL)
				return (-1);
		}
		i++;
	}
	return (1);
}

char *erase_redir(char *cmd)
{
	int i;
	int end;

	i = 0;
	while (cmd[i] != '\0' && cmd[i + 1] != '\0') //il y a sans doute une plus opti a faire
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
			{
				if (cmd[i] == '\"' || cmd[i] == '\'')
				{
					end = avoid_quotes(cmd, i);
					while (i < end)
						cmd[i++] = ' ';
				}
				else
					cmd[i++] = ' ';
			}
		}
		if (cmd[i] == '\0')
			break;
		i++;
	}
	return (cmd);
}