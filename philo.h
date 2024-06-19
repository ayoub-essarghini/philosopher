#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int n_finished_meals; // Added to count finished meals
    long start_time;      // Added to track start time
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t dead_lock;
    int running;
    t_philo *philos;
} t_data;

#endif
