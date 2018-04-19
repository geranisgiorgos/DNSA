#include <stdio.h>
#include "graph.h"
#include "netgen.h"
#include "dnepsa.h"
#include "main.h"

// Set the parameters of the dual tree to be generated
void set_parms(long parms[],char *arguments[]){

    if(COMMAND_LINE==1) {
        parms[0]=NODES=atoi(arguments[3]); // nodes         /*comment for gener_graphs*/
        //parms[0]=NODES=atoi(arguments[2]); // nodes           /*uncomment for gener_graphs*/
        //parms[1]=SOURCES=atoi(arguments[4]); // sources       /*uncomment for gener_graphs*/
        //parms[2]=SINKS=atoi(arguments[5]); // sinks           /*uncomment for gener_graphs*/
        parms[3]=DENSITY=atoi(arguments[4]); // density     /*comment for gener_graphs*/
        //parms[3]=DENSITY=atoi(arguments[4]); // density       /*uncomment for gener_graphs*/
        //parms[4]=MINCOST=atoi(arguments[8]); // mincost       /*uncomment for gener_graphs*/
        //parms[5]=MAXCOST=atoi(arguments[9]); // maxcost       /*uncomment for gener_graphs*/
        //parms[6]=SUPPLY=atoi(arguments[10]); // total supply  /*uncomment for gener_graphs*/
        //parms[7]=TSOURCES=atoi(arguments[6]); // tsources     /*uncomment for gener_graphs*/
        //parms[8]=TSINKS=atoi(arguments[7]); // tsinks         /*uncomment for gener_graphs*/
        //parms[9]=HICOST=atoi(arguments[11]); // hicost        /*uncomment for gener_graphs*/

    } else{
        parms[0]=NODES1;		    /* number of nodes */
        parms[1]=SOURCES;		/* number of sources (including transshipment) */
        parms[2]=SINKS;      	/* number of sinks (including transshipment) */
        parms[3]=DENSITY1;		/* number of (requested) arcs */
        parms[4]=MINCOST;		/* minimum cost of arcs */
        parms[5]=MAXCOST;		/* maximum cost of arcs */
        parms[6]=SUPPLY;		/* total supply */
        parms[7]=TSOURCES;		/* transshipment sources */
        parms[8]=TSINKS;		/* transshipment sinks */
        parms[9]=HICOST;		/* percent of skeleton arcs given maximum cost */
        parms[10]=CAPACITATED;	/* percent of arcs to be capacitated */
        parms[11]=MINCAP;		/* minimum capacity for capacitated arcs */
        parms[12]=MAXCAP;		/* maximum capacity for capacitated arcs */
    }
}

// reads graph from netgen, or from file or by hand
// depending on NETGEN, FROMFILE definitions
int get_graph(){
    if (NETGEN){
        // Generate a random graph by using netgen. SEED is the random number seed
        netgen(seed, parms);
        //print_graph();
    }
    else if(FROMFILE){
        //printf("Reading from file...%s",INFILE);
        // read the graph from a file where it is stored
        read_from_file(INFILE);
        }
    else
        // Give a graph by hand
        set_graph();
    return 0;
}

