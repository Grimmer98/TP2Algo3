#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include <iomanip>

using namespace std;

enum { WHITE = 0, GREY = 1, BLACK = 2 };

int N, R, W, U, V;
vector<tuple<int, int, int>> listOfEdges;
vector<tuple<int, int>> offices;
vector<vector<tuple<int, int>>> edges;
vector<int> status;
float count_U, count_V;

void dfs(int v){
    status[v] = GREY;

    for (tuple<int, int> u : edges[v]){
        if(status[get<0>(u)] == WHITE){
            if(get<1>(u) <= R)
                count_U += U*get<1>(u);
            else
                count_V += V*get<1>(u);

            dfs(get<0>(u));
        }
    }
    status[v] = BLACK;
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
            if(i != j) listOfEdges.push_back(make_tuple(weight(i, j), i, j));
        }
    }
}

void merge(int inicio, int mid, int fin){
    vector<tuple<int, int, int>> left, right;

    right = vector<tuple<int, int, int>>(mid - inicio + 1);
    left = vector<tuple<int, int, int>>(fin - mid);

    for(int k = 0; k < left.size(); k++)
        left[k] = listOfEdges[inicio + k];

    for(int k = 0; k < right.size(); k++)
        right[k] = listOfEdges[mid + 1 + k];

    int indexLeft = 0, indexRight = 0, indexInicio = inicio;
    while(indexLeft < fin - mid && indexRight < mid - inicio + 1){
        if(get<0>(left[indexLeft]) < get<0>(right[indexRight])){
            listOfEdges[indexInicio] = left[indexLeft];
            indexLeft++;
        } else {
            listOfEdges[indexInicio] = right[indexRight];
            indexRight++;
        }
        indexInicio++;
    }

    while(indexLeft < fin - mid){
        listOfEdges[indexInicio] = left[indexLeft];
        indexLeft++;
        indexInicio++;
    }

    while(indexRight < mid - inicio + 1){
        listOfEdges[indexInicio] = right[indexRight];
        indexRight++;
        indexInicio++;
    }
}

void mergeSort(int inicio, int fin){
    if(inicio >= fin)
        return;
    
    int mid = inicio + (fin - inicio)/2;
    mergeSort(inicio, mid);
    mergeSort(mid + 1, fin);
    merge(inicio, mid, fin);
}

void kruskal(){
    DSU dsu(N);
    
    for(tuple<int, int, int> e : listOfEdges){
        if(N == W) return;
        if(dsu.find(get<1>(e)) != dsu.find(get<2>(e))){
            dsu.unite(get<1>(e), get<2>(e));
            edges[get<1>(e)].push_back(make_tuple(get<2>(e), get<0>(e)));
            edges[get<2>(e)].push_back(make_tuple(get<1>(e), get<0>(e)));
            N--;
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
        status = vector<int>(N, WHITE);
        listOfEdges = vector<tuple<int, int, int>>();
        edges = vector<vector<tuple<int, int>>>(N);
        count_U = count_V = 0;
        for (int i = 0; i < N; i++){
            cin >> office_x >> office_y;
            offices[i] = make_tuple(office_x, office_y);
        }

        buildEdges(); 
        mergeSort(0, listOfEdges.size() - 1); 
        kruskal();
        
        for(int v = 0; v < offices.size(); v++)
            if (status[v] == WHITE)
                dfs(v);
        
        k++;
        
        printf("Caso #%d: %0.3f %0.3f\n", k, count_U, count_V);
    }
}
