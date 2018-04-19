#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "nepsa.h"
#include "netgen.h"
#include "main.h"

// Returns 0 if the solution is primal feasible and dual feasible
// Returns 1 if the solution is primal feasible but not dual feasible
// Returns 2 if the solution is not primal feasible and dual feasible
// Returns 3 if the solution is not primal feasible and not dual feasible
// Returns 4 if the graph is disconnected
// Returns 5 if the problem is infeasible
// Returns -1 if it doesn't find an initial dual feasible solution
// Returns 6 if problem is unbounded

//if getgraph is 1 then function get_graph() is called in order to read a graph
//if it is 0 then the graph has already been loaded

// niter is the number of iterations

int nepsa(double *objval,int getgraph,long *niter,double *cputime){


    PQ=(int *)malloc((DENSITY1)*sizeof(int));

   // thesi= the position of minimum ratio
   int thesi;
   // theta1 and theta2 values
   double th1,th2;
   // positions of theta1 and theta2 (in FROM,TO arrays)
   int pos1,pos2;
   // the objective value
   double z;
   // iteration is the No of the current iteration
   long iteration=0;
   //*/fprintf(OUT,"Algorithm NEPSA\n");
   // Initialization of parameters
   //if(COMMAND_LINE==0) set_parms(parms);
   // Loading graph
   if(getgraph==1)get_graph();
   // Get clock time
   clock_t time1,time2;
   time1=clock();
   // Apply method of big M
   add_artif_node();
   // If TOFILE=1 then the results are stored to a file
   //*/if (TOFILE) {outfile=fopen(OUTFILE,"w");}
   //*/print_graph();
   find_primal_tree();
   //if STORE_GRAPH is defined then store the graph into file STORE_FILE
   //*/if(STORE_GRAPH) store_graph(STORE_FILE);
   // Print initial Basic tree
   //*/fprintf(OUT,"\nInitial Basic Tree:\n");
   //*/print_tree(BasicTree);
   // convert tree into parent's aray form
   convert();
   // Calculate flows x
   calc_X();
   //print flows x
   //*/if(DETAILS) print_X();
   // calculate w
   calc_w();
   // print w
   //*/if(DETAILS1) print_w();
   // calculate s
   calc_s();
   // print s
   //*/if(DETAILS1) print_s();
   calc_X_flows();
   // calculate sets P and Q
   calc_PQ();
   //*/if(DETAILS) print_PQ();
   // Calculate D direction
   calc_d();
   //*/if(DETAILS1) print_D();
   // calculate objective function value
   z=calcZ();
   //*/fprintf(OUT,"\nObjective value:%f",z);
   // Start of Iterations
   while(!empty_P()){
       //*/fprintf(OUT,"\n\n------------------------------\n");
       //*/fprintf(OUT,"Iteration No %ld ",++iteration);
       iteration++;
       //*/fprintf(OUT,"\n------------------------------\n");
       if(check_d()==1) {
           //*/fprintf(OUT,"\nUnbounded Problem\n");
           *niter=iteration;
           *objval=z;
           return 6;
           }
       else if(check_d()==2) {
            //*/fprintf(OUT,"End!");
            break;
            }
        // calculate minimum ratio a=logos
        logos=calc_a(&thesi);
        //*/if(DETAILS) fprintf(OUT,"\nMinimum ratio=%0.1f for arc (%d,%d).",logos,FROM[thesi],TO[thesi]);
        //*/if(DETAILS) fprintf(OUT,"\nLeaving arc: (%d,%d)",FROM[thesi],TO[thesi]);
        // calculation of theta1 at position=pos1
        th1=find_theta1(FROM[thesi],TO[thesi],&pos1);
        //*/if(DETAILS) fprintf(OUT,"\nTheta1=%f\n",th1);
        // calculation of theta2 at position=pos2
        th2=find_theta2(FROM[thesi],TO[thesi],&pos2);
        //*/if(DETAILS) fprintf(OUT,"\nTheta2=%f\n",th2);
        // comparison of theta1 and theta2
        if(th1<=th2) {
            //*/if(DETAILS) fprintf(OUT,"\nType A iteration:");
            //*/if(DETAILS) fprintf(OUT," Entering arc: (%d,%d)\n",FROM[pos1],TO[pos1]);
            // Add into tree entering arc FROM[pos1] -> TO[pos1]
            // Take out the leaving arc. get_pos_par(...)+1 gives the leaving arc
            // into parents aray format
            renew_tree(get_pos_inpar(FROM[thesi],TO[thesi])+1,FROM[pos1],TO[pos1]);
            //Take the entering arc out of set P. It is basic now, so we set
            // the value to 0 (in array PQ)

            PQ[pos1]=0;
            }
        else {
            //*/if(DETAILS) fprintf(OUT,"\nType B iteration:");
            //*/if(DETAILS) fprintf(OUT," Entering arc: (%d,%d)\n",FROM[pos2],TO[pos2]);
            // Add into tree entering arc FROM[pos2] -> TO[pos2]
            // Take out the leaving arc. get_pos_par(...)+1 gives the leaving arc
            // into parents aray format
            renew_tree(get_pos_inpar(FROM[thesi],TO[thesi])+1,FROM[pos2],TO[pos2]);
            //Take the entering arc out of set Q. It is basic now, so we set
            // the value to 0 (in array PQ)
            PQ[pos2]=0;
            }
        // Add the leaving arc (k,l) into set Q
        if(FROM[thesi] == NODES1 || TO[thesi] == NODES1) PQ[thesi]=-2;
        else PQ[thesi]=1;
        //*/if(DETAILS) fprintf(OUT,"\nNew tree:");
        //*/if(DETAILS) print_tree(BasicTree);
        // Update values of arrays x,w,s,D
        convert();
        // Calculate flows x
        calc_X();
        //print flows x
        //*/if(DETAILS) print_X();
        // calculate w
        calc_w();
        // print w
        //*/if(DETAILS1) print_w();
        // calculate s
        calc_s();
        // print s
        //*/if(DETAILS1) print_s();
        //*/if(DETAILS) print_PQ();
        calc_X_flows();
        calc_d();
        //*/if(DETAILS1) print_D();
        // objective value
        z=calcZ();
        //*/fprintf(OUT,"\nObjective value:%f",z);
        //*/if(check_X()==0)
            //*/fprintf(OUT,"\n+Primal Feasible");
        //*/else
            //*/fprintf(OUT,"\n-Not Primal Feasible");
        //*/if(check_s()==0)
            //*/fprintf(OUT,"\n+Dual Feasible");
        //*/else
            //*/fprintf(OUT,"\n-Not Dual Feasible");
        if(check_X()==0 && check_s()==0) break;
     }
     time2=clock();
     *cputime = (double)(time2 - time1)/(CLOCKS_PER_SEC );
     // number of iterations
     *niter=iteration;
     *objval=z;
     if(check_X()==0) {
           if(check_artif_basic()) return 5;
		   if(check_s()==0) return 0;
		   else return 1;
         }
	  else
	  	   if(check_s()==0) return 2;
  		   else return 3;
}

