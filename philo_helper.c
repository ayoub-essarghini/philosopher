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
    pthread_mutex_lock(&philosopher->params->ayoub);
    if (philosopher->params->running == 1)
    {
        printf("%ld %d %s\n", get_time() - philosopher->params->start_time, philosopher->id, status);
    }
    pthread_mutex_unlock(&philosopher->params->ayoub);
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
    pthread_mutex_init(&(*data)->ayoub, NULL);
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
        write(STDERR_FILENO, "Error\n", 6);
        exit(1);
    }
    (*data)->n_philos = ft_atoi(av[1]);
    if ((*data)->n_philos <= 0)
    {
        write(STDERR_FILENO, "Error\n", 6);
        exit(1);
    }

    (*data)->t_die = ft_atol(av[2]);
    (*data)->t_eat = ft_atol(av[3]);
    (*data)->t_sleep = ft_atol(av[4]);
    if (ac == 6)
        (*data)->n_meals = ft_atoi(av[5]);
    else
        (*data)->n_meals = -1;

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
