#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

ifstream f("dfs.in");
ofstream g("dfs.out");

bool vizitat_transpusa[100005], vizitat[100005];

class graf{
private:
    int n, m, cost_total,curent,parinte[100001],dimensiune[100001],distanta[1000], nr_parcurgeri[1000];
    vector <vector <int> > muchii;
    vector <pair<int,int> >graf_ponderat;
    vector<pair<int,pair<int,int>>> muchii_cu_cost;
    vector <int> adj_list[100001];
    vector < pair<int,int> > v[100005];
    vector<int> graf_normal[100005];
    vector<int> graf_transpus[100005];
    vector<int> componente_conexe[100005];
    stack<int> S;
    bool InCoada[1000];
    bool vizitate[50001];
    priority_queue <pair<int, int>, vector <pair<int,int>>, greater<pair<int,int>>> pq;


public:
    graf(int n, int m);
    graf(int n);
    void citire_orientat(int m);
    void citire_neorientat(int m);
    void citire_orientat_adj_list();
    void citire_neorientat_adj_list();
    //-------------------------bfs              https://www.infoarena.ro/job_detail/2822363?action=view-source
    void bfs(int s);
    //-------------------------dfs              https://www.infoarena.ro/job_detail/2822469
    void dfs(int s);
    void conexe();
    //-------------------------havel hakimi
    bool Havel_Hakimi(vector<int>v, int n);
    //-------------------------sortare topologica   https://www.infoarena.ro/job_detail/2822400
    void st(int s);
    void sortare_topologica();
    //--------------------------CTC                 https://www.infoarena.ro/job_detail/2822154
    void Citire_cu_transpusa(); //ca sa formeze si transpusa
    void dfs_kosaraju(int nod);
    void dfs_transpus(int nod, int ct);
    void scc();
    //tema 2
    //--------------------------Dijkstra            https://www.infoarena.ro/job_detail/2816714
    void dijkstra(int nod, vector <pair<int, int>>G[50001]);
    void afiseaza_dijkstra();
    void init_dijkstra(int n);
    void dijkstraa(int n, int m);
    //--------------------------Bellman-Ford        https://www.infoarena.ro/job_detail/2822338
    void bellman_ford(int s, vector <pair<int, int>>G[1000]);
    //--------------------------APM
    void kruskall();
    //--------------------------Paduri de multimi disjuncte     https://www.infoarena.ro/job_detail/2816668
    int radacina_disjuncte(int n);
    void uniune_disjuncte(int x, int y);
    void init(int n);
    bool aceeasi_padure(int x, int y);
    void disjuncte(int n);
    //tema 3
    //--------------------------Diametrul arborelui             https://www.infoarena.ro/job_detail/2814377
    void bfs_darb(int s, int &ultim, int &dist_max);
    void darb();
    //--------------------------Roy-Floyd                       https://www.infoarena.ro/job_detail/2822228
    void roy_floyd(int M[105][105]);
    void royfloyd();
    //tema 4
    //--------------------------Ciclu Eulerian                  https://www.infoarena.ro/job_detail/2822150
    void CicluEuler();
    void Euler(int Start);
};
//---------------------------------------------------------TEMA-1-------------------------------------------------------//
graf::graf(int n, int m)
{
    this->n=n;
    this->m=m;
}
graf::graf(int n)
{
    this->n=n;
}


void graf::citire_neorientat(int m)
{
    int nod1, nod2;
    for (int i = 0; i <= m; i++)
    {
        f >> nod1 >> nod2;
        muchii[nod1].push_back(nod2);
        muchii[nod2].push_back(nod1);
    }
}
void graf::citire_orientat(int m)
{
    int nod1, nod2;
    for (int i = 0; i <= m; i++)
    {
        f >> nod1 >> nod2;
        muchii[nod1].push_back(nod2);
    }
}

void graf::citire_orientat_adj_list()
{
    for(int i = 1; i <= m; ++i)
    {
        int nod1, nod2;
        f>>nod1>>nod2;
        adj_list[nod1].push_back(nod2);
    }
}
void graf::citire_neorientat_adj_list()
{
    for(int i = 1; i <= m; ++i)
    {
        int nod1, nod2;
        f>>nod1>>nod2;
        adj_list[nod1].push_back(nod2);
        adj_list[nod2].push_back(nod1);
    }
}



