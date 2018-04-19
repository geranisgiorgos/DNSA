#ifndef MAIN_H
#define MAIN_H
#define MAX_INT 10320000

// If command line is 1 then we expect the user to give graph arguments on the command line
#define COMMAND_LINE 1

// 0 means we give a graph by hand (function setgraph())
// 1 means it uses netgen to produce a random graph
#define NETGEN 1

void init();

int dnepsa(double *,int,long *,double *);
int nepsa(double *,int,long *,double *);
void run_algorithm();
void run_rand_algorithm();
int run_multi_algorithm();
int gener_graphs(int );
int solve_graphs(int,int);
void dir_name(char *);

#endif
