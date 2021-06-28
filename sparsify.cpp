// sparsify a graph by removing landmarks

#include<iostream>
#include<stdint.h>
#include<vector>
#include<map>
#include<set>
#include<math.h>
#include<queue>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include <bits/stdc++.h>

using namespace std;

#define processedFile "twitter_pro.txt"
#define resultFile "twitter_spsf_20.txt"

void sparsify(int k);

int main()
{
    sparsify(20);
    return 0;
}


void sparsify(int k)
{
    int countVertex;
    int countEdge;
    ifstream graphFile(processedFile);
    stringstream ss;
    string lineC;
    getline(graphFile,lineC);
    ss << lineC;
    ss >> countVertex;
    ss >> countEdge;
    vector<int> temp ;
    vector<vector<int > > adjList(countVertex,temp);
    ss.clear();
    ss.str("");

    //read the graph file
    int v;
    int d;
    while(getline(graphFile,lineC))
    {
        ss << lineC;
        ss >> v;
        ss >> d;
        vector<int> tempN(d,0);
        for(int i=0;i<d;i++){
            ss >> tempN[i];
        }
        adjList[v].swap(tempN);
        ss.clear();
        ss.str("");
    }
    graphFile.close();

    // sparsify the graph by removing all edges from landmarks
    for(int i=0;i<k;i++){
        adjList[i].clear();
    }

    // sparsify the graph by removing all edges to landmarks
    for(int i=0;i<k;i++)
    {
        for(int j=0;j<countVertex;j++)
        {
            adjList[j].erase(remove(adjList[j].begin(),adjList[j].end(),i),adjList[j].end());
        }
    }

    ofstream resultGraph(resultFile);
    for(int i=0;i<countVertex;i++)
    {
        resultGraph << i << " " << adjList[i].size() << " ";
        for(int j=0;j<adjList[i].size();j++){
            resultGraph << adjList[i][j] << " ";
        }
        resultGraph << endl;
    }
    resultGraph.close();
    return ;
}