void graf :: bfs(int s)
{
    queue <int> q;
    bool vizitat[100001];
    int distanta[100001];

    vizitat[s] = 1;
    q.push(s); //adaugam in coada s-ul de start si il marcam ca vizitat si distanta 0
    distanta[s]=0;

    while(!q.empty())//daca avem elemente in coada, executam:
    {
        int curent = q.front(); //nodul curent devine cel mai vechi nod adaugat in coada
        q.pop();
        //parcurgem lista de adicaneta pt a gasi varfurile adiacente nevizitate ale noduliui curent
        for(auto i : adj_list[curent])
            if(!vizitat[i])
            {
                vizitat[i] = 1;
                q.push(i);
                distanta[i] = distanta[curent] + 1;
            }
    }

    for(int i = 1; i <= n; ++i)
        if(vizitat[i])
            g << distanta[i] << " ";
        else
            g << -1 << " ";
}




void graf::dfs(int start_node)
{
    vizitate[start_node] = 1;
    for(int i: adj_list[start_node])
        if(!vizitate[i])
            dfs(i);
}
void graf::conexe()
{
    int nr_comp = 0;
    for(int i=1;i<=n;++i)
        if(!vizitate[i])
            {
                nr_comp=nr_comp+1;;
                dfs(i);
            }
    g<< nr_comp;
}





bool Havel_Hakimi(vector<int> &vect, int n)
{
    bool stare=1;
    while (stare)
    {
        sort(vect.begin(), vect.end(), greater<int>());
        if (vect[0] == 0)
            return true; //verificam daca primul element e 0 dupa sortare

        int a = vect[0];           //salvam valoarea primului el inainte sa il stergem
        vect.erase(vect.begin() + 0); //stergem primul element
        if (a > vect.size())
            return false; //atunci nu vom avea cum sa scadem 1 din primele a elemente ramase
        for (int i = 0; i < a; i++)
        {
            vect[i]--;
            if (vect[i] < 0)
                return false; //oprim executarea deoarece o valoare nu poate fi negativa in cazul in care putem avea graf
        }
    }
}





void graf :: st(int s)
{
    vizitate[s] = true;
    for(int i : adj_list[s])
        if(!vizitate[i])
            st(i);
    S.push(s);
}

void graf ::sortare_topologica()
{

    for(int i = 1; i <= n; ++i)
        if(!vizitate[i])
            st(i);
    while(!S.empty())
    {g << S.top() << " ";
    S.pop();}//afisam stiva cu rezultatul final
}






void graf :: Citire_cu_transpusa()
{
    for ( int i = 1; i <= m; i++ )
    {   int x, y;
        f >> x >> y;
        graf_normal[x].push_back(y);
        graf_transpus[y].push_back(x);}
}
void graf :: dfs_kosaraju(int nod)
{
    vizitat[nod] = 1;
    for ( auto i : graf_normal[nod] )
        if ( !vizitat[i] )
            dfs_kosaraju(i);
    S.push(nod);
}
void graf :: scc()
{
    int scc_nr = 0;
    while(!S.empty())
    {   int curent=S.top();
        S.pop();
        if(!vizitat_transpusa[curent])
        {scc_nr ++;
        dfs_transpus(curent, scc_nr);}
    }

    g<<scc_nr<<endl;
    for (int i=1; i<= scc_nr; i++)
    {   for(int j: componente_conexe[i])
                g<<j<<" ";
        g<<endl;
    }
}
void graf::dfs_transpus(int nod, int componenta)
{
    vizitat_transpusa[nod]=1;
    componente_conexe[componenta].push_back(nod);
    for (int i: graf_transpus[nod])
        if (!vizitat_transpusa[i])
            dfs_transpus(i, componenta);
}





void componente_tare_conexe(){
    int n ,m ;
    f >>n>>m;
    graf G(n,m);
    G.Citire_cu_transpusa();
     for (int i=1; i<=n; i++)
        if (!vizitat[i])
            G.dfs_kosaraju(i);
    G.scc();
}
////------------------------------------------TEMA-2------------------------------------------------------------///




void graf::init_dijkstra(int n){
    for(int i=1;i<=n;i++)
        {distanta[i] = INT_MAX;
        vizitate[i]=false;}
}
void graf::dijkstra(int s, vector <pair<int, int>>G[50001])
{
    init_dijkstra(n);
    distanta[s] = 0;
    pq.push(make_pair(distanta[s], s));
    vizitate[s] = true;

    while(!pq.empty()){
        curent=pq.top().second;
        vizitate[curent]=false;
        pq.pop();

    for(size_t i = 0; i < G[curent].size(); i++)
    {
        int vecin = G[curent][i].first;
        int cost = G[curent][i].second;

        if(distanta[curent] + cost < distanta[vecin])
        {
            distanta[vecin] = distanta[curent] + cost;
            if(!vizitate[vecin])
            {
                pq.push(make_pair(distanta[vecin], vecin));
                vizitate[vecin] = true;
            }


        }
    }

}}
void graf::afiseaza_dijkstra(){
    for(int i = 2; i <= n; i++)
        if(distanta[i] !=INT_MAX)
            g << distanta[i] << " ";
        else
            g <<" 0 "<<" ";
}
void graf::dijkstraa(int n, int m){
    int start_edge, end_edge, cost;
    vector <pair<int, int>>G[50001];
    for(int i = 0; i < m; i++)
    {
        f >> start_edge >> end_edge >> cost;
        G[start_edge].push_back(make_pair(end_edge, cost));
    }
    dijkstra(1, G);
    afiseaza_dijkstra();
}