void find_primal_tree(){
     int i;
     for(i=0;i<NODES;i++)
       if(B[i]>0){
         BasicTree0[i]=i+1;
         BasicTree1[i]=NODES1;
         }
       else {
         BasicTree0[i]=NODES1;
         BasicTree1[i]=i+1;
         }
     }

// PQ contains -1 if the corresponding arc belongs to P
// It contains 1 if  the arc belongs to Q and 0 for the basic arcs
// i.e. contains both P and Q
// it contains -2 for artificial arcs not participating into the process
// (i.e. have been deleted)
void calc_PQ(){
     int i;
     for(i=0;i<DENSITY1;i++){
	 	if(is_basic(FROM[i],TO[i])) PQ[i]=0;
        else if(FROM[i] == NODES1 || TO[i] == NODES1) PQ[i]=-2;
        else if(s[i]<0) PQ[i]=-1;
        else PQ[i]=1;
        }
     }

// Print sets I_ and I+
void print_PQ(){
	 int i;
     fprintf(OUT,"\nP=[");
     for (i=0;i<DENSITY1;i++)
         if(PQ[i]==-1) fprintf(OUT," (%d,%d) ",FROM[i],TO[i]);
     fprintf(OUT,"]\n");
     fprintf(OUT,"\nQ=[");
     for (i=0;i<DENSITY1;i++)
         if(PQ[i]==1) fprintf(OUT," (%d,%d) ",FROM[i],TO[i]);
     fprintf(OUT,"]\n");

	 }


	 // Calculates direction d which gives the second dual feasible flow yij
