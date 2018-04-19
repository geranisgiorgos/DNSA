#ifndef DNEPSA_H
#define DNEPSA_H

#include <stdio.h>
#include "graph.h"

//if SHOWSTEPS=1 shows step by step on screen
#define SHOWSTEPS 0

// if SHOW_ITER is 1 then when the results are shown on screen
// it waits to press enter after each operation
#define SHOW_ITER  0

// If 1 details for the steps executed are also stored to file or displayed on screen
#define DETAILS 0
#define DETAILS1 0  // More details in printing

// If FROMFILE is 1 then the graph is read from the file specified by INFILE
#define FROMFILE 0
#define INFILE "input/instance_1"

// if 1 then stores the graph into the specified file
#define STORE_GRAPH 0
#define STORE_FILE "input/graph.txt"

// 1 means the results are stored into a file
// the filename is specified by OUTFILE
// also OUT has to be specified to be outfile
#define TOFILE 0

// if OUT is stdout the program gives results to the screen
// otherwise if OUT is outfile the program stores the results
// to the file specified by OUTFILE
#define OUTFILE "results/dnepsa-res.txt"

#define OUT outfile
//#define OUT stdout

// E is an error acceptance limit (because of  roundings...)
#define E 0.001

// If GIDEN is 1 then store the graph in GIDEN format
#define GIDEN 0
#define GIDEN_FILE "giden-nodes100.gdn"

// if STORE_ARRAYS is 1 then the problem parameters in array format
// are stored into the files specified
#define STORE_ARRAYS 0
#define OUT_A "results/A.txt"
#define OUT_B "results/B.txt"
#define OUT_b "results/supplies.txt"
#define OUT_c "results/c.txt"

// I contains -1 if the corresponding arc belongs to I_
// I contains 1 if  belongs to I+ and 0 those arcs that are not basic
// i.e. contains both I_ and I+
//int I[DENSITY1];
int *I;

// The minimum ratio for the choice of the entering arc
double logos;

// J is an array with a value for every arc
// It contains 1 if the corresponding arc belongs to J_
// otherwise it contains 0
//int J[DENSITY1];
int *J;

// Cycle is the cycle created after adding the entering arc to the basic tree
// -1 if same direction as the inserted arc, 1 for opposite direction, 0 not in cycle
//int Cycle[NODES1];
int *Cycle;

// Declaration of the file where storing the results (if TOFILE is 1)
// and the file from which the network format is read (is FROMFILE is 1)
FILE *outfile, *infile;

// Objective value
double z;

// Array containing the problem parameters
long parms[PROBLEM_PARMS];

//***************** Declarations of functions
void calc_I();
void print_I();
int calc_J();
int check_J();
void print_J_();
double calc_logos(int *);
//void find_th1(double *,int *,int *);
//void find_th2(double *,int *,int *);
int find_theta(double *theta,int *pos,int thesi);
////void find_thetas(double *,double *,int *,int *,int *,int *);
void calc_D();
void renew_DS(int,int,int);
void calc_Y_flows();
void print_Y_flows();
int check_Y();
#endif
