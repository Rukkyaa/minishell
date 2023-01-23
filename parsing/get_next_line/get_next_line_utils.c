/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 12:30:48 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/23 12:06:03 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*gen_answer(char *new_reserve)
{
	int		i;
	int		a;
	char	*answer;

	i = 0;
	a = -1;
	while (new_reserve[i] != '\0' && new_reserve[i] != '\n')
		i++;
	if (new_reserve[i] == '\n')
		i++;
	answer = malloc((i + 1) * sizeof(char));
	if (!answer)
		return (NULL);
	while (++a < i)
		answer[a] = new_reserve[a];
	answer[a] = '\0';
	return (answer);
}

char	*error_case(char *buf, char *reserve, int p)
{
	if (buf)
		free(buf);
	if (p == 0)
		return (reserve);
	if (reserve && reserve != NULL)
	{
		free(reserve);
		reserve = NULL;
	}
	return (NULL);
}
