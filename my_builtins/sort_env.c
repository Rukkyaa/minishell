/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:54:40 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/13 14:40:14 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static unsigned int	size_env(char **env)
{
	unsigned int	len;

	len  = -1;
	while (env[++len])
		;
	return (len);
}

char	**env_to_char_export(t_env *env)
{
	char **reforged;
	int i;
	int e;
	t_env *temp;

	i = 0;
	e = 0;
	if (!env || env == NULL)
		return (NULL);
	temp = env;
	while (temp != NULL && i++ > -1)
		temp = temp->next;
	reforged = malloc((i + 1) * sizeof(char *));
	if (!reforged)
		return (NULL);
	while (e < i)
	{
		reforged[e] = ft_strjoin(env->key, "=\"");
		reforged[e] = ft_strjoin_spe(reforged[e], env->value);
		reforged[e] = ft_strjoin_spe(reforged[e], "\"");
		if (reforged[e++] == NULL)
			return (free_tab(reforged), NULL);
		env = env->next;
	}
	return (reforged[e] = NULL, reforged);
}

char	**sort_env(char **env)
{
	unsigned const int	size = size_env(env);
	unsigned int		i;
	unsigned int		j;
	char				*tmp;
	
	i = -1;
	while (++i < size - 1)
	{
		j = i;
		while (++j < size)
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
		}
	}
	i = -1;
	while (env[++i])
		printf("declare -x %s\n", env[i]);
	// printf("Size : %d\n", size);
	return (free_tab(env), NULL);
}