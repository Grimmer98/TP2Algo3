#include <vector>
#include <stdio.h>
#include <stack>
#include <list>

using namespace std;

enum { WHITE = 0, GREY = 1, BLACK = 2 };
stack<int> dfs_stack;
vector<int> stateInitial, stateTransposed, stateFinal;
vector<vector<int>> edges, edgesT;
list<int> cfcs;
vector<bool> sol;

void transposed_dfs(int v){
    stateTransposed[v] = GREY;
    
    for (int u : edgesT[v])
        if (stateTransposed[u] == WHITE)
            transposed_dfs(u);
    
    stateTransposed[v] = BLACK;
}

void initial_dfs(int v) {
    stateInitial[v] = GREY;
    
    for (int u : edges[v])
        if (stateInitial[u] == WHITE)
            initial_dfs(u);
    
    dfs_stack.push(v);
    stateInitial[v] = BLACK;
}

void final_dfs(int v){
    stateFinal[v] = GREY;
    
    for (int u : edges[v])
        if (stateFinal[u] == WHITE)
            final_dfs(u);
    
    stateFinal[v] = BLACK;
}

list<int> minimalSet(){
    list<int> res;

    for (int v = 0; v < stateInitial.size(); v++)
        if (stateInitial[v] == WHITE)
            initial_dfs(v);

    while (!dfs_stack.empty()){
        int v = dfs_stack.top(); dfs_stack.pop();
        if (stateTransposed[v] == WHITE){
            cfcs.push_back(v); transposed_dfs(v);
        }
    }

    for (auto it = cfcs.begin(); it != cfcs.end(); ++it){
        if (stateFinal[*it] == WHITE){
            sol[*it] = true; final_dfs(*it);
        }
    }

    for (int v = 0; v < sol.size(); v++) 
        if(sol[v])
            res.push_back(v + 1);
    return res;
}

int main(int argc, char const *argv[]){
    int n, m;
    scanf("%d %d", &n, &m);

    stateTransposed = stateInitial = stateFinal = vector<int>(n, WHITE);
    
    edges.resize(n);
    edgesT.resize(n);
    sol.resize(n, false);

    int v, w; 
    for (int i = 0; i < m; i++){
        scanf("%d %d", &v, &w);
        edges[v - 1].push_back(w - 1);
        edgesT[w - 1].push_back(v - 1);
    }

    list<int> res = minimalSet();
    printf("%li\n", res.size());
    for (int v : res){
        printf("%d ", v);
    }
}