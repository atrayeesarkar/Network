#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <map>
// #include <omp.h>
// #include <mpi.h>
#include "Network_Type.hpp"
#include <vector>
#include <stack>
#include <ctime>
#include <random>
#include <sstream>

// ---- functions to get run time---//
std::stack<clock_t> ticktock_stack;
void tic(){
    ticktock_stack.push(clock());
}
void toc(){
    std::cout << "Time elapsed: "
    << ((double)(clock()-ticktock_stack.top()))/CLOCKS_PER_SEC
    << std::endl;
    ticktock_stack.pop();
}
// --------------//

//-----random number generator---------//
std::mt19937 generator;
std::uniform_real_distribution< > distribution {0.0, 1.0} ;

void InitializeRandomNumGen(long seed)
{
	generator = std::mt19937(seed );
}
//----------------//

int nodenum; // declaring global variable nodenum


//------function adjacency_matrix-------//
std::vector<std::vector<int >> adjacency_matrix(std::string filename1, std::string filename2){ 
    // filemane1 corresponds to .ids file; filename 2 corresponds to .topo file
    // filename1 and filename2 both has the absolute path embedded in them
    std::string nodeName;
    int nodeID, numlines, lines; 
 
	numlines = 0;   
	
    std::ifstream f(filename1);
    if(!static_cast<bool>(f))
        {
            std::cout << "Error reading the .ids file." << "\n";
            }
    std::map <std::string, int> id;
    while(f >> nodeName >> nodeID)
    {
        id[nodeName] = nodeID;
        numlines++;
    }
    f.close();

    nodenum = numlines;
    std::vector<int> temp;
    std::vector<std:: vector<int > > adjacency; //initializing
    for(int i=0;i<nodenum;i++){
        temp.push_back(0);
    }
    for(int i=0;i<nodenum;i++){
        adjacency.push_back(temp);
    }
    std::ifstream f2(filename2);
 
    
    std::string line;
    constexpr char delim = '\t';
    if(f2.is_open()){
    while(getline(f2,line))
    {
        std::stringstream ss(line); // breaking into words
        std::string source;
        std::string target;
        int interactiontype;
        std::getline(ss, source , delim );
        std::getline(ss, target , delim ); 
        ss >> interactiontype;
        adjacency[id[target]][id[source]] = interactiontype;
        if(adjacency[id[target]][id[source]] ==2)
        {
            adjacency[id[target]][id[source]] = -1;            // adjacency of the biological betwork
        }
    }
    }

    f2.close();
    //randomization
    for(int k = 0;k<(nodenum)*(nodenum);k++) //# of randomization steps = (node #)^2 //
   
    {
        std::vector<int> A;
        std::vector<int> B;
        for(int i = 0;i<nodenum;i++)
        {
            for(int j = 0;j<nodenum;j++)
            {
                if(adjacency[j][i]==0)
                {

                }
                else
                {
               
                    A.push_back(i); //source list
                    B.push_back(j); //target list
              
                
                }
            
            }
    
        }
        //random integers to choose two edges (pairs of source and target)
        int r1 = generator() % A.size(); 
        int r2 = generator() % A.size();
     
        int a = A[r1];//source
        int b = B[r1];//target
        int c = A[r2];//source
        int d = B[r2];//target

        int counter = 0;
     
        while(a==d || b==c || adjacency[d][a]!=0 || adjacency[b][c]!=0) // make sure there is no outdegrees from source of one edge to target of other edge and avoid self loops
        {
            counter = counter+1;
          
            r1 = generator() % A.size();
            r2 = generator() % A.size();
            a = A[r1];
            b = B[r1];
            c = A[r2];
            d = B[r2];
        }
        // // ----------------preserve total no. of +ve and -ve edges------------
        // if (adjacency[b][a]==adjacency[d][c]){
        //     adjacency[d][a]=adjacency[b][a];
        //     adjacency[b][c]=adjacency[d][c];
        // }
        // else{
        //     float r3 =  generator()/(RAND_MAX+1.0);
        //     if(r3>=0.5){
        //     adjacency[d][a] =+1;
        //     adjacency[b][c] =-1;
        //     }
        //     else{
        //         adjacency[d][a] =-1;
        //         adjacency[b][c] =+1;
        //     }
        // }
        // // ------------------------------

        // // --------sign of edges not preserved-------
        float r3 =  generator()/(generator.max()+1.0);
        if(r3>=0.5){
            adjacency[d][a] =+1;
            }
            else{
                adjacency[d][a] =-1;
            }
            float r4 =  generator()/(generator.max()+1.0);
        if(r4>=0.5){
            adjacency[b][c] =+1;
            }
            else{
                adjacency[b][c] =-1;
            }
       // // -----------------------------
        adjacency[b][a] = 0;
        adjacency[d][c] = 0;
         
    }
    return(adjacency);
}

