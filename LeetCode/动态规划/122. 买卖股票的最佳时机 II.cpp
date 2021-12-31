class Solution {
public:
    int maxProfit(vector<int>& prices) {
        /*---method 1:小dp---*/
        int maxnMoney = 0;
        int minn = 1e4 + 1;
        for (auto& it : prices) {
            if (it < minn) {
                minn = it;
            } else {
                maxnMoney += it-minn;
                minn = it;
            }
        }
        return maxnMoney;
    }
};