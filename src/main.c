#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include <windows.h>

#include "graph.h"
#include "main.h"
#include "dnepsa.h"
#include "dnsa.h"

#define PROBLEMS "networks"
#define DIR_PRE "networks-"
#define FILE_PRE "instance-"

#ifndef NO_ART_NODE
    // NEPSA's results filename
    #define SOL_FILE "_networks-solutions-nepsa.txt"
#else
#ifdef DNSA
       // Dual Network Simplex' results filename
       #define SOL_FILE "_networks-solutions-dnsa.txt"
#else
     // DNEPSA's results filename
     #define SOL_FILE "_networks-solutions-dnepsa.txt"
#endif
#endif

int main(int argc,char *argv[]){
    //int no_graphs=0;  /*uncomment for gener_graphs*/
    int from,to;        /*comment for gener_graphs*/

    set_parms(parms,argv);  // Set parametres (No of nodes-arcs etc.
    init();     //Initialization of arrays etc. (dynamic arrays)
    //*/*/if (TOFILE) outfile=fopen(OUTFILE,"w");
	//run_algorithm();
	//getchar();
	//run_rand_algorithm();
	//run_multi_algorithm(7);

	/*uncomment for gener_graphs*/
	//if(COMMAND_LINE==1) //if no_of_graphs  (to be genetrated) is given in command line
    //    no_graphs=atoi(argv[1]);
    //else
    //    no_graphs=10; // number of graphs generated
	//gener_graphs(no_graphs);

	/*comment for gener_graphs*/
	if(COMMAND_LINE==1) {//if from, to  (to be solved) are given in command line
        from=atoi(argv[1]);
        to=atoi(argv[2]);
	} else {
        from=1;
        to=10;
        }
    solve_graphs(from,to);
    //getchar();

    return 1;
   }

//Arrays initialization (using malloc)
void init(){
    w=(float *)malloc((NODES1)*sizeof(float));
    s=(float *)malloc((DENSITY1)*sizeof(float));
    t=(float *)malloc((DENSITY1)*sizeof(float));
    Checked=(int *)malloc((NODES1)*sizeof(int));

    X_flows=(int *)malloc((DENSITY1)*sizeof(int));
    D=(int *)malloc((DENSITY1)*sizeof(int));
    Y_flows=(int *)malloc((DENSITY1)*sizeof(int));

    Tree=(int *)malloc((NODES1)*sizeof(int));
    Orientation=(int *)malloc((NODES1)*sizeof(int));
    C_par=(int *)malloc((NODES1)*sizeof(int));

    X=(int *)malloc((NODES1)*sizeof(int));
    BasicTree0=(int *)malloc((NODES1-1)*sizeof(int));
    BasicTree1=(int *)malloc((NODES1-1)*sizeof(int));

    I=(int *)malloc((DENSITY1)*sizeof(int));
    J=(int *)malloc((DENSITY1)*sizeof(int));
    Cycle=(int *)malloc((NODES1)*sizeof(int));
}

// Produces the direcory name dir[] where the networks to be solved are stored
void dir_name(char *dir){
     //nodes[] is the NODES1 in string format
     //arcs[] is the DENSITY1 in string format
     char nodes[10],arcs[10];
    // convert NODES1 and DENSITY1 into string
    // itoa(NODES,nodes,10); // works only in windows
    //itoa(DENSITY,arcs,10);
    sprintf(nodes,"%ld",parms[0]);  // works also in linux
    sprintf(arcs,"%ld",parms[3]);
    mkdir(PROBLEMS,S_IRWXU);    // Creating directory PROBLEMS
    strcat(dir,PROBLEMS);
    strcat(dir,"/");   //Gia windows prepei na baloume strcat(file,"\\");
    strcat(dir,DIR_PRE);
    strcat(dir,nodes);
    strcat(dir,"x");
    strcat(dir,arcs);
    mkdir(dir,S_IRWXU); // Creating directory for the networks capacity
}

// Produces the file name file[] that stores the network to be solved
// the file name has the suffix i
void file_name(char *file,char *dir,int i){
        char no[4];
        //itoa(i+1,no,10);
        sprintf(no,"%d",i+1);
        // adds after the directory the filename (include FILE_PRE plus i)
        strcat(file,dir);
        strcat(file,"/");  //Gia windows prepei na baloume strcat(file,"\\");
        strcat(file,FILE_PRE);
	    strcat(file,no);
}

// generates n graphs in PROBLEMS
// where a directory of the form DIR_PREnxm is created
// (n,m is no of nodes and arcs)
// In this directory files with name FILE_PREi is created
// (i takes values from 1 to n)
// Each file contains a network of NODES1 nodes and DENSITY1 arcs
int gener_graphs(int n){
    long grseed;
    int i;
    // build directory name
    char dir[30]="";
    // take the proper dir name
    dir_name(dir);
    mkdir(dir,S_IRWXU);
    // convert i into string
    for(i=0;i<n;i++){
        sleep(1); //wait. It is needed for random number generation...
                    //sleep(1) gia linux, sleep(1000) gia windows
        // filename prefix
        char file[50]="";
        // take the proper file name
	    file_name(file,dir,i);
        // generates a random number for seed
        srand(time(NULL)+ getpid());
	    grseed=rand();
	    // generates and store graph
	    gener_store_graph(grseed,file);
     }
     return 0;
}

