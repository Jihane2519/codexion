/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbounou <jbounou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 21:56:10 by jbounou           #+#    #+#             */
/*   Updated: 2026/08/16 22:00:20 by jbounou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	sim;
	pthread_t		monitor;
	int				i;

	if (parse_args(argc, argv, &config)
		|| init_simulation(&sim, &config))
		return (1);
	pthread_create(&monitor, NULL, monitor_routine, &sim);
	i = 0;
	while (i < config.nbr_of_coders)
	{
		pthread_create(&sim.coders[i].thread, NULL,
			coder_routine, &sim.coders[i]);
		i++;
	}
	i = 0;
	while (i < config.nbr_of_coders)
	{
		pthread_join(sim.coders[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	cleanup_simulation(&sim);
	return (0);
}