void calc_d(){
     int i;
	 // for the basic arcs it is dij=1 if xij<0 otherwise dij=1
     for(i=0;i<DENSITY1;i++)
         if(!is_basic(FROM[i],TO[i]))
             if(s[i]<0) D[i]=1;
             else D[i]=0;
         // for the basic arcs calculate the sum of hij for the non basic
		 // arcs (i,j) that belong to P
         else D[i]=get_sum_h(i);
     }

// It takes an arc FROM[i]->TO[i] and calculates the sum of hij for the non basic
// arcs (u,v) for which it is s(u,v)<0
int get_sum_h(int i){
	int j,x,sum=0;
	for(j=0;j<DENSITY1;j++){
            // we don't count artificial arcs that have been deleted (-2 in PQ)
 			if(s[j]<0 && PQ[j]!=-2){
				x=get_pos_inpar(FROM[i],TO[i]);
				sum+=cycling(FROM[j],TO[j],x+1);
				}
			}
	return (-1)*sum;
}

//returns 0 if P not empty, otherwise returns 1
int empty_P(){
     int i;
     for (i=0;i<DENSITY1;i++)
         if(PQ[i]==-1) return 0;
     return 1;
}

// returns 2 if d == 0
// returns 1 if vector d>=0
// else returns 0
int check_d(){
    int i,iszero=1;
    for(i=0;i<DENSITY1;i++)
       if(D[i]<0) return 0;
    for(i=0;i<DENSITY;i++)
       if(D[i]!=0) iszero=0;
    if (iszero) return 2;
    return 1;
}

// It computes the minimum ratio a=xij/-dij
// thesi gives the position of the minimum ratio
double calc_a(int *thesi){
     int i;
     double min=MAX_INT;
     *thesi=-1;
     for(i=0;i<DENSITY1;i++){
          if(D[i]<0)
           if(min>=-(double)X_flows[i]/D[i]){
   					  min = -(double)X_flows[i]/D[i];
				 	  *thesi=i;
		   			  }
          }
     return min;
     }

// Calculates theta1. It take as parameter the leaving arc (k,l)
// and the position of the arc giving the minimum
double find_theta1(int k,int l, int *pos){
       double min=MAX_INT;
       int i,orient;
       *pos=-1;
       for(i=0;i<DENSITY1;i++){
           orient=cycling(FROM[i],TO[i],get_pos_inpar(k,l)+1);
           // check the arcs of set P (sij<0) having opposite orientation (hij=1)
           if(PQ[i] == -1 && orient==1 && min>-s[i]) {
                     min=-s[i];
                     *pos=i;
                     }
           }
       return min;
       }


// Calculates theta2. It take as parameter the leaving arc (k,l)
// and the position of the arc giving the minimum
double find_theta2(int k,int l, int *pos){
       double min=MAX_INT;
       int i,orient;
       *pos=-1;
       for(i=0;i<DENSITY1;i++){
           orient=cycling(FROM[i],TO[i],get_pos_inpar(k,l)+1);
           // check the arcs of set Q (sij>=0) having same orientation (hij=-1)
           if(PQ[i] == 1 && s[i]>=0 && orient==-1 && min>s[i]) {
                      min=s[i];
                      *pos=i;
                      }
           }
       return min;
       }

// adds the new artificial node (method of big M)

void add_artif_node(){
     //long M=calc_M();
     long M=100000;
     int i;
     B[NODES]=0;
     for(i=0;i<=NODES;i++)
       if(B[i]<=0){
         FROM[DENSITY+i]=NODES1;
         TO[DENSITY+i]=i+1;
         C[DENSITY+i]=M;
         }
       else{
         FROM[DENSITY+i]=i+1;
         TO[DENSITY+i]=NODES1;
         C[DENSITY+i]=M;
         }
}

long calc_M(){
     return (NODES-1)*calc_sumB()*calc_maxC()+1;
     }

long calc_sumB(){
     long s=0;
     int i;
     for(i=0;i<NODES;i++)
        if(B[i]>0) s+=B[i];
     return s;
     }

long calc_maxC(){
     long max=-1,i;
     for(i=0;i<NODES;i++)
        if(C[i]>max) max=C[i];
        else if((C[i]<0 && (-1)*C[i]>max)) max=-C[i];
     return max;
     }
// It returns 1 if an artificial arc having non zero flowexists into the basic tree
// In that case the problem is infeasible
int check_artif_basic(){
    int i;
    for(i=DENSITY;i<DENSITY1;i++){
          if(is_basic(FROM[i],TO[i]) && X_flows[i]!=0) return 1;
    }
    return 0;
}
