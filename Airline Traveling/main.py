import sys
from heapq import heappush, heappop
from collections import defaultdict

def main():
    input = sys.stdin.readline
    
    INF = float('inf')
    
    line = input().split()
    if not line:
        return
    
    N, K = int(line[0]), int(line[1])
    
    C = [0] * N
    coins = []
    seen = [False] * 2005
    
    line = input().split()
    for i in range(1, N):
        x = int(line[i-1])
        C[i] = x
        d = 2 * x
        if not seen[d]:
            seen[d] = True
            coins.append(d)
    
    Q = int(input())
    
    if not coins:
        for _ in range(Q):
            A, B = map(int, input().split())
            if A == B:
                d = 0
            elif A == 0:
                d = C[B]
            elif B == 0:
                d = C[A]
            else:
                d = C[A] + C[B]
            
            T = K - d
            print("Yes" if T == 0 else "No")
        return
    
    m = min(coins)
    dist = [INF] * m
    pq = []
    
    dist[0] = 0
    heappush(pq, (0, 0))
    
    while pq:
        d, u = heappop(pq)
        if d != dist[u]:
            continue
        
        for c in coins:
            v = (u + c) % m
            nd = d + c
            if nd < dist[v]:
                dist[v] = nd
                heappush(pq, (nd, v))
    
    for _ in range(Q):
        A, B = map(int, input().split())
        
        if A == B:
            d = 0
        elif A == 0:
            d = C[B]
        elif B == 0:
            d = C[A]
        else:
            d = C[A] + C[B]
        
        T = K - d
        if T < 0:
            print("No")
            continue
        
        r = T % m
        print("Yes" if dist[r] <= T else "No")

if __name__ == "__main__":
    main()