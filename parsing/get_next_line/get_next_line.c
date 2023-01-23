/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 12:07:10 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/23 13:35:10 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	return_exist(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '\0' && i < (int)ft_strlen(s))
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strcatspe(char *s1, char *s2)
{
	int		index1;
	int		index2;
	char	*s3;

	if (s1 == NULL)
		return (ft_strcatspe(s2, "\0"));
	s3 = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!s3)
		return (NULL);
	index1 = -1;
	index2 = 0;
	while (s1[++index1] != '\0')
		s3[index1] = s1[index1];
	while (s2[index2] != '\0')
		s3[index1++] = s2[index2++];
	s3[index1] = '\0';
	return (s3);
}

char	*maj_reserve(int fd, char *reserve)
{
	char	*buf;
	char	*new_reserve;
	int		p;

	if (return_exist(reserve) == 1)
		return (reserve);
	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	p = read(fd, buf, (size_t)BUFFER_SIZE);
	if (p == 0 && ft_strlen(reserve) == 0)
		return (error_case(buf, reserve, 1));
	else if (p == 0 || p == -1)
		return (error_case(buf, reserve, p));
	buf[p] = '\0';
	new_reserve = ft_strcatspe(reserve, buf);
	if (!new_reserve)
		return (NULL);
	if (reserve)
		free(reserve);
	if (buf)
		free(buf);
	return (maj_reserve(fd, new_reserve));
}

char	*clean_reserve(int i, char *reserve)
{
	char	*reserve_cleaned;
	int		a;

	a = 0;
	if (!reserve || ft_strlen(reserve) - i < 1)
	{
		if (reserve)
			free(reserve);
		return (NULL);
	}
	reserve_cleaned = malloc((ft_strlen(reserve) - i) * sizeof(char));
	if (!reserve_cleaned)
	{
		if (reserve)
			free(reserve);
		return (NULL);
	}
	while (reserve[++i] != '\0')
		reserve_cleaned[a++] = reserve[i];
	reserve_cleaned[a] = '\0';
	if (reserve)
		free(reserve);
	return (reserve_cleaned);
}

char	*get_next_line(int fd)
{
	static char	*reserve;
	char		*new_reserve;
	char		*answer;

	printf("toto\n");
	if (fd == -42)
	{
		if (reserve && reserve != NULL)
			free(reserve);
		reserve = NULL;
		return (NULL);
	}
	if (!reserve)
		reserve = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	new_reserve = maj_reserve(fd, reserve);
	if (!new_reserve)
		return (NULL);
	answer = gen_answer(new_reserve);
	if (!answer)
		return (NULL);
	reserve = clean_reserve(ft_strlen(answer) - 1, new_reserve);
	if (!reserve)
		return (NULL);
	return (answer);
}
/*#include <stdio.h>
#include <fcntl.h>
int main()
{
	char *line;
	int i = 1;
	int fd;
	
	fd = open("Test_files/long2k.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("ligne %d: %s\n", i++, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);	
}*/
