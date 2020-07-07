#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stdint.h>
#include<bits/stdc++.h>
#include<sys/time.h>

#define k           20
#define diameter    30
#define INF         99

#define graphFile   "spsf_20.txt" 
#define indexFile   "index_20.txt" 

using namespace std;

struct pairLM{
    int lm1;
    int lm2;

    int to1;
    int to2;
    pairLM(int a,int b, int c, int d)
    {
      lm1 = a;
      lm2 = b;
      to1 = c;
      to2 = d;
    };
};

class spQuery{

private:
    int countVertex;

    int *queueA, frontA, endA; //simulate a queue "queueA"
    int *queueB, frontB, endB; //simulate a queue "queueB"
    int *levelARecord, levelA; //simulate a queue "levelARecord"
    int *levelBRecord, levelB; //simulate a queue "levelBRecord"
    int *meet, endMeet;//simulate a queue "meet"
    int8_t* dist; //a distance list

    vector<vector<int> > adjList;
    uint8_t** labels;
    uint8_t** highway;
    vector<vector<string> > highwayPath;

    int maxDistance;
    vector<pairLM> combination;

    vector<vector<int> > pairs;
    long totalTime;
    int case1, case2, case3, case4;

    void initialization()
    {
        countVertex = adjList.size();

        queueA = new int[countVertex];
        queueB = new int[countVertex];

        levelARecord = new int[diameter];
        levelBRecord = new int[diameter];

        meet =  new int[countVertex/10];
        dist =  new int8_t[countVertex];
        memset(dist,INF,countVertex);

        case1 = 0;
        case2 = 0;
        case3 = 0;
    }

    void loadSparsifiedGraph()
    {
        int u, d;
        string line;
        stringstream ss;
        ifstream graph(graphFile);
        while(getline(graph,line))
        {
            ss << line;
            ss >> u;
            ss >> d;
            vector<int> temp(d,0);
            for(int i=0;i<d;i++)
            {
                ss >> temp[i];
            }
            adjList.push_back(temp);
            ss.clear();
            ss.str("");
        }
        graph.close();
    }

    void loadLabels()
    {
        int u,v,d;
        labels = new uint8_t*[countVertex];
        for(int i=0;i<countVertex;i++) {
            labels[i] = new uint8_t[k];
        }
        highway = new uint8_t*[k];
        vector<string> tempp(k,"");
        for(int i=0;i<k;i++)
        {
            highway[i] = new uint8_t[k];
            highwayPath.push_back(tempp);
        }
        stringstream ss;
        string line;
        ifstream labelFile(indexFile);
        while(getline(labelFile, line))
        {
            if(line=="###") break;
            ss << line;
            ss >> u;
            ss >> v;
            ss >> d;
            highway[u][v] = d;
            highway[v][u] = d;
            highwayPath[u][v] = " " ;//ss.str();
            ss.clear();
            ss.str("");
        }
        while(getline(labelFile, line))
        {
            ss << line;
            ss >> u;
            for(int i=0;i<k;i++) {
                ss >> v;
                //if(v==0) cout << ss.str() << endl;
                labels[u][i] = v;
            }
            ss.clear();
            ss.str("");
        }
        cout << "|V|: " << countVertex << endl;
        labelFile.close();
    }

    unsigned int rand_32()
    {
        return (rand()&0x3)<<30 | rand()<<15 | rand();
    }

    void generateTest(int testTime)
    {
        vector<int> p(2);
        for(int i=0;i<testTime;i++)
        {
            p[0] = rand_32()%countVertex;
            p[1] = rand_32()%countVertex;
            if(p[0]<k || p[1]<k)
            {
                p[0]+=k;
                p[1]+=k;
            }
            pairs.push_back(p);
        }
    }

public:
    spQuery()
    {
        loadSparsifiedGraph();
        initialization();
        loadLabels();
    }

