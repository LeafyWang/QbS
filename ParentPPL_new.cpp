#include<iostream>
#include<vector>
#include<queue>
#include<map>
#include<set>
#include<math.h>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<stdint.h>
#include <bits/stdc++.h>

#include<sys/time.h>

#define INF 99

#define originFile "test_pro.txt"
#define outputFile "test_parentpll.txt"
#define inputFile  "test_parentpll.txt"

using namespace std;

struct label{
    int         l;
    uint8_t     d;
    vector<int> parent;
    label(int x,int y,vector<int> v){
        l = x;
        d = y;
        parent = v;
    }
};

struct edge{
    int a,b;
    edge(int x,int y)
    {
        a = min(x,y);
        b = x+y-a;
    }
};

struct mid{
    int m;
    vector<int> parentS;
    vector<int> parentT;
    mid(int x, vector<int> y, vector<int> z)
    {
        x = m;
        y = parentS;
        z = parentT;
    }
};
class pathPLL
{
private:
    int countVertex;

    vector<vector<label> > labelList;

    long labelSize;

    set<vector<int> > subPro;

public:
    pathPLL()
    {
        //
    }

    void labelConstruction()
    {
        ifstream graphFile(originFile);

        stringstream ss;
        string lineC;

        getline(graphFile,lineC);
        ss << lineC;
        ss >> countVertex;
        ss.clear();
        ss.str("");

        vector<vector<int> > adjList;

        vector<label> labelTemp;

        int u,v,d;
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
            labelList.push_back(labelTemp);
            ss.clear();
            ss.str("");
        }
        graphFile.close();

        uint8_t* dist = new uint8_t[countVertex];
        uint8_t* prune= new uint8_t[countVertex];

        long parentSize = 0;
        timeval st,ed;

        gettimeofday(&st,NULL);
        for(int i=0;i<countVertex;i++) // a pruned BFS
        {
            queue<int> qLabel;
            queue<int> qPrune;

            memset(dist,INF,countVertex);
            memset(prune,0,countVertex);

            vector<vector<int> > parentsList;
            parentsList.resize(countVertex);

            qLabel.push(i);
            dist[i] = 0;
            qLabel.push(-1);

            while(qLabel.front()!=-1)
            {
                while(true)
                {
                    u = qLabel.front();
                    qLabel.pop();

                    if(u==-1)
                    {
                        qLabel.push(-1);
                        qPrune.push(-1);
                        break;
                    }

                    label temp(i,dist[u],parentsList[u]);
                    labelList[u].push_back(temp);

                    if(prune[u] || u<i)
                    {
                        qPrune.push(u);
                    }
                    else{
                        for(int j=0;j<adjList[u].size();j++)
                        {
                            v = adjList[u][j];
                            if(dist[v]==INF)
                            {
                                dist[v] = dist[u] + 1;
                                parentsList[v].push_back(u);
                                qLabel.push(v);
                                parentSize++;
                            }
                            else if(dist[v]==dist[u]+1)
                            {
                                parentsList[v].push_back(u);
                                parentSize++;
                            }
                        }
                    }
                }
                while(true)
                {
                    u = qPrune.front();
                    qPrune.pop();

                    if(u==-1) break;

                    for(int j=0;j<adjList[u].size();j++)
                    {
                        v = adjList[u][j];
                        if(dist[v]==INF)
                        {
                            dist[v] = dist[u] + 1;
                            qPrune.push(v);
                        }
                        else if(dist[v]==dist[u]+1 && prune[v]==0)
                        {
                            prune[v] = 1;
                        }
                    }
                }
            }

        }
        gettimeofday(&ed,NULL);
        long sumTime = (ed.tv_sec-st.tv_sec)*1000000 + (ed.tv_usec-st.tv_usec);
        cout << "construction time:" << sumTime << endl;

