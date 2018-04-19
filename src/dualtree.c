#include <stdio.h>
#include "netgen.h"
#include "dualtree.h"
#include "graph.h"
#include "dnepsa.h"
#include "main.h"

#define SEED1    342343422    // seed for the choice of start node of dual tree

void init_dual(){
    R.Arcs[0]=(int *)malloc((DENSITY1)*sizeof(int));
    R.Arcs[1]=(int *)malloc((DENSITY1)*sizeof(int));
    S.Arcs[0]=(int *)malloc((DENSITY1)*sizeof(int));
    S.Arcs[1]=(int *)malloc((DENSITY1)*sizeof(int));
    Base.Arcs[0]=(int *)malloc((DENSITY1)*sizeof(int));
    Base.Arcs[1]=(int *)malloc((DENSITY1)*sizeof(int));
}

// Function to find the initial Dual Tree
// It uses Klingman's method
// Returns 1 for disconnected graph, 0 for success
int find_dual_tree(){
     int p=0,q,i,thesi,thesi_min=0,thesi_max=0;
     long int bf=0,br=0;

     init_dual();
     tree_pos=0;
     for(i=0;i<NODES1;i++) {
        T[i]=0;  // T=empty
        w1[i]=MAX_INT;
        thesi_w1[i]=-1;
        w2[i]=0;
        thesi_w2[i]=-1;
        }
     // print_w1();
     Base.size=0;  // B=empty set
     //if(DETAILS) fprintf(OUT,"\n\nFinding Initial Dual Tree\n\n");
     // The choice of the starting node depends on SEED1
     srand(SEED1);
     // Select a random starting node. We could fix it to a standard starting node
     // by giving for example
	 //q=3; // if we want to srart from a certain node
     q=rand()/(int)(((unsigned)RAND_MAX + 1) / NODES1)+1;
     p++;  // p shows the number of the cycle (repetition)
     w[q-1]=sum_costs();  // w for starting node
     T[q-1]=1; //Put q into set T. Use q-1 since T starts from 0
    while(p<NODES1){
     findS();   // Find set S of leaving arcs
     if (S.size>0){
        for(i=0;i<NODES1;i++)  // renewal of w1
             if( T[i]==0 && (thesi=exists(q,i+1 ))>-1){  // if (q,i) is an arc
                 if(w1[i]>w[q-1]+C[thesi]){
                      thesi_w1[i]=q-1;
                      w1[i]=w[q-1]+C[thesi];
                      }
                }
        }
     findR();
     if (R.size>0){
        for(i=0;i<NODES1;i++)  // renewal of w2
             if( T[i]==0 && (thesi=exists(i+1,q ))>-1){  // if (i,q) is an arc
                 if(w2[i]<w[q-1]-C[thesi]){
                      thesi_w2[i]=q-1;
                      w2[i]=w[q-1]-C[thesi];
                      }
                }
        }

     if(S.size==0 && R.size == 0) {
                  fprintf(OUT,"\nDisconnected Graph\n");
                  return 1;
                  }
     else {
      if(S.size>0) {
         wf=min_w1(&thesi_min); //wf is the minimum w1 at position thesi_min
         bf=-(B[thesi_min]+sum_from(thesi_min)+sum_to(thesi_min));
        }
      if(R.size>0) {
         wr=max_w2(&thesi_max); //wr is the maximum w2 at position thesi_max
         br=-(B[thesi_max]+sum_from(thesi_max)+sum_to(thesi_max));
        }
       if (S.size == 0) {
                  q=thesi_max+1;
                  w[q-1]=wr;
                  //if(DETAILS) fprintf(OUT,"\nAdding edge %d -> %d",q,thesi_w2[q-1]+1);
                  BasicTree0[tree_pos]=q;
                  BasicTree1[tree_pos]=thesi_w2[q-1]+1;
                  tree_pos++;
                  }
       else if(R.size == 0) {
                  q=thesi_min+1;
                  w[q-1]=wf;
                  //if(DETAILS) fprintf(OUT,"\nAdding edge %d -> %d",thesi_w1[q-1]+1,q);
                  BasicTree0[tree_pos]=thesi_w1[q-1]+1;
                  BasicTree1[tree_pos]=q;
                  tree_pos++;
                  }
       else if(wf*bf >= wr*br) {
                  q=thesi_min+1;
                  w[q-1]=wf;
                  //if(DETAILS) fprintf(OUT,"\nAdding edge %d -> %d",thesi_w1[q-1]+1,q);
                  BasicTree0[tree_pos]=thesi_w1[q-1]+1;
                  BasicTree1[tree_pos]=q;
                  tree_pos++;
                  }
       else {
            q=thesi_max+1;
            w[q-1]=wr;
            //if(DETAILS) fprintf(OUT,"\nAdding edge %d -> %d",q,thesi_w2[q-1]+1);
            BasicTree0[tree_pos]=q;
            BasicTree1[tree_pos]=thesi_w2[q-1]+1;
            tree_pos++;
            }
       T[q-1]=1;
     }
     p++;  // Loop continues
     }

    return 0;
    }


