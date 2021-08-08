class Solution {
public:
    int tribonacci(int n) {
        /*----method 1:easyjob----*/
        int a=0, b=1, c=1;
        if (n == 0) return a;
        if (n == 1||n==2)return b;
        while((n--)>=3) {
            int tmp = a+b+c;
            a=b;
            b=c;
            c=tmp;
        }
        return c;
    }
};