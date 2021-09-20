class Solution {
public:
    int lengthOfLastWord(string s) {
        /*---method 1:暴力模拟---*/
        int length = s.size();
        int size = 0, flag = 1;
        for (int i=length-1;i >= 0;i--) {
            if (s[i] == ' ' && flag) continue;
            else flag = 0;
            if (s[i] == ' ' && !flag) return size;
            size++;
        }
        return size;
    }
};