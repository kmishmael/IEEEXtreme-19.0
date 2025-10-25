#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
const int MOD = 1000000007;

void generateTriangularData(int limit, vector<int> &triNums, vector<int> &signs, vector<int> &coeffs) {
    triNums.clear();
    signs.clear();
    coeffs.clear();

    for (int i = 1;; ++i) {
        long long tri = 1LL * i * (i + 1) / 2;
        if (tri > limit) break;

        triNums.push_back((int)tri);
        int sign = (i & 1) ? 1 : -1;
        signs.push_back(sign);
        coeffs.push_back(sign * (2 * i + 1));
    }
}

void generatePentagonalCoeffs(int limit, vector<int> &pentVals) {
    pentVals.assign(limit + 1, 0);

    for (int i = 1;; ++i) {
        long long p1 = 1LL * i * (3LL * i - 1) / 2;
        long long p2 = 1LL * i * (3LL * i + 1) / 2;
        if (p1 > limit && p2 > limit) break;

        int val = (i & 1) ? -1 : 1;
        if (p1 <= limit) pentVals[(int)p1] = val;
        if (p2 <= limit) pentVals[(int)p2] = val;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int maxLimit;
    if (!(cin >> maxLimit)) return 0;

    vector<int> triA, triSigns, triCoeffs;
    vector<int> triB, triSignsB, triCoeffsB;

    generateTriangularData(maxLimit, triA, triSigns, triCoeffs);
    generateTriangularData(maxLimit, triB, triSignsB, triCoeffsB); // reuse triCoeffs for main calculation

    vector<int> pentagonal;
    generatePentagonalCoeffs(maxLimit, pentagonal);

    vector<int> seqA(maxLimit + 1, 0);
    seqA[0] = 1;
    for (int n = 1; n <= maxLimit; ++n) {
        long long sum = pentagonal[n];
        for (size_t j = 0; j < triB.size() && triB[j] <= n; ++j) {
            sum += 1LL * triCoeffs[j] * seqA[n - triB[j]];
        }
        sum = (sum % MOD + MOD) % MOD;
        seqA[n] = (int)sum;
    }

    vector<int> seqU(maxLimit + 1, 0);
    for (int n = 1; n <= maxLimit; ++n) {
        long long acc = 0;
        for (size_t k = 0; k < triA.size() && triA[k] <= n; ++k) {
            acc += 1LL * triSigns[k] * seqA[n - triA[k]];
        }
        acc = (acc % MOD + MOD) % MOD;
        seqU[n] = (int)acc;
    }

    for (int i = 1; i <= maxLimit; ++i) {
        if (i > 1) cout << ' ';
        cout << seqU[i];
    }
    cout << '\n';

    return 0;
}