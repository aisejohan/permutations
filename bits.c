#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/* Compile with "gcc -O2 -o coin_flip bits.c" or similar */

/* Random sequence of integers each having 31 random bits */
void rand_flip(int n, int *flip)
{
	int i;

	i = 0;
	while (i < n) {
		flip[i] = rand();
		i++;
	}
}

void print_flip(int n, int *flip)
{
	int i, m, j, b;

	i = 0;
	while (i < n) {
		m = flip[i];
		j = 0;
		while (j < 31) {
			b = m & 1;
			printf("%d", b);
			m = m >> 1;
			j++;
		}
		i++;
	}
	printf("\n");
}

int max_run(int n, int *flip)
{
	int maximum, current, b, i, m, j, bb;

	maximum = 0;
	current = 0;
	b = flip[0] & 1;
	i = 0;
	while (i < n) {
		m = flip[i];
		j = 0;
		while (j < 31) {
			bb = m & 1;
			if (b == bb) {
				current++;
			} else {
				if (current > maximum) { maximum = current; }
				current = 1;
				b = bb;
			}
			m = m >> 1;
			j++;
		}
		i++;
	}
	return(maximum);
}

void print_stats(int n, int *stats)
{
	int i;

	printf("[%d", stats[0]);
	i = 1;
	while (i < 31*n) {
		printf(", %d", stats[i]);
		i++;
	}
	printf("]\n");
}

void get_data(int *pn, int *pnr)
{
	int success;

	printf("Please enter nr coin flips:\t\t ");
	success = 0;
	while (!success) {
		scanf("%d", pn);
		if ((*pn <= 0) || (*pn > 100000)) {
			printf("Choose between 0 and 100000. Try again:\t ");
		} else {
			success = 1;
		}
	}
	printf("Please enter nr of runs:\t\t ");
	success = 0;
	while (!success) {
		scanf("%d", pnr);
		if (*pnr <= 0) {
			printf("Choose a positive number. Try again:\t ");
		} else {
			success = 1;
		}
	}
	return;
}

void set_seed(unsigned int seed)
{
	int fd, uit;

	if (seed) {
		srand(seed);
		return;
	}
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		printf("Unable to open /dev/urandom. ");
		printf("Setting seed for random generator to 666.\n");
		seed = 666;
	} else {
		uit = read(fd, &seed, sizeof(seed));
		if (uit <= 0) {
			printf("Failure reading /dev/urandom. ");
			printf("Setting seed for random generator to 666.\n");
			seed = 666;
		}
	}
	srand(seed);
	uit = close(fd);
	return;
}

int main()
{
	int n, nr, i, maximum;
	int *flip, *stats;
	clock_t start, end;
	double cpu_time_used;

	set_seed(0);

	get_data(&n, &nr);
	n = 1 + (n / 31);
	printf("We round up to the nearest multiple of 31 coint flips.\n");
	printf("You chose %d flips running %d times...\n", 31*n, nr);
	flip = (int *)malloc(n*sizeof(int));
	stats = (int *)calloc(n*31, sizeof(int));
	start = clock();
	i = 0;
	while (i < nr) {
		rand_flip(n, flip);
		maximum = max_run(n, flip);
		stats[maximum]++;
		/*
		if (maximum > 30) {
			print_flip(n, flip);
			printf("Maximum: %d\n", maximum);
		}
		*/
		i++;
	}
	end = clock();
	printf("Statistics of your run: \n");
	print_stats(n, stats);
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time used: %f seconds.\n", cpu_time_used);

	return(0);
}
