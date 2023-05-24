#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;
typedef vector<vector<long long int>> grafo;

enum { NO_LO_VI = 0, EMPECE_A_VER = 1, TERMINE_DE_VER = 2 };

grafo G;
vector<bool> visited, visited_count;
vector<int> bridges;
vector<vector<double>> A;
vector<int> tin;
vector<int> low;
long long int counter = 1;
int timer;

void dfs_bridges(long long int v, long long int father=-1){
    visited[v] = true;
    tin[v] = low[v] = timer++;
    for (int to : G[v]) {
        if (to == father) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs_bridges(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] > tin[v])
                bridges[to] = 0;
        }
    }
}

void find_bridges(int n) {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    for (int i = 0; i < n; i++) {
        if (!visited[i])
            dfs_bridges(i);
    }
}

void dfs_count(long long int v){
    visited_count[v] = true;
    for(auto u : G[v]){
        if(!visited_count[u] && bridges[u]!=0){
            counter++;
            dfs_count(u);
        }
    }
}

double combinatorio(long long int n, int k){
    if(k==0) return 1;
    if(n==k) return 1;
    if(A[n-1][k]==0){A[n-1][k]=combinatorio(n-1,k);}
    if(A[n-1][k-1]==0){A[n-1][k-1]=combinatorio(n-1,k-1);}
    return A[n-1][k]+A[n-1][k-1];
}

double juegoParaDos(long long int n){
    visited.resize(n, false);
    visited_count.resize(n, false);

    find_bridges(n);
        
    vector<long long int> sizeCC;
    for(int i = 0; i < n; i++){
        if(!visited_count[i]){
            dfs_count(i);
            if(counter >= 2) sizeCC.push_back(counter); counter = 1;
        }
    }
    //printf("tamanio CC's: ");
    //for(int i=0;i<sizeCC.size();i++){printf("%d ", sizeCC[i]);}
    //printf("\n");
    double res = 0;
    for(int i = 0; i < sizeCC.size(); i++){
        res += combinatorio(sizeCC[i] ,2);
    }
    return 1- (res/combinatorio(n,2));
}


int main(int argc, const char* argv[]){
    long long int n, m, v, w;
    scanf("%lld %lld", &n, &m);
    G.resize(n);
    for(int i = 0; i < m; i++){
        scanf("%lld %lld",&v ,&w);
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
    }
    tin.resize(n,-1);
    low.resize(n,-1);
    bridges.resize(n,-1);
    A.resize(n+1, vector<double>(3, 0));
    printf("%.5f", juegoParaDos(n));

}