// Function to set a graph by hand
int set_graph(){
/***Example 1*********
    // Set supplies
     B[0]=8;
     B[1]=8;
     B[2]=0;
     B[3]=-9;
     B[4]=-7;
     B[5]=0;
     B[6]=0;

     // Set arcs and costs

     FROM[0]=2; TO[0]=1; C[0]=8;
     FROM[1]=3; TO[1]=1; C[1]=20;
     FROM[2]=1; TO[2]=6; C[2]=7;
     FROM[3]=1; TO[3]=7; C[3]=5;
     FROM[4]=2; TO[4]=3; C[4]=1;
     FROM[5]=3; TO[5]=4; C[5]=1;
     FROM[6]=7; TO[6]=3; C[6]=4;
     FROM[7]=5; TO[7]=4; C[7]=9;
     FROM[8]=4; TO[8]=7; C[8]=9;
     FROM[9]=7; TO[9]=4; C[9]=9;
     FROM[10]=6; TO[10]=5; C[10]=8;
     FROM[11]=7; TO[11]=5; C[11]=8;
     FROM[12]=7; TO[12]=6; C[12]=6;
     return 13;
*****/

/****** Example 2 *******
     B[0]=0;
     B[1]=0;
     B[2]=-6;
     B[3]=-6;
     B[4]=-2;
     B[5]=9;
     B[6]=5;


     FROM[0]=2; TO[0]=1; C[0]=7;
     FROM[1]=1; TO[1]=3; C[1]=48;
     FROM[2]=1; TO[2]=4; C[2]=28;
     FROM[3]=1; TO[3]=5; C[3]=10;
     FROM[4]=6; TO[4]=1; C[4]=56;
     FROM[5]=2; TO[5]=3; C[5]=65;
     FROM[6]=4; TO[6]=2; C[6]=38;
     FROM[7]=2; TO[7]=5; C[7]=7;
     FROM[8]=6; TO[8]=2; C[8]=48;
     FROM[9]=7; TO[9]=2; C[9]=33;
     FROM[10]=6; TO[10]=3; C[10]=108;
     FROM[11]=4; TO[11]=5; C[11]=15;
     FROM[12]=7; TO[12]=5; C[12]=19;
     FROM[13]=6; TO[13]=7; C[13]=24;
     return DENSITY1;


******************/

/****** Example 3 *******
     B[0]=2;
     B[1]=8;
     B[2]=0;
     B[3]=0;
     B[4]=-6;
     B[5]=-4;

     FROM[0]=1; TO[0]=2; C[0]=3;
     FROM[1]=1; TO[1]=3; C[1]=1;
     FROM[2]=2; TO[2]=3; C[2]=3;
     FROM[3]=2; TO[3]=4; C[3]=1;
     FROM[4]=2; TO[4]=5; C[4]=2;
     FROM[5]=3; TO[5]=4; C[5]=2;
     FROM[6]=4; TO[6]=3; C[6]=2;
     FROM[7]=3; TO[7]=6; C[7]=8;
     FROM[8]=4; TO[8]=5; C[8]=4;
     FROM[9]=4; TO[9]=6; C[9]=1;

****/
/**** Example 4
     B[0]=2;
     B[1]=8;
     B[2]=0;
     B[3]=0;
     B[4]=-6;
     B[5]=-4;

     FROM[0]=1; TO[0]=2; C[0]=3;
     FROM[1]=1; TO[1]=3; C[1]=1;
     FROM[2]=2; TO[2]=3; C[2]=3;
     FROM[3]=2; TO[3]=4; C[3]=1;
     FROM[4]=2; TO[4]=5; C[4]=2;
     FROM[5]=3; TO[5]=4; C[5]=2;
     FROM[6]=4; TO[6]=3; C[6]=2;
     FROM[7]=3; TO[7]=6; C[7]=8;
     FROM[8]=4; TO[8]=5; C[8]=4;
     FROM[9]=4; TO[9]=6; C[9]=1;

     return 10;
***/
/*****
     B[0]=5;
     B[1]=5;
     B[2]=0;
     B[3]=0;
     B[4]=-6;
     B[5]=-4;

     FROM[0]=1; TO[0]=2; C[0]=2;
     FROM[1]=2; TO[1]=1; C[1]=1;
     FROM[2]=2; TO[2]=3; C[2]=3;
     FROM[3]=3; TO[3]=2; C[3]=4;
     FROM[4]=4; TO[4]=5; C[4]=7;
     FROM[5]=5; TO[5]=4; C[5]=5;
     FROM[6]=5; TO[6]=6; C[6]=3;
     FROM[7]=6; TO[7]=5; C[7]=2;
     FROM[8]=1; TO[8]=4; C[8]=8;
     FROM[9]=4; TO[9]=1; C[9]=4;
     FROM[10]=2; TO[10]=5; C[10]=2;
     FROM[11]=5; TO[11]=2; C[11]=1;
     FROM[12]=3; TO[12]=6; C[12]=5;
     FROM[13]=6; TO[13]=3; C[13]=6;
     FROM[14]=1; TO[14]=5; C[14]=2;
     FROM[15]=5; TO[15]=1; C[15]=3;
     FROM[16]=5; TO[16]=3; C[16]=4;
     FROM[17]=3; TO[17]=5; C[17]=3;
     FROM[18]=2; TO[18]=4; C[18]=2;
     FROM[19]=4; TO[19]=2; C[19]=1;
     FROM[20]=2; TO[20]=6; C[20]=2;
     FROM[21]=6; TO[21]=2; C[21]=2;


     return 14;
*****/
/*** Example new-1 ****
B[0]=4;
B[1]=1;
B[2]=5;
B[3]=4;
B[4]=-6;
B[5]=-4;
B[6]=-4;

FROM[0]=1;TO[0]=7;C[0]=10;
FROM[1]=1;TO[1]=5;C[1]=120;
FROM[2]=2;TO[2]=5;C[2]=47;
FROM[3]=2;TO[3]=7;C[3]=16;
FROM[4]=3;TO[4]=6;C[4]=122;
FROM[5]=3;TO[5]=7;C[5]=100;
FROM[6]=4;TO[6]=6;C[6]=81;
FROM[7]=4;TO[7]=5;C[7]=96;
FROM[8]=5;TO[8]=6;C[8]=129;
FROM[9]=5;TO[9]=7;C[9]=28;
FROM[10]=6;TO[10]=7;C[10]=90;
FROM[11]=6;TO[11]=4;C[11]=33;
FROM[12]=6;TO[12]=3;C[12]=67;
*********/

/*** Example paper-Balcor ****/
B[0]=3;
B[1]=5;
B[2]=3;
B[3]=3;
B[4]=-6;
B[5]=-8;

FROM[0]=1;TO[0]=5;C[0]=16;
FROM[1]=1;TO[1]=6;C[1]=41;
FROM[2]=2;TO[2]=5;C[2]=57;
FROM[3]=2;TO[3]=6;C[3]=104;
FROM[4]=3;TO[4]=5;C[4]=107;
FROM[5]=3;TO[5]=6;C[5]=130;
FROM[6]=4;TO[6]=5;C[6]=32;
FROM[7]=4;TO[7]=6;C[7]=84;
FROM[8]=5;TO[8]=3;C[8]=71;
FROM[9]=6;TO[9]=3;C[9]=43;
FROM[10]=6;TO[10]=4;C[10]=0;
FROM[11]=6;TO[11]=5;C[11]=1;
/*********/

return DENSITY1;
}

