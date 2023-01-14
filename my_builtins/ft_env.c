/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:35:36 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/14 00:38:50 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->code != 3)
		{
			ft_putstr_fd(env->key, 1);
			ft_putstr_fd("=", 1);
			if (env->code == 1)
				ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
