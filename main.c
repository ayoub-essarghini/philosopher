#include "philosopher.h"

// Initialize the simulation parameters
void init_params(t_params *params, int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        exit(1);
    }

    params->num_philosophers = atoi(argv[1]);
    params->time_to_die = atol(argv[2]);
    params->time_to_eat = atol(argv[3]);
    params->time_to_sleep = atol(argv[4]);
    params->num_meals = (argc == 6) ? atoi(argv[5]) : -1;
    params->simulation_running = 1;

    params->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * params->num_philosophers);
    for (int i = 0; i < params->num_philosophers; i++) {
        pthread_mutex_init(&params->forks[i], NULL);
    }
    pthread_mutex_init(&params->print_lock, NULL);
    pthread_mutex_init(&params->state_lock, NULL);
    params->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher) * params->num_philosophers);
}

// Initialize the philosophers
void init_philosophers(t_params *params) {
    for (int i = 0; i < params->num_philosophers; i++) {
        params->philosophers[i].id = i;
        params->philosophers[i].meals_eaten = 0;
        params->philosophers[i].last_meal_time = get_time();
        params->philosophers[i].left_fork = &params->forks[i];
        params->philosophers[i].right_fork = &params->forks[(i + 1) % params->num_philosophers];
        params->philosophers[i].params = params;
    }
}

// Free allocated resources
void free_resources(t_params *params) {
    for (int i = 0; i < params->num_philosophers; i++) {
        pthread_mutex_destroy(&params->forks[i]);
    }
    pthread_mutex_destroy(&params->print_lock);
    pthread_mutex_destroy(&params->state_lock);
    free(params->forks);
    free(params->philosophers);
}

// Get the current time in milliseconds
long get_time() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

// Philosopher routine
void *philosopher_routine(void *arg) {
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_params *params = philosopher->params;

    while (params->simulation_running) {
        // Check if simulation is still running
        // pthread_mutex_lock(&params->state_lock);
        // if (!params->simulation_running) {
        //     // pthread_mutex_unlock(&params->state_lock);
        //     break;
        // }
        // pthread_mutex_unlock(&params->state_lock);

        // Think
        print_status(philosopher, "is thinking");
        
        // Take forks
        pthread_mutex_lock(philosopher->left_fork);
        print_status(philosopher, "has taken a left fork");
        pthread_mutex_lock(philosopher->right_fork);
        print_status(philosopher, "has taken a right fork");
        
        // Eat
        print_status(philosopher, "is eating");
        pthread_mutex_lock(&params->state_lock);
        philosopher->last_meal_time = get_time();
        pthread_mutex_unlock(&params->state_lock);
        usleep(params->time_to_eat * 1000);
        philosopher->meals_eaten++;
        
        // Put down forks
        pthread_mutex_unlock(philosopher->right_fork);
        pthread_mutex_unlock(philosopher->left_fork);
        
        // Sleep
        print_status(philosopher, "is sleeping");
        usleep(params->time_to_sleep * 1000);
        
        // Check if done
        if (params->num_meals != -1 && philosopher->meals_eaten >= params->num_meals) {
            break;
        }
    }
    return NULL;
}

// Monitoring routine
void *monitor_routine(void *arg) {
    t_params *params = (t_params *)arg;

    while (1) {
        // usleep(1000);  // Check every millisecond

        for (int i = 0; i < params->num_philosophers; i++) {
            // pthread_mutex_lock(&params->state_lock);
            long time_since_last_meal = get_time() - params->philosophers[i].last_meal_time;
            if (time_since_last_meal > params->time_to_die) {
                print_status(&params->philosophers[i], "died");
                params->simulation_running = 0;
                pthread_mutex_unlock(&params->state_lock);
                return NULL;
            }
            // pthread_mutex_unlock(&params->state_lock);
        }

        // pthread_mutex_lock(&params->state_lock);
        // if (!params->simulation_running) {
        //     pthread_mutex_unlock(&params->state_lock);
        //     break;
        // }
        // pthread_mutex_unlock(&params->state_lock);
    }
    return NULL;
}

// Print the status of a philosopher
void print_status(t_philosopher *philosopher, const char *status) {
    pthread_mutex_lock(&philosopher->params->print_lock);
    printf("%ld %d %s\n", get_time(), philosopher->id + 1, status);
    pthread_mutex_unlock(&philosopher->params->print_lock);
}

int main(int argc, char **argv) {
    t_params params;

    // Initialize simulation parameters and philosophers
    init_params(&params, argc, argv);
    init_philosophers(&params);

    // Create philosopher threads
    for (int i = 0; i < params.num_philosophers; i++) {
        pthread_create(&params.philosophers[i].thread, NULL, philosopher_routine, &params.philosophers[i]);
    }

    // Create monitor thread
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_routine, &params);

    // Join philosopher threads
    for (int i = 0; i < params.num_philosophers; i++) {
        pthread_join(params.philosophers[i].thread, NULL);
    }

    // Join monitor thread
    pthread_join(monitor_thread, NULL);

    // Free resources
    free_resources(&params);

    return 0;
}
