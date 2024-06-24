#include "philo.h"

void clean_up(t_data *data)
{
    int i;
    i = 0;

    while (i < data->n_philos)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->print_lock);
    pthread_mutex_destroy(&data->dead_lock);
    free(data->forks);
    free(data->philos);
    free(data);

}
int main(int ac, char **av)
{
    t_data *data;
    init_data(&data, ac, av);
    init_philos(data);

    data->start_time = get_time();

    pthread_t controller_th;
    pthread_create(&controller_th, NULL, controller_routine, data);

    int i = 0;
    while (i < data->n_philos)
    {
        pthread_create(&data->philos[i].thread, NULL, ft_routine, &data->philos[i]);
        i++;
    }
    i = 0;
    while (i < data->n_philos)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
    }
    pthread_join(controller_th, NULL);
    clean_up(data);

}
