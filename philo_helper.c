#include "philo.h"

void init_philos(t_data *data)
{
    int i = 0;
    while (i < data->n_philos)
    {
        data->philos[i].id = i + 1;
        data->philos[i].last_meal = get_time();
        data->philos[i].meals_eaten = 0;
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
        data->philos[i].params = data;
        i++;
    }
}

void print_status(t_philo *philosopher, const char *status)
{
    pthread_mutex_lock(&philosopher->params->dead_lock);
    if (philosopher->params->running == 1)
    {
        pthread_mutex_lock(&philosopher->params->print_lock);
        printf("%ld %d %s\n", get_time() - philosopher->params->start_time, philosopher->id, status);
        pthread_mutex_unlock(&philosopher->params->print_lock);
    }
    pthread_mutex_unlock(&philosopher->params->dead_lock);
}

void init_mutex(t_data **data)
{
    int i = 0;
    while (i < (*data)->n_philos)
    {
        pthread_mutex_init(&(*data)->forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&(*data)->print_lock, NULL);
    pthread_mutex_init(&(*data)->dead_lock, NULL);
    (*data)->philos = (t_philo *)malloc(sizeof(t_philo) * (*data)->n_philos);
    if ((*data)->philos == NULL)
    {
        perror("MEMORY philo");
        exit(1);
    }
}

void init_data(t_data **data, int ac, char **av)
{
    check_valid_args(ac);
    *data = (t_data *)malloc(sizeof(t_data));
    if (*data == NULL)
    {
        perror("MEMORY");
        exit(1);
    }
    (*data)->n_philos = ft_atoi(av[1]);
    (*data)->t_die = ft_atol(av[2]);
    (*data)->t_eat = ft_atol(av[3]);
    (*data)->t_sleep = ft_atol(av[4]);
    (*data)->n_meals = (ac == 6) ? ft_atoi(av[5]) : -1;
    (*data)->running = 1;
    (*data)->n_finished_meals = 0;
    (*data)->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (*data)->n_philos);
    if ((*data)->forks == NULL)
    {
        perror("MEMORY");
        exit(1);
    }
    init_mutex(data);
}
