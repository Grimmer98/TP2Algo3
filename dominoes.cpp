#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

enum { WHITE = 0, GREY = 1, BLACK = 2 };
vector<int> stack, state, stateT, stateFinal;
vector<vector<int>> edges, edgesT;
vector<int> cfcs;
vector<bool> sol;

void transposed_dfs(int v){
    stateT[v] = GREY;
    
    for (int u : edgesT[v])
        if (stateT[u] == WHITE)
            transposed_dfs(u);
    
    stateT[v] = BLACK;
}

void initial_dfs(int v) {
    state[v] = GREY;
    
    for (int u : edges[v])
        if (state[u] == WHITE)
            initial_dfs(u);
    
    stack.insert(stack.begin(), v);
    state[v] = BLACK;
}

void final_dfs(int v){
    stateFinal[v] = GREY;
    
    for (int u : edges[v])
        if (stateFinal[u] == WHITE)
            final_dfs(u);
    
    stateFinal[v] = BLACK;
}

vector<int> minimalSet(){
    vector<int> res;

    for (int v = 0; v < state.size(); v++)
        if (state[v] == WHITE)
            initial_dfs(v);

    for (int v : stack){
        if (stateT[v] == WHITE){
            cfcs.push_back(v); transposed_dfs(v); 
        }
    }

    for (int v : cfcs){
        if (stateFinal[v] == WHITE){
            sol[v] = true; final_dfs(v);
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

    stateT = state = stateFinal = vector<int>(n, WHITE);
    
    edges.resize(n);
    edgesT.resize(n);
    sol.resize(n, false);

    int v, w; 
    for (int i = 0; i < m; i++){
        scanf("%d %d", &v, &w);
        edges[v - 1].push_back(w - 1);
        edgesT[w - 1].push_back(v - 1);
    }

    vector<int> res = minimalSet();
    printf("%li\n", res.size());
    for (int v : res){
        printf("%d ", v);
    }
}