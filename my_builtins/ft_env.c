/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 23:35:36 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/23 10:48:28 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(t_env *envbis)
{
	t_env	*env;

	env = envbis;
	while (env)
	{
		if (env->code != 3 && env->code != 4)
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