void graf::bellman_ford (int s, vector <pair<int, int>>G[1000]){
    queue<int>q;
    bool exista_cost_negativ = false;
    for(int i=0;i<=n;i++)
        {distanta[i]=100000;
        InCoada[i]=false;
        nr_parcurgeri[i]=0;}
    distanta[s]=0;
    q.push(s);
    InCoada[s] = true;
    while(q.empty() == false && exista_cost_negativ == false)
    {
        int nod_curent = q.front();
        nr_parcurgeri[nod_curent]=1;
        q.pop();
        InCoada[nod_curent]=false;
        for ( size_t i = 0; i < G[nod_curent].size(); i++ )
        {
             int vecin = G[nod_curent][i].first;
             int cost = G[nod_curent][i].second;
             if(distanta[nod_curent]+cost< distanta[vecin])
             {
                 distanta[vecin]=distanta[nod_curent]+cost;
                 if(InCoada[vecin]==false)
                    {q.push(vecin);
                    InCoada[vecin]=true;}
             }
             nr_parcurgeri[vecin]++;
             if(nr_parcurgeri[i]>=n) //adica avem ciclu negativ,avand un nod care isi micsoreaza costul in continuare la mai mult de n-1 iteratii
             {
                 exista_cost_negativ=true;
                 break;
             }
        }
    }
    if (exista_cost_negativ==false){
        for(int i=2; i <=n;i++)
            g<<distanta[i]<<" ";
    }
    else g<<"Graful contine un ciclu negativ";
}






int graf :: radacina_disjuncte(int n)
{
   while(n!=parinte[n]){
        parinte[n]=radacina_disjuncte(parinte[n]);
        return parinte[n];
    }
    return n;
}

void graf :: uniune_disjuncte(int x, int y)
{
    if ( radacina_disjuncte(x) != radacina_disjuncte(y) )
    {
        if ( dimensiune[radacina_disjuncte(x)] > dimensiune[radacina_disjuncte(y)] )
            {parinte[radacina_disjuncte(y)] = radacina_disjuncte(x);
            dimensiune[x] += dimensiune[y];}
        else
        {
            parinte[radacina_disjuncte(x)] = radacina_disjuncte(y);
            dimensiune[y] += dimensiune[x];
        }
    }
}
void graf :: init(int n){
    for ( int i = 1; i <= n; i++ )
        {parinte[i] = i;
        dimensiune[i]=1;}
}

bool graf::aceeasi_padure(int x, int y){
    if (radacina_disjuncte(x)==radacina_disjuncte(y))
        return true;
    return false;
}

void graf :: disjuncte(int n)
{
    int comanda,x,y;

    init(n);
    for (int i = 1;i <= m;i++)
    { f>>comanda>>x>>y;
        if (comanda == 1)
            uniune_disjuncte(x,y);
        else if(comanda ==2)
        {if (aceeasi_padure(x,y))
                g<<"DA\n";
        else g<<"NU\n";}
    }
}





void graf:: kruskall(){

        sort(muchii_cu_cost.begin(),muchii_cu_cost.end());
        for(auto i:muchii_cu_cost) //ne folosim de functiile de la paduri de multimi disjuncte care erau deja implementate
            if(!aceeasi_padure(i.second.first , i.second.second)) //verificam sa nu se formeze ciclu
            {   uniune_disjuncte(i.second.second,i.second.first);
                graf_ponderat.push_back(make_pair(i.second.first, i.second.second)); //adaugam in arborele de afisare
                cost_total=cost_total+i.first;  } //salvam costul
                g<<cost_total<<endl; //afisam costul arborelui

        g<<graf_ponderat.size()<<endl; //afisam nr de linii
        for(size_t i=0; i< graf_ponderat.size(); i++)
            g<<graf_ponderat[i].first<<" "<<graf_ponderat[i].second<<endl; //afisam capetele muchiilor din arbore doua cate doua, cum se specifica in cerinta

}