//----------------//



void getstablestate(int I, std::vector<std::vector<int>> adjacency1, std::vector<int> state1, std::ofstream& f3, std::ofstream& f4, std::ofstream& f5, std::ofstream& f6) 
{
    std::vector<int> state = state1;
    replace(state.begin(),state.end(),0,-1);
    std::vector<int> nextstate = state;
    std::vector<std::vector<int>> storestate ;
    storestate.push_back(nextstate);
    std::vector<int> stablestate;
    std::vector<std::vector<int>> adjacency = adjacency1;
    int nodetoupdate;
    int Flag=0;
    for (int i = 0; i < 2000; i++)
    
   {    
        std::vector<int> tmpstate = state;
       
        for(int l=0;l<nodenum;l++)
        {
            nodetoupdate =l;
    
            int count = 0;
            for (int j = 0; j < nodenum; j++)
            {
                count += adjacency[nodetoupdate][j]*tmpstate[j]; 
            
            }
    
            if(count == 0)
            {
            nextstate[nodetoupdate] = tmpstate[nodetoupdate];
            }
            else if(count > 0)
            {   
                nextstate[nodetoupdate] = +1;
            }
            else 
            {
                nextstate[nodetoupdate] = -1;
            }
            
        }
        storestate.push_back(nextstate);
        state = nextstate; // here I am storing the next iterations in storestate
        
        // for next ieration i+1 I am checking whether the state in that iteration (stored in storestate(i+1,:)), I am checking whether it maches with any of the stored state
        // in "storestate" at i=0:i and calculating the period from there and printing all those stored state into the file as steady state/cycle.
         //since storestate has i+1 rows where (i+1)th row is nextstate
        for(int k = i; k>=0;k--)
        {   
            
            int period;
            if (nextstate==storestate[k])
            {    
                // period = i-k;
                period = i-k+1;
                stablestate = nextstate;
                // for(int l = k; l<i;l++)
                for(int l = k; l<=i;l++)
                {
                    f5<< I <<" "<<period<<" ";
                    for(int j = 0;j<storestate[l].size();j++)
                    {
                        f5 << storestate[l][j] << " ";
                    }   
                    f5 << "\n";
                

                // ------- frustration ------//
                    int p;
                    int F=0;
                    for (int ii = 0; ii < (nodenum); ii++)
                    {      
                        for (int jj = 0; jj < (nodenum); jj++)
                        {      
                            p = storestate[l][ii]*adjacency[ii][jj]*storestate[l][jj];
                            if(p<0){
                                F+=1;
                             }
                        } 
                    }
                    f6<< I <<" "<<period<<" "<<F << "\n";
                }
        
                
                Flag = 1;
               
                break;
            }
            
             
        }   
        if(Flag == 1)
        {
            
            break;
        }  
    }


    if(stablestate.empty())
        {
            std::cout<<"empty"<<"\n";
        }

       


    //////////////////////////////////////////


   

    for(int nodetodelete = 0; nodetodelete <nodenum; nodetodelete++)
    {  
        std::vector<int> state = state1;
        replace(state.begin(),state.end(),0,-1);
        std::vector<std::vector<int>> adjacency = adjacency1;
        state.erase(state.begin()+nodetodelete);
         adjacency.erase(adjacency.begin()+nodetodelete);
        for(int k = 0; k<adjacency.size();k++){
            adjacency[k].erase(adjacency[k].begin()+nodetodelete);
        }
  
        std::vector<int> nextstate = state;
        int nodetoupdate;
        int Flag=0;
        std::vector<std::vector<int>> storestate ;
        storestate.push_back(nextstate);
        std::vector<int> stablestate;
        for (int i = 0; i < 2000; i++)
        {    
            std::vector<int> tmpstate = state;
            for(int l = 0;l < (nodenum-1); l++)
            {
                nodetoupdate = l; //-------------- CHECH THIS ----------//
                int count = 0;
                for (int j = 0; j < (nodenum-1); j++)
                {
                    count += adjacency[nodetoupdate][j]*tmpstate[j]; 
                }
    
                if(count == 0)
                {
                    nextstate[nodetoupdate] = tmpstate[nodetoupdate];
                }
                else if(count > 0)
                {
                    nextstate[nodetoupdate] = +1;
                }   
                else 
                {
                    nextstate[nodetoupdate] = -1;
                    
                }
            }
            storestate.push_back(nextstate);
            state = nextstate; 
 
            for(int k = i; k>=0;k--)
            {   
            
            int period;
            if (nextstate==storestate[k])
            {    
                period = i-k+1;
                // std::cout<< nodetodelete<<" "<<period<<" "<<i<<" "<<"\n";
                stablestate = nextstate;
                for(int l = k; l<=i;l++)
                {
                    
                    f3<< I <<" "<<nodetodelete<<" "<<period<<" ";
                    for(int j = 0;j<storestate[l].size();j++)
                    {
                        f3 << storestate[l][j] << " ";
                    }   
                    f3 << "\n";

                // ------- frustration ------//
                    int p;
                    int F=0;
                    for (int ii = 0; ii < (nodenum-1); ii++)
                    {      
                        for (int jj = 0; jj < (nodenum-1); jj++)
                            {      
                                p = storestate[l][ii]*adjacency[ii][jj]*storestate[l][jj];
                                if(p<0){
                                    F+=1;
                                }
                            } 
                    }
                    f4<<I<<" "<<period<<" "<<nodetodelete<<" "<< F << "\n";
                    }
        
                
                Flag = 1;
                break;
            }
        }   
            if(Flag == 1)
            {
                break;
            }
        }
    }
}


