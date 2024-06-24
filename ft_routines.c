#include "philo.h"

int routine_2(t_philo *philo, t_data *data)
{
    pthread_mutex_lock(philo->right_fork);
    print_status(philo, "has taken a right fork");
    print_status(philo, "is eating");
    pthread_mutex_lock(&data->dead_lock);
    philo->last_meal = get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&data->dead_lock);
    my_usleep(data->t_eat * 1000);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    if (data->n_meals != -1 && philo->meals_eaten >= data->n_meals)
    {
        pthread_mutex_lock(&data->dead_lock);
        data->n_finished_meals++;
        if (data->n_finished_meals >= data->n_philos)
            data->running = 0;
        pthread_mutex_unlock(&data->dead_lock);
        return (-1);
    }
    print_status(philo, "is sleeping");
    my_usleep(data->t_sleep * 1000);
    return (0);
}
int is_died(t_data *data)
{
    pthread_mutex_lock(&data->dead_lock);
    if (data->running != 1)
    {
        pthread_mutex_unlock(&data->dead_lock);
        return (0);
    }
    pthread_mutex_unlock(&data->dead_lock);
    return (1);
}
void *ft_routine(void *params)
{
    t_philo *philo;
    t_data *data;
    philo = (t_philo *)params;
    data = philo->params;

    while (1)
    {
        if (is_died(data) == 0)
            break;
        print_status(philo, "is thinking");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a left fork");
        if (data->n_philos == 1)
        {
            my_usleep(data->t_die * 1000);
            break;
        }
        if (routine_2(philo, data) == -1)
            break;
    }
    return NULL;
}

void *controller_routine(void *args)
{
    t_data *data;
    int i;
    long t_lst_meal;

    data = (t_data *)args;
    while (1)
    {
        my_usleep(1000);
        i = 0;
        while (i < data->n_philos)
        {
            t_lst_meal = get_time() - data->philos[i].last_meal;
            if (t_lst_meal > data->t_die)
            {
                print_status(&data->philos[i], "died");
                data->running = 0;
                return NULL;
            }
            i++;
        }
        if (data->running != 1)
            break;
    }
    return NULL;
}
