//Implementation for Dijkstra's Single source shortest path algorithm Using Fibonacci Heap
//This is an optimized algorithm Uses Fibonacci Heap instead of Binary Heap So the Complexity Reduced from O(E*log(V))to O(E+V*(log(V))
 
#include <iostream>
#include <queue>
#include <vector>
#include <climits>
#include "fiboheap.h"
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <algorithm>
using namespace std;
#define INF INT_MAX //Infinity
 
const int Size=4598500; //Maximum possible number of vertices. Preallocating space for DataStructures accordingly
int Distance[Size]; //Stores shortest Distancetance
bool Visited[Size]; //Determines whether the node has been Visited or not
int Parent[Size];   //For Storing the parent for Printing the Shortest Path
vector<int> *SSSPTREE_OUT;
vector<int> *SSSPTREE_IN;
void Dijkstra(vector<pair<int,int> > Graph[],int source, int n) //Algorithm for SSSP 
{
    for(int i=0;i<n+1;i++) 
    {
        Distance[i]=INF;    //Set initial Distances to Infinity   
        Parent[i]=-1;       //Set initial Parent to -1   
        Visited[i]=0;       //Set intitial All vetices are not visited
    }
    FibHeap<pair<int,int>> pq;
    pq.push(make_pair((Distance[source]=0),source));//Pushing the source with Distance from itself as 0
    while(!pq.empty())
    {
        pair<int, int> curr=pq.top(); //Current vertex. The shortest Distance for this has been found
        pq.pop();
        int cw=curr.first,cv=curr.second; //'cw' the final shortest Distance for this vertex
        if(Visited[cv]) //If the vertex is already Visited, no point in exploring adjacent vertices
            continue;
        Visited[cv]=true; 
        for(int i=0;i<Graph[cv].size();i++) //Visiting through all adjacent vertices
            if(!Visited[Graph[cv][i].first] && Graph[cv][i].second+cw<Distance[Graph[cv][i].first]) //If this node is not Visited and the current Parent node Distancetance+Distancetance from there to this node is shorted than the initial Distancet set to this node, update it
            {
                pq.push(make_pair((Distance[Graph[cv][i].first]=Graph[cv][i].second+cw),Graph[cv][i].first)); //Set the new Distance and add to priority queue
                Parent[Graph[cv][i].first]=cv;  //Set the Parent of the vertex
            }
    }
}
void Tout_TREECreation(vector<pair<int,int> > Graph[], vector<int> repl_path,int n)
{
    SSSPTREE_OUT=new vector<int>[n+1];
    vector<int> included(n+1);
    included.push_back(repl_path[0]);
    for(int i=0;i<repl_path.size()-1;i++)
    {
        int x=repl_path[i];
        int y=repl_path[i+1];
        SSSPTREE_OUT[x].push_back(y);
        included.push_back(y);
    }
	for(int i=1;i<=n;i++)
	{
		int to,from,weight;
		from=i;
		for(int j=0;j<Graph[i].size();j++)
		{
			to=Graph[i][j].first;
			weight=Graph[i][j].second;
			if((Distance[from]!=INF)&&(Distance[to]!=INF))
			{
				if(Distance[from]+weight==Distance[to])
				{
                    if(find(included.begin(),included.end(),to)==included.end())
                    {
                            SSSPTREE_OUT[from].push_back(to);
                            included.push_back(to);
                    }
				}
			}		
		}
	}
}
void Tin_TREECreation(vector<pair<int,int> > Graph[], vector<int> repl_path,int n)
{
    SSSPTREE_IN=new vector<int>[n+1];
    vector<int> included;
    included.push_back(repl_path[repl_path.size()-1]);
    for(int i=repl_path.size()-1;i>0;i--)
    {
        int x=repl_path[i];
        int y=repl_path[i-1];
        SSSPTREE_IN[x].push_back(y);
        included.push_back(y);
    }
    for(int i=1;i<=n;i++)
    {
        int to,from,weight;
        from=i;
        for(int j=0;j<Graph[i].size();j++)
        {
            to=Graph[i][j].first;
            weight=Graph[i][j].second;
            if((Distance[from]!=INF)&&(Distance[to]!=INF))
            {
                if(Distance[from]+weight==Distance[to])
                {
                    if(find(included.begin(),included.end(),to)==included.end())
                    {
                            SSSPTREE_IN[from].push_back(to);
                            included.push_back(to);
                    }
                }
            }       
        }
    }
}
int main() //Driver Function for Dijkstra SSSP
{
    int n,m;//Number of vertices and edges
    vector<pair<int,int> > *Graph; //Adjacency list
    vector<pair<int,int> > *Rev_Graph;
    int x,y,w;
    int source;
    vector<int> repl_path;
    ifstream fin;
    cout<<"Enter name of the input graph file name"<<endl;
    string input;
    cin>>input;
    input=input+".txt";
    fin.open(input, ios::in);
    int count=0;
    if (fin >> n >> m)  //Taking vertices and edges input from file
    {
        Graph=new vector<pair<int,int> >[n+1];
        Rev_Graph=new vector<pair<int,int> >[n+1];
        while(fin >> x >> y >> w)              //Taking edges with wait input from file
        {    
            Graph[x].push_back(make_pair(y,w));
            // Graph[y].push_back(make_pair(x,w));
            Rev_Graph[y].push_back(make_pair(x,w));    
            if(count==m-1)
                break;
            count++;
        }
        fin >> source;
        while(fin>>x)
        {
        	repl_path.push_back(x);
        }
    }
    fin.close();
    // cout<<repl_path[repl_path.size()-1]<<endl;
    Dijkstra(Rev_Graph,repl_path[repl_path.size()-1],n);
    Tin_TREECreation(Rev_Graph,repl_path,n);
    Dijkstra(Graph,source,n);//single source shortest path from source (Also passing the Graph and number of vertices as parameter)    
    Tout_TREECreation(Graph,repl_path,n);
    ofstream outfile;
    cout<<"Enter the name of the text file whhere you wnt to store the output"<<endl;
    string output;
    cin>>output;
    output = output+".txt";
    outfile.open(output);
    outfile<<"Source is: "<<source<<". The shortest Distance to every other vertex from here is: \n";
    for(int i=1;i<=n;i++)//Printing final shortest Distance from source in File
    {
        outfile <<"Vertex: "<<i<<" , Distance: ";
        Distance[i]!=INF? outfile<<Distance[i]<<" ," : outfile<<"-1 ,";
        // cout<<Parent[i]<<" ";
        if(i==source)
        {
                outfile<<"Source \n";
                continue;
        }    
        if(Distance[i]==INF)
        {
                outfile<<"Unreachable \n";
                continue;
        }
        outfile<<"Path:";
        std::vector<int> v;
        for(int x=i;x!=-1;x=Parent[x])
        {
            v.push_back(x);
            // if(x==i)
            //     outfile<<x;
            // else
            //     outfile<<"-->"<<x;
        }
        reverse(v.begin(),v.end());
        outfile<<v[0];   
        for(int l=1;l<v.size();l++)
            outfile<<"-->"<<v[l];   
        v.clear();
        outfile<<"\n";
    }
    return 0;
}