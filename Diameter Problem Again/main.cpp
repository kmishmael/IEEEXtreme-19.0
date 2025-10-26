#include <bits/stdc++.h>
using namespace std;

static inline int readInt(){
    int x=0, s=1, c=getchar_unlocked();
    while(c!='-' && (c<'0'||c>'9')) c=getchar_unlocked();
    if(c=='-'){ s=-1; c=getchar_unlocked(); }
    while(c>='0'&&c<='9'){ x = x*10 + (c-'0'); c=getchar_unlocked(); }
    return x*s;
}
static inline void writeInt(long long x){
    if(x==0){ putchar_unlocked('0'); putchar_unlocked('\n'); return; }
    if(x<0){ putchar_unlocked('-'); x=-x; }
    char s[32]; int n=0;
    while(x){ s[n++]=char('0'+(x%10)); x/=10; }
    while(n--) putchar_unlocked(s[n]);
    putchar_unlocked('\n');
}

struct Edge { int to, w; };
int N;
vector<vector<Edge>> G;

vector<char> dead;
vector<int> parentTmp, szTmp, visTmp;

struct Item { int c, tag, dist; }; 
vector<vector<Item>> chainInfo;      

static uint32_t SEED = 0x9e3779b9;
static inline uint32_t rngu(){
    SEED ^= SEED << 13;
    SEED ^= SEED >> 17;
    SEED ^= SEED << 5;
    return SEED;
}

const int NEG_INF = -1000000000;

struct Best2 {
    int v1, t1;
    int v2, t2;
};
static inline Best2 emptyBest(){ return {NEG_INF, -1, NEG_INF, -1}; }
static inline void pushCand(Best2 &b, int v, int t){
    if(t==-1 || v<=NEG_INF) return;
    if(b.t1==t){
        if(v>b.v1) b.v1=v;
    }else if(v>b.v1){
        b.v2=b.v1; b.t2=b.t1;
        b.v1=v; b.t1=t;
    }else if(b.t2==t){
        if(v>b.v2) b.v2=v;
    }else if(v>b.v2){
        b.v2=v; b.t2=t;
    }
}

struct TNode {
    int l, r;          
    uint32_t prio;
    int key;            

    int leaf_val;     
    int leaf_tag;      

    int a1, t1;      
    int a2, t2;       
};
vector<TNode> pool; 

static inline int newNode(int key, int val, int tag){
    pool.push_back({});
    int id = (int)pool.size()-1;
    TNode &x = pool[id];
    x.l = x.r = 0;
    x.prio = rngu();
    x.key = key;
    x.leaf_val = val;
    x.leaf_tag = tag;
    x.a1 = val; x.t1 = tag;
    x.a2 = NEG_INF; x.t2 = -1;
    return id;
}
static inline void pull(int id){
    TNode &x = pool[id];
    Best2 b = emptyBest();
    if(x.l){
        pushCand(b, pool[x.l].a1, pool[x.l].t1);
        pushCand(b, pool[x.l].a2, pool[x.l].t2);
    }
    pushCand(b, x.leaf_val, x.leaf_tag);
    if(x.r){
        pushCand(b, pool[x.r].a1, pool[x.r].t1);
        pushCand(b, pool[x.r].a2, pool[x.r].t2);
    }
    x.a1=b.v1; x.t1=b.t1; x.a2=b.v2; x.t2=b.t2;
}
static void splitKey(int id, int key, int &L, int &R){ // L: keys <= key
    if(!id){ L=R=0; return; }
    if(pool[id].key <= key){
        splitKey(pool[id].r, key, pool[id].r, R);
        L=id; pull(L);
    }else{
        splitKey(pool[id].l, key, L, pool[id].l);
        R=id; pull(R);
    }
}
static int mergeTreap(int A, int B){
    if(!A||!B) return A?A:B;
    if(pool[A].prio < pool[B].prio){
        pool[A].r = mergeTreap(pool[A].r, B);
        pull(A); return A;
    }else{
        pool[B].l = mergeTreap(A, pool[B].l);
        pull(B); return B;
    }
}
static inline void insertKey(int &root, int key, int val, int tag){
    int L=0, R=0;
    splitKey(root, key, L, R);
    int node = newNode(key, val, tag);
    root = mergeTreap(mergeTreap(L, node), R);
}
static inline void eraseKey(int &root, int key){
    int A=0, B=0, C=0;
    splitKey(root, key, A, C);      
    splitKey(A, key-1, A, B);       
    // drop B
    root = mergeTreap(A, C);
}
static inline Best2 queryRange(int &root, int Lkey, int Rkey){
    int A=0, B=0, C=0;
    splitKey(root, Rkey, A, C);         
    splitKey(A, Lkey-1, A, B);      
    Best2 ans = emptyBest();
    if(B){
        ans = {pool[B].a1, pool[B].t1, pool[B].a2, pool[B].t2};
    }
    root = mergeTreap(mergeTreap(A, B), C);
    return ans;
}

vector<int> roots;

vector<int> get_component_nodes(int start){
    vector<int> nodes; nodes.reserve(256);
    vector<int> st; st.push_back(start);
    visTmp[start]=1;
    while(!st.empty()){
        int u=st.back(); st.pop_back();
        nodes.push_back(u);
        for(const auto &e: G[u]){
            int v=e.to;
            if(dead[v] || visTmp[v]) continue;
            visTmp[v]=1; st.push_back(v);
        }
    }
    for(int u: nodes) visTmp[u]=0;
    return nodes;
}

