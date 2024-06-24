#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct philo
{
    int id;
    long last_meal;
    int meals_eaten;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct data *params;
} t_philo;

typedef struct data
{
    int n_philos;
    long t_die;
    long t_eat;
    long t_sleep;
    int n_meals;
    int n_finished_meals;
    long start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t dead_lock;
    int running;
    t_philo *philos;
} t_data;

void init_data(t_data **data, int ac, char **av);
void *controller_routine(void *args);
void *ft_routine(void *params);
long get_time();
int my_usleep(useconds_t usec);
void check_valid_args(int ac);
void init_philos(t_data *data);
long	ft_atol(const char *nptr);
int	ft_atoi(const char *nptr);
void print_status(t_philo *philosopher, const char *status);

#endif
