#include "philo.h"

long get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);

}

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

void init_data(t_data **data, int ac, char **av)
{
    if (ac != 5 && ac != 6)
    {
        write(2, "ERROR\n", 6);
        exit(1);
    }
    *data = (t_data *)malloc(sizeof(t_data));
    if (*data == NULL)
    {
        perror("MEMORY");
        exit(1);
    }
    (*data)->n_philos = atoi(av[1]);
    (*data)->t_die = atol(av[2]);
    (*data)->t_eat = atol(av[3]);
    (*data)->t_sleep = atol(av[4]);
    (*data)->n_meals = (ac == 6) ? atoi(av[5]) : -1;
    (*data)->running = 1;
    (*data)->n_finished_meals = 0;
    (*data)->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (*data)->n_philos);
    if ((*data)->forks == NULL)
    {
        perror("MEMORY");
        exit(1);
    }
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

void *ft_routine(void *params)
{
    t_philo *philo = (t_philo *)params;
    t_data *data = philo->params;

    while (1)
    {
        pthread_mutex_lock(&data->dead_lock);
        if (data->running != 1)
        {
            pthread_mutex_unlock(&data->dead_lock);
            break;
        }
        pthread_mutex_unlock(&data->dead_lock);

        print_status(philo, "is thinking");

        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a left fork");

        // Check if there's only one philosopher
        if (data->n_philos == 1)
        {
            usleep(data->t_die * 1000); // Philosopher will die after t_die time
            print_status(philo, "died");
            pthread_mutex_unlock(philo->left_fork);
            break;
        }

        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a right fork");

        print_status(philo, "is eating");
        pthread_mutex_lock(&data->dead_lock);
        philo->last_meal = get_time();
        philo->meals_eaten++;
        pthread_mutex_unlock(&data->dead_lock);
        usleep(data->t_eat * 1000);

        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);

        if (data->n_meals != -1 && philo->meals_eaten >= data->n_meals)
        {
            pthread_mutex_lock(&data->dead_lock);
            data->n_finished_meals++;
            if (data->n_finished_meals >= data->n_philos)
                data->running = 0;
            pthread_mutex_unlock(&data->dead_lock);
            break;
        }

        print_status(philo, "is sleeping");
        usleep(data->t_sleep * 1000);
    }
    return NULL;
}


void *controller_routine(void *args)
{
    t_data *data = (t_data *)args;

    while (1)
    {
        usleep(1000);
        int i = 0;
        while (i < data->n_philos)
        {
            // pthread_mutex_lock(&data->dead_lock);
            long t_lst_meal = get_time() - data->philos[i].last_meal;
            if (t_lst_meal > data->t_die)
            {
                print_status(&data->philos[i], "died");
                data->running = 0;
                // pthread_mutex_unlock(&data->dead_lock);
                return NULL;
            }
            // pthread_mutex_unlock(&data->dead_lock);
            i++;
        }
        // pthread_mutex_lock(&data->dead_lock);
        if (data->running != 1)
        {
            // pthread_mutex_unlock(&data->dead_lock);
            break;
        }
        // pthread_mutex_unlock(&data->dead_lock);  
    }
    return NULL;
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

    // Cleanup resources
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

    return 0;
}
