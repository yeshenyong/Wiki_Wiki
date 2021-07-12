class Solution {
public:
    string reverseLeftWords(string s, int n) {
        /*-------method 1: 从第k个开始，每次与前面第k个进行位置交换-------------*/
        /*
            时间复杂度O(n)  空间复杂度O(1)
            例如: abcdefg,  k=2
            先 ab 反转 
            再 cdefg 反转
            再整体反转
            
            反过来也是一个道理==局部反转+整体反转
         */
         /*
         int length = s.size();
         reverse(s.begin(), s.end());
         reverse(s.begin(), s.begin()+length-n);
         reverse(s.begin()+length-n, s.end());
         */
         /*
         reverse(s.begin(), s.begin()+n);
         reverse(s.begin()+n, s.end());
         reverse(s.begin(), s.end());
         */
         return s;
    }
};