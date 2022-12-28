/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 12:30:48 by gduhau            #+#    #+#             */
/*   Updated: 2022/12/27 14:52:08 by gabrielduha      ###   ########.fr       */
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
	if (reserve)
		free(reserve);
	return (NULL);
}
