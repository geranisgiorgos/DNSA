#ifndef DNSA_H
#define DNSA_H

// E is an error acceptance limit (because of  roundings...)
#define E 0.001

//if SHOWSTEPS=1 shows step by step on screen
//#define SHOWSTEPS 0
// if SHOW_ITER is 1then when the results are shown on screen
// it waits to press enter after each operation
#define SHOW_ITER  0
// If 1 details for the steps executed are also stored to file or displayed on screen
#define DETAILS 0
#define DETAILS1 0  // More details in printing


// if 1 then stores the graph into the specified file
//#define STORE_GRAPH 0
//#define STORE_FILE "input/bug1_new.txt"

// If GIDEN is 1 then store the graph in GIDEN format
//#define GIDEN 1
//#define GIDEN_FILE "giden-file-300nodes.gdn"

// If FROMFILE is 1 then the graph is read from the file specified by INFILE
#define FROMFILE 0
//#define INFILE "input/5/501x12525/instance_2"
//#define INFILE "input/bug1_1.txt"
// 1 means the results are stored into a file
// the filename is specified by OUTFILE
// also OUT has to be specified to be outfile
//#define TOFILE 1
// if OUT is stdout the program gives results to the screen
// otherwise if OUT is outfile the program stores the results
// to the file specified by OUTFILE
//#define OUTFILE "results/res1.txt"
//#define OUT outfile
//#define OUT stdout

// Tree is the array of the parents for the current Tree
// Orientation contains the orientation of each array
// C_par contains the costs per unit cij for the current tree
///int Tree[NODES1],Orientation[NODES1],C_par[NODES1];

// X contains the xij values for each basic arc (i,p(i))
///long X[NODES1];

// I contains -1 if the corresponding arc belongs to I_
// I contains 1 if  belongs to I+ and 0 those arcs that are not basic
// i.e. contains both I_ and I+
//int I[DENSITY1];
//int I[];

// X_flows contains the flows for the arcs (i,j) (same order as in FROM[] and TO[])
// D contains the dij values
// Y_flows contains the y flows for arcs (i,j)
///int D[DENSITY1];
///int X_flows[DENSITY1],Y_flows[DENSITY1];

// The basic tree in a form where we have 2 rows and a column for each arc
// In NEPSA we have NODES+1 nodes so the tree contains NODES arcs
///int BasicTree[2][NODES1-1];

// Cycle is the cycle created after adding the entering arc to the basic tree
// -1 if same direction as the inserted arc, 1 for opposite direction, 0 not in cycle
///int Cycle[NODES1];

// Declaration of the file where storing the results (if TOFILE is 1)
// and the file from which the network format is read (is FROMFILE is 1)
FILE *outfile, *infile;

// The minimum ratio for the choice of the entering arc
double logos;

// Array containing the problem parameters
long parms[PROBLEM_PARMS];

double calc_a();
void find_xkl(double *,int *,int *);
double calc_enter(int *,int);

int dnsa(double *,int ,long *, double *);
#endif
