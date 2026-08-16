#include "header.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	sim;
	pthread_t		monitor;
	int				i;

	if (parse_args(argc, argv, &config))
		return (1);
	if (init_simulation(&sim, &config))
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