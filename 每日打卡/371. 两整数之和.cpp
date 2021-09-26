class Solution {
public:
    int getSum(int a, int b) {
        /*---method 1:位运算---*/
        while(b != 0) {
            unsigned int carry = (unsigned int) (a & b) << 1;
            a = a ^ b;
            b = carry;
        }
        return a;
    }
};