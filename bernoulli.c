#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define KRP 100
#define NP 100
#define dt 0.01
#define END_TIME 50.0

#define frand() (rand()/(RAND_MAX+1.0))


typedef struct {
	int index;
	double weight;
} PARTICLE;

void init(PARTICLE **p, double conc, int index)
{
	int krp, np;
	double part, total;
	for (krp = 0; krp < KRP; krp++)
	{
		total = conc;
		for (np = 0; np < NP; np++)
		{
			p[krp][np].index = index;
			part = frand() * total / NP;
			p[krp][np].weight = part;
			if (np != NP-1) {total -= part;}
			else {p[krp][np].weight = total; if (total < 0) printf("total = %4.4e\n", total);}
		}
	}
}

void collision(PARTICLE **p)
{
	int krp, mp, lp;
	PARTICLE *pm, *pl;
	for (krp = 0; krp < KRP; krp++)
	{
		for (mp = 0; mp < (NP-1); mp++)
		{
			for (lp = mp+1; lp < NP; lp++)
			{
				pm = &(p[krp][mp]);
				pl = &(p[krp][lp]);
				double max_weight = (pl->weight > pm->weight) ? pl->weight:pm->weight;
				double prob = (max_weight * dt);
				if (frand() < prob)
				{
//					printf("max_weight = %4.4e\n", max_weight);
					if (pl->weight < pm->weight)
					{
//						printf("old(pl<pm) (%i %4.4e) (%i %4.4e) mass = %4.4e\n", pl->index, pl->weight, pm->index, pm->weight, pl->index*pl->weight + pm->index*pm->weight);
						pl->index += pm->index;
						pm->weight -= pl->weight;
//						printf("new (%i %4.4e) (%i %4.4e) mass = %4.4e\n", pl->index, pl->weight, pm->index, pm->weight, pl->index*pl->weight + pm->index*pm->weight);
					} else if (pm->weight < pl->weight)
					{
//						printf("old(pl>pm) (%i %4.4e) (%i %4.4e) mass = %4.4e\n", pl->index, pl->weight, pm->index, pm->weight, pl->index*pl->weight + pm->index*pm->weight);
						pm->index += pl->index;
						pl->weight -= pm->weight;
//						printf("new (%i %4.4e) (%i %4.4e) mass = %4.4e\n", pl->index, pl->weight, pm->index, pm->weight, pl->index*pl->weight + pm->index*pm->weight);
					}
					else
					{
//						printf("old(pl=pm) (%i %4.4e) (%i %4.4e) mass = %4.4e\n", pl->index, pl->weight, pm->index, pm->weight, pl->index*pl->weight + pm->index*pm->weight);
						pl->index += pm->index;
						pm->index = pl->index;
						pl->weight *= 0.5;
						pm->weight *= 0.5;
//						printf("new (%i %4.4e) (%i %4.4e) mass = %4.4e\n", pl->index, pl->weight, pm->index, pm->weight, pl->index*pl->weight + pm->index*pm->weight);
					}
				}
			}
		}
	}
}

void print(PARTICLE **p, int step)
{
	int krp, np;
	PARTICLE *a;
	double sum_weight = 0, mass = 0.0;
	for (krp = 0; krp < KRP; krp++)
	{
		for (np = 0; np < NP; np++)
		{
			a = &(p[krp][np]);
			sum_weight += a->weight;
			mass += a->weight * a->index;
		}
	}
	printf("%i; %4.4e; %4.4e; %4.4e\n", step, step * dt, (sum_weight/KRP), (mass/KRP));
}

int main()
{
	PARTICLE **p;
	p = malloc(KRP * sizeof(PARTICLE*));
	int krp;
	for (krp = 0; krp < KRP; krp++){p[krp] = malloc(NP*sizeof(PARTICLE));}
	init(p, 1.0, 1);
	int step;
	int step_num = END_TIME / dt;
	for (step = 0; step < step_num; step++)
	{
		print(p, step);
		collision(p);
	}
	for (krp = 0; krp < KRP; krp++){free(p[krp]);}
	free(p);
}
