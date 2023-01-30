/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:07:10 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/30 15:12:11 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	abort_pipe(t_minishell *elem, t_all *p)
{
	if (close(elem->fd[0]) == -1 || close(elem->fd[1]) == -1)
		end_process(p, 1);
	end_process(p, 1);
}

void	abort_pipe2(t_minishell *elem, t_all *p)
{
	if (close(elem->fd[0]) == -1 || close(elem->fd[1]) == -1
		|| close(elem->next->fd[0]) == -1 || close(elem->next->fd[1]) == -1)
		end_process(p, 1);
	end_process(p, 1);
}

void	abort_pipe3(t_minishell *elem, t_all *p)
{
	if (close(elem->fd[0]) == -1 || close(elem->next->fd[0]) == -1
		|| close(elem->next->fd[1]) == -1)
		end_process(p, 1);
	end_process(p, 1);
}

// int	end_pipe2(t_minishell *elem, t_all *p, t_tree *start)
// {
// 	if (close_all(elem->fd[0], elem->fd[1], NULL, elem->pid) == -1)
// 		return (create_signal(), init_signal(0), -1);
// 	elem = elem->next;
// 	if (elem->next->next == NULL)
// 		return (create_signal(), init_signal(0), last_pipe(elem, p, start));
// 	return (create_signal(), init_signal(0), mid_pipe(elem, p, start));
// }

// int	end_pipe3(t_minishell *elem)
// {
// 	int	status2;

// 	close(elem->fd[0]);
// 	close(elem->fd[1]);
// 	if (waitpid(elem->pid, NULL, 0) == -1)
// 		return (create_signal(), init_signal(0), -1);
// 	if (waitpid(elem->next->pid, &status2, 0) == -1
// 		|| ((WIFEXITED(status2)) && WEXITSTATUS(status2) != 0))
// 		return (create_signal(), init_signal(0), WEXITSTATUS(status2));
// 	return (create_signal(), init_signal(0), 0);
// }
