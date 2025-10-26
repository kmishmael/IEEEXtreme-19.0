#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
const i64 NINF = (i64)-4e18;

struct Hull {
    vector<i64> M, B;
    int head = 0;
    static bool bad(i64 m1, i64 b1, i64 m2, i64 b2, i64 m3, i64 b3) {
        __int128 lhs = (__int128)(b3 - b1) * (m1 - m2);
        __int128 rhs = (__int128)(b2 - b1) * (m1 - m3);
        return lhs <= rhs;
    }
    void add(i64 m, i64 b) {
        while (M.size() >= 2 && bad(M[M.size()-2], B[B.size()-2], M.back(), B.back(), m, b)) {
            M.pop_back(); B.pop_back();
            if (head > (int)M.size()-1) head = (int)M.size()-1;
        }
        M.push_back(m); B.push_back(b);
        if (head >= (int)M.size()) head = (int)M.size()-1;
    }
    inline i64 f(int idx, i64 x) const { return M[idx]*x + B[idx]; }
    i64 query(i64 x) {
        if (M.empty()) return NINF;
        while ((int)M.size() - head >= 2 && f(head, x) <= f(head+1, x)) ++head;
        return f(head, x);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string a, b;
    if (!(cin >> n)) return 0;
    cin >> a;
    cin >> m;
    cin >> b;
    i64 c_match, c_mismatch, c_gap;
    cin >> c_match >> c_mismatch >> c_gap;

    auto X_for = [&](i64 t) -> i64 { return (-2LL * c_gap) * t; };

    vector<i64> M_prev(m+1, NINF), M_cur(m+1, NINF);
    vector<i64> GA_prev(m+1, NINF), GA_cur(m+1, NINF);
    vector<i64> GB_prev(m+1, NINF), GB_cur(m+1, NINF);
    vector<Hull> colHull(m+1);

    M_prev[0] = 0;
    for (int j = 1; j <= m; ++j) {
        M_prev[j] = NINF;
        GB_prev[j] = NINF;
        GA_prev[j] = c_gap * 1LL * j * j;
    }
    for (int j = 0; j <= m; ++j) {
        i64 base2 = max(M_prev[j], GA_prev[j]);
        colHull[j].add(0, base2);
    }

    for (int i = 1; i <= n; ++i) {
        i64 add_i = c_gap * 1LL * i * i;
        i64 x_i = X_for(i);
        for (int j = 0; j <= m; ++j) {
            GB_cur[j] = colHull[j].query(x_i);
            if (GB_cur[j] != NINF) GB_cur[j] += add_i;
        }
        GB_cur[0] = c_gap * 1LL * i * i;

        M_cur[0] = NINF;
        for (int j = 1; j <= m; ++j) {
            i64 sc = (a[i-1] == b[j-1]) ? c_match : c_mismatch;
            i64 prev_best = max(M_prev[j-1], max(GA_prev[j-1], GB_prev[j-1]));
            M_cur[j] = (prev_best == NINF) ? NINF : (prev_best + sc);
        }

        Hull rowHull;
        {
            i64 base = max(M_cur[0], GB_cur[0]);
            rowHull.add(0, base);
        }
        GA_cur[0] = NINF;
        for (int j = 1; j <= m; ++j) {
            i64 val = rowHull.query(X_for(j));
            GA_cur[j] = (val == NINF) ? NINF : (val + c_gap * 1LL * j * j);
            i64 base_t = max(M_cur[j], GB_cur[j]);
            rowHull.add(j, base_t + c_gap * 1LL * j * j);
        }

        for (int j = 0; j <= m; ++j) {
            i64 base2 = max(M_cur[j], GA_cur[j]);
            colHull[j].add(i, base2 + c_gap * 1LL * i * i);
        }

        swap(M_prev, M_cur);
        swap(GA_prev, GA_cur);
        swap(GB_prev, GB_cur);
        fill(M_cur.begin(), M_cur.end(), NINF);
        fill(GA_cur.begin(), GA_cur.end(), NINF);
        fill(GB_cur.begin(), GB_cur.end(), NINF);
    }

    i64 ans = max(M_prev[m], max(GA_prev[m], GB_prev[m]));
    cout << ans << '\n';
    return 0;
}
