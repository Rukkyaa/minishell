/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-heredocs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 13:15:43 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/28 12:22:28 by gabrielduha      ###   ########.fr       */
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

int heredoc_count(char *line, int index)
{
	int i;
	int compt;

	i = index;
	compt = 0;
	while (line[i + 2] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
		if (line[i] == '<' && line[i + 1] == '<' && line[i + 2] != '<')
			compt++;
		i++;
	}
	return (compt);
}

//char *find_last_limiter(char *line)
//{
//	int i;
//	int compt;
//	int d;
//	char *limiter;

//	compt = 0;
//	i = ft_strlen(line) - 2;
//	while (i > 0 && line[i] != '<' && line[i - 1] != '<')
//		i--;
//	i += 2;
//	while (is_whitespace(line[i]) == 1)
//		i++;
//	d = i;
//	while (is_whitespace(line[d++]) == 0)
//		compt++;
//	limiter = malloc((compt + 1) * sizeof(char));
//	if (!limiter)
//		return (NULL);
//	compt = 0;
//	while (i < d)
//		limiter[compt++] = line[i++];
//	return(limiter[compt] = '\0', limiter);
//}

//char *get_text(char *last_limiter)
//{
//	char *text;
//	char *lect;

//	if (last_limiter == NULL || !last_limiter)
//		return (NULL);
//	text = malloc(1);
//	if (!text)
//		return (NULL);
//	text[0] = '\0';
//	lect = get_next_line(0);
//	while (ft_strcmp(lect, last_limiter) != 0)
//	{
//		text = ft_strjoin_spe(text, lect);
//		free(lect);
//		lect = get_next_line(0);
//	}
//	free(lect);
//	lect = get_next_line(-42);
//	return (free(last_limiter), text);
//}

char	*generate_name(char *limiter)
{
	char	*title;

	if (ft_strlen(limiter) == 0)
	{
		title = malloc(2);
		if (!title)
			return (NULL);
		title[0] = 'a';
		title[1] = '\0';
	}
	else
		title = ft_strdup(limiter);
	if (title == NULL)
		return (NULL);
	while (access(title, F_OK) == 0)
	{
		title = ft_strjoin_spe(title, "p");
		if (title == NULL)
			return (NULL);
	}
	return (free(limiter), title);
}

char *find_lim(char *line)
{
	int i;
	int compt;
	int d;
	char *limiter;

	i = 0;
	compt = 0;
	while (line[i + 1] != '\0' && !(line[i] == '<' && line[i + 1] == '<'))
		i++;
	i += 2;
	while (is_whitespace(line[i]) == 1)
		i++;
	d = i;
	while (is_whitespace(line[d]) == 0)
	{
		d++;
		compt++;
	}
	limiter = malloc((compt + 1) * sizeof(char));
	if (!limiter)
		return (NULL);
	compt = 0;
	while (i < d)
		limiter[compt++] = line[i++];
	return(limiter[compt] = '\0', limiter);
}

char	*gen_new_limiter(char *limiter)
{
	char	*newlimiter;

	if (!limiter || limiter == NULL)
		return (NULL);
	newlimiter = malloc((ft_strlen(limiter) + 2) * sizeof(char));
	if (!newlimiter)
		return (NULL);
	if (ft_strlen(limiter) != 0)
		newlimiter = ft_strcpy(newlimiter, limiter);
	newlimiter[ft_strlen(limiter)] = '\n';
	newlimiter[ft_strlen(limiter) + 1] = '\0';
	return (free(limiter), newlimiter);
}

//cas d'erreur a traiter si le malloc de find lim echoue

char *clean_heredoc_line(char *line, char *filename)
{
	char *limiter;
	int i;
	int length;
	int length2;
	char *new_line;

	limiter = find_lim(line);
	length = 0;
	length2 = 0;
	i = 0;
	while (line[i + 1] != '\0' && !(line[i] == '<' && line[i + 1] == '<'))
	{
		length++;
		i++;
	}
	i += 2;
	while (is_whitespace(line[i]) == 1 && line[i] != '\0')
		i++;
	i += ft_strlen(limiter);
	while (i < ft_strlen(line) && line[i++] != '\0')
		length2++;
	new_line = malloc((++length + length2 + ft_strlen(filename) + 1) * sizeof(char));
	if (!new_line)
		return (free(limiter), NULL);
	i = -1;
	while (++i < length)
		new_line[i] = line[i];
	new_line[i] = '\0';
	new_line = ft_strcat(new_line, filename);
	length = i + ft_strlen(filename);
	i += 2;
	while (is_whitespace(line[i]) == 1)
		i++;
	i += ft_strlen(limiter);
	while (line[i] != '\0')
		new_line[length++] = line[i++];
	return (free(limiter), new_line);
}

int fill_file(char **here_docs, char **line, int max, int nb) //gerer les cas avec quotes
{
	int fd;
	char *newlimiter;
	char *lect;

	if (nb == max)
		return (here_docs[nb] = NULL, 1);
	here_docs[nb] = generate_name(find_lim(*line));
	if (here_docs[nb] == NULL)
		return (-1);
	fd = open(here_docs[nb], O_CREAT | O_RDWR | O_EXCL);
	if (fd == -1)
		return (free(here_docs[nb]), -1);
	newlimiter = gen_new_limiter(find_lim(*line));
	lect = get_next_line(0);
	while (ft_strcmp(lect, newlimiter) != 0)
	{
		write(fd, lect, ft_strlen(lect));
		free(lect);
		lect = get_next_line(0);
	}
	free(lect);
	lect = get_next_line(-42);
	*line = clean_heredoc_line(*line, here_docs[nb]);
	if (line == NULL)
		return (free(newlimiter), close (fd), free(here_docs[nb]), -1);
	return (free(newlimiter), close (fd), fill_file(here_docs, line, max, ++nb));
}

//regler le sujet si il n'y a pas d'espace entre le limiter et <<
//sujet quand il y en a plus de 1 

char **get_here_docs(char **line)
{
	char **here_docs;

	if (heredoc_count(*line, 0) == 0 || ft_strlen(*line) < 3)
		return (NULL);
	here_docs = malloc((heredoc_count(*line, 0) + 1) * sizeof(char *));
	if (!here_docs)
		return (NULL);
	//text = get_text(find_last_limiter(line));
	if (fill_file(here_docs, line, heredoc_count(*line, 0), 0) == -1)
		return (NULL); //clean all
	return (here_docs);
}