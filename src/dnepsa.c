#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dnepsa.h"
#include "graph.h"
#include "netgen.h"
#include "main.h"
#include "dualtree.h"


// Returns 0 if the solution is primal feasible and dual feasible
// Returns 1 if the solution is primal feasible but not dual feasible
// Returns 2 if the solution is not primal feasible and dual feasible
// Returns 3 if the solution is not primal feasible and not dual feasible
// Returns 4 if the graph is disconnected
// Returns 5 if the problem is infeasible
// Returns 6 if problem is unbounded (in this case the algorithm caqnnot find
//           an initial dual feasible solution)
// Returns 7 if the algorithm cannot find theta (infinite loop???)

//if getgraph is 1 then function get_graph() is called in order to read a graph
//if it is 0 then the graph has already been loaded


int dnepsa(double *objval,int getgraph,long *niter, double *cputime){
    // position of leaving arc in FROM TO array
    int pos;
    // position of leaving arc in array of parents form
    int pos_p;
    // theta value
    double theta;
    // thesi: the position chosen (by the minimum ratio) for the entering arc
    int thesi;
    int pivot_type;
    // iteration is the No of the current iteration
    long iteration=0;
    // optimal becomes 1 when we find an optimal solution
    int optimal=0;
    // Initialization of parameters
    //if(COMMAND_LINE==0) set_parms(parms);
    // Loading graph
    if(getgraph==1)get_graph();
    clock_t time1,time2;
    // take time
    time1=clock();
    // If TOFILE=1 then the results are stored to a file
    // Print indormation about the graph
    //print_graph();
    //*/if(SHOWSTEPS) getchar();
    // if GIDEN is 1 then store the graph in GIDEN's format
    //*/if(GIDEN) print_to_giden();

    // Find an initial dual feasible tree BasicTree
    // If the graph is disconnected, find_dual_tree returns 4 and we have to stop
    if(find_dual_tree()==1) {
                        return 4;
                        }
    //if STORE_GRAPH is defined then store the graph into file STORE_FILE
    //if(STORE_GRAPH) store_graph(STORE_FILE);
    // if STORE_ARRAYS is defined then stores problem in arrays format
    //if (STORE_ARRAYS) store_arrays();
    // Print the initial dual feasible tree
    //*/fprintf(OUT,"\nInitial Dual Feasible Tree:\n");
    //*/print_tree(BasicTree);
    //*/fprintf(OUT,"\nInitial Dual Feasible Tree:\n");
    //*/print_tree(BasicTree);
    //*/if(SHOWSTEPS) getchar();
    // Convert BasicTree into parents-array form.
    // It produces Tree which is of this form
    // It also fills in array C_par which contains the costs per unit cij
    // for the arcs of the tree

    //convert(Tree,BasicTree);
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
    // Check sij values. If it is sij>=0 then it is really a dual feasible solution
//    if(check_s()!=0){
//        fprintf(OUT,"\n-Not Dual Feasible solution\n");
//        *niter=0;
//        return 6;
//        }
    //*/else fprintf(OUT,"\n\n+Dual Feasible solution\n");

    // Check whether the initial dual feasible tree is also feasible (ie optimal)
    if(check_X()==0) {
            //*/fprintf(OUT,"\n+Primal Feasible Solution\n");
            time2=clock();
            *cputime = (time2 - time1)/(double)(CLOCKS_PER_SEC );
            // Calcute and print objective value
            *objval=calcZ();
            *niter=0;
            return 0;
            }
	// Formulate sets I_ and I+. These are both kept in array I
    calc_I();
    //*/if(DETAILS) print_I();
    // Calculate D direction
    calc_D();
    //*/if (DETAILS1) print_D();
    // Form set J_
    int j_=calc_J();
    // Print set J_
    //*/if(DETAILS) print_J_();
    // Start of Iterations
    while(!optimal){
       iteration++;
       //*/fprintf(OUT,"\n------------------------------\n");
       // If J_=empty then infeasible problem
       // It is sure here that I_ is not empty (we have check_X ....)
         if(j_==1){
            //*/fprintf(OUT,"\nThe problem is infeasible!!!");
            return 5;
            }
       // Calculate the minimum ratio -sij/dij and return position thesi
       // this gives the entering arc
       logos=calc_logos(&thesi);
       // Print details about entering arc and minimum ratio
       //*/if(DETAILS) fprintf(OUT,"\nMinimum ratio=%0.1f for arc (%d,%d).",logos,FROM[thesi],TO[thesi]);
       //*/if(DETAILS) fprintf(OUT,"\Entering arc: (%d,%d)",FROM[thesi],TO[thesi]);

       pivot_type=find_theta(&theta,&pos_p,thesi);
       // if cannot find theta (infinite loop)
       if(pivot_type==3) {
            time2=clock();
            *cputime = (time2 - time1)/(double)(CLOCKS_PER_SEC );
            *objval=calcZ();
            *niter=iteration;
            return 7;
       }
       renew_DS(pos_p,thesi,pivot_type);
       renew_tree(pos_p+1,FROM[thesi],TO[thesi]);
       if(Orientation[pos_p]==0) pos=get_index(pos_p+1,Tree[pos_p]);
       else pos=get_index(Tree[pos_p],pos_p+1);
       //Take the leaving arc out of set I_.
       I[pos]=0;
       I[thesi]=1;  // The entering arc goes into I+
       // Convert tree it into parents-array form
       //convert(Tree,BasicTree);
       convert();
       //*/if(DETAILS1)print_par_tree(Tree);
       // Calculate and print xij flows
       calc_X();
       //*/if(DETAILS)print_X();
       // calculate dual variables w
       //calc_w();
       //*/if(DETAILS1) print_w();
       // calculate and print sij values
       //calc_s();
       //*/if(DETAILS1)print_s();
       if(check_X()==0){
           //*/fprintf(OUT,"\n+Primal Feasible");
           optimal=1;
           }
        //*/else
            //*/fprintf(OUT,"\n-Not Primal Feasible");
        //*/if(check_s()==0)
            //*/fprintf(OUT,"\n+Dual Feasible");
        //*/else
            //*/fprintf(OUT,"\n-Not Dual Feasible");
        // Calculate and print objective value
        //z=calcZ();
        //*/fprintf(OUT,"\nObjective value:%0.2f\n",z);
        //*/fprintf(OUT,"\n------------------------------\n");
        calc_X_flows();
        //calc_D();
        //*/if (DETAILS1) print_D();
        j_=calc_J();
      } // End of While
      // Set objective value (returned as parameter)
      time2=clock();
      *cputime = (time2 - time1)/(double)(CLOCKS_PER_SEC );
      *objval=calcZ();
      // number of iterations
      *niter=iteration;
      if(check_X()==0)
			   if(check_s()==0) return 0;
	   		   else return 1;
	  else
	  	   if(check_s()==0) return 2;
  		   else return 3;
 }