// Prints the graph onto the screen
void print_graph(){
     int i, arcs=DENSITY1;
     fprintf(OUT,"\n--------------------------\n");
     fprintf(OUT,"GRAPH\n--------------------------\n");
     fprintf(OUT,"Arcs\n");
     for (i=0;i<arcs;i++)
              fprintf(OUT,"%d -> %d c=%d\n",FROM[i],TO[i],C[i]);
     fprintf(OUT,"\n------------------\n");
     fprintf(OUT,"Nodes\n");
     for(i=0;i<NODES1;i++)
              fprintf(OUT,"%d b=%d\n",i+1,B[i]);
     fprintf(OUT,"\n--------------------------\n");
}

// Calculates the sum of all costs cij in the graph
double sum_costs(){
      double sum=0;
      int i;
      for (i = 0;i<DENSITY1;i++)
          sum +=C[i];
      return sum;
      }

// Prints a tree that is an array of 2 rows containing the arcs, i.e. the 1st
// row contains the FROM nodea and the 2nd row contains the TO nodes
void print_tree(int Tree[2][NODES1-1]){
     int i;
     for(i=0;i<NODES1-1;i++){
         fprintf(OUT,"(%d,%d) ",Tree[0][i],Tree[1][i]);
         if ((i+1)%6==0) fprintf(OUT,"\n");
         }
     fprintf(OUT,"\n");
}

