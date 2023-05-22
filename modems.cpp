#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include <iomanip>

using namespace std;

enum { NO_LO_VI = 0, EMPECE_A_VER = 1, TERMINE_DE_VER = 2 };

int N, R, W, U, V;
vector<tuple<int, int, int>> listOfEdges;
vector<tuple<int, int>> offices;
list<tuple<int, int, int>> treeEdges;
vector<bool> hasModem;
vector<vector<tuple<int, int>>> edges;
vector<int> status;
float count_U, count_V;

void dfs(int v){
    status[v] = EMPECE_A_VER;

    for (tuple<int, int> u : edges[v]){
        if(status[get<0>(u)] == NO_LO_VI){
            if(get<1>(u) <= R)
                count_U += U*get<1>(u);
            else
                count_V += V*get<1>(u);

            dfs(get<0>(u));
        }
    }
    status[v] = TERMINE_DE_VER;
}

struct DSU{
    DSU(int n){
        padre = rank = vector<int>(n);
        for(int v = 0; v < n; v++) padre[v] = v;
    }

    int find(int v){
        if(v == padre[v]) return v;
        return padre[v] = find(padre[v]);
    }

    void unite(int u, int v){
        u = find(u), v = find(v);
        if(u == v) return;
        if(rank[u] < rank[v]) swap(u, v);
        padre[v] = padre[u];
        rank[u] = max(rank[u],rank[v]+1);
    }

    vector<int> padre;
    vector<int> rank;
};

int weight(int i, int j){
    return  abs(get<0>(offices[i]) - get<0>(offices[j])) +
            abs(get<1>(offices[i]) - get<1>(offices[j]));
}

void buildEdges(){
    for (int i = 0; i < offices.size(); i++){
        for (int j = 0; j < offices.size(); j++){
            if(i != j) 
                listOfEdges.push_back(make_tuple(weight(i, j), i, j));
        }
    }
}

void sortEdges(){
    for (int i = 0; i < listOfEdges.size(); i++){
        int min = i;
        for (int j = i + 1; j < listOfEdges.size(); j++){
            if(get<0>(listOfEdges[j]) < get<0>(listOfEdges[i]))
                min = j;
        }
        tuple<int, int, int> aux = listOfEdges[i];
        listOfEdges[i] = listOfEdges[min];
        listOfEdges[min] = aux;
    }
}

void kruskal(int n){
    DSU dsu(n);
    treeEdges = list<tuple<int, int, int>>();
    for(tuple<int, int, int> e : listOfEdges){
        //si (u,v) es arista segura
        if(dsu.find(get<1>(e)) != dsu.find(get<2>(e))){
            // agregar
            dsu.unite(get<1>(e), get<2>(e));
            treeEdges.push_back(make_tuple(get<1>(e), get<2>(e), get<0>(e)));
        }
    }
}

int main(int argc, char const *argv[]){
    int c, k = 0;
    int office_x, office_y;

    cin >> c;
    while (k < c){
        cin >> N >> R >> W >> U >> V;

        offices = vector<tuple<int, int>>(N);
        hasModem = vector<bool>(N, false);
        listOfEdges = vector<tuple<int, int, int>>();
        edges = vector<vector<tuple<int, int>>>(N + 1);
        count_U = count_V = 0;
        for (int i = 0; i < N; i++){
            cin >> office_x >> office_y;
            offices[i] = make_tuple(office_x, office_y);
        }

        buildEdges();

        sortEdges();
        
        kruskal(N);

        for (auto it = treeEdges.rbegin(); it != treeEdges.rend() && W > 0; ++it){
            int u = get<0>(*it);
            int v = get<1>(*it);
            int weight = get<2>(*it);

            if(!hasModem[u]){ 
                hasModem[u] = true; 
                W--; 
                edges[N].push_back(make_tuple(u, 0)); 
            }

            if(W > 0 && !hasModem[v]) { 
                hasModem[v] = true; 
                W--; 
                treeEdges.pop_back(); 
                edges[N].push_back(make_tuple(v, 0));
            }
        }

        status = vector<int>(N + 1, NO_LO_VI);
        for (tuple<int, int, int> e : treeEdges){
            edges[get<0>(e)].push_back(make_tuple(get<1>(e), get<2>(e)));
            edges[get<1>(e)].push_back(make_tuple(get<0>(e), get<2>(e)));
        }
        
        dfs(N);

        k++;
        
        printf("Caso #%d: %0.3f %0.3f\n", k, count_U, count_V);
    }
}