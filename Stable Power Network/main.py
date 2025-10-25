import sys
from heapq import heappush, heappop

class DSU:
    def __init__(self, n=0):
        self.init(n)
    
    def init(self, n):
        self.p = list(range(n + 1))
        self.sz = [1] * (n + 1)
    
    def find(self, x):
        if self.p[x] != x:
            self.p[x] = self.find(self.p[x])
        return self.p[x]
    
    def unite(self, a, b):
        a = self.find(a)
        b = self.find(b)
        if a == b:
            return False
        if self.sz[a] < self.sz[b]:
            a, b = b, a
        self.p[b] = a
        self.sz[a] += self.sz[b]
        return True
    
    def same(self, a, b):
        return self.find(a) == self.find(b)


def main():
    input = sys.stdin.read().split()
    idx = 0
    
    T = int(input[idx])
    idx += 1
    
    for _ in range(T):
        N = int(input[idx])
        M = int(input[idx + 1])
        idx += 2
        
        edges = []
        for i in range(M):
            u = int(input[idx])
            v = int(input[idx + 1])
            w = int(input[idx + 2])
            r = int(input[idx + 3])
            idx += 4
            edges.append((u, v, w, r))
        
        # Sort by r first, then by w
        edges.sort(key=lambda e: (e[3], e[2]))
        
        dsu = DSU(N)
        Rstar = -1
        for u, v, w, r in edges:
            dsu.unite(u, v)
            if dsu.same(1, N):
                Rstar = r
                break
        
        if Rstar == -1:
            print(-1)
            continue
        
        # Build graph with edges having r <= Rstar
        g = [[] for _ in range(N + 1)]
        for u, v, w, r in edges:
            if r > Rstar:
                break
            g[u].append((v, w))
            g[v].append((u, w))
        
        # Dijkstra's algorithm
        INF = float('inf')
        dist = [INF] * (N + 1)
        dist[1] = 0
        pq = [(0, 1)]
        
        while pq:
            d, u = heappop(pq)
            if d != dist[u]:
                continue
            if u == N:
                break
            for v, w in g[u]:
                nd = d + w
                if nd < dist[v]:
                    dist[v] = nd
                    heappush(pq, (nd, v))
        
        print(Rstar, dist[N])


if __name__ == "__main__":
    main()