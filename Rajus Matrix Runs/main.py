import sys
sys.setrecursionlimit(10**7)

def solve():
    rows, cols = map(int, input().split())
    grid = [list(map(int, input().split())) for _ in range(rows)]
    
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    
    memo = [[-1] * cols for _ in range(rows)]
    
    def dfs(row, col):
        if memo[row][col] != -1:
            return memo[row][col]
        
        longest = 1
        for delta_row, delta_col in directions:
            next_row, next_col = row + delta_row, col + delta_col
            if 0 <= next_row < rows and 0 <= next_col < cols and grid[next_row][next_col] > grid[row][col]:
                longest = max(longest, 1 + dfs(next_row, next_col))
        
        memo[row][col] = longest
        return longest
    
    result = 0
    for row in range(rows):
        for col in range(cols):
            result = max(result, dfs(row, col))
    
    print(result)

if __name__ == "__main__":
    solve()