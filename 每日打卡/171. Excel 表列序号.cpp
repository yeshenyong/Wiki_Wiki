class Solution {
public:
    int titleToNumber(string columnTitle) {
        /*--------method 1:每日打卡+简单题----------*/
        int32_t sum = 0;
        for (auto& it:columnTitle) {
            sum = (sum * 26 + (it-'A'+1));
        }  
        return sum;
    }
};