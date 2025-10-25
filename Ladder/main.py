import sys

LIMIT = 100

binomial = [[0] * (LIMIT + 1) for _ in range(LIMIT + 1)]
for row in range(LIMIT + 1):
    binomial[row][0] = 1
    for col in range(1, row + 1):
        if col < row:
            binomial[row][col] = binomial[row - 1][col - 1] + binomial[row - 1][col]
        else:
            binomial[row][col] = 1

def round_half_even(numerator: int, denominator: int) -> int:
    quotient, remainder = divmod(numerator, denominator)
    double_remainder = remainder << 1
    if double_remainder < denominator:
        return quotient
    if double_remainder > denominator:
        return quotient + 1
    return quotient if (quotient & 1) == 0 else quotient + 1

def solve_case(param_s: int, param_n: int) -> int:
    lower_bound = (param_n + 1) // 2
    moment2 = 0
    moment4 = 0
    
    for index in range(lower_bound, param_n + 1):
        coefficient = binomial[index][param_n - index]
        moment2 += coefficient << (param_n - index)
        moment4 += coefficient << (2 * param_n - 2 * index)
    
    return round_half_even(19 * param_s * moment4, moment2 * moment2)

def main():
    input_data = list(map(int, sys.stdin.read().strip().split()))
    if not input_data:
        return
    
    iterator = iter(input_data)
    num_cases = next(iterator)
    results = []
    
    for _ in range(num_cases):
        first_param = next(iterator)
        second_param = next(iterator)
        results.append(str(solve_case(first_param, second_param)))
    
    print("\n".join(results))

if __name__ == "__main__":
    main()