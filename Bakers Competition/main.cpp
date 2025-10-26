#include <bits/stdc++.h>
using namespace std;

const int N = 1'000'000;
const int BLOCK_SIZE = 700;

vector<int> primes;
vector<int> primeIdx(N + 1, -1);

void sieve() {
    vector<bool> isPrime(N + 1, true);
    isPrime[0] = isPrime[1] = false;
    
    for (int i = 2; i * i <= N; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }
    
    primes.reserve(80000);
    for (int i = 2; i <= N; ++i) {
        if (isPrime[i]) {
            primeIdx[i] = primes.size();
            primes.push_back(i);
        }
    }
}

struct Semiprime {
    int val;
    int pidx, qidx;
};

vector<Semiprime> semiprimes;
vector<int> semival;

void buildSemiprimes() {
    semiprimes.reserve(200000);
    
    for (int i = 0; i < (int)primes.size(); ++i) {
        long long p = primes[i];
        if (p * p > N) break;
        
        for (int j = i; j < (int)primes.size(); ++j) {
            long long q = primes[j];
            long long product = p * q;
            if (product > N) break;
            
            semiprimes.push_back({(int)product, i, (i == j ? -1 : j)});
        }
    }
    
    sort(semiprimes.begin(), semiprimes.end(), 
         [](const Semiprime& a, const Semiprime& b) { return a.val < b.val; });
    
    semival.reserve(semiprimes.size());
    for (const auto& s : semiprimes) {
        semival.push_back(s.val);
    }
}

struct Query {
    int l, r, id;
    
    bool operator<(const Query& other) const {
        int block1 = l / BLOCK_SIZE;
        int block2 = other.l / BLOCK_SIZE;
        if (block1 != block2) return block1 < block2;
        return (block1 & 1) ? r > other.r : r < other.r;
    }
};

vector<int> cnt;
long long currentPairs = 0;

inline long long C2(long long x) {
    return x * (x - 1) / 2;
}

void add(int pos) {
    const Semiprime& s = semiprimes[pos];
    
    if (s.pidx >= 0) {
        currentPairs -= C2(cnt[s.pidx]);
        cnt[s.pidx]++;
        currentPairs += C2(cnt[s.pidx]);
    }
    
    if (s.qidx >= 0) {
        currentPairs -= C2(cnt[s.qidx]);
        cnt[s.qidx]++;
        currentPairs += C2(cnt[s.qidx]);
    }
}

void remove(int pos) {
    const Semiprime& s = semiprimes[pos];
    
    if (s.pidx >= 0) {
        currentPairs -= C2(cnt[s.pidx]);
        cnt[s.pidx]--;
        currentPairs += C2(cnt[s.pidx]);
    }
    
    if (s.qidx >= 0) {
        currentPairs -= C2(cnt[s.qidx]);
        cnt[s.qidx]--;
        currentPairs += C2(cnt[s.qidx]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    sieve();
    buildSemiprimes();
    
    int T;
    cin >> T;
    
    vector<Query> queries;
    vector<long long> ans(T, 0);
    queries.reserve(T);
    
    for (int i = 0; i < T; ++i) {
        int L, R;
        cin >> L >> R;
        
        int l = lower_bound(semival.begin(), semival.end(), L) - semival.begin();
        int r = upper_bound(semival.begin(), semival.end(), R) - semival.begin() - 1;
        
        if (l <= r) {
            queries.push_back({l, r, i});
        }
    }
    
    if (!queries.empty()) {
        sort(queries.begin(), queries.end());
        
        cnt.assign(primes.size(), 0);
        currentPairs = 0;
        
        int curL = 0, curR = -1;
        
        for (const Query& q : queries) {
            while (curR < q.r) add(++curR);
            while (curR > q.r) remove(curR--);
            while (curL < q.l) remove(curL++);
            while (curL > q.l) add(--curL);
            
            ans[q.id] = currentPairs;
        }
    }
    
    for (int i = 0; i < T; ++i) {
        cout << ans[i] << '\n';
    }
    
    return 0;
}