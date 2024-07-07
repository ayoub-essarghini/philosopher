#include "philo.h"

int routine_2(t_philo *philo, t_data *data)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    }
    pthread_mutex_lock(&data->ayoub);
    philo->last_meal = get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&data->ayoub);
    print_status(philo, "is eating");
    my_usleep(data->t_eat);
    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
    }
    else
    {
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
    }
    if (data->n_meals != -1 && philo->meals_eaten >= data->n_meals)
    {
        pthread_mutex_lock(&data->dead_lock);
        data->n_finished_meals++;
        if (data->n_finished_meals >= data->n_philos)
            data->running = 0;
        pthread_mutex_unlock(&data->dead_lock);
        return (-1);
    }
    // pthread_mutex_lock(&data->dead_lock);
    print_status(philo, "is sleeping");
    // pthread_mutex_unlock(&data->dead_lock);
    my_usleep(data->t_sleep);
    print_status(philo, "is thinking");

    return (0);
}
int is_died(t_data *data)
{
    pthread_mutex_lock(&data->ayoub);
    if (data->running != 1)
    {
        pthread_mutex_unlock(&data->ayoub);
        return (0);
    }
    pthread_mutex_unlock(&data->ayoub);
    return (1);
}
void *ft_routine(void *params)
{
    t_philo *philo;
    t_data *data;
    philo = (t_philo *)params;
    data = philo->params;

    // if (philo->id % 2 == 0)
    //     usleep(500);
    while (1)
    {
        if (is_died(data) == 0)
            break;

        if (philo->id % 2 == 0)
        {
            usleep(200);
            pthread_mutex_lock(philo->left_fork);
            print_status(philo, "has taken a fork");
        }
        else
        {
            pthread_mutex_lock(philo->right_fork);
            print_status(philo, "has taken a fork");
        }
        if (data->n_philos == 1)
        {
            my_usleep(data->t_die);
            pthread_mutex_unlock(philo->left_fork);
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
        my_usleep(1);
        i = 0;
        while (i < data->n_philos)
        {
            pthread_mutex_lock(&data->ayoub);
            t_lst_meal = get_time() - data->philos[i].last_meal;
            if (t_lst_meal > data->t_die)
            {
                printf("%ld %d %s\n", get_time() - data->philos[i].params->start_time, data->philos[i].id, "died");

                // print_status(&data->philos[i], "died");
                data->running = 0;
                pthread_mutex_unlock(&data->ayoub);
                return NULL;
            }
            i++;
            pthread_mutex_unlock(&data->ayoub);
        }
        pthread_mutex_lock(&data->dead_lock);
        if (data->running != 1)
        {
            pthread_mutex_unlock(&data->dead_lock);
            break;
        }
        pthread_mutex_unlock(&data->dead_lock);
    }
    return NULL;
}