int find_centroid_on_nodes(const vector<int>& nodes){
    for(int u: nodes){ parentTmp[u]=-1; szTmp[u]=0; visTmp[u]=0; }
    int root = nodes[0];
    vector<int> order; order.reserve(nodes.size());
    vector<int> st; st.push_back(root); parentTmp[root]=0; visTmp[root]=1;
    while(!st.empty()){
        int u=st.back(); st.pop_back();
        order.push_back(u);
        for(const auto &e: G[u]){
            int v=e.to; if(dead[v] || visTmp[v]) continue;
            parentTmp[v]=u; visTmp[v]=1; st.push_back(v);
        }
    }
    for(int u: nodes) visTmp[u]=0;
    for(int i=(int)order.size()-1;i>=0;--i){
        int u=order[i];
        szTmp[u]=1;
        for(const auto &e: G[u]){
            int v=e.to; if(dead[v] || parentTmp[v]!=u) continue;
            szTmp[u]+=szTmp[v];
        }
    }
    int total = (int)nodes.size();
    int best = root, bestMax = INT_MAX;
    for(int u: nodes){
        int mx = total - szTmp[u];
        for(const auto &e: G[u]){
            int v=e.to; if(dead[v] || parentTmp[v]!=u) continue;
            mx = max(mx, szTmp[v]);
        }
        if(mx < bestMax){ bestMax=mx; best=u; }
    }
    return best;
}

void add_distances_from_centroid(int c){
    int tagCounter = 0;
    for(const auto &e0: G[c]){
        int v0=e0.to; if(dead[v0]) continue;
        int tag = tagCounter++;
        vector<tuple<int,int,int>> st;
        st.emplace_back(v0, c, e0.w);
        while(!st.empty()){
            auto [u,p,d] = st.back(); st.pop_back();
            chainInfo[u].push_back({c, tag, d});
            for(const auto &e: G[u]){
                int v=e.to; if(v==p || dead[v]) continue;
                st.emplace_back(v, u, d + e.w);
            }
        }
    }
    int tagSelf = tagCounter;
    chainInfo[c].push_back({c, tagSelf, 0});
}

void decompose(int start){
    auto nodes = get_component_nodes(start);
    int c = find_centroid_on_nodes(nodes);
    add_distances_from_centroid(c);
    dead[c]=1;
    for(const auto &e: G[c]){
        int v=e.to; if(dead[v]) continue;
        decompose(v);
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    N = readInt();
    G.assign(N+1, {});
    for(int i=0;i<N-1;i++){
        int u = readInt();
        int v = readInt();
        int w = readInt(); 
        G[u].push_back({v,w});
        G[v].push_back({u,w});
    }

    dead.assign(N+1, 0);
    parentTmp.assign(N+1, -1);
    szTmp.assign(N+1, 0);
    visTmp.assign(N+1, 0);
    chainInfo.assign(N+1, {});
    decompose(1);

    roots.assign(N+1, 0);

    vector<vector<int>> posOf(N+1);

    int M = readInt();
    vector<int> A; A.reserve(M);

    pool.reserve( (size_t) (25ll * M + 100) );
    pool.push_back({}); // id 0 = null

    long long last_ans = 0;

    for(int qi=0; qi<M; ++qi){
        int type = readInt();
        if(type==1){
            int x = readInt();
            x = (int)(((long long)x ^ llabs(last_ans)) % N) + 1;

            A.push_back(x);
            int pos = (int)A.size();
            posOf[x].push_back(pos);

            const auto &vec = chainInfo[x];
            for(const auto &it: vec){
                insertKey(roots[it.c], pos, it.dist, it.tag);
            }
        }else if(type==2){
            if(!A.empty()){
                int pos = (int)A.size();
                int y = A.back(); A.pop_back();
                posOf[y].pop_back();
                const auto &vec = chainInfo[y];
                for(const auto &it: vec){
                    eraseKey(roots[it.c], pos);
                }
            }
        }else{
            int l = readInt();
            int r = readInt();
            int x = readInt();

            x = (int)(((long long)x ^ llabs(last_ans)) % N) + 1;
            int nA = (int)A.size();
            l = (int)(((long long)l ^ llabs(last_ans)) % nA) + 1;
            r = (int)(((long long)r ^ llabs(last_ans)) % nA) + 1;
            if(l>r) swap(l,r);

            int bestAns = NEG_INF;

            const auto &cv = chainInfo[x];
            for(const auto &it: cv){
                auto b = queryRange(roots[it.c], l, r);
                int pick = (b.t1 != it.tag ? b.v1 : b.v2);
                if(pick > NEG_INF) bestAns = max(bestAns, pick + it.dist);
            }

            auto &pv = posOf[x];
            if(!pv.empty()){
                auto itp = lower_bound(pv.begin(), pv.end(), l);
                if(itp != pv.end() && *itp <= r) bestAns = max(bestAns, 0);
            }

            writeInt(bestAns);
            last_ans = bestAns;
        }
    }
    return 0;
}