// Initialize the parents-array tree
void init_tree_par(){
     int i;
     for(i=0;i<NODES1;i++){
         Tree[i]=0;
         Orientation[i]=0;
         X[i]=0;
         Checked[i]=0;
         }
}

// Prints the basic tree which is in the parents form
void print_par_tree(int Tree[NODES1]){
     int i;
     fprintf(OUT,"\nParents Tree: [");
     for(i=0;i<NODES1;i++)
          fprintf(OUT," %d ",Tree[i]);
     fprintf(OUT,"]\n");
     fprintf(OUT,"\nOrientations: [");
     for(i=0;i<NODES1;i++)
          fprintf(OUT," %d ",Orientation[i]);
     fprintf(OUT,"]\n");

}

// Converts a tree into parents-array form
// Uses function convert_tree
//void convert(int *T,int *DualT[2]){
void convert(){
       init_tree_par();
       //Tree[ROOT-1]=-1;  //We put -1 for the root
       Tree[ROOT-1]=-1;
       //convert_tree(DualT,T,ROOT);
       convert_tree(ROOT);
       // Fill in the C_par array which contains the costs cij
       // for the arcs in the current tree
       fill_Cpar();
}

// Converts a tree from the arcs form (in a 2xNODES1 array) to parents array form
// Orientation contains the Orientations of the arcs
// C_par contains the cij values for the arcs
// Used by function convert
/*void convert_tree(int *Tree_par,int cur){
     int i;
     Checked[cur-1]=1;
     for(i=0;i<NODES1-1;i++){
          if (BasicTree0[i]==cur){
             if( Checked[BasicTree1[i]-1] == 0){
                      Tree_par[BasicTree1[i]-1]=cur;
                      Orientation[BasicTree1[i]-1]=1;
                      convert_tree(Tree_par,BasicTree1[i]);
                      }
             }
          if (BasicTree1[i]==cur){
             if( Checked[BasicTree0[i]-1] == 0) {
                        // If we have arc (i,p(i)) then orientation=0
                        // or we could put cur into Tree_par
                          Tree_par[BasicTree0[i]-1]=cur;
                          Orientation[BasicTree0[i]-1]=0;
                          convert_tree(Tree_par,BasicTree0[i]);
                          }
             }
          }
}
*/

void convert_tree(int cur){
     int i;
     Checked[cur-1]=1;
     for(i=0;i<NODES1-1;i++){
          if (BasicTree0[i]==cur){
             if( Checked[BasicTree1[i]-1] == 0){
                      Tree[BasicTree1[i]-1]=cur;
                      Orientation[BasicTree1[i]-1]=1;
                      convert_tree(BasicTree1[i]);
                      }
             }
          if (BasicTree1[i]==cur){
             if( Checked[BasicTree0[i]-1] == 0) {
                        // If we have arc (i,p(i)) then orientation=0
                        // or we could put cur into Tree_par
                          Tree[BasicTree0[i]-1]=cur;
                          Orientation[BasicTree0[i]-1]=0;
                          convert_tree(BasicTree0[i]);
                          }
             }
          }
}