// I contains -1 if the corresponding arc belongs to I_
// I contains 1 if  the arc belongs to I+ and 0 for the non-basic arcs
// i.e. contains both I_ and I+
void calc_I(){
     int i;
     for(i=0;i<DENSITY1;i++){
	 	if(!is_basic(FROM[i],TO[i])) I[i]=0;
        else if(X_flows[i]<0) I[i]=-1;
        else I[i]=1;
        }
     }

//// Print sets I_ and I+
//void print_I(){
//	 int i;
//     fprintf(OUT,"\nI_=[");
//     for (i=0;i<DENSITY1;i++)
//         if(I[i]==-1) fprintf(OUT," (%d,%d) ",FROM[i],TO[i]);
//     fprintf(OUT,"]\n");
//     fprintf(OUT,"\nI+=[");
//     for (i=0;i<DENSITY1;i++)
//         if(I[i]==1) fprintf(OUT," (%d,%d) ",FROM[i],TO[i]);
//     fprintf(OUT,"]\n");
//	 }

// J is an array with a value for every arc
// It contains 1 if the corresponding arc belongs to J_
// otherwise it contains 0
// returns 1 if J_ is empty
int calc_J(){
     int i;
     int j=1;
     for(i=0;i<DENSITY1;i++)
        // if D[i]<0 then it is for sure negative
        if(D[i]<0) {
                 J[i]=1;
                 j=0;
                 }
        else
             J[i]=0;
     return j;
     }

// Checks set J_. If J_ is empty it returns 1
// otherwise it returns 0 (not empty)
int check_J(){
    int i;
    //int empty=1;
    for(i=0;i<DENSITY1;i++)
        if(J[i]==1) return 0;
    return 1;
}

