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

#include<sys/time.h>

#define processedFile "twitter_pro.txt"

#define resultFile "twitter_indexInfo_20.txt"
#define resultFile2 "twitter_indexInfo_20.txt"

using namespace std;


struct label{
    int lm;
    int dist;
    label(int a,int b){
        lm = a;
        dist = b;
    }
};

long indexConstructionEfficient(int k);

int main(){

    int k = 20;

    int testT = 1;
    long timeAll = 0;
    for(int i=0;i<testT;i++) timeAll = timeAll + indexConstructionEfficient(k);
    cout << endl << timeAll/testT << "us";
    return 0;
}

long indexConstructionEfficient(int k)
{
    int countVertex = 0;
    int countEdge = 0;

    // read the graph file
    ifstream graphFile(processedFile);

    stringstream ss;
    string lineC;

    getline(graphFile,lineC);
    ss << lineC;
    ss >> countVertex;
    ss >> countEdge;

    vector<vector<int > > adjList;
    ss.clear();
    ss.str("");

    int u,v,d,x;
    //read the graph
    while(getline(graphFile,lineC))
    {
        ss << lineC;
        ss >> v; //vertex id
        ss >> d; //degree
        vector<int> tempD(d,0);
        for(int i=0;i<d;i++){
            ss >> tempD[i];
        }
        adjList.push_back(tempD);
        ss.clear();
        ss.str("");
    }
    graphFile.close();

    uint8_t** highway = new uint8_t*[k];
    for(int i=0;i<k;i++)
    {
        highway[i] = new uint8_t[k];
    }
    vector<string> path(k,"");
    vector<vector<string> > highwayPath(k,path);

    /*int cntLm = 0;
    while(cntLm<k)
    {
        getline(landmarkFile,lineC);
        ss << lineC;
        ss >> v;
        landmarks[cntLm] = v;
        trans[v] = cntLm++;
        ss.clear();
        ss.str("");
    }
    landmarkFile.close();*/

    //construct the indices
    uint8_t** labels = new uint8_t*[countVertex];
    for(int i=0;i<countVertex;i++)
    {
        labels[i] = new uint8_t[k];
        memset(labels[i],99,k*sizeof(uint8_t));
    }

    uint8_t* dist = new uint8_t[countVertex];
    //vector<int> temp;

    int* uncovered  = new int[countVertex]; //queue<int> uncovered;
    int* covered    = new int[countVertex];   //queue<int> covered;

    int unsize = 0;
    int cosize = 0;
    int unp = 0;
    int cop = 0;

    bool flag1, flag2;

    timeval st,ed;
    gettimeofday(&st,NULL);
    long numberOfLabels = 0;

    for(int i=0;i<k;i++)
    {   //i is the root
        labels[i][i] = 0;
        unsize = 0;
        cosize = 0;

        unp = 0;
        cop = 0;

        memset(dist,99,countVertex*sizeof(uint8_t));

        //vector<vector<int> > pred(countVertex,temp);

        uncovered[unsize++] = i;    //uncovered.push(l);
        uncovered[unsize++] = -1;   //uncovered.push(-1);
        covered[cosize++] = -1;     //covered.push(-1);
        dist[i] = 0;
        flag1 = true;
        flag2 = true;

        while(flag1||flag2)
        {
            while(flag1)
            {
                u = uncovered[unp++];//u = uncovered.front();//uncovered.pop();

                if(u==-1)
                {
                    if(unp==unsize) {
                        flag1 = false; //if(uncovered.front()==-1) flag1 = 0;
                    }
                    else{
                        uncovered[unsize++] = -1;//else uncovered.push(-1);
                    }
                    break;
                }
                for(d=0;d<adjList[u].size();d++)//for(v:adjList[u])
                {
                    v = adjList[u][d];
                    if(dist[v]==99)
                    {
                        dist[v] = dist[u] + 1;
                        //pred[v].push_back(u);
                        if(v<k) covered[cosize++] = v;//covered.push(v);
                        else{
                            uncovered[unsize++] = v;//uncovered.push(v);
                            labels[v][i] = dist[v];
                            numberOfLabels++;
                        }
                    }
                    //else if(dist[v]==dist[u]+1) pred[v].push_back(u);
                }
            }

            while(flag2)
            {
                u = covered[cop++];   //u = covered.front(); //covered.pop();
                if(u==-1)
                {
                    if(!flag1 && cop==cosize) {
                        flag2 = false;//if(!flag1 && covered.front()==-1) flag2 = 0;
                    }
                    else{
                        covered[cosize++] = -1; //else covered.push(-1);
                    }
                    break;
                }
                if(u<k)
                {
                    labels[u][i] = dist[u];
                    if(u>i)
                    {
                        highway[i][u] = dist[u];
                        highway[u][i] = dist[u];
                        //numberOfLabels++;
                        queue<int> bq;
                        set<int> bs;

                        bq.push(u);
                        bs.insert(u);
                        while(!bq.empty()){
                            v = bq.front();
                            bq.pop();
                            for(int j=0;j<adjList[v].size();j++)//(d:adjList[v])
                            {
                                d = adjList[v][j];
                                if(dist[d]==dist[v]-1)
                                {
                                    if(bs.count(d)==0)
                                    {
                                        bs.insert(d);
                                        bq.push(d);
                                    }
                                    ss << v << " " << d << " ";
                                }
                            }
                        }
                        highwayPath[i][u] = ss.str();
                        highwayPath[u][i] = highwayPath[i][u];
                        ss.str("");
                        ss.clear();
                    }
                }

                for(d = 0;d<adjList[u].size();d++)//for(v:adjList[u])
                {
                    v = adjList[u][d];
                    if(dist[v]==99)
                    {
                        dist[v] = dist[u] + 1;
                        //pred[v].push_back(u);
                        covered[cosize++] = v;//covered.push(v);
                    }
                    //else if(dist[v]==dist[u]+1) pred[v].push_back(u);
                }
            }
        }
    }
    gettimeofday(&ed,NULL);
    long ti = 1000000*(ed.tv_sec - st.tv_sec) + (ed.tv_usec - st.tv_usec);
    //cout << ti << "us" << endl;
    cout << numberOfLabels << "labels";

    ofstream indexInfo(resultFile2);
    for(int i=0;i<k;i++)
    {
        highway[i][i] = 0;
        for(int j=0;j<k;j++)
        {
            u = highway[i][j];
            indexInfo << i << " " << j << " " << u << " " << highwayPath[i][j] << endl;
            //cout << "P" << highwayPath[i][j] << endl;
        }
    }
    indexInfo << "###" << endl;
    for(int i=0;i<countVertex;i++)
    {
        indexInfo << i;
        for(int j=0; j<k; j++)
        {
            int p = labels[i][j];
            indexInfo<< " " << p ;
        }
        indexInfo << endl;
    }
    indexInfo.close();
    return ti;
};