// Renews tree
void renew_tree(int exerx,int eiserx1,int eiserx2){
  int i;
  for(i=0;i<NODES1-1;i++){
    if(BasicTree0[i] == exerx && BasicTree1[i] == Tree[exerx-1]){
        //if(DETAILS) fprintf(OUT,"\n\nPut into Basic Tree arc (%d,%d) for",eiserx1,eiserx2);
        //if(Orientation[exerx-1]==0) {if(DETAILS) fprintf(OUT," (%d,%ld)",exerx,Tree[exerx-1]);}
        //else if(DETAILS) fprintf(OUT," (%d,%d)",Tree[exerx-1],exerx);
        BasicTree0[i]=eiserx1;
        BasicTree1[i]=eiserx2;
        break;
        }
    else if(BasicTree0[i] == Tree[exerx-1] && BasicTree1[i] == exerx){
        //if(DETAILS) fprintf(OUT,"\n\nPut into Basic Tree arc (%d,%d) for",eiserx1,eiserx2);
        //if(Orientation[exerx-1]==0) {if(DETAILS) fprintf(OUT," (%d,%d)",exerx,Tree[exerx-1]);}
        //else if(DETAILS) fprintf(OUT," (%d,%d)",Tree[exerx-1],exerx);
        BasicTree0[i]=eiserx1;
        BasicTree1[i]=eiserx2;
        break;
        }

  }
}

// fills in array C_par which contains the cij values for arcs (i,p(i))
//i.e. in parents form for the tree
void fill_Cpar(){
    int i,j=0,found=0;
    for(i=0;i<NODES1;i++){
        found=0; j=0;
        while(!found && j<DENSITY1){
            if ((FROM[j] == i+1 && TO[j] == Tree[i] && Orientation[i] == 0) ||
                (FROM[j] == Tree[i] && TO[j] == i+1 && Orientation[i] == 1)){
                found=1;
                C_par[i]=C[j];
                }
            j++;
            }
        }
    }

// Calculates the xij values. Tree is in the form of table of parents
// Sup contains the supplies
void calc_X(){
     int i,notend=1;
     int Tree1[NODES1];
     long B1[NODES1];
     // Make copies of B and Tree
     for(i=0;i<NODES1;i++) {
          Tree1[i]=Tree[i];
          B1[i]=B[i];
          }
     // We pass the Tree table
    while(notend){
     notend=0;
     i=0;
     while(i<NODES1){
          //if a node does not exist then we can compute its xij
          if (Tree1[i]!=-1 && !exists_in_tree_par(i+1,Tree1)) {
             notend=1;
             // compute xij value and new B value...
             if(Orientation[i]==1) {X[i]=-B1[i];B1[Tree1[i]-1]-=X[i];}
             else {X[i]=B1[i];B1[Tree1[i]-1]+=X[i];}
             // if an edge has been computed set it to -1 (finished)
             Tree1[i]=-1;
             }
          i++;
          }
     }
}

//Calculates the value of the objective function ...
double calcZ(){
    double z=0;
    int i;
    for(i=0;i<NODES1;i++)
        z+=C_par[i]*X[i];
    return z;
}

// Returns 1 if x exists into a tree in parents form
//i.e. if x is a parent node
int exists_in_tree_par(int x, int tree[NODES1]){
    int i=0,found=0;
    while(i<NODES1 && !found){
         if (tree[i] == x) found=1;
         i++;
         }
    return found;
    }

//Prints xij flows
void print_X(){
     int i;
     fprintf(OUT,"\nX-Flow:\n\n");
     for(i=1;i<NODES1;i++){
       if(Orientation[i]==0)
          fprintf(OUT,"X(%d,%d)=%d ",i+1,Tree[i],X[i]);
       else
          fprintf(OUT,"X(%d,%d)=%d ",Tree[i],i+1,X[i]);
       if(i%5==0) fprintf(OUT,"\n");
       }
     }

//Returns 0 if it is >=0 for each xij, otherwise 1
int check_X(){
     int i;
     for(i=0;i<NODES1;i++){
           if(X[i]<0) return 1;
           }
    return 0;
}

// returns 1 if flow X is ekfylismeno
// else returns 0
int isekfyl(){
     int i;
     for(i=0;i<NODES1;i++){
           if(X[i]==0) return 1;
           }
    return 0;
}

