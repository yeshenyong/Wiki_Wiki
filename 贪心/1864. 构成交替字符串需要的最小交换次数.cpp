class Solution {
public:
    int minSwaps(string s) {
        /*---method 1:确定所有情况+枚举---*/
        int n0 = count(s.begin(), s.end(), '0');
        int n1 = count(s.begin(), s.end(), '1');
        int result = INT_MAX;
        int length = s.size();
        //1010...
        if (n1 >= n0 && n0 + 1 >= n1) {
            int diff = 0;
            for (int i=0;i < length;i++) {
                if ((i+1)%2 != s[i]-'0') {
                    diff++;
                }
            }
            result = min(result, diff/2);
        }
        //0101
        if (n0 >= n1 && n1 + 1 >= n0) {
            int diff = 0;
            for (int i=0;i < length;i++) {
                if ((i)%2 != s[i]-'0') {
                    diff++;
                }
            }
            result = min(result, diff/2);
        }
        if (result == INT_MAX) return -1;
        return result;
    }
};