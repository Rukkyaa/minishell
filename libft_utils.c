/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:28:50 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/19 13:29:19 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