// takes an arc x ->  y as  argument and returns the index (position) in the
// FROM[] TO[] arrays where it is stored
int get_index(int x,int y){
	int i;
	for(i=0;i<DENSITY1;i++)
 	    if(FROM[i]==x)
 	         if(TO[i]==y) return i;
    return -1;
}

// Calculates the sij values for a graph
void calc_s(){
     int i;
     for(i=0;i<DENSITY1;i++){
         s[i]=C[i]+w[FROM[i]-1]-w[TO[i]-1];
         }
}

// Print the sij values
void print_s(){
     int i;
     fprintf(OUT,"\n\n--------------------------------\n");
     fprintf(OUT,"\ns values:\n");
     for(i=0;i<DENSITY1;i++){
         fprintf(OUT,"s%d.%d=%0.1f ",FROM[i],TO[i],s[i]);
         }
     fprintf(OUT,"\n--------------------------------\n");
}

//Returns 0 if it is >=0 for each sij, otherwise 1
int check_s(){
     int i;
     // we don't haev to check artificial arcs
     // so, the loop goes up to DENSITY (not DENSITY1)
     for(i=0;i<DENSITY;i++){
           if(s[i]<-E) {
		   			   return 1;
					   }
           }
    return 0;
}

// Calculates the wi values for every node i
void calc_w(){
    int i,k,w1[NODES1];
    int finish=0;
    for (i=0;i<NODES1;i++){
        w1[i]=0;
    }
    w[ROOT-1]=0;
    w1[ROOT-1]=1;
    while(!finish){
     finish=1;
     k=0;
     while(k<NODES1){
      for(i=0;i<NODES1;i++)
        if(w1[i]==0 && Tree[i] == k+1 && w1[k] == 1) {
            finish=0;
            if (Orientation[i] == 1) w[i]=w[k]+C_par[i];    // find cij!!! Check also Orientation
            else w[i]=w[k]-C_par[i];
            w1[i]=1;
            }
     k++;
     }
    }
}

// Prints wi values
void print_w(){
    int i;
    fprintf(OUT,"W values:\n");
    for(i=0;i<NODES1;i++){
        fprintf(OUT,"w[%d]=%0.1f\n",i+1,w[i]);
    }
    fprintf(OUT,"\n");
}

// Returns 1 if x exists in the array of parents (ie if it is a parent)
// It takes 2 nodes and detects the cycle where they belong
// Actually the 2 nodes are the edges of the entering arc
// Array Cycle contains either +1 or -1 for the nodes that belong to the cycle
// and 0 for the others. This function uses function cycling(u,v,w) which returns
// either 0 (if ,v,w not all into the cycle) or 1 or -1 (depending on the orientation)
void find_cycle(int u,int v){
     int i;//,j;
     for(i=0;i<NODES1;i++){
        Cycle[i]=cycling(u,v,i+1);
        }
    //*/if(DETAILS1) fprintf(OUT,"\nCycle: (%d,%d) ",u,v);
    ///for(i=0;i<NODES1;i++){
        ///j=Cycle[i];
        ///if(j!=0)
            ///if(Orientation[i]==0)
               ///{if(DETAILS1) fprintf(OUT," (%d,%d) ",i+1,Tree[i]);}
            ///else
               ///{if(DETAILS1) fprintf(OUT," (%d,%d) ",Tree[i],i+1);}
     ///}
    }