int main(){
    tic(); // to get run time of the code
    
    InitializeRandomNumGen(12345678902); // random seed for random number generator
    // std::cout<<generator()<<"\n"<<generator()<<"\n";
    // InitializeRandomNumGen(-156094345);
    std::vector<int> stablestate; 
    int n = 72; // no. of nodes
    // int n = 5;
    std::vector<int> tmp1(n,0); 

    std::vector<std::vector<int>> state ; // initializing 2d array (mx72) 'state'. 
    int m = 65536; // no. of initial states loaded.  
    // int m = 32;
    for(int i = 0 ; i< m; i ++)
    {
        state.push_back(tmp1);
        
    }
    


    std::ifstream f(std::string("inputfiles/initial_states.txt")); // opening file contianing initial states
    // std::ifstream f(std::string("inputfiles/initial_states_inkatz_1.txt"));
    for(int i = 0; i< m ; i++) // loading the inital states inito the 2d array 'state'
    {
        for(int j = 0; j< n; j++)
        {
            f >> state[i][j];
          
        } 
       
    }
    for(int k = 0;k<100;k++) //100 random networks. This for loop creates 100 randomization starting from the biological network adjacency matrix 
    {
        std::cout<<k<<"\n"; //printing the randomization id on the terminal to check progress
        // outputfiles. xxx_8.1.x is for sign of edges NOT preserved randomization. xxx_8.2.x is for sign of edges preserved randomization.  
        std::ofstream f3("outputfiles/RUN72/output_mycode_nodedelete_randomization_tv_test_correct1_balanced_synchro_period8.1.0_"+std::to_string(k)+".log");
        std::ofstream f4("outputfiles/RUN72/output_mycode_nodedelete_randomization_frustration_tv_test_correct1_balanced_synchro_period8.1.0_"+std::to_string(k)+".log");   
        std::ofstream f5("outputfiles/RUN72/output_mycode_randomization_tv_test_correct1_balanced_synchro_period8.1.0_"+std::to_string(k)+".log");
        std::ofstream f6("outputfiles/RUN72/output_mycode_randomization_frustration_tv_test_correct1_balanced_synchro_period8.1.0_"+std::to_string(k)+".log"); 
        std::ofstream f7("outputfiles/RUN72/output_mycode_randomization_adjacency_tv_test_correct1_balanced_synchro_period8.1.0_"+std::to_string(k)+".log");  
        //function adjacency matrix to generate randomized adjacency matrix from the node ids and network topology input files.
        std::vector<std::vector<int>> adjacency = adjacency_matrix(std::string("inputfiles/emt72network.ids"), std::string ("inputfiles/emt72network.topo"));

        
     
        for (int ii = 0; ii < (nodenum); ii++) //printing randomized adjacency matrix in the ourput file
        {    
            for (int jj = 0; jj < (nodenum); jj++)
            {      
                f7<<adjacency[ii][jj]<<" ";
                    
            }
            f7<<"\n";
        }

       
        std::vector<int> state1; // declaring 1d array 'state1'
        int initialstatenum = 65536; // number of initial states to be used. Not used in the current version
        
        // for(int i= 0; i< initialstatenum; i++)
        for(int i= 0; i< 500; i++) //initial states # 500 // loops over different initial states of the network.
        { 
            state1 = state[i]; // state1 is one row of 2d array state 
            
            //getstatbelstate function performs synchronous update of the states and checks for statble state/cycles and calculate frustration of the stable states/ cycles. Does the same for node deletion  
            getstablestate(i,adjacency, state1, f3, f4, f5, f6);
            
        }
    
        f3.close(); //closing files
        f4.close();
    }
    toc(); // to get run time of the code
    return 0;
}

