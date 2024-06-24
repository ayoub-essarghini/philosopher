#include "philo.h"

void check_valid_args(int ac)
{
    if (ac != 5 && ac != 6)
    {
        write(2, "ERROR\n", 6);
        exit(1);
    }
}
int	ft_atoi(const char *nptr)
{
	int		res;
	int		sign;
	int		i;

	i = 0;
	res = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
		{
			sign = -1;
		}
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - '0';
		i++;
	}
	return (sign * res);
}
long	ft_atol(const char *nptr)
{
	long		res;
	int		sign;
	int		i;

	i = 0;
	res = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
		{
			sign = -1;
		}
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - '0';
		i++;
	}
	return (sign * res);
}

long get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

int my_usleep(useconds_t usec)
{
    struct timespec req, rem;

    // Convert microseconds to seconds and nanoseconds
    req.tv_sec = usec / 1000000;
    req.tv_nsec = (usec % 1000000) * 1000;

    while (nanosleep(&req, &rem) == -1)
    {
        // Check if we need to continue sleeping for the remaining time
        req = rem;
    }
    // Return 0 on success
    return 0;
}