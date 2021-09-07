class Solution {
public:
    int balancedStringSplit(string s) {
        /*---method 1:贪心---*/
        int lcount = 0, rcount = 0;
        int balancedStr = 0;
        for (auto &it : s) {
            if (it == 'L') lcount++;
            else rcount++;
            if (lcount == rcount) {
                balancedStr++;
                lcount=0;
                rcount=0;
            }
        }
        return balancedStr;
    }
};