/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7-print.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:24:51 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/28 12:46:32 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_tree(t_tree *start, int i)
{
	printf("B%d       %s\n", i, start->cmd);
	if (start->and != NULL)
	{
		printf("AND\n");
		print_tree(start->and, ++i);
	}
	if (start->or != NULL)
	{
		printf("OR\n");
		print_tree(start->or, ++i);
	}
}

void print_pipe(t_minishell *elem, int i)
{
	int d;

	d = 0;
	printf("\n	|PIPE %d\n", i);
	while (elem->cmd[d] != NULL)
	{
		printf("	|cmd[%d]: %s\n", d, elem->cmd[d]);
		d++;
	}
	printf("	|Infile : %s\n", elem->infile);
	printf("	|Outfile : %s\n\n", elem->outfile);
	if (elem->next != NULL)
		print_pipe(elem->next, ++i);
}

void print_cmd(t_tree *start, int i)
{
	printf("	|OPERATION N^%d---------- (%s)\n", i, start->cmd);
	print_pipe(start->first_elem, 1);
	printf("	------------------------ \n\n");
	if (start->and != NULL)
	{
		printf("AND\n");
		print_cmd(start->and, ++i);
	}
	if (start->or != NULL)
	{
		printf("OR\n");
		print_cmd(start->or, ++i);
	}
}

void print_here_doc(char **here_docs)
{
	int i;

	i = 0;
	if (here_docs == NULL)
		return ;
	printf("-------HERE_DOC-------\n");
	while (here_docs[i] != NULL)
	{
		printf("here_doc[%d] = %s\n", i, here_docs[i]);
		i++;
	}
	printf("here_doc[%d] = %s\n", i, here_docs[i]);
	printf("----------------------\n");
}