//// Prints set J_
//void print_J_(){
//     int i;
//     fprintf(OUT,"\nJ_ contains:\n");
//     for(i=0;i<DENSITY1;i++)
//        if(J[i]==1) fprintf(OUT,"   (%d,%d) with s=%0.1f\n",FROM[i],TO[i],s[i]);
//     }

// Calculates direction D which gives the second dual feasible flow yij
void calc_D(){
     int i;
	 // for the basic arcs it is dij=1 if xij<0 otherwise dij=0
     for(i=0;i<DENSITY1;i++)
         if(is_basic(FROM[i],TO[i]))
             if(X_flows[i]<0) D[i]=1;
             else D[i]=0;
         // for the non basic arcs calculate the sum of hij for the basic
		 // arcs (i,j) for which it is xij<0
         else
            D[i]=get_sumh(i);
     }


// It computes the minimum ratio sij/-dij
// thesi gives the position of the minimum ratio
double calc_logos(int *thesi){
     int i;
     double min=MAX_INT;
     *thesi=-1;
     for(i=0;i<DENSITY1;i++){
          if(J[i]==1)
           if(min>-s[i]/D[i]){
   					  min = -s[i]/D[i];
				 	  *thesi=i;
		   			  }
          }
     return min;
     }


//// Calculates flow yij which is dual feasible
//void calc_Y_flows(){
//     int i;
//     for(i=0;i<DENSITY1;i++)
//          Y_flows[i]=X_flows[i]+logos*D[i];
//}

//// Prints flow yij
//void print_Y_flows(){
//     int i;
//     printf("\nY-Flows:\n");
//     for(i=0;i<DENSITY1;i++)
//          printf("Y(%d,%d)=%d ",FROM[i],TO[i],Y_flows[i]);
//}
//
//// Checks flow Y
//int check_Y(){
//    int i;
//    for(i=0;i<DENSITY1;i++){
//       if (Y_flows[i]<(-1)*E) return 1;
//       }
//    return 0;
//    }


// Calculate the leaving arc position
// returns 0 if type A iteration
// returns 1 if type B iteration
// pos is the position of the leaving arc (in FROM TO table)
// thesi is the position for the entering arc
int find_theta(double *theta,int *pos,int thesi){
     int i;
     double theta1,theta2;
     int thesi1,thesi2;
     //int thesi_p;
     theta1=MAX_INT;
     thesi1=-1;
     theta2=MAX_INT;
     thesi2=-1;
     for(i=0;i<NODES1;i++){
         if( X[i]<0 && cycling(FROM[thesi],TO[thesi],i+1) == -1 ) {
                if(theta1>=-X[i]) {

                   theta1=-X[i];
                   thesi1=i;
                   }
         }
     // if arc FROM[i]->TO[i] belongs to I+ and is of opposite orientation as leaving arc
     else if( X[i]>=0 && cycling(FROM[thesi],TO[thesi],i+1) == 1 ) {
                if(theta2>=X[i]) {
                   theta2=X[i];
                   thesi2=i;
                   }
         }
     }
     if(theta1<=theta2) {
                  if(theta1==MAX_INT) return 3;
                  *theta=theta1;
                  *pos=thesi1;
                  return 0;
                  }
     else         {
                  *theta=theta2;
                  *pos=thesi2;
                  return 1;
                  }
}

// Renews vector D. It takes as parameters the position of the leaving arc
// (in parents' array), the position of the entering arc (in FROM-TO form)
// and the pivot type
void renew_DS(int pos_p,int thesi,int pivot_type){
     int i;
     int dgh=D[thesi];
     double sgh=s[thesi];
     if(pivot_type==0)
         for(i=0;i<DENSITY1;i++){
                                 if(cycling(FROM[i],TO[i],pos_p+1)==1){
                                                                      D[i]+=dgh;
                                                                      s[i]+=sgh;
                                                                      }
                                 else if(cycling(FROM[i],TO[i],pos_p+1)==-1){
                                                                      D[i]-=dgh;
                                                                      s[i]-=sgh;
                                                                      }
                                 }
     else
         for(i=0;i<DENSITY1;i++){
                                 if(cycling(FROM[i],TO[i],pos_p+1)==1) {
                                                                      D[i]-=dgh;
                                                                      s[i]-=sgh;
                                                                      }
                                 else if(cycling(FROM[i],TO[i],pos_p+1)==-1){
                                                                      D[i]+=dgh;
                                                                      s[i]+=sgh;
                                                                      }
                                 }


     }
