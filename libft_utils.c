/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:28:50 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/07 17:41:33 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int is_whitespace(char c)
{
	if (c == 32 || (c>= 10 && c<=14))
		return (1);
	return (0);
}

int	ft_isprint(int c)
{
	if (c > 32 && c <= 126) //j'exclue le espace, pt a revoir dans les cas des quotes
		return (1);
	return (0);
}

char	*ft_strcat(char *s1, char *s2)
{
	int	index1;
	int	index2;

	index1 = 0;
	index2 = 0;
	while (s1[index1] != '\0')
		index1++;
	while (s2[index2] != '\0')
		s1[index1++] = s2[index2++];
	s1[index1] = '\0';
	return (s1);
}

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	if (!dst)
		return (NULL);
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strdup(const char *s1)
{
	char	*p;

	if (!s1)
		return (NULL);
	p = (char *) malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (!(p))
		return (NULL);
	return (ft_strcpy(p, s1));
}

char	*ft_strjoin_spe(char *s1, char *s2)
{
	char	*str;
	int		i;

	i = -1;
	if (!s1 || s1 == NULL)
		return (s2);
	str = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!(str))
		return (free(s1), NULL);
	while (s1[++i] != '\0')
		str[i] = s1[i];
	str[i] = '\0';
	str = ft_strcat(str, s2);
	return (free(s1), str);
}