void findS(){
     int i;
     S.size=0; //Midenismos tou S
     for (i = 0;i<DENSITY1;i++){
         if (T[FROM[i]-1]==1 && T[TO[i]-1]==0){ // if FROM[i] is in T
            S.Arcs[0][S.size]=FROM[i];
            S.Arcs[1][S.size]=TO[i];
            S.size++;
            }
         }
     }

void findR(){
     int i;
     R.size=0; //Midenismos tou S
     for (i = 0;i<DENSITY1;i++){
         if (T[TO[i]-1]==1 && T[FROM[i]-1]==0){ // if TO[i] is in T
            R.Arcs[0][R.size]=FROM[i];
            R.Arcs[1][R.size]=TO[i];
            R.size++;
            }
         }
     }

void print_T(){
     int i;
     //if(DETAILS) fprintf(OUT,"\nT={");
     for(i=0;i<NODES1;i++)
        if(DETAILS) if (T[i]==1) fprintf(OUT," %d",i+1);
     if(DETAILS) fprintf(OUT," }\n");
     }

void print_S(){
     int i;
     if(DETAILS) fprintf(OUT,"\nS={");
     for(i=0;i<S.size;i++){
         if(DETAILS) fprintf(OUT," (%d,%d)",S.Arcs[0][i],S.Arcs[1][i]);
         }
     if(DETAILS) fprintf(OUT," }");
     }

void print_R(){
     int i;
     if(DETAILS) fprintf(OUT,"\nR={");
     for(i=0;i<R.size;i++){
         if(DETAILS) fprintf(OUT," (%d,%d)",R.Arcs[0][i],R.Arcs[1][i]);
         }
     if(DETAILS) fprintf(OUT," }");
     }



void print_w1(){
     int i;
     if(DETAILS) fprintf(OUT,"\nw1={");
     for(i=0;i<NODES1;i++)
         if(DETAILS) fprintf(OUT," %0.1f",w1[i]);
     if(DETAILS) fprintf(OUT," }");
     }

void print_w2(){
     int i;
     if(DETAILS) fprintf(OUT,"\nw2={");
     for(i=0;i<NODES1;i++)
         if(DETAILS) fprintf(OUT," %0.1f",w2[i]);
     if(DETAILS) fprintf(OUT," }");
     }

float min_w1(int *thesi){
      int i;
      float min;
      *thesi=-1;
      min=MAX_INT;
      for(i=0;i<NODES1;i++)
         if(T[i]==0)
            if(min>w1[i]) {
                min=w1[i];
                *thesi=i;
                }
      return  min;
      }

float max_w2(int *thesi){
      int i;
      float max;
      *thesi=-1;
      max=0;
      for(i=0;i<NODES1;i++)
         if(T[i]==0)
            if(max<w2[i]) {
                max=w2[i];
                *thesi=i;
                }
      return  max;
      }

long int sum_from(int p){
     int i;
     long int sum=0;
     for(i=0;i<NODES1;i++)
        if(T[i]==0 && (exists(p+1,i+1)>=0)){
             sum+=B[i];
             }
     return sum;
     }

long int sum_to(int p){
     int i;
     long int sum=0;
     for(i=0;i<NODES1;i++)
        if(T[i]==0 && exists(i+1,p+1)>=0){
             sum+=B[i];
             }
     return sum;
     }
