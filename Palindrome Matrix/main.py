class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
    
    def find(self, i):
        if self.parent[i] == i:
            return i
        self.parent[i] = self.find(self.parent[i])
        return self.parent[i]
    
    def unite(self, i, j):
        root_i = self.find(i)
        root_j = self.find(j)
        if root_i != root_j:
            self.parent[root_i] = root_j


def main():
    N, M = map(int, input().split())
    
    grid = []
    for _ in range(N):
        grid.append(input().strip())
    
    dsu = DSU(N * M)
    
    def to_1d(r, c):
        return r * M + c
    
    for i in range(N):
        j = 0
        while j < M:
            if grid[i][j] == '.':
                j += 1
                continue
            
            j_end = j
            while j_end + 1 < M and grid[i][j_end + 1] != '.':
                j_end += 1
            
            left = j
            right = j_end
            while left < right:
                dsu.unite(to_1d(i, left), to_1d(i, right))
                left += 1
                right -= 1
            
            j = j_end + 1
    
    for j in range(M):
        i = 0
        while i < N:
            if grid[i][j] == '.':
                i += 1
                continue
            
            i_end = i
            while i_end + 1 < N and grid[i_end + 1][j] != '.':
                i_end += 1
            
            top = i
            bottom = i_end
            while top < bottom:
                dsu.unite(to_1d(top, j), to_1d(bottom, j))
                top += 1
                bottom -= 1
            
            i = i_end + 1
    
    component_counts = {}
    for i in range(N):
        for j in range(M):
            if grid[i][j] != '.':
                root = dsu.find(to_1d(i, j))
                if root not in component_counts:
                    component_counts[root] = [0] * 10
                component_counts[root][int(grid[i][j])] += 1
    
    best_digit_for_root = {}
    for root, counts in component_counts.items():
        min_primary_cost = float('inf')
        min_secondary_cost = float('inf')
        best_d = 0
        
        component_size = sum(counts)
        
        for d in range(10):
            current_primary_cost = 0
            current_secondary_cost = d * component_size
            
            for orig_d in range(10):
                current_primary_cost += abs(d - orig_d) * counts[orig_d]
            
            if current_primary_cost < min_primary_cost:
                min_primary_cost = current_primary_cost
                min_secondary_cost = current_secondary_cost
                best_d = d
            elif current_primary_cost == min_primary_cost and current_secondary_cost < min_secondary_cost:
                min_secondary_cost = current_secondary_cost
                best_d = d
        
        best_digit_for_root[root] = best_d
    
    for i in range(N):
        row = []
        for j in range(M):
            if grid[i][j] == '.':
                row.append('.')
            else:
                row.append(str(best_digit_for_root[dsu.find(to_1d(i, j))]))
        print(''.join(row))


if __name__ == "__main__":
    main()