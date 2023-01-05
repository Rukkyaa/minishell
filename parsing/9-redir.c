/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9-redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:32:08 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/05 15:09:48 by gduhau           ###   ########.fr       */
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
			maillon->infile = ft_trim(get_filename(cmd, i + 1));
			if (maillon->infile == NULL)
				return (-1);
		}
		else if (cmd[i] != '>' && cmd[i + 1] == '>' && cmd[i + 2] != '>') //NB: il peut y avoir plusieurs fois la meme redirection
		{
			maillon->outfile = ft_trim(get_filename(cmd, i + 2));
			if (maillon->outfile == NULL)
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
			maillon->outfile = ft_trim(get_filename(cmd, i + 3));
			maillon->append = 1;
			if (maillon->outfile == NULL)
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
	while (cmd[i + 1] != '\0') //il y a sans doute une plus opti a faire
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
		i++;
	}
	return (cmd);
}