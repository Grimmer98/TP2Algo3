#include <stdio.h>
#include <vector>
#include <tuple>

using namespace std;
typedef vector<vector<tuple<int,int>>> grafo;

enum { NO_LO_VI = 0, EMPECE_A_VER =1, TERMINE_DE_VER = 2 };

grafo G;
vector<vector<int>> treeEdges;
vector<int> parents;
vector<int> status;
vector<int> status_count;
vector<vector<int>> A;
int count =0;

void dfs_bridges(int v, int father,vector<int> &up,vector<int> &down){
    status[v] = EMPECE_A_VER;
    for (auto u : (G[v])){
        if (status[get<0>(u)] == NO_LO_VI){
            parents[get<0>(u)] = v;
            treeEdges[v].push_back(get<0>(u));
            dfs_bridges(get<0>(u),v,up,down);
        }else if (get<0>(u)!=father && get<0>(u)>v){//nodo gris o negro
            up[v]++; down[get<0>(u)]++;
        }
    }
    status[v] = TERMINE_DE_VER;
}

int cover(int v, int father,vector<int> &up,vector<int> &down, vector<int>& bridges){
    if(bridges[v]!=-1) return bridges[v];
    int res=0;
    res += up[v];
    res -= down[v];
    for(int child : treeEdges[v]){
        if(child!=father) res+= cover(child,v,up,down,bridges);
    }
    bridges[v] = res;
    return res;
}

void dfs_count(int v){
    status_count[v] = EMPECE_A_VER;
    for(auto u : G[v]){
        if(status_count[get<0>(u)]== NO_LO_VI && get<1>(u)==0){
            count++;
            dfs_count(get<0>(u));
        }
    }
    status_count[v] = TERMINE_DE_VER;
}

int combinatorio(int n, int k){
    for (int i = 0; i <= n; i++){A[i][0] = 1;}
    for (int i = 0; i <= k; i++){A[i][i] = 1;}
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= min(i, k); j++){
            A[i][j] = A[i-1][j] + A[i-1][j-1];
        }
    }
    return A[n][k];
}


double juegoParaDos(int n,vector<int> &up,vector<int> &down, vector<int>& bridges){
    treeEdges.resize(n,vector<int>());
    status.resize(n,NO_LO_VI);
    parents.resize(n,0);
    status_count.resize(n,NO_LO_VI);
    // si bridges[i]==0, entonces el la arista del nodo i con su padre
    // es un puente.
    vector<int> roots;
    for(int i=0; i<n;i++){
        if(status[i]==NO_LO_VI){roots.push_back(i);dfs_bridges(i,-1,up,down);}
    }
    for(int i=0;i<roots.size();i++){cover(roots[i],-1,up,down,bridges);}
    for(int i=1;i<n;i++){
        if(bridges[i]==0){
            get<1>(G[i][parents[i]]) = 1;
            get<1>(G[parents[i]][i]) = 1;
        }
    }
    vector<int> sizeCC;
    for(int i=0; i<n; i++){
        if(status_count[i]==NO_LO_VI){
            dfs_count(i);
            if(count>=2)sizeCC.push_back(count); count = 0;
        }
    }
    int res = 0;
    for(int i=0;i<sizeCC.size();i++){
        printf("%d", sizeCC[i]);
        res+=combinatorio(sizeCC[i],2);
    }
    double x = (double)res/(double)combinatorio(n,2);
    return x;
}


int main(int argc, const char* argv[]){
    int n,m,v,w;
    scanf("%d %d", &n, &m);
    G.resize(n,vector<tuple<int,int>>());
    for(int i=0;i<m;i++){
        scanf("%d %d",&v ,&w);
        G[v-1].push_back(make_tuple(w-1,0));
        G[w-1].push_back(make_tuple(v-1,0));
    }
    vector<int> up(n,0);
    vector<int> down(n,0);
    vector<int> bridges(n,-1);
    A.resize(n+1, vector<int>(n+1, 0));
    printf("%.5f", juegoParaDos(n,up,down,bridges));

}