class Solution {
public
    int fib(int n) {
        ---method 1动态规划---
        int mod = 1e9 + 7;
        if (n == 1  n == 0) return n;
        int a = 0, b = 1;
        n-=1;
        while(n--) {
            int tmp = (a + b)%mod;
            a = b;
            b = tmp;
        }
        return b;
    }
};