// u is a node and v is its parent. Orientation u -> v
// x is a node where (x,p(x)) belongs to the basic-tree
// The function returns 0 if (x,p(x)) does not belong to the cycle
// it returns -1 if (x,p(x)) belongs to the cycle and is of same
//  orientation as (u,v) and it returns 1 if is of opposite orientation
// We follow the parents of u. If we find x then it is found
// Do the same for v. If we cannot find x among the parents of u or v, then no cycle
// containing (x,p(x))
// To avoid the case where there is no cycle, but x belongs to the same clause as
// u and v we make use of tha variable same...
int cycling(int u,int v,int x){
    int found1=0,found2=0,u1,v1;
    u1=u-1;v1=v-1;
    while (!found1 && Tree[u1]!=-1){
        if(u1==x-1) {
            found1=1;
            }
        u1=Tree[u1]-1;
        }
    while (!found2 && Tree[v1]!=-1){
        if(v1==x-1) {
            found2=1;
            }
        v1=Tree[v1]-1;
        }
    if(found1 && !found2)
        if(Orientation[x-1]==1) return -1;
        else return 1;
    else if(found2 && !found1)
        if(Orientation[x-1]==1) return 1;
        else return -1;
    else return 0;
    }

// Returns 1 if an arc x->y is basic
int is_basic(int x, int y){
    int found=0;
    if((Tree[x-1]==y && Orientation[x-1]==0)||
       (Tree[y-1]==x && Orientation[y-1]==1)) found=1;
    return found;
    }

// Returns the flow for the arc x->y (it could be basic or non-basic arc)
// It takes the arc x->y as parameters and checks to find the arc into the
// parents array Tree and then takes the flow from array X
int get_flow(int x, int y){
    if (Tree[x-1]==y && Orientation[x-1]==0) return X[x-1];
    else if(Tree[y-1]==x && Orientation[y-1]==1) return X[y-1];
    else return 0;
}


// Fills in array X_flows. It takes every arc one by one, checks the corresponding flow and
// fills array X_flows
void calc_X_flows(){
     int i;
     for(i=0;i<DENSITY1;i++)
         if(is_basic(FROM[i],TO[i])) X_flows[i]=get_flow(FROM[i],TO[i]);
         //X_flows[i]=get_flow(FROM[i],TO[i]);
}

//void print_X_flows(){
//     int i;
//     printf("X-Flows:\n");
//     for(i=0;i<DENSITY1;i++)
//          printf("X(%d,%d)=%d ",FROM[i],TO[i],X_flows[i]);
//}


int get_sumh(int i){
	int j,sum=0;
	for(j=0;j<NODES1;j++){
 			if(X[j]<0){
				sum+=cycling(FROM[i],TO[i],j+1);
				}
			}
	return sum;
}

// Takes an arc x->y and returns the position (index)
// where it corresponds into the parent's form array-tree (array named Tree)
int get_pos_inpar(int x,int y){
	int i;
	for(i=0;i<NODES1;i++)
	    if((i+1==x && Tree[i]==y && Orientation[i]==0) ||
		    (i+1==y && Tree[i]==x && Orientation[i]==1)) return i;
	return -1;
}

// read the graph format from data stored in a file
// the filename is specified by INFILE in dnepsa.h
// the data read is stored into arrays FROM, TO, C, B
int read_from_file(char *filename){
    int x1,x2,x3,x4,x5;
    int i,res;
    if((infile=fopen(filename,"r"))==NULL) {
                                         printf("Error opening file %s\n",filename);
                                         return 0;
                                         };

    while (fgetc(infile)!='\n');
    for (i=0;i<DENSITY;i++){
        res=fscanf(infile,"%d%d%d%d%d",&x1,&x2,&x3,&x4,&x5);
//fscanf(infile,"%d%d%d%d",&x1,&x2,&x3,&x4);
        FROM[i]=x1; TO[i]=x2;C[i]=x3;
        }
    for(i=0;i<NODES;i++){
        res=fscanf(infile,"%d%d",&x1,&x2);
//        fscanf(infile,"%d",&x2);
        B[x1-1]=x2;
        B[i]=x2;
        }
    fclose(infile);
    return 1;
}

