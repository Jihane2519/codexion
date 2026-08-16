/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbounou <jbounou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 21:29:03 by jbounou           #+#    #+#             */
/*   Updated: 2026/08/16 21:34:01 by jbounou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	insert_heap(t_dongle *dongle, t_edf info)
{
	t_edf	temp;
	int		i;
	int		parent;

	dongle->quee[dongle->size] = info;
	dongle->size++;
	i = dongle->size - 1;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (dongle->quee[i].deadline
			> dongle->quee[parent].deadline)
			break ;
		if (dongle->quee[i].deadline == dongle->quee[parent].deadline
			&& dongle->quee[i].id > dongle->quee[parent].id)
			break ;
		temp = dongle->quee[i];
		dongle->quee[i] = dongle->quee[parent];
		dongle->quee[parent] = temp;
		i = parent;
	}
}

static int	get_smallest(t_dongle *dongle, int left, int right)
{
	if (right < dongle->size
		&& (dongle->quee[right].deadline
			< dongle->quee[left].deadline
			|| (dongle->quee[right].deadline
				== dongle->quee[left].deadline
				&& dongle->quee[right].id
				< dongle->quee[left].id)))
		return (right);
	return (left);
}

static int	is_parent_smaller(t_dongle *dongle, int index, int smallest)
{
	if (dongle->quee[index].deadline
		< dongle->quee[smallest].deadline)
		return (1);
	if (dongle->quee[index].deadline
		== dongle->quee[smallest].deadline
		&& dongle->quee[index].id
		< dongle->quee[smallest].id)
		return (1);
	return (0);
}

void	insert_down(t_dongle *dongle)
{
	int		index;
	int		left;
	int		right;
	int		smallest;
	t_edf	temp;

	index = 0;
	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		if (left >= dongle->size)
			break ;
		smallest = get_smallest(dongle, left, right);
		if (is_parent_smaller(dongle, index, smallest))
			break ;
		temp = dongle->quee[index];
		dongle->quee[index] = dongle->quee[smallest];
		dongle->quee[smallest] = temp;
		index = smallest;
	}
}

t_edf	pop_heap(t_dongle *dongle)
{
	t_edf	result;

	result = dongle->quee[0];
	dongle->quee[0] = dongle->quee[dongle->size - 1];
	dongle->size--;
	insert_down(dongle);
	return (result);
}
