/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 18:12:28 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/28 12:24:28 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//char	*clean_up(char *line, int dep, int length)
//{
//	char *text;
//	int i;

//	i = dep;
//	if (length + dep > ft_strlen(line))
//		return (NULL);
//	while (is_whitespace(line[i]) == 1 || line[i] == '<' || line[i] == '>')
//	{
//		i++;
//		length++;
//	}
//	text = malloc((ft_strlen(line) - length + 1) * sizeof(char));
//	if (!text)
//		return (NULL);
//	i = -1;
//	while (++i < dep)
//		text[i] = line[i];
//	while (line[i + length] != '\0' && line[i + length] != '|') //condition a surveiller pour les pipes multiples
//	{
//		text[i] = line[i + length];
//		i++;
//	}
//	return (text[i] = '\0', free(line), text);
//}

// char *extrac_cmd(char *line, char **redir)
// {
// 	int i;
// 	char *cmd;

// 	if (!line)
// 		return (NULL);
// 	cmd = ft_strdup(line);
// 	if (redir[0] && ft_strlen(redir[0]) != 0)
// 	{
// 		i = 0;
// 		while (cmd[i] != '<' && cmd[i+1] != '<')
// 			i++;
// 		cmd = clean_up(cmd, i, ft_strlen(redir[0]));
// 	}
// 	if (redir[1] && ft_strlen(redir[1]) != 0)
// 	{
// 		i = 0;
// 		while (cmd[i] != '>' && cmd[i+1] != '>')
// 			i++;
// 		cmd = clean_up(cmd, i, ft_strlen(redir[1]));
// 	}
// 	if (redir[2] && ft_strlen(redir[2]) != 0)
// 	{
// 		i = 0;
// 		while (cmd[i] != '>' && cmd[i+1] == '>')
// 			i++;
// 		cmd = clean_up(cmd, i, ft_strlen(redir[2]));
// 	}
// 	return (cmd);
// }

//t_minishell	*gen_maillon(char *line)
//{
//	t_minishell *elem;

//	elem = malloc(sizeof(t_minishell));
//	if (!elem)
//		return (NULL);
//	if (get_redirection(line, elem) == -1)
//		return (free(elem), NULL);
//	//elem->cmd = ft_split(extrac_cmd(line, elem->redirections), ' ');
//	//if (!elem->cmd)
//	//	return (free_tab(elem->redirections), free(elem), NULL);
//	return (elem);
//}
