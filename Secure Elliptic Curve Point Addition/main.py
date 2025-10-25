def mod(n, p):
    return ((n % p) + p) % p

def power(base, exp, p):
    res = 1
    base = mod(base, p)
    
    while exp > 0:
        if exp % 2 == 1:
            res = mod(res * base, p)
        exp = exp >> 1
        base = mod(base * base, p)
    return res

def mod_inverse(n, p):
    return power(n, p - 2, p)

def main():
    T = int(input())
    for _ in range(T):
        a, b, p, x1, y1, x2, y2 = map(int, input().split())
        
        if x1 == x2 and mod(y1 + y2, p) == 0:
            print("POINT_AT_INFINITY")
        
        elif x1 == x2:
            num = mod(3 * x1 * x1 + a, p)
            den = mod(2 * y1, p)
            lambda_val = mod(num * mod_inverse(den, p), p)
            
            x3 = mod(lambda_val * lambda_val - 2 * x1, p)
            y3 = mod(lambda_val * mod(x1 - x3, p) - y1, p)
            
            print(x3, y3)
        
        else:
            num = mod(y2 - y1, p)
            den = mod(x2 - x1, p)
            lambda_val = mod(num * mod_inverse(den, p), p)
            
            x3 = mod(lambda_val * lambda_val - x1 - x2, p)
            y3 = mod(lambda_val * mod(x1 - x3, p) - y1, p)
            
            print(x3, y3)

if __name__ == "__main__":
    main()