//---------------------------------------------TEMA-3-------------------------------------------------------------------------------------//


//DARB


 void graf::bfs_darb(int s, int &ultim, int &dist_max) //am modificat primul bfs ca sa poata salva elem si dist pana la el
{
    dist_max=0;
    queue<int> q;
    int vizitat[100001];
    int distanta[100001];
    for (int i = 0; i < n; i++)
        {vizitat[i] = false;
        distanta[i]=0;}        //populeza vectorii si ii initializeaza ca si nevizitati
    q.push(s); //adaugam in coada s-ul de start si il marcam ca vizitat si distanta 0
    vizitat[s] = true;
    distanta[s] = 1;
    while (!q.empty()) //daca avem elemente in coada, executam:
    {
        int curent = q.front(); //nodul curent devine cel mai vechi nod adaugat in coada
        q.pop();
        //parcurgem lista de adicaneta pt a gasi varfurile adiacente nevizitate ale noduliui curent
        for (auto i:adj_list[curent])
        {
            if (!vizitat[i])
            {
                vizitat[i] = true;
                q.push(i);
                distanta[i] = distanta[curent] + 1;
                if(distanta[i]>dist_max)        //atunci cand gasim dist max salvam dist si elem
                {
                    dist_max=distanta[i];
                    ultim=i;
                }
            }
        }
    }

}
void graf::darb(){
    int start_node, end_node, distanta_maxima;
    f >> n;

    int nod1, nod2;
    for(int i = 1; i <= n; ++i)
    {
        f >> nod1 >> nod2;
        adj_list[nod1].push_back(nod2);
        adj_list[nod2].push_back(nod1);
    }

    bfs_darb(1, start_node, distanta_maxima);       //bfs de la nod 1 la prim din bfs final
    bfs_darb(start_node, end_node, distanta_maxima); //bfs de la prim la ultimul nod din bfs final

    g << distanta_maxima;
}



//--------------Roy-Floyd----------------------------------//



void graf::roy_floyd(int M[105][105]){

    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= n; ++j)
            if(i == j)
                M[i][j] = 0;
            else if(M[i][j] == 0 && i != j)
                M[i][j] = 10001;

    for(int k=0;k<n;k++)
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if (M[i][k]+M[k][j]<M[i][j])
                    M[i][j]=M[i][k]+M[k][j];

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if ( M[i][j] == 10001)
                M[i][j]==0;

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
            if(M[i][j] != 10001)
                g<<M[i][j]<<" ";
            else g << 0 << " ";
        g<<endl;
    }
}

void graf::royfloyd(){
    int n,i ,j;
    f>>n;
    int M[105][105];
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            f>>M[i][j];
    graf G(n);

    G.roy_floyd(M);
}
//---------------------------------------------TEMA-4-------------------------------------------------------------------------------------//



//-----------Ciclu-Eulerian-------//
void graf::Euler(int s)
{
    bool vizitat[500005];
    vector<int> rezultat;  //stocam rez final
    vector<int> vect; //dam push_back elem cu care urm sa lucram
    vect.push_back(s);      //adaugam nod start
    while (!vect.empty())     //cat timp mai avem elem in vect
    {
        int curent=vect.back(); //noteam el curent
        if (!v[curent].empty()) //daca mai are noduri la care se ajunge pornind din el
        {
            int urm=v[curent].back().first;
            int nr=v[curent].back().second;
            v[curent].pop_back(); //stergem elementul din vector
            if (!vizitat[nr])    //daca la muchia cu nr respectiv nu s-a ajuns inca
            {
                vizitat[nr]=true;     //marcam vizitata si o adaugam in vect de prelucrare
                vect.push_back(urm);
            }
        }
        else
        {
            vect.pop_back();        //cand se termina elem stergem elem pe care l-am lucrat
            rezultat.push_back(curent); //il adaugam in vect final
        }
    }
        for (size_t i=0; i<rezultat.size(); i++)
            g <<rezultat[i]<<" ";
}

void graf::CicluEuler()
{
    int x, y, grd[100005];
    bool vizitat[100005];
    for (int i=1; i<=m; i++)
    {
        f>>x>>y;
        v[x].push_back(make_pair(y,i));
        v[y].push_back(make_pair(x,i));
        grd[x]++;//gradul lui x
        grd[y]++;//gradul lui y
    }

    for ( int i=0; i<=n; i++ )
        if ( grd[i]%2==1 )
        {
            g << "-1";
            return;
        }
    Euler(1);
}

int main()
{

}
