#include <stdio.h>
#include <vector>
#include <set>

using namespace std;
typedef vector<vector<int>> grafo;

enum { NO_LO_VI = 0, EMPECE_A_VER =1, TERMINE_DE_VER = 2 };

grafo G;
vector<int> treeEdges;
vector<int> estado;
vector<set<int>> ciclos;
vector<int> niveles;

set<int> combinar(set<int> s, set<int> t){
    set<int> res = s;
    for(auto x : t){
        res.insert(x);
    }
    return res;
}

void dfs(int v){
    estado[v] = EMPECE_A_VER;
    for (int u : G[v]){
        if(niveles[u]==-1)niveles[u] = niveles[v]+1;
        if (estado[u] == NO_LO_VI){
            treeEdges[u] = v;
            dfs(u);
        }else if(estado[u]!=NO_LO_VI && treeEdges[v]!=u){
            bool combino = false;
            set<int> aux;
            int x = u;
            int y = v; 
            if(niveles[u]<niveles[v]){
                x = v;
                y = u;
            }
            while(y!=x){
                aux.insert(x);
                x = treeEdges[x];
            }
            aux.insert(y);
            if(ciclos.size()>0){
                for(int i=0;i<ciclos.size();i++){
                    set<int> combinacion = combinar(aux,ciclos[i]);
                    if(aux.size() + ciclos[i].size() 
                        > combinacion.size())
                        {
                            ciclos[i] = combinacion;
                            combino = true;     
                        }
                }
            }
            if(!combino)ciclos.push_back(aux);
        }
    }
    estado[v] = TERMINE_DE_VER;
}

int combinatorio(int n, int k){
    vector<vector<int>> A(n+1, vector<int>(k+1, 0));

    for (int i = 0; i <= n; i++){
        A[i][0] = 1;
    }
    
    for (int i = 0; i <= k; i++){
        A[i][i] = 1;
    }
    
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= min(i, k); j++){
            A[i][j] = A[i-1][j] + A[i-1][j-1];
        }
    }
    return A[n][k];
}

double juegoParaDos(int n){
    treeEdges.resize(n);
    treeEdges[0] = 0;
    estado.resize(n,NO_LO_VI);
    niveles.resize(n,-1);
    niveles[0] = 1;
    dfs(0);
    vector<int> sizes(ciclos.size());
    for(int i=0;i<ciclos.size();i++){
        sizes[i] = ciclos[i].size();
    }
    int res = 0;
    for(int i=0; i<sizes.size();i++){
        printf("%d\n", sizes[i]);
        res += combinatorio(sizes[i],2);
    }
    float x = (float)res/(float)combinatorio(n,2);
    
    return x;
}


int main(int argc, const char* argv[]){
    int n,m,v,w;
    scanf("%d %d", &n, &m);
    G.resize(n);
    for(int i=0;i<m;i++){
        scanf("%d %d",&v ,&w);
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
    }
    estado.resize(n);
    printf("%.5f", juegoParaDos(n));

}