        //output label information to a file
        labelSize = 0;
        ofstream result(outputFile);
        for(int i=0;i<countVertex;i++)
        {
            result << i << " " << labelList[i].size();
            labelSize += labelList[i].size();
            for(int j=0;j<labelList[i].size();j++)
            {
                //if(labelList[i][j].l==i) continue; // remove the label from itself
                result << " " << labelList[i][j].l << " " << int(labelList[i][j].d) << " " << (labelList[i][j].parent).size() << " " ;
                for(int k=0;k<labelList[i][j].parent.size();k++)
                    result<< (labelList[i][j].parent)[k]  << " " ;
            }
            result << endl;
        }
        result.close();
        cout << "Label Size:" << labelSize << endl;
        cout << "Number of parents:" << parentSize << endl;
    }

    void input() //get label information from a file
    {
        ifstream labelFile(inputFile);

        int u, c, a, b, x, y;
        string line;
        stringstream ss;
        while(getline(labelFile, line))
        {
            ss << line;
            ss >> u;
            ss >> c;
            vector<label> labels;
            for(int i=0;i<c;i++) {
                ss >> a;
                ss >> b;
                ss >> x;
                vector<int> p(x);
                for(int j=0;j<x;j++)
                {
                    ss >> p[j];
                }
                label temp(a,b,p);
                labels.push_back(temp);
            }
            labelList.push_back(labels);
            ss.clear();
            ss.str("");
        }
        countVertex = labelList.size();
        cout << "|V|: " << countVertex << endl;
        labelFile.close();
    }

    int QueryDistance(int s,int t)
    {
        int mind = INF;
        int i=0;
        int j=0;
        while(i<labelList[s].size() && j<labelList[t].size())
        {
            if(labelList[s][i].l==labelList[t][j].l)
            {
                mind = min(mind, labelList[s][i].d+labelList[t][j].d);
                i++;
                j++;
            }
            else if(labelList[s][i].l<labelList[t][j].l)
                i++;
            else
                j++;
        }
        return mind;
    }

    void QueryPath(int s,int t)
    {
        vector<int> tp(2,s);
        if(s<t) tp[1] = t;
        else tp[0] = t;
        if(subPro.count(tp)==1) return;
        subPro.insert(tp);

        // cout << s <<  " " << t << endl;

        int mind = INF;
        int dtemp;

        vector<mid> midpoints;

        int i=0;
        int j=0;
        while(i<labelList[s].size() && j<labelList[t].size())
        {

            if(labelList[s][i].l==labelList[t][j].l)
            {
                dtemp = labelList[s][i].d+labelList[t][j].d;

                if(mind==dtemp)
                {
                    mid mp(labelList[s][i].l, labelList[s][i].parent, labelList[t][j].parent);
                    midpoints.push_back(mp);
                }

                else if(mind>dtemp){
                    mind = dtemp;
                    vector<mid> ().swap(midpoints);
                    mid mp(labelList[s][i].l,labelList[s][i].parent,labelList[t][j].parent);
                    midpoints.push_back(mp);
                }
                i++;
                j++;
            }
            else if(labelList[s][i].l<labelList[t][j].l){
                i++;
            }
            else{
                j++;
            }
        }

        if(mind==1||mind==2)
        {
            //cout << s << " " << t << endl;
            return;
        }
        else if(mind==INF) return;
        else{
            for(int i=0;i<midpoints.size();i++)
            {
                for(int j=0;j<midpoints[i].parentS.size();j++)
                {
                    QueryPath(midpoints[i].parentS[j], midpoints[i].m);
                }
                for(int j=0;j<midpoints[i].parentT.size();j++)
                {
                    QueryPath(midpoints[i].parentT[j], midpoints[i].m);
                }
            }
        }
        return;
    }

    void optQueryPath(int s, int t)
    {
        subPro.clear();
        QueryPath(s,t);
        return ;
    }

    unsigned int rand_32()
    {
        return (rand()&0x3)<<30 | rand()<<15 | rand();
    }

    void QueryTest(int testTime)
    {
        timeval st,ed;
        gettimeofday(&st,NULL);
        int s, t;
        for(int i=0;i<testTime;i++)
        {
            s = rand_32()%countVertex;
            t = rand_32()%countVertex;
            optQueryPath(s,t);
        }
        gettimeofday(&ed,NULL);
        long sumTime = (ed.tv_sec-st.tv_sec)*1000000 + (ed.tv_usec-st.tv_usec);
        cout << "average query time:" << sumTime*1.0/testTime << endl;

        return ;
        float sumd = 0;
        int dt;
        for(int i=0;i<testTime;i++)
        {
            s = rand_32()%countVertex;
            t = rand_32()%countVertex;
            dt = QueryDistance(s,t);
            if(dt!=INF) sumd += dt;
        }
        cout << "average distance:" << sumd/testTime << endl;

    }
};

int main()
{
    srand(1);
    pathPLL test = pathPLL();
   // test.labelConstruction();
    test.input();
    //test.optQueryPath(0,5);
    //cout << "finished reading..." << endl;
    //test.QueryTest(10000);
    return 0;
}
