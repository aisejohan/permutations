#include <stdlib.h>
#include <stdio.h>

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

int main()
{
	int n, i, maximum;
	int *flip, *stats;

	srand(12);

	n = 4;
	flip = (int *)malloc(n*sizeof(int));
	stats = (int *)calloc(n*31, sizeof(int));
	i = 0;
	while (i < 800000) {
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
	print_stats(n, stats);

	return(0);
}
