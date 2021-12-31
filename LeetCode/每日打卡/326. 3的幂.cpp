class Solution {
public:
    bool isPowerOfThree(int n) {
        /*---method 1:暴力模拟---*/
        while (n > 1) {
            if (n % 3 != 0) return false;
            n /= 3;
        }
        if (n == 1) return true;
        return false;
    }
};