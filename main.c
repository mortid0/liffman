#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define frand() (rand()/(RAND_MAX+1.0))

#define NP 1000
#define dt 0.01
#define NSTEP 500

typedef struct {
	double weight;
	int index;
} PARTICLE;

void collision(PARTICLE *p)
{
	double p_all = 0.0;
	int i;
	double max_weight = 0.0;
	for (i = 0; i < NP; i++)
	{
		p_all += p[i].weight;
	}
	p_all *= dt;
	int nc = 0; // number of collisions
	for (i = 0; i < NP; i++)
	{
		double p_j = p_all - p[i].weight * dt;
		if (frand() < p_j) {nc++;}
		if (p[i].weight > max_weight) {max_weight = p[i].weight;}
	}
	nc /= 2;
	if (0 == nc) {return;}
	printf("collision nc = %i\n", nc);
	while (nc > 0){
		int m, n;
		n = random() % NP;
		m = random() % NP;
		while (n==m) {m = random() % NP;}
		double min_wt = (p[m].weight < p[n].weight)?(p[m].weight):(p[n].weight);;
//		double prob = min_wt * dt;
		double prob = min_wt / max_weight;
//		printf("p[%i]{w:%3.3e i:%i} p[%i]{w: %3.3e i:%i}\n",n,p[n].weight, p[n].index, m, p[m].weight, p[m].index);
		if (frand() < prob)
		{
//			printf("before p[%i]{w:%3.3e i:%i} p[%i]{w: %3.3e i:%i}\n",n,p[n].weight, p[n].index, m, p[m].weight, p[m].index);
			nc--;
			int sum_index = p[n].index + p[m].index;
			min_wt *= 0.5;
			p[n].weight -= min_wt;
			p[m].weight -= min_wt;
			if (frand() < min_wt / p[n].weight) {p[n].index = sum_index;}
			if (frand() < min_wt / p[m].weight) {p[m].index = sum_index;}
//			printf("after p[%i]{w:%3.3e i:%i} p[%i]{w: %3.3e i:%i}\n",n,p[n].weight, p[n].index, m, p[m].weight, p[m].index);
		}
	}
}

#define MP 10
void out(PARTICLE *p, double timestamp)
{
	int i;
	double conc[MP];
	for (i = 0; i < MP; i++)
	{
		conc[i] = 0.0;
	}
	for (i = 0; i < NP; i++)
	{
		conc[0] += p[i].weight;
		if (p[i].index < MP) {conc[p[i].index] += p[i].weight;}
	}
	printf("%3.3e;\t", timestamp);
	for (i = 0; i < MP; i++)
	{
		printf("%3.3e;\t", conc[i]);
	}
	printf("\n");
}

int main()
{
	int step = 0;
	int i;
	PARTICLE *p;
	p = malloc(NP * sizeof(PARTICLE));
	for (i = 0; i < NP; i++)
	{
		p[i].weight = 1.0/NP;
		p[i].index = 1;
	}
	printf("# time;\ttot_conc;\tconc[1]\t...\tconc[%i]\n", MP);
	for (step = 0; step < NSTEP; step++)
	{
		collision(p);
		out(p, step * dt);
	}
	free(p);
	return 0;
}

