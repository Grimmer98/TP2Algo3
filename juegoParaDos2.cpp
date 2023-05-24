#include <vector>
#include <iostream>
#include <list>
#include <iomanip>

using namespace std;

enum { WHITE = 0, GREY = 1, BLACK = 2 };

vector<vector<int>> G;
vector<list<int>> treeEdges;
vector<int> status, status_count; 
vector<int> dp; vector<int> up; vector<int> down;
vector<vector<int>> A;
int count = 1;

float combinatorio(long long int n, int k){
    for (int i = 0; i <= n; i++){A[i][0] = 1;}
    for (int i = 0; i <= k; i++){A[i][i] = 1;}
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= min(i, k); j++){
            A[i][j] = A[i-1][j] + A[i-1][j-1];
        }
    }
    return A[n][k];
}

void dfs_bridges(int v, int father){
    status[v] = GREY;
    for (auto u : G[v]){
        if (status[u] == WHITE){
            treeEdges[v].push_back(u);
            dfs_bridges(u, v);
        } else if (u != father && u > v){
            up[v]++; down[u]++;
        }
    }
    status[v] = BLACK;
}

int cover(int v, int father = -1){
    if(dp[v] != -1) return dp[v];
    int res = 0;
    res += up[v];
    res -= down[v];
    for(int child : treeEdges[v]){
        if(child != father) res += cover(child, v);
    }
    dp[v] = res;
    return res;
}

void dfs_count(int v){
    status_count[v] = GREY;
    for(auto u : G[v]){
        if(status_count[u] == WHITE && dp[u] != 0){
            count++;
            dfs_count(u);
        }
    }
    status_count[v] = BLACK;
}

float juegoParaDos(int n){
    treeEdges = vector<list<int>>(n);
    status_count = status = vector<int>(n, WHITE);

    list<int> roots;
    for(int i = 0; i < n; i++){
        if(status[i] == WHITE){ 
            roots.push_back(i); 
            dfs_bridges(i, -1);
        }
    }

    for (int v : roots) cover(v);

    list<int> sizeCC;
    for(int i = 0; i < n; i++){
        if(status_count[i] == WHITE){
            dfs_count(i);
            if(count >= 2) 
                sizeCC.push_back(count); 
            count = 1;
        }
    }
    int res = 0;

    for (int v : sizeCC)
        res += combinatorio(v, 2);
    
    return 1 - res / combinatorio(n, 2);
}

int main(int argc, const char* argv[]){
    int n, m, v, w;
    scanf("%d %d", &n, &m);

    G = vector<vector<int>>(n, vector<int>());
    for(int i = 0; i < m; i++){
        scanf("%d %d",&v ,&w);
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
    }

    up = down = vector<int>(n, 0);
    dp = vector<int>(n, -1);
    A.resize(n + 1, vector<int>(3, 0));
    printf("%.5f", juegoParaDos(n));
}