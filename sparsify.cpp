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
#define resultFile_k1 "twitter_spsf_20.txt"
#define resultFile_k2 "twitter_spsf_40.txt"
#define resultFile_k3 "twitter_spsf_60.txt"
#define resultFile_k4 "twitter_spsf_80.txt"
#define resultFile_k5 "twitter_spsf_100.txt"

void sparsify(int k);
void sparsifySeries(int k1, int k2, int k3, int k4, int k5);

int main()
{
    //sparsify(20);
    sparsifySeries(20,40,60,80,100);
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

void sparsifySeries(int k1, int k2, int k3, int k4, int k5)
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

    // sparsify the graph by removing all edges to landmarks
    for(int i=0;i<k1;i++)
    {
        adjList[i].clear(); // sparsify the graph by removing all edges from landmarks
        for(int j=0;j<countVertex;j++)
        {
            adjList[j].erase(remove(adjList[j].begin(),adjList[j].end(),i),adjList[j].end());
        }
    }
    ofstream resultGraph1(resultFile_k1);
    for(int i=0;i<countVertex;i++)
    {
        resultGraph1 << i << " " << adjList[i].size() << " ";
        for(int j=0;j<adjList[i].size();j++){
            resultGraph1 << adjList[i][j] << " ";
        }
        resultGraph1 << endl;
    }
    resultGraph1.close();

    //-----------------------------------------------------
    // sparsify the graph by removing all edges to landmarks
    for(int i=k1;i<k2;i++)
    {
        adjList[i].clear();// sparsify the graph by removing all edges from landmarks
        for(int j=0;j<countVertex;j++)
        {
            adjList[j].erase(remove(adjList[j].begin(),adjList[j].end(),i),adjList[j].end());
        }
    }
    ofstream resultGraph2(resultFile_k2);
    for(int i=0;i<countVertex;i++)
    {
        resultGraph2 << i << " " << adjList[i].size() << " ";
        for(int j=0;j<adjList[i].size();j++){
            resultGraph2 << adjList[i][j] << " ";
        }
        resultGraph2 << endl;
    }
    resultGraph2.close();

    //-------------------------------

    // sparsify the graph by removing all edges from or to landmarks
    for(int i=k2;i<k3;i++)
    {
        adjList[i].clear(); // sparsify the graph by removing all edges from landmarks
        for(int j=0;j<countVertex;j++)
        {
            adjList[j].erase(remove(adjList[j].begin(),adjList[j].end(),i),adjList[j].end());
        }
    }
    ofstream resultGraph3(resultFile_k3);
    for(int i=0;i<countVertex;i++)
    {
        resultGraph3 << i << " " << adjList[i].size() << " ";
        for(int j=0;j<adjList[i].size();j++){
            resultGraph3 << adjList[i][j] << " ";
        }
        resultGraph3 << endl;
    }
    resultGraph3.close();

    //-------------------------------
    // sparsify the graph by removing all edges to landmarks
    for(int i=k3;i<k4;i++)
    {
        adjList[i].clear();
        for(int j=0;j<countVertex;j++)
        {
            adjList[j].erase(remove(adjList[j].begin(),adjList[j].end(),i),adjList[j].end());
        }
    }
    ofstream resultGraph4(resultFile_k4);
    for(int i=0;i<countVertex;i++)
    {
        resultGraph4 << i << " " << adjList[i].size() << " ";
        for(int j=0;j<adjList[i].size();j++){
            resultGraph4 << adjList[i][j] << " ";
        }
        resultGraph4 << endl;
    }
    resultGraph4.close();

    //------------------------------
    // sparsify the graph by removing all edges to landmarks
    for(int i=k4;i<k5;i++)
    {
        adjList[i].clear(); // sparsify the graph by removing all edges from landmarks
        for(int j=0;j<countVertex;j++)
        {
            adjList[j].erase(remove(adjList[j].begin(),adjList[j].end(),i),adjList[j].end());
        }
    }
    ofstream resultGraph5(resultFile_k5);
    for(int i=0;i<countVertex;i++)
    {
        resultGraph5 << i << " " << adjList[i].size() << " ";
        for(int j=0;j<adjList[i].size();j++){
            resultGraph5 << adjList[i][j] << " ";
        }
        resultGraph5 << endl;
    }
    resultGraph5.close();
    return ;


}
