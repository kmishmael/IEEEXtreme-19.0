#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
const int64 MOD = 1000000007LL;

int64 addmod(int64 a, int64 b){ a += b; if(a >= MOD) a -= MOD; return a; }
int64 submod(int64 a, int64 b){ a -= b; if(a < 0) a += MOD; return a; }
int64 mulmod(int64 a, int64 b){ return ( (__int128)a * b ) % MOD; }

int64 count_xor_zero_u(const array<long long,4>& U){
    for(int i=0;i<4;i++) if(U[i] < 0) return 0;
    const int TOP = 60;
    vector<int64> dp(16, 0), ndp(16, 0);
    dp[(1<<4)-1] = 1; // all tight initially
    for(int bit = TOP; bit >= 0; --bit){
        fill(ndp.begin(), ndp.end(), 0);
        // precompute ubits
        int ub[4];
        for(int i=0;i<4;i++) ub[i] = ( (U[i] >> bit) & 1LL );
        // iterate current masks
        for(int mask=0; mask<16; ++mask){
            if(dp[mask] == 0) continue;
            // iterate assignments of 4 bits (0..15) whose popcount is even
            for(int x=0; x<16; ++x){
                if(__builtin_popcount(x) % 2) continue; // need even number of ones
                bool ok = true;
                int newmask = 0;
                for(int i=0;i<4;i++){
                    int bit_i = (x >> i) & 1;
                    if( (mask >> i) & 1 ){ // tight for Ui
                        if(bit_i > ub[i]) { ok = false; break; }
                        if(bit_i == ub[i]) newmask |= (1<<i);
                        // else (bit_i < ub[i]) newmask bit becomes 0 => already 0
                    } else {
                        // already loose, stays loose (0)
                    }
                }
                if(!ok) continue;
                ndp[newmask] = addmod(ndp[newmask], dp[mask]);
            }
        }
        dp.swap(ndp);
    }
    int64 res = 0;
    for(int mask=0; mask<16; ++mask) res = addmod(res, dp[mask]);
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int Q;
    if(!(cin >> Q)) return 0;
    while(Q--){
        long long A1,B1,A2,B2,A3,B3,A4,B4;
        cin >> A1 >> B1 >> A2 >> B2 >> A3 >> B3 >> A4 >> B4;
        // total tuples:
        int64 total = 1;
        total = mulmod(total, (B1 - A1 + 1) % MOD);
        total = mulmod(total, (B2 - A2 + 1) % MOD);
        total = mulmod(total, (B3 - A3 + 1) % MOD);
        total = mulmod(total, (B4 - A4 + 1) % MOD);

        // inclusion-exclusion to compute number of tuples with xor == 0 inside [Ai, Bi]
        int64 zeros = 0;
        // for mask subset where bit i = 1 means we choose Ai-1 as upper bound for that variable
        for(int mask = 0; mask < 16; ++mask){
            array<long long,4> U;
            U[0] = ( (mask & 1) ? (A1 - 1) : B1 );
            U[1] = ( (mask & 2) ? (A2 - 1) : B2 );
            U[2] = ( (mask & 4) ? (A3 - 1) : B3 );
            U[3] = ( (mask & 8) ? (A4 - 1) : B4 );
            int bits = __builtin_popcount(mask);
            int64 val = count_xor_zero_u(U);
            if(bits % 2) zeros = submod(zeros, val);
            else zeros = addmod(zeros, val);
        }

        int64 ans = submod(total, zeros);
        cout << ans << '\n';
    }
    return 0;
}