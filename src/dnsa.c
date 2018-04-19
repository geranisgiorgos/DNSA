#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "netgen.h"
#include "dnsa.h"
#include "main.h"
#include "dualtree.h"
#include "dnepsa.h"


int dnsa(double *objval,int getgraph,long *niter, double *cputime){
    // thesi: the position chosen  for the entering arc
    int thesi;
    // th is the minimum negative xij flow value
    // sgh is the reduced cost value of entering arc (g,h)
    double th,sgh;
    // position of leaving arc (giving th value) in FROM,TO arrays
    int pos1;
    // thesi1 the corresponding index in parents tree array
    int thesi1;
    // iteration is the No of the current iteration
    long iteration=0;
    // optimal becomes 1 when we find an optimal solution
    int optimal=0;
    // Read the graph parameters (from file graph.h)
    // Initialization of parameters
    //if(COMMAND_LINE==0) set_parms(parms);
    // Loading graph
    if(getgraph==1) get_graph();
    clock_t time1,time2;
    // take current time
    time1=clock();
    // If TOFILE=1 then the results are stored to a file
    //*/if (TOFILE) {outfile=fopen(OUTFILE,"w");}
    // Print indormation about the graph
    //*/print_graph();
//*/print_graph();
    //*/if(SHOWSTEPS) getchar();
    // if GIDEN is 1 then store the graph in GIDEN's format
//*/if(GIDEN) print_to_giden();
    // Find an initial dual feasible tree BasicTree
    // If the graph is disconnected, find_dual_tree returns 4 and we have to stop
    if(find_dual_tree()==1) {
                        return 4;
                        }
    //if STORE_GRAPH is defined then store the graph into file STORE_FILE
    //*/if(STORE_GRAPH) store_graph(STORE_FILE);
    // if STORE_ARRAYS is defined then stores problem in arrays format
    //if (STORE_ARRAYS) store_arrays();
    // Print the initial dual feasible tree
//*/fprintf(OUT,"\nInitial Dual Feasible Tree:\n");
//*/print_tree(BasicTree);
//*/if(SHOWSTEPS) getchar();
    // Convert BasicTree into parents-array form.
    // It produces Tree which is of this form
    // It also fills in array C_par which contains the costs per unit cij
    // for the arcs of the tree
    convert();
    // Print the tree into parents array form
//*/if(DETAILS1) print_par_tree(Tree);
    // Calculate xij flows for the Tree (array of parents form)
    calc_X();
    // Print xij flows
//*/if(DETAILS) print_X();
    // calculate dual variables w
    calc_w();
    // print w
//*/if(DETAILS1) print_w();
    // Calculate sij values
    calc_s();
    // Print sij and wi values
//*/if(DETAILS1) print_s();
    // It needs the X flows in FROM[], TO[] form to calculate direction D
    calc_X_flows();
//*/print_X_flows();
    // Check sij values. If it is sij>=0 then it is really a dual feasible solution
//*/    if(check_s()!=0)
//*/    {
//*/fprintf(OUT,"\n-Not Dual Feasible solution\n");
//*/        *niter=0;
//*/        return 6;
//*/        }
//*/else fprintf(OUT,"\n\n+Dual Feasible solution\n");

    // Check whether the initial dual feasible tree is also feasible (ie optimal)
    if(check_X()==0) {
//*/fprintf(OUT,"\n+Primal Feasible Solution\n");
            optimal=1;
            // Calcute and print objective value
            *objval=calcZ();
//*/fprintf(OUT,"\nObjective value:%0.2f\n",*objval);
            *niter=0;
            return 0;
            }
	// Formulate set I that shows which flows are negative
    calc_I();
//*/if(DETAILS) print_I();
    *objval=calcZ();
//*/fprintf(OUT,"\nObjective value:%0.2f\n",*objval);
    // Start of Iterations
    while(!optimal){
       iteration++;
       //*/fprintf(OUT,"\nIteration %d\n------------------------------\n",iteration);
       // Calculate the minimum negative flow
       find_xkl(&th,&thesi1,&pos1);
//*/if(DETAILS)fprintf(OUT,"\nxkl=%0.1f thesi:%d in parent's index:%d",th,pos1,thesi1);
//*/if(DETAILS)fprintf(OUT,"\nLeaving arc:(%d,%d)",FROM[pos1],TO[pos1]);
       // Calculate the minimum -sij in order to find the entering arc
       sgh=calc_enter(&thesi,thesi1);
//*/if(DETAILS) printf("\nsgh=%f for entering arc:(%d,%d)",sgh,FROM[thesi],TO[thesi]);
///      // Renews the basic tree
      renew_tree(thesi1+1,FROM[thesi],TO[thesi]);
        // Print the new Basic Tree
//*/if(DETAILS)fprintf(OUT,"\n\nNew Tree:\n");
//*/if(DETAILS) print_tree(BasicTree);
        // Convert it into parents-array form
        convert();
//*/if(DETAILS1)print_par_tree(Tree);
        // Calculate and print xij flows
        calc_X();
//*/if(DETAILS)print_X();
        // calculate dual variables w
        calc_w();
//*/if(DETAILS1) print_w();
        // calculate and print sij values
        calc_s();
//*/if(DETAILS1)print_s();
        calc_X_flows();
        if(check_X()==0){
//*/fprintf(OUT,"\n+Primal Feasible");
            optimal=1;
            }
       // Calculate and print objective value
        *objval=calcZ();
//*/fprintf(OUT,"\nObjective value:%0.2f\n",*objval);
//*/fprintf(OUT,"\n------------------------------\n");
        // Renew set I
        calc_I();
    } // End of While
     // Set objective value (returned as parameter)
      time2=clock();
      *cputime = (time2 - time1)/(double)(CLOCKS_PER_SEC );
//      // number of iterations
      *niter=iteration;
      if(check_X()==0)
			   if(check_s()==0) return 0;
	   		   else return 1;
	  else
	  	   if(check_s()==0) return 2;
  		   else return 3;
 }

 // Calculates xkl
 // thesi_i is the position of the leaving arc in FROM-TO form
 // thesi is the position of the leaving arc in parents-array format
void find_xkl(double *th,int *thesi,int *thesi_i){
     int i;
    *th=MAX_INT;
    *thesi_i=-1;
    for(i=0;i<DENSITY1;i++){
    if(I[i]==-1) if(X_flows[i]<*th) {
                 *th=X_flows[i];
                 *thesi_i=i;
                 }
    }
    *thesi=get_pos_inpar(FROM[*thesi_i],TO[*thesi_i]);
    }

double calc_enter(int *thesi,int leaving_pos){
       int i;
       double min=MAX_INT;
       *thesi=-1;
       for(i=0;i<DENSITY1;i++){
               if(min>s[i] && !is_basic(FROM[i],TO[i]) && cycling(FROM[i],TO[i],leaving_pos+1)==-1){
               min=s[i];
               *thesi=i;
               }
       }
       return min;
       }
