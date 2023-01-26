/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-heredocs3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 21:58:30 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/26 21:58:56 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*gen_new_limiter(char *limiter)
{
	char	*newlimiter;

	newlimiter = malloc((ft_strlen(limiter) + 2) * sizeof(char));
	if (!newlimiter)
		return (alert_case(limiter), NULL);
	if (ft_strlen(limiter) != 0)
		newlimiter = ft_strcpy(newlimiter, limiter);
	newlimiter[ft_strlen(limiter)] = '\n';
	newlimiter[ft_strlen(limiter) + 1] = '\0';
	return (alert_case(limiter), newlimiter);
}

char	*clean_heredoc_line(char *line, char *filename, char *LIM, int *alert)
{
	int		i;
	char	*new_line;
	char	*reste;

	if (*alert == -1)
		return (free(line), NULL);
	new_line = ft_substr(line, 0, *alert);
	if (!new_line || new_line == NULL)
		return (alert_case(LIM), free(line), NULL);
	i = ft_strlen(new_line) - 1;
	while (i >= 0 && is_whitespace(new_line[i]) == 1)
		i--;
	new_line[i] = ' ';
	new_line = ft_strjoin_spe(new_line, filename);
	if (new_line == NULL || !new_line)
		return (alert_case(LIM), free(line), NULL);
	if (ft_strlen(LIM) == 0)
		reste = ft_substr(line, *alert + 2, ft_strlen(line));
	else
		reste = ft_substr(line, *alert + ft_strlen(LIM), ft_strlen(line));
	new_line = ft_strjoin_spe(new_line, reste);
	if (!new_line || new_line == NULL)
		return (alert_case(LIM), free(line), alert_case(reste), NULL);
	return (alert_case(LIM), free(line), alert_case(reste), new_line);
}

void	alert_case(char *str)
{
	if (str != NULL)
		free(str);
	return ;
}

void	warning(char *str, int nb)
{
	int	i;

	i = 0;
	printf(
		"warning: here-document at line %d delimited by end-of-file (wanted `",
		nb);
	if (ft_strlen(str) == 0 || (ft_strlen(str) == 1 && str[0] == '\n'))
		printf("')\n");
	else
	{
		while (i < ft_strlen(str) - 1)
			printf("%c", str[i++]);
		printf("')\n");
	}
}

int	signals_hdoc(int opt)
{
	if (opt == 0)
	{
		if (stop_signals() == 1 || create_signal_here() == -1)
			return (-1);
		return (g_sig.p_status = 2, 0);
	}
	else if (opt == 1)
	{
		g_sig.p_status = 1;
		if (stop_signals() == 1 || create_signal() == -1)
			return (-1);
		if (g_sig.sig_int != 1)
			init_signal(0);
		return (0);
	}
	return (-1);
}
