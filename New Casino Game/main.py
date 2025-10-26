MOD = 998244353

def solve():
    t = int(input())
    for _ in range(t):
        n = int(input())
        if n % 2 == 0:
            print(0)
        else:
         
            inv_n = pow(n, MOD - 2, MOD)
            print(inv_n % MOD)

solve()