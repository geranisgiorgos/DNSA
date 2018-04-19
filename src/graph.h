#ifndef GRAPH_H
#define GRAPH_H
#define ROOT 1

// if NO_ART_NODE is commended then the program creates an extra artificial node
// plus extra arcs (for the big M method)
//i.e. if NO_ART_NODE is NOT defined => NEPSA is called
// else if DNSA is defined DNSA is called
// else DNEPSA is called
#define NO_ART_NODE
#define DNSA

// step for giden graph
#define STEP 100

/******* Problem parameters *************/
// It is NODES=SOURCES+SINKS+TSOURCES+TSINKS
/*
#define PROBLEM_PARMS 13	// No of parameters (number of lines below)
#define NODES	      10	// No of nodes
#define DENSITY       30 	// No of arcs
// The below parameters are used by NETGEN
#define SOURCES        4	// number of sources
#define SINKS	       4   // number of sinks
#define MINCOST        0	// minimum cost of arcs
#define MAXCOST       30	// maximum cost of arcs
#define SUPPLY	      200	// total supply
#define TSOURCES       1	// transshipment sources
#define TSINKS	       1	// transshipment sinks
#define HICOST	      10	// percent of skeleton arcs given maximum cost
#define CAPACITATED    0	// percent of arcs to be capacitated
#define MINCAP	       0    // minimum capacity for capacitated arcs
#define MAXCAP	       0    // maximum capacity for capacitated arcs
#define SEED     1001992789  // seed for random graph generation
*/

#define PROBLEM_PARMS 13
long int NODES=10;	// No of nodes
long int DENSITY=30; 	// No of arcs
// The below parameters are used by NETGEN
long int SOURCES=4;	// number of sources
long int SINKS=4;   // number of sinks
int MINCOST=0;	// minimum cost of arcs
int MAXCOST=30;	// maximum cost of arcs
long int SUPPLY=200;	// total supply
long int TSOURCES=1;	// transshipment sources
long int TSINKS=1;	// transshipment sinks
long int HICOST=10;	// percent of skeleton arcs given maximum cost
#define CAPACITATED    0	// percent of arcs to be capacitated
#define MINCAP	       0    // minimum capacity for capacitated arcs
#define MAXCAP	       0    // maximum capacity for capacitated arcs
#define SEED     1001992789  // seed for random graph generation


#ifdef NO_ART_NODE
#define NODES1 NODES
#define DENSITY1 DENSITY
#else
#define NODES1 NODES+1
#define DENSITY1 DENSITY+NODES
#endif

long int seed;

//double w[NODES1],s[DENSITY1],t[DENSITY1];
float *w,*s,*t;

//int Checked[NODES1];
int *Checked;

// X_flows contains the flows for the arcs (i,j) (same order as in FROM[] and TO[])
// D contains the dij values
// Y_flows contains the y flows for arcs (i,j)
//int X_flows[DENSITY1],D[DENSITY1],Y_flows[DENSITY1];
int *X_flows,*D,*Y_flows;

// Tree is the array of the parents for the current Tree
// Orientation contains the orientation of each array
// C_par contains the costs per unit cij for the current tree
//int Tree[NODES1],Orientation[NODES1],C_par[NODES1];
int *Tree,*Orientation,*C_par;

// X contains the xij values for each basic arc (i,p(i))
//long X[NODES1];
int *X;

// The basic tree in a form where we have 2 rows and a column for each arc
//int BasicTree[2][NODES1-1];
int *BasicTree0;
int *BasicTree1;

int tree_pos;
// functions used
void set_parms(long [],char **);
int get_graph();
int set_graph();
void print_graph();
double sum_costs();
void print_tree(int[2][NODES1-1]);
void init_tree_par();
//void convert(int T[NODES1],int DualT[2][NODES1-1]); // uses convert_tree
void convert(); // uses convert_tree
void convert_tree(int);
void renew_tree(int,int,int);
void print_par_tree(int []);
void fill_Cpar();


void calc_X();
double calcZ();
int exists_in_tree_par(int , int []);
void print_X();
int check_X();
int isekfyl();
int get_index(int,int);

void calc_s();
void print_s();
int check_s();
void calc_w();
void print_w();

int cycling(int ,int, int);
void find_cycle(int,int);

int is_basic(int,int);
int get_flow(int,int);
void calc_X_flows();
void print_X_flows();
int exists(int,int);
void print_D();

int get_sumh(int);
int get_pos_inpar(int ,int );

int read_from_file(char *);
void store_arrays();
void store_graph(char *);
void print_togiden();
void gener_store_graph();
#endif

