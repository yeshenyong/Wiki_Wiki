class Solution {
public:
    int maxProfit(vector<int>& prices) {
        /*---method 1:小dp---*/
        int maxMoney = 0;
        int minn = 1e5+1;
        for (auto &it : prices) {
            if (it >= minn) {
                maxMoney = maxMoney<it-minn?it-minn:maxMoney;
            }
            minn = it<minn?it:minn;
        }
        return　maxMoney;
    }
};