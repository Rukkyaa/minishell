/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-heredocs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 13:15:43 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/24 00:50:27 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (0);
	else if ((!s1 && s2) || (s1 && !s2))
		return (-1);
	while (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	heredoc_count(char *line, int index)
{
	int	i;
	int	compt;

	i = index;
	compt = 0;
	while (line[i] != '\0' && line[i + 1] != '\0' && line[i + 2] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
		if (i == ft_strlen(line))
			break ;
		if (line[i] != '\0' && line[i] == '<' && line[i + 1] != '\0'
			&& line[i + 1] == '<' && line[i + 2] != '\0'
			&& line[i + 2] != '<')
			compt++;
		i++;
	}
	return (compt);
}

char	*generate_name(char *limiter)
{
	char	*title;

	if (ft_strlen(limiter) == 0)
	{
		title = malloc(2);
		if (!title)
			return (free(limiter), NULL);
		title[0] = 'a';
		title[1] = '\0';
	}
	else
		title = ft_strdup(limiter);
	if (title == NULL)
		return (free(limiter), NULL);
	while (access(title, F_OK) == 0)
	{
		title = ft_strjoin_spe(title, "p");
		if (title == NULL)
			return (free(limiter), NULL);
	}
	return (free(limiter), title);
}

char	*find_lim(char *line, int *alert)
{
	int		i;
	int		compt;
	int		d;
	char	*limiter;

	i = 0;
	compt = 0;
	while (line[i] != '\0' && line[i + 1] != '\0' && !(line[i] == '<' && line[i + 1] == '<'))
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
		else
			i++;
	}
	i += 2;
	while (line[i] != '\0' && is_whitespace(line[i]) == 1)
		i++;
	d = i;
	*alert = i;
	while (line[d] != '\0' && is_whitespace(line[d]) == 0) // peut aussi etre un pipe ou un op logique ?
	{
		compt++;
		d++;
	}
	limiter = malloc((compt + 1) * sizeof(char));
	if (!limiter)
		return (NULL);
	compt = 0;
	while (line[i] != '\0' && i < d)
		limiter[compt++] = line[i++];
	return (limiter[compt] = '\0', ft_trim_quotes(limiter, alert));
}

void	alert_case(char *str);

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
		reste = ft_substr(line, *alert + 2, ft_strlen(line)); //verifier si il n'y a pas plus de cas particuliers style <<"''"
	else
		reste = ft_substr(line, *alert + ft_strlen(LIM), ft_strlen(line));
	//if (!reste || reste == NULL)
	//	return (free(LIM), free(line), free(new_line), NULL);
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

int	fill_file(t_all *p, char **line, int max, int nb)
{
	int		fd;
	char	*newlimiter;
	char	*lect;
	int		alert;
	int		prev;

	alert = 0;
	if (nb == max)
		return (p->here_docs[nb] = NULL, 1);
	p->here_docs[nb] = generate_name(find_lim(*line, &alert));
	if (alert == -1 || p->here_docs[nb] == NULL)
		return (alert_case(p->here_docs[nb]), p->here_docs[nb] = NULL, -1);
	fd = open(p->here_docs[nb], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (free(p->here_docs[nb]), p->here_docs[nb] = NULL, -1);
	newlimiter = gen_new_limiter(find_lim(*line, &alert));
	if (newlimiter == NULL || alert == -1)
		return (alert_case(newlimiter), free(p->here_docs[nb]), p->here_docs[nb] = NULL, -1);
	stop_signals();
	create_signal_here();
	g_sig.p_status = 2;
	lect = replace_var(get_next_line(0), p);
	if (lect != NULL && ft_strlen(lect) >= 1 && lect[ft_strlen(lect) - 1] == '\n')
		prev = 1;
	else 
		prev = 0;
	while (ft_strcmp(lect, newlimiter) != 0 && g_sig.sig_int == 0) // cas ou limiter == NULL
	{
		if (lect != NULL)
			write(fd, lect, ft_strlen(lect));
		if (g_sig.sig_int == 0)
		{
			if (lect != NULL)
				free(lect); //reactiver pour  limiter les leaks, pb avec par exemple deux cat<<"" a la suite
			lect = replace_var(get_next_line(0), p);
		}
		if (prev == 1 && lect == NULL)
		{
			printf("warning: here-document at line delimited by end-of-file (wanted `%s')\n", newlimiter); //reduce the last char
			break;
		}
		if (lect != NULL && ft_strlen(lect) >= 1 && lect[ft_strlen(lect) - 1] == '\n')
			prev = 1;
		else 
			prev = 0;
	}
	g_sig.p_status = 1;
	stop_signals();
	create_signal();
	init_signal(0);
	if (lect != NULL) //CHECK LEAKS
		free(lect);
	if (nb + 1 == max) //&& g_sig.sig_quit == 0 && g_sig.sig_int == 0) //GROS CHECK DE LEAKS A FAIRE SUITE A INTEGRATION DES SIGNAUX
		lect = get_next_line(-42);
	*line = clean_heredoc_line(*line, p->here_docs[nb], find_lim(*line, &alert), &alert);
	if (*line == NULL)
		return (free(newlimiter), close (fd), free(p->here_docs[nb]), p->here_docs[nb] = NULL, -1);
	return (free(newlimiter), close (fd), fill_file(p, line, max, ++nb));
}

//regler le sujet si il n'y a pas d'espace entre le limiter et <<
//sujet quand il y en a plus de 1 

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
