#ifndef DUALTREE_H
#define DUALTREE_H
#include "graph.h"

float wf,wr;

int find_dual_tree();

double sum_costs();
void print_graph();
void findS();
void findR();
void print_basic_tree();
void findS();
void findR();
void print_T();
void print_S();
void print_R();
void print_w1();
void print_w2();
float min_w1(int *);
float max_w2(int *);
long int sum_from(int);
long int sum_to(int);
void init_dual();

// S:Leaving Arcs to T, R: arcs coming to T, Base: arcs selected
struct arc_set{
       int size;
       //int Arcs[2][DENSITY1];
       int *Arcs[2];
       } R, S, Base;



//************ Functions

#endif
