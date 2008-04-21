
#include <stdlib.h>
#include <stdio.h>

/* Knuth shuffle */
void rand_perm(int n, int *perm)
{
	int i,j;

	i = 0;
	while (i < n) {
		j = rand() % (i+1);
		perm[i] = perm[j];
		perm[j] = i;
		i++;
	}
}

/* List cycles in increasing lenghts */
int cycles(int n, int *perm, int *lijst)
{
	int i, j, k, l, m, len;
	char *done;

	done = (char *) calloc(n, sizeof(char));

	k = 0;
	i = 0;
	while (i < n) {
		if (!done[i]) {
			len = 1;
			done[i] = 1;
			j = perm[i];
			while (j != i) {
				len++;
				done[j] = 1;
				j = perm[j];
			}
			l = 0;
			while ((l < k) && (lijst[l] <= len)) l++;
			m = k - 1;
			while (m >= l) {
				lijst[m+1] = lijst[m];
				m--;
			}
			lijst[l] = len;
			k++;
		}
		i++;
	}
	
	free(done);

	return(k);
}

/* Merge two sorted lists of integers */
int *merge(int *v, int *w)
{
	int a,b,i,j,k;
	int *u;

	a = v[0];
	b = w[0];

	u = (int *) malloc((a + b + 1)*sizeof(int));
	u[0] = -1;
	i = 1;
	j = 1;
	k = 1;
	while ((i <= a) && (j<= b)) {
		if (v[i] < w[j]) {
			if (u[k-1] < v[i]) {
				u[k] = v[i];
				k++;
			}
			i++;
		} else {
			if (u[k-1] < w[j]) {
				u[k] = w[j];
				k++;
			}
			j++;
		}
	}
	while (i <= a) {
		if (u[k-1] < v[i]) {
			u[k] = v[i];
			k++;
		}
		i++;
	}
	while (j <= b) {
		if(u[k-1] < w[j]) {
			u[k] = w[j];
			k++;
		}
		j++;
	}
	u[0] = k-1;
	return(u);
}

/* Lists sums of cycle lengths */
int *cuts(int n, int k, int *lijst)
{
	int i, j;
	int *old;
	int *new;
	int *tus;

	old = malloc(2*sizeof(int));
	old[0] = 1;
	old[1] = 0;

	i = 0;
	while (i < k) {
		new = malloc((old[0]+1)*sizeof(int));
		new[0] = old[0];
		j = 1;
		while (j <= old[0]) {
			new[j] = old[j] + lijst[i];
			j++;
		}
		tus = merge(new, old);
		free(new);
		free(old);
		old = tus;
		i++;
	}
	return(old);
}

/* Prints a permutation */
void print_perm(int n, int *perm)
{
	int i;

	i = 0;
	while (i < n-1) {
		printf("%d, ",perm[i]);
		i++;
	}
	printf("%d.\n", perm[n-1]);
}

/* Prints cycle lenghts */
void print_cycle_lengths(int k, int *lijst)
{
	int i;

	i = 0;
	while (i < k-1) {
		printf("%d, ",lijst[i]);
		i++;
	}
	printf("%d.\n", lijst[k-1]);
}

/* Prints sums of cycle lengths */
void print_cuts(int *degrees)
{
	int i;

	i = 1;
	while (i < degrees[0]) {
		printf("%d, ",degrees[i]);
		i++;
	}
	printf("%d.\n", degrees[degrees[0]]);
}

/* Print stats */
void print_stats(int n, int *stats, int target, int nr)
{
	int i, close, print_range;
	print_range = 9;

/*	printf("Out of %d random permutations of length %d.\n", nr, n);
	printf("Target %d. The statistics for closests cuts is: \n", target);*/

	close = stats[target]; /* You could replace this */

/*	i = target - print_range;
	if (i < 0) i = 0;
	while (i < target + print_range && i <= n) {
		printf("%d, ",stats[i]);
		i++;
	}
	printf("%d.\n", stats[i]);
	printf("The number of close ones is %d out of %d.\n", close, nr); */
	printf("Percentage %f.\n", 100*((float) close)/((float) nr));
}

/* Find closest cut to target */
int closest_cut(int n, int *degrees, int target)
{
	int i, above, below;
	
	below = 1;
	above = degrees[0];

	while (below + 1 < above) {
		i = (below + above) / 2;
		if (degrees[i] < target) {
			below = i;
		} else {
			above = i;
		}
	}
	if (degrees[above]-target <= target-degrees[below]) {
		return(above);
	} else {
		return(below);
	}
}

int main()
{
	int i, j, n, k, c, run, nr_runs, target;
	int *perm, *lijst, *degrees, *stats;

	srand(12);

	target = 12;
	n = 3000;
	run = 200000;
	nr_runs = 1000;

	j = 0;
	while (j < nr_runs) {
		perm = (int *)malloc(n*sizeof(int));
		lijst = (int *)malloc(n*sizeof(int));
		stats = (int *)calloc(n+1, sizeof(int));
		i=0;
		while(i < run) {

			rand_perm(n, perm);
/*			printf("The permutation is: \n");
			print_perm(n, perm); */

			k = cycles(n, perm, lijst);
/*			printf("The number of cycles is %d.\n", k);
			printf("The lengths of the cycles are: \n");
			print_cycle_lengths(k, lijst); */

			degrees = cuts(n, k, lijst);
/*			printf("The lengths of the cuts are: \n");
			print_cuts(degrees);*/

			c = closest_cut(n, degrees, target);
/*			printf("The closest cut is at %d\n",degrees[c]); */

			stats[degrees[c]]++;

			free(degrees);
			i++;
		}
		print_stats(n, stats, target, run);
		j++;
		n += 1;
		free(perm);
		free(lijst);
		free(stats);
	}
	return(0);
}