    void seriesTest(int testTime)
    {
        generateTest(testTime);
        timeval st,ed;
        case1 = 0;
        case2 = 0;
        case3 = 0;
        case4 = 0;
        gettimeofday(&st,NULL);
        for(int i=0;i<testTime;i++)
        {
            queryEfficient(i);
        }
        gettimeofday(&ed,NULL);
        long sumTime = (ed.tv_sec-st.tv_sec)*1000000 + (ed.tv_usec-st.tv_usec);
        cout << "Average query time is " << sumTime/testTime << endl;
        cout << "Meet not empty: " << case1 << endl;
        cout << "Reach the upper bound: " << case2 << endl;
        cout << "Miss some level: " << case3 << " " << case4 << endl;
    }
    void queryEfficient(int test)
    {
        int a = pairs[test][0];
        int b = pairs[test][1];
        //get the upper bound and the graph sketch based on the label information
        vector<pairLM> ().swap(combination);
        maxDistance = 99;
        int maxASide = 0;
        int maxBSide = 0;
        int d;
        for(int i=0;i<k;i++)
        {
            if(labels[a][i]==INF) continue;
            for(int j=0;j<k;j++)
            {
                if(labels[b][j]==INF) continue;
                d  = labels[a][i] + labels[b][j] + highway[i][j];
                if(d<maxDistance)
                {
                    vector<pairLM> ().swap(combination);
                    pairLM p(i, j, labels[a][i], labels[b][j]);
                    combination.push_back(p);
                    maxDistance = d;
                    maxASide = labels[a][i]-1;
                    maxBSide = labels[b][j]-1;
                }
                else if(d==maxDistance)
                {
                    pairLM p(i, j, labels[a][i], labels[b][j]);
                    combination.push_back(p);
                    maxASide = max(maxASide,labels[a][i]-1);
                    maxBSide = max(maxBSide,labels[b][j]-1);
                }
            }
        }
        if(maxDistance>=diameter) return;

        dist[a] = 1; //from 1 to 2, 3, 4...
        dist[b] = -1;//from -1 to -2, -3, -4...

        frontA = 0; //initialize the queueA by setting the front pointer to 0;
        frontB = 0; //initialize the queueB by setting the front pointer to 0;
        queueA[0] = a; //push a into queueA;
        endA = 1;
        queueB[0] = b; //push b into queueB;
        endB = 1;
        levelARecord[0] = 1;
        levelBRecord[0] = 1;
        levelA = 0;
        levelB = 0;
        endMeet = 0;

        int u,v;

        //part A: bi-BFS search on the sparsifed graph
        int choice;
        while(endMeet==0 && (levelA+levelB)<maxDistance) //the level-bounded bi-BFS (based on the upper bound)
        {
            choice = 1;
            if(levelB<maxDistance-maxASide)
            {
                if(levelA<maxDistance-maxBSide && endA > endB) choice = 0;
                else if(levelA>=maxDistance-maxBSide) choice = 0;
            }
            if(choice==1)
            {
                while(frontA<levelARecord[levelA])
                {
                    u = queueA[frontA++];
                    for(int i=0;i<adjList[u].size();i++)
                    {
                        v = adjList[u][i];
                        if(dist[v]==INF) //v is unvisited before
                        {
                            dist[v] = dist[u] + 1;
                            queueA[endA++] = v; //push v into next level of queueA
                        }
                        else if(dist[v]<0) //v has been visited from side b
                        {
                            meet[endMeet++] = v; //v is on the level where the bi-BFS meets
                            dist[v] = dist[u] + 1; //set dist[v]>0 for consistency
                            queueA[endA++] = v;
                        }
                    }
                }
                levelARecord[++levelA] = endA;
            }
            else //search from side b.
            {
                while(frontB<levelBRecord[levelB])
                {
                    u = queueB[frontB++];
                    for(int i=0;i<adjList[u].size();i++)
                    {
                        v = adjList[u][i];
                        if(dist[v]==INF) //v is unvisited before
                        {
                            dist[v] = dist[u] - 1;
                            queueB[endB++] = v;
                        }
                        else if(dist[v]>0) //v has been visited from side a
                        {
                            meet[endMeet++] = v; //v is on the level where the bi-BFS meets
                            dist[v] = dist[u] - 1;
                            queueB[endB++] = v;
                        }
                    }
                }
                levelBRecord[++levelB] = endB;
            }
        }

        //part b: go back and find edges
        frontA = endA;
        frontB = endB;
        vector<vector<int> > sideA(maxDistance);
        vector<vector<int> > sideB(maxDistance);

        if(endMeet!=0) case1++;
        if(levelA+levelB==maxDistance)
        {
            case2++;
            for(int i=0;i<combination.size();i++)
            {
                sideA[(combination[i].to1)-1].push_back(combination[i].lm1);
                sideB[(combination[i].to2)-1].push_back(combination[i].lm2);
            }
        }

        for(int i=0;i<endMeet;i++) //push all predecessors of v w.r.t a and b to queueA and queueB separately
        {
            u = meet[i];
            for(int j=0;j<adjList[u].size();j++)
            {
                v = adjList[u][j];
                if(dist[v]==levelA) //predecessor of v w.r.t a
                {
                    //cout << u << " " << v << " ";
                    queueA[endA++] = v;
                }
                else if(dist[v]==0-levelB) //predecessors of v w.r.t b
                {
                    //cout << u << " " << v << " ";
                    queueB[endB++] = v;
                }
            }
            dist[u] = INF;
        }

        if(!sideA[levelA].empty())
        {
            for(int i=levelARecord[levelA-1];i<levelARecord[levelA];i++)
            {
                u = queueA[i];
                for(int j=0;j<sideA[levelA].size();j++)
                {
                    if(labels[u][sideA[levelA][j]]==1)
                    {
                        //cout << u << " " << sideA[levelA][j] << " " ;
                        for(int z=j+1;z<sideA[levelA].size();z++)
                        {
                            if(labels[u][sideA[levelA][z]]==1)
                            {
                                //cout <<
                            }
                        }
                        if(dist[u]==INF) break;
                        for(int z=0;z<adjList[u].size();z++)
                        {
                            if(dist[adjList[u][z]]==levelA)
                            {
                                queueA[endA++] = adjList[u][z];
                            }
                        }
                        break;
                    }
                }
                dist[u] = INF;
            }
        }
        else{
            for(int i=levelARecord[levelA-1];i<levelARecord[levelA];i++) dist[queueA[i]] = INF;
        }

        if(!sideB[levelB].empty())
        {
            for(int i=levelBRecord[levelB-1];i<levelBRecord[levelB];i++)
            {
                u = queueB[i];
                for(int j=0;j<sideB[levelB].size();j++)
                {
                    if(labels[u][sideB[levelB][j]]==1)
                    {
                        //cout << u << " " << sideB[levelB][j] << " " ;
                        for(int z=j+1;z<sideB[levelB].size();z++)
                        {
                            if(labels[u][sideB[levelB][z]]==1)
                            {
                                //cout
                            }
                        }
                        if(dist[u]==INF) break;
                        for(int z=0;z<adjList[u].size();z++)
                        {
                            if(dist[adjList[u][z]]==0-levelB)
                            {
                                queueB[endB++] = adjList[u][z];
                            }
                        }
                        break;
                    }
                }
                dist[u] = INF;
            }
        }
        else{
            for(int i=levelBRecord[levelB-1];i<levelBRecord[levelB];i++) dist[queueB[i]] = INF;
        }
        levelA--; levelB--;

        while(levelA>0)
        {
            int p = endA;
            while(frontA!=p) //queueA not empty
            {
                u = queueA[frontA++];
                if(dist[u]==INF) continue; //a vertex may appear in the queue more than once.
                for(int i=0;i<adjList[u].size();i++)
                {
                    v = adjList[u][i];
                    if(dist[v]+1==dist[u]) //predecessors w.r.t a
                    {
                        queueA[endA++] = v;
                        //cout << u << " " << v << " ";
                    }
                }
                dist[u] = INF; //avoid visiting a vertex twice by setting its dist=99
            }
            if(!sideA[levelA].empty())
            {
                for(int i=levelARecord[levelA-1];i<levelARecord[levelA];i++)
                {
                    u = queueA[i];
                    for(int j=0;j<sideA[levelA].size();j++)
                    {
                        if(labels[u][sideA[levelA][j]]==1)
                        {
                            //cout << u << " " << sideA[levelA][j] << " " ;
                            for(int z=j+1;z<sideA[levelA].size();z++)
                            {
                                if(labels[u][sideA[levelA][z]]==1)
                                {
                                    //cout <<
                                }
                            }
                            if(dist[u]==INF)break;
                            for(int z=0;z<adjList[u].size();z++)
                            {
                                if(dist[adjList[u][z]]==levelA)
                                {
                                    queueA[endA++] = adjList[u][z];
                                }
                            }
                            break;
                        }
                    }
                    dist[u] = INF;
                }
            }
            else{
                for(int i=levelARecord[levelA-1];i<levelARecord[levelA];i++) dist[queueA[i]] = INF;
            }
            levelA--;
        }

        while(levelB>0)
        {
            int p = endB;
            while(frontB!=p)
            {
                u = queueB[frontB++];
                if(dist[u]==INF) continue;
                for(int i=0;i<adjList[u].size();i++)
                {
                    v = adjList[u][i];
                    if(dist[v]==dist[u]+1)
                    {
                        queueB[endB++] = v;
                        //cout << u << " " << v << " ";
                    }
                }
                dist[u] = INF;
            }
            if(!sideB[levelB].empty())
            {
                for(int i=levelBRecord[levelB-1];i<levelBRecord[levelB];i++)
                {
                    u = queueB[i];
                    for(int j=0;j<sideB[levelB].size();j++)
                    {
                        if(labels[u][sideB[levelB][j]]==1)
                        {
                            //cout << u << " " << sideB[levelB][j] << " " ;
                            for(int z=j+1;z<sideB[levelB].size();z++)
                            {
                                if(labels[u][sideB[levelB][z]]==1)
                                {
                                    //cout <<
                                }
                            }
                            if(dist[u]==INF)break;
                            for(int z=0;z<adjList[u].size();z++)
                            {
                                if(dist[adjList[u][z]]==0-levelB)
                                {
                                    queueB[endB++] = adjList[u][z];
                                }
                            }
                            break;
                        }
                    }
                    dist[u] =  INF;
                }
            }
            else{
                for(int i=levelBRecord[levelB-1];i<levelBRecord[levelB];i++) dist[queueB[i]] = INF;
            }
            levelB--;
        }
        dist[a] = INF;
        dist[b] = INF;

        return;
    }

};

int main()
{
    srand(1);
    cout << k;
    spQuery test = spQuery();
    test.seriesTest(1000);
    return 0;
}
