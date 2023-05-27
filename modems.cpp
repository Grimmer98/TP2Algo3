#include <iostream>
#include <bits/stdc++.h>
#include <tuple>
#include <list>
#include <iomanip>
#include <math.h>

using namespace std;

int N, R, W, U, V;
vector<tuple<double, int, int>> listOfEdges;
list<double> treeEdges;
vector<tuple<int, int>> offices;
double count_U, count_V;

double weight(int i, int j){
    double a = get<0>(offices[i]) - get<0>(offices[j]);
    double b = get<1>(offices[i]) - get<1>(offices[j]);
    return sqrt(a*a + b*b);
}

void buildEdges(){
    for (int i = 0; i < offices.size(); i++){
        for (int j = i + 1; j < offices.size(); j++){
            listOfEdges.push_back(make_tuple(weight(i, j), i, j));
        }
    }
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
        rank[u] = max(rank[u], rank[v]+1);
    }

    vector<int> padre;
    vector<int> rank;
};

void kruskal(){
    DSU dsu(N);
    
    for(tuple<double, int, int> e : listOfEdges){
        if(N == W) return;
        if(dsu.find(get<1>(e)) != dsu.find(get<2>(e))){
            dsu.unite(get<1>(e), get<2>(e));
            treeEdges.push_back(get<0>(e));
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
        listOfEdges = vector<tuple<double, int, int>>();
        treeEdges = {};
        count_U = count_V = 0;
        for (int i = 0; i < N; i++){
            cin >> office_x >> office_y;
            offices[i] = make_tuple(office_x, office_y);
        }

        buildEdges();
        sort(listOfEdges.begin(), listOfEdges.end());
        kruskal();
        
        for(double w : treeEdges){
            if(w <= R) count_U += (double)U * w;
            else count_V += (double)V * w;
        }

        k++;
        
        printf("Caso #%d: %0.3f %0.3f\n", k, count_U, count_V);
    }
}