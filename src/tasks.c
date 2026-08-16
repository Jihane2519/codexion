/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbounou <jbounou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 21:08:12 by jbounou           #+#    #+#             */
/*   Updated: 2026/08/16 23:53:15 by jbounou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		if (take_dongle(coder, coder->left_dongle))
			return (1);
		if (take_dongle(coder, coder->right_dongle))
		{
			release_dongle(coder->left_dongle);
			return (1);
		}
	}
	else
	{
		if (take_dongle(coder, coder->right_dongle))
			return (1);
		if (take_dongle(coder, coder->left_dongle))
		{
			release_dongle(coder->right_dongle);
			return (1);
		}
	}
	return (0);
}

int	compile(t_coder *coder)
{
	long	compile_start;

	if (take_dongles(coder))
		return (1);
	compile_start = get_time_ms();
	pthread_mutex_lock(&coder->sim->pause_print);
	printf("%ld %d is compiling\n",
		compile_start - coder->sim->start_time, coder->id);
	pthread_mutex_unlock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	coder->last_compile = compile_start;
	coder->nbr_of_compilations++;
	pthread_mutex_unlock(&coder->sim->pause);
	coder_sleep(coder, coder->sim->config.time_to_compile);
	release_dongle(coder->left_dongle);
	release_dongle(coder->right_dongle);
	return (0);
}

void	debug(t_coder *coder)
{
	long	debug_start;

	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	debug_start = get_time_ms();
	pthread_mutex_lock(&coder->sim->pause_print);
	printf("%ld %d is debugging\n",
		debug_start - coder->sim->start_time, coder->id);
	pthread_mutex_unlock(&coder->sim->pause_print);
	coder_sleep(coder, coder->sim->config.time_to_debug);
}

void	refactor(t_coder *coder)
{
	long	refactor_start;

	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	refactor_start = get_time_ms();
	pthread_mutex_lock(&coder->sim->pause_print);
	printf("%ld %d is refactoring\n",
		refactor_start - coder->sim->start_time, coder->id);
	pthread_mutex_unlock(&coder->sim->pause_print);
	coder_sleep(coder, coder->sim->config.time_to_refactor);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->nbr_of_compilations
		== coder->sim->config.nbr_of_compiles_required)
		coder->done = 1;
	pthread_mutex_unlock(&coder->sim->pause);
}
