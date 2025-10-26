#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K;
    if(!(cin >> N >> K)) return 0;
    vector<int> A(N+1);
    int maxA = 0;
    for(int i=1;i<=N;i++){ cin >> A[i]; maxA = max(maxA, A[i]); }

    vector<int> thresholds;
    for(int t=0; (1<<t) <= maxA; ++t) thresholds.push_back(1<<t);
    int maxTpow = thresholds.empty() ? 0 : (31 - __builtin_clz(maxA));
    thresholds.push_back(1 << (maxTpow+1));

    int Tcnt = (int)thresholds.size();
    vector<vector<ll>> fvals(K+1, vector<ll>(Tcnt, 0LL));

    for(int tid=0; tid<Tcnt; ++tid){
        int T = thresholds[tid];
        vector<int> pos;
        pos.reserve(N+2);
        pos.push_back(0);
        for(int i=1;i<=N;i++){
            if(A[i] >= T) pos.push_back(i);
        }
        pos.push_back(N+1);
        int m = (int)pos.size() - 2;
        if(m <= 0){
            continue;
        }

        vector<ll> L(m+1), sumL(m+1);
        for(int i=1;i<=m;i++){
            ll left = pos[i-1] + 1;
            ll right = pos[i];
            L[i] = right - (pos[i-1]);
            sumL[i] = L[i] * (left + right) / 2;
        }
        vector<ll> R(m+1), sumR(m+1);
        for(int t=1;t<=m;t++){
            ll left = pos[t];
            ll right = pos[t+1] - 1;
            R[t] = (pos[t+1] - pos[t]);
            sumR[t] = R[t] * (left + right) / 2;
        }
        vector<ll> S1(m+3, 0), S2(m+3, 0);
        for(int t=m;t>=1;--t){
            S1[t] = S1[t+1] + sumR[t];
            S2[t] = S2[t+1] + R[t];
        }

        for(int k=1;k<=K;k++){
            if(k > m){ fvals[k][tid] = 0; continue; }
            ll fk = 0;
            int upTo = m - k + 1;
            for(int i=1;i<=upTo;i++){
                int idx = i + k - 1;
                ll s1 = S1[idx];
                ll s2 = S2[idx];
                fk += L[i] * (s1 + s2) - s2 * sumL[i];
            }
            fvals[k][tid] = fk;
        }
    }

    vector<ll> ans(K+1, 0);
    int realTcount = Tcnt - 1;
    for(int k=1;k<=K;k++){
        ll Sk = 0;
        for(int t=0;t<realTcount; ++t){
            ll diff = fvals[k][t] - fvals[k][t+1];
            Sk += (ll)t * diff;
        }
        ans[k] = Sk;
    }

    for(int k=1;k<=K;k++){
        if(k>1) cout << ' ';
        cout << ans[k];
    }
    cout << '\n';
    return 0;
}
