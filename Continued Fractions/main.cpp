#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int GENERATOR = 3;

int addMod(int x, int y) {
    x += y;
    if(x >= MOD) x -= MOD;
    return x;
}

int subMod(int x, int y) {
    x -= y;
    if(x < 0) x += MOD;
    return x;
}

long long powMod(long long base, long long exp) {
    long long result = 1 % MOD;
    while(exp) {
        if(exp & 1) result = result * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return result;
}

int modInverse(int value) {
    return (int)powMod(value, MOD - 2);
}

void ntt(vector<int>& arr, bool inverse) {
    int sz = arr.size();
    static vector<int> bitRev;
    
    if ((int)bitRev.size() != sz) {
        int logSize = __builtin_ctz(sz);
        bitRev.assign(sz, 0);
        for(int idx = 0; idx < sz; idx++)
            bitRev[idx] = (bitRev[idx >> 1] >> 1) | ((idx & 1) << (logSize - 1));
    }
    
    for(int idx = 0; idx < sz; idx++)
        if(idx < bitRev[idx]) swap(arr[idx], arr[bitRev[idx]]);
    
    for(int blockSize = 1; blockSize < sz; blockSize <<= 1) {
        long long rootOfUnity = powMod(GENERATOR, (MOD - 1) / (blockSize << 1));
        if(inverse) rootOfUnity = modInverse(rootOfUnity);
        
        for(int start = 0; start < sz; start += blockSize << 1) {
            long long omega = 1;
            for(int offset = 0; offset < blockSize; offset++) {
                int even = arr[start + offset];
                int odd = (int)((long long)arr[start + offset + blockSize] * omega % MOD);
                arr[start + offset] = even + odd < MOD ? even + odd : even + odd - MOD;
                arr[start + offset + blockSize] = even - odd >= 0 ? even - odd : even - odd + MOD;
                omega = omega * rootOfUnity % MOD;
            }
        }
    }
    
    if(inverse) {
        int invSize = modInverse(sz);
        for(int &element: arr) element = (long long)element * invSize % MOD;
    }
}

vector<int> multiply(const vector<int>& poly1, const vector<int>& poly2) {
    if(poly1.empty() || poly2.empty()) return {};
    
    int sz = 1;
    while(sz < (int)poly1.size() + (int)poly2.size() - 1) sz <<= 1;
    
    vector<int> transformed1(poly1.begin(), poly1.end());
    vector<int> transformed2(poly2.begin(), poly2.end());
    transformed1.resize(sz);
    transformed2.resize(sz);
    
    ntt(transformed1, false);
    ntt(transformed2, false);
    
    for(int idx = 0; idx < sz; idx++)
        transformed1[idx] = (long long)transformed1[idx] * transformed2[idx] % MOD;
    
    ntt(transformed1, true);
    transformed1.resize(poly1.size() + poly2.size() - 1);
    return transformed1;
}

vector<int> polyAdd(const vector<int>& poly1, const vector<int>& poly2) {
    vector<int> result(max(poly1.size(), poly2.size()));
    for(size_t idx = 0; idx < result.size(); ++idx) {
        int coeff1 = idx < poly1.size() ? poly1[idx] : 0;
        int coeff2 = idx < poly2.size() ? poly2[idx] : 0;
        result[idx] = addMod(coeff1, coeff2);
    }
    return result;
}

vector<int> polySub(const vector<int>& poly1, const vector<int>& poly2) {
    vector<int> result(max(poly1.size(), poly2.size()));
    for(size_t idx = 0; idx < result.size(); ++idx) {
        int coeff1 = idx < poly1.size() ? poly1[idx] : 0;
        int coeff2 = idx < poly2.size() ? poly2[idx] : 0;
        result[idx] = subMod(coeff1, coeff2);
    }
    return result;
}

vector<int> polyTrim(vector<int> poly) {
    while(!poly.empty() && poly.back() == 0) poly.pop_back();
    return poly;
}

vector<int> polyScalarMul(const vector<int>& poly, int scalar) {
    vector<int> result(poly.size());
    for(size_t idx = 0; idx < poly.size(); ++idx)
        result[idx] = (long long)poly[idx] * scalar % MOD;
    return result;
}

vector<int> polyInv(const vector<int>& poly, int degree) {
    vector<int> invPoly(1, modInverse(poly[0]));
    int currentDeg = 1;
    
    while(currentDeg < degree) {
        currentDeg <<= 1;
        vector<int> polyCut(min((int)poly.size(), currentDeg));
        for(int idx = 0; idx < (int)polyCut.size(); ++idx) polyCut[idx] = poly[idx];
        
        vector<int> temp = multiply(multiply(invPoly, invPoly), polyCut);
        invPoly.resize(currentDeg);
        
        for(int idx = 0; idx < currentDeg; idx++) {
            int coefficient = idx < (int)invPoly.size() ? invPoly[idx] : 0;
            int subtract = idx < (int)temp.size() ? temp[idx] : 0;
            invPoly[idx] = subMod(addMod(coefficient, coefficient), subtract);
        }
    }
    
    invPoly.resize(degree);
    return invPoly;
}

vector<int> polyDivMod(const vector<int>& dividend, const vector<int>& divisor) {
    vector<int> trimmedDividend = polyTrim(dividend);
    vector<int> trimmedDivisor = polyTrim(divisor);
    
    int degDividend = trimmedDividend.size();
    int degDivisor = trimmedDivisor.size();
    
    if(degDividend < degDivisor) return trimmedDividend;
    
    vector<int> revDividend = trimmedDividend;
    reverse(revDividend.begin(), revDividend.end());
    
    vector<int> revDivisor = trimmedDivisor;
    reverse(revDivisor.begin(), revDivisor.end());
    
    int quotientDeg = degDividend - degDivisor + 1;
    vector<int> invRevDivisor = polyInv(revDivisor, quotientDeg);
    
    vector<int> revDividendCut(revDividend.begin(), revDividend.begin() + quotientDeg);
    vector<int> revQuotient = multiply(revDividendCut, invRevDivisor);
    revQuotient.resize(quotientDeg);
    
    vector<int> quotient = revQuotient;
    reverse(quotient.begin(), quotient.end());
    
    vector<int> product = multiply(trimmedDivisor, quotient);
    vector<int> remainder = polySub(trimmedDividend, product);
    remainder.resize(degDivisor - 1);
    
    return polyTrim(remainder);
}

struct MultiEval {
    int numPoints;
    vector<vector<int>> segmentTree;
    
    MultiEval(const vector<int>& evaluationPoints) {
        numPoints = evaluationPoints.size();
        segmentTree.resize(4 * max(1, numPoints));
        if(numPoints > 0) constructTree(1, 0, numPoints - 1, evaluationPoints);
    }
    
    void constructTree(int node, int leftBound, int rightBound, const vector<int>& points) {
        if(leftBound == rightBound) {
            segmentTree[node] = { (MOD - points[leftBound]) % MOD, 1 };
        } else {
            int midPoint = (leftBound + rightBound) / 2;
            constructTree(node << 1, leftBound, midPoint, points);
            constructTree(node << 1 | 1, midPoint + 1, rightBound, points);
            segmentTree[node] = multiply(segmentTree[node << 1], segmentTree[node << 1 | 1]);
        }
    }
    
    void evaluateRecursive(const vector<int>& polynomial, int node, int leftBound, int rightBound, vector<int>& output) {
        vector<int> remainder = polyDivMod(polynomial, segmentTree[node]);
        
        if(leftBound == rightBound) {
            output[leftBound] = remainder.empty() ? 0 : remainder[0];
            return;
        }
        
        int midPoint = (leftBound + rightBound) / 2;
        evaluateRecursive(remainder, node << 1, leftBound, midPoint, output);
        evaluateRecursive(remainder, node << 1 | 1, midPoint + 1, rightBound, output);
    }
    
    vector<int> evaluate(const vector<int>& polynomial) {
        vector<int> output(numPoints);
        if(numPoints == 0) return {};
        evaluateRecursive(polynomial, 1, 0, numPoints - 1, output);
        return output;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int numRoots, numOperations;
    if(!(cin >> numRoots >> numOperations)) return 0;
    
    vector<int> roots(numRoots);
    for(int idx = 0; idx < numRoots; idx++) {
        long long value;
        cin >> value;
        roots[idx] = value % MOD;
    }
    
    vector<pair<int, long long>> operations(numOperations);
    for(int idx = 0; idx < numOperations; idx++) {
        int opType;
        cin >> opType;
        if(opType == 1) {
            long long value;
            cin >> value;
            operations[idx] = {1, value % MOD};
        } else {
            operations[idx] = {2, 0};
        }
    }
    
    long long rootSum = 0;
    for(int value : roots) rootSum = (rootSum + value) % MOD;
    
    function<vector<int>(int, int)> constructPolynomial = [&](int left, int right) -> vector<int> {
        if(left == right) return vector<int>{roots[left] % MOD, 1};
        int mid = (left + right) / 2;
        auto leftPoly = constructPolynomial(left, mid);
        auto rightPoly = constructPolynomial(mid + 1, right);
        return multiply(leftPoly, rightPoly);
    };
    
    vector<int> polynomial = constructPolynomial(0, numRoots - 1);
    
    vector<int> derivative(polynomial.size() ? polynomial.size() - 1 : 0);
    for(size_t idx = 1; idx < polynomial.size(); ++idx)
        derivative[idx - 1] = (long long)polynomial[idx] * idx % MOD;
    
    vector<int> matrix_a(numOperations), matrix_b(numOperations);
    vector<int> matrix_c(numOperations), matrix_d(numOperations);
    
    int alpha = 1, beta = 0, gamma = 0, delta = 1;
    vector<int> evalPointIndex(numOperations, -1);
    vector<int> evalPoints;
    evalPoints.reserve(numOperations);
    
    for(int idx = 0; idx < numOperations; idx++) {
        if(operations[idx].first == 1) {
            int operand = operations[idx].second;
            alpha = (alpha + (long long)operand * gamma) % MOD;
            beta = (beta + (long long)operand * delta) % MOD;
        } else {
            swap(alpha, gamma);
            swap(beta, delta);
        }
        
        matrix_a[idx] = alpha;
        matrix_b[idx] = beta;
        matrix_c[idx] = gamma;
        matrix_d[idx] = delta;
        
        if(gamma != 0) {
            int evalPoint = (long long)delta * modInverse(gamma) % MOD;
            evalPointIndex[idx] = evalPoints.size();
            evalPoints.push_back(evalPoint);
        }
    }
    
    vector<int> polyValues, derivValues;
    if(!evalPoints.empty()) {
        MultiEval evaluator(evalPoints);
        polyValues = evaluator.evaluate(polynomial);
        derivValues = evaluator.evaluate(derivative);
    }
    
    alpha = 1;
    beta = 0;
    gamma = 0;
    delta = 1;
    
    for(int idx = 0; idx < numOperations; idx++) {
        if(operations[idx].first == 1) {
            int operand = operations[idx].second;
            alpha = (alpha + (long long)operand * gamma) % MOD;
            beta = (beta + (long long)operand * delta) % MOD;
        } else {
            swap(alpha, gamma);
            swap(beta, delta);
        }
        
        if(gamma == 0) {
            int invDelta = modInverse(delta);
            long long term1 = (long long)alpha * invDelta % MOD * rootSum % MOD;
            long long term2 = (long long)numRoots * (long long)beta % MOD * invDelta % MOD;
            long long answer = (term1 + term2) % MOD;
            cout << answer << '\n';
        } else {
            int pointIdx = evalPointIndex[idx];
            int polyVal = polyValues[pointIdx];
            int derivVal = derivValues[pointIdx];
            int sumAtPoint = (long long)derivVal * modInverse(polyVal) % MOD;
            
            int invGamma = modInverse(gamma);
            long long term1 = (long long)numRoots * alpha % MOD * invGamma % MOD;
            long long determinant = ((long long)alpha * delta - (long long)beta * gamma) % MOD;
            if(determinant < 0) determinant += MOD;
            
            long long invGammaSq = (long long)invGamma * invGamma % MOD;
            long long term2 = determinant * invGammaSq % MOD * sumAtPoint % MOD;
            long long answer = (term1 - term2) % MOD;
            if(answer < 0) answer += MOD;
            
            cout << answer << '\n';
        }
    }
    
    return 0;
}