//Stores the graph into file STORE_FILE
void store_graph(char *store_file){
    FILE *storef;
     int i;
    storef=fopen(store_file,"w");
    fprintf(storef,"%ld %ld\n",NODES1,DENSITY1);
    for(i=0;i<DENSITY1;i++){
        fprintf(storef,"%d %d %d 100000 0\n",FROM[i],TO[i],C[i]);
        }
    for(i=0;i<NODES1;i++){
        fprintf(storef,"%d %d\n",i+1,B[i]);
        }
    fclose(storef);
}

// Stores arrays with the problem formed in matrix notation (for use with matlab)
void store_arrays() {
        // Open the proper files
        FILE *out_A,*out_B,*out_b,*out_c;
        out_A=fopen(OUT_A,"w");
        out_B=fopen(OUT_B,"w");
        out_b=fopen(OUT_b,"w");
        out_c=fopen(OUT_c,"w");
        int p,q;
        //Store A into file without using internal array
        // we also store a last column containing 0 0...0 1
        // so that array A has rank equal to NODES1
        for(p=0;p<NODES1;p++){
            for(q=0;q<DENSITY1;q++){
                if(p==FROM[q]-1)fprintf(out_A,"1\t");
                else if(p==TO[q]-1)fprintf(out_A,"-1\t");
                else fprintf(out_A,"0\t");
                }
            if(p==NODES1-1) fprintf(out_A,"1\n");
            else fprintf(out_A,"0\n");
            }
        // It seems MATlab does not read properly line 501
        // so store it separately
        // store the supplies into file out_b
        for(p=0;p<NODES1;p++){
            fprintf(out_b,"%d\t",B[p]);
            }
        // store the costs per unit into file out_c and 0 at the end for
        // the new column in A
        for(q=0;q<DENSITY1;q++){
            fprintf(out_c,"%d\t",C[q]);
            }
        fprintf(out_c,"0");
        // store into a file tha Basic tree. Also at the store the new column in A
        // which considered basic
        for(p=0;p<NODES1-1;p++){
            fprintf(out_B,"%d\t",exists(BasicTree0[p],BasicTree1[p])+1);
            }
        fprintf(out_B,"%ld",DENSITY1+1);
}

// If asked (in header file) the graph is stored into GIDEN form
void print_to_giden(){
    FILE *outfile;
     int i;
    outfile=fopen(GIDEN_FILE,"w");
    fprintf(outfile,"v:2.0a\n");
    fprintf(outfile,"graph:%ld,%ld,directed\n",NODES1,DENSITY1);
    for(i=0;i<NODES1;i++){
        fprintf(outfile,"node:id=%d,x=%d,y=%d\n",i+1,(i%4)*STEP+STEP,(i/4)*STEP+STEP);
    }
    for(i=0;i<DENSITY1;i++){
         fprintf(outfile,"edge:id=%d,e1=%d,e2=%d,lp=1,spline=false\n",i+1,FROM[i],TO[i]);
    }
    fprintf(outfile,"node data labels:1,supply,i\n");
    for(i=0;i<NODES1;i++){
        fprintf(outfile,"node data values:%d,%d\n",i+1,B[i]);
    }
    fprintf(outfile,"edge data labels:2,cost,i,capacity,i\n");
    for(i=0;i<DENSITY1;i++){
         fprintf(outfile,"edge data values:%d,%d,%d\n",i+1,C[i],10000);
    }
}

int exists(int p,int q){
    int i,thesi=-1;
    for(i=0;i<DENSITY1;i++)
         if(FROM[i]==p && TO[i]==q) thesi=i;
    return thesi;
    }

// generates a new graph by running netgen and by using grseed
// it also stores the graph into a file

void gener_store_graph(long grseed,char *filename){
    netgen(grseed, parms);
    store_graph(filename);
    }


// Prints vector D
void print_D(){
     int i;
     printf("\nD vector:\n");
     for(i=0;i<DENSITY1;i++)
          printf("D(%d,%d)=%d ",FROM[i],TO[i],D[i]);
     }