// Solves the MCNFPs generated automatically by function gener_graphs
// The solutions are put in file SOL_FILE in the same directory as the networks.
// It actually solves the networks from problem m to problem n
// (the first problem is numbered as 0)
int solve_graphs(int m,int n){
    int i;
   	long int result=-1, niter;
	double z,cputime=0;
	FILE *outf;
    char dir[100]="";
    char outfile[100]="";

	m--;n--;
    dir_name(dir); //directory name
    strcat(outfile,dir);
    strcat(outfile,"/");
    strcat(outfile,SOL_FILE);
    // open solution's file name
    if((outf=fopen(outfile,"w"))==NULL) {
         printf("Cannot open output file:%s\n",outfile);
         return 0;
         };
    fprintf(outf,"DNEPSA\n\n");
    fprintf(outf,"Nodes: %ld  Arcs:%ld\n",NODES,DENSITY);
    fprintf(outf,"_____________________________________________\n\n");
    fprintf(outf,"Instance \t\t Result \t Value \t\t\t\t niter\t\t\tCPU\n");
    fprintf(outf,"_________________________________________________________________________________________________________\n\n");
    // for all filenames
    for(i=m;i<=n;i++){
        char infile[50]="";
          // take input file name
        file_name(infile,dir,i);
        if(read_from_file(infile)){
	    #ifndef NO_ART_NODE
        // Running NEPSA algorithm
         result=nepsa(&z,1,&niter,&cputime);
         #else
         #ifdef DNSA
	     // Running Dual Network Simplex algorithm
         result=dnsa(&z,1,&niter,&cputime);
         #else
	     //Running DNEPSA algorithm
	     result=dnepsa(&z,1,&niter,&cputime);
         #endif
         #endif

         if(result==5)
             fprintf(outf,"%d \t\t\t %ld \t\t infeasible \t\t\t %ld\t\t\t%f\n",i+1,result,niter,cputime);
         else  if(result==4)
             fprintf(outf,"%d \t\t\t %ld \t\t disconnected \t\t\t %ld\t\t\t%f\n",i+1,result,niter,cputime);
         else
             fprintf(outf,"%d \t\t\t %ld \t\t %0.2f \t\t\t %ld\t\t\t%f\n",i+1,result,z,niter,cputime);
         }
         printf("#%ld\n",result);
         }
    fclose(outf);

    return 0;
}

 // runs dnepsa or nepsa (depending on NO_ART_NODE) by using SEED specified in graph.h
 // the network it solves depend on the values of NETGEN etc in dnepsa.h
 void run_algorithm(){
	long int result;
    long niter;
	double z,cputime;
	seed=SEED;
	#ifndef NO_ART_NODE
    fprintf(OUT,"Running NEPSA algorithm");
    result=nepsa(&z,1,&niter,&cputime);
    fprintf(OUT,"\nres=%ld niter=%ld obj=%f for seed=%ld",result,niter,z,seed);
    #endif
    #ifdef DNSA
	fprintf(OUT,"Running Dual Network Simplex algorithm");
    result=dnsa(&z,1,&niter,&cputime);
    fprintf(OUT,"\nres=%ld niter=%ld obj=%f for seed=%ld",result,niter,z,seed);
    #else
	fprintf(OUT,"Running DNEPSA algorithm");
    result=dnepsa(&z,1,&niter,&cputime);
    fprintf(OUT,"\nres=%ld niter=%ld obj=%f for seed=%ld",result,niter,z,seed);
    #endif
    }


// runs dnepsa or nepsa (depending on NO_ART_NODE) by using different (random) seeds in every exection
// the network it solves depend on the values of NETGEN etc in dnepsa.h
void run_rand_algorithm(){
     long int result;
     long niter;
	 double z,cputime;
     srand(time(NULL)+ getpid());
	 seed=rand();
	#ifdef NO_ART_NODE
	fprintf(OUT,"Running DNEPSA algorithm");
    result=dnepsa(&z,1,&niter,&cputime);
    #else
    fprintf(OUT,"Running NEPSA algorithm");
    result=nepsa(&z,1,&niter,&cputime);
    #endif
    fprintf(OUT,"\nResult=%ld, objective=%f for seed=%ld",result,z,seed);
     }

// Runs n times algorithm dnepsa with randomly generated seeds
 // the network it solves depend on the values of NETGEN etc in dnepsa.h
int run_multi_algorithm(int n){
	 int result,i;
 	 double z,cputime;
 	 long niter;
	 // In this loop we can run many different problems
	 for(i=0;i<n;i++){
	 	// get a random number seed
        srand(time(NULL)+ getpid());
        // get a random number to use as a seed by netgen
	    seed=rand();
	    #ifdef NO_ART_NODE
        result=dnepsa(&z,1,&niter,&cputime);
        #else
        result=nepsa(&z,1,&niter,&cputime);
        #endif
	 	printf("\nResult=%d, objective=%f for seed=%ld",result,z,seed);
		}
	 return 0;
	 }
