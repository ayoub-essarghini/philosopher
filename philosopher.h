#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

// Structure to store philosopher information
typedef struct s_philosopher {
    int id;
    int meals_eaten;
    long last_meal_time;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_params *params;
} t_philosopher;

// Structure to store simulation parameters
typedef struct s_params {
    int num_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int num_meals;
    int simulation_running;
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t state_lock;
    t_philosopher *philosophers;
} t_params;

// Function prototypes
void init_params(t_params *params, int argc, char **argv);
void init_philosophers(t_params *params);
void free_resources(t_params *params);
long get_time();
void *philosopher_routine(void *arg);
void *monitor_routine(void *arg);
void print_status(t_philosopher *philosopher, const char *status);

#endif
