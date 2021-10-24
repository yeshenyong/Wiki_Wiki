class Solution {
private:
    int minnPrice;
public:
    void dfs(vector<int>& cul, vector<int>&price, vector<vector<int>>& special, vector<int>& needs, int money) {
        bool flag = true;
        int single = 0;
        for (int i=0;i < cul.size();i++) {
            if (cul[i] > needs[i]) return;
            if (cul[i] < needs[i]) {
                flag = false;
                single += (needs[i] - cul[i]) * price[i];
            }
        }
        if (flag) {
            minnPrice = money > minnPrice ? minnPrice : money;
            return;
        } else {
            minnPrice = (money+single) > minnPrice ? minnPrice : (money+single);
        }
        for (int i=0;i < special.size();i++) {
            for (int j=0;j < special[0].size()-1;j++) {
                cul[j] += special[i][j];
            }
            dfs(cul, price, special, needs, money + special[i][special[0].size()-1]);
            for (int j=0;j < special[0].size()-1;j++) {
                cul[j] -= special[i][j];
            }
        }

    }


    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        /*---method 1:模拟---*/
        /*
            1.本身可以无限次购买
            2.可以购买自己同时购买其他大礼包
            3.购买自己同时, 其他单个购买
        */
        int n = price.size();
        vector<int> cul(n, 0);
        minnPrice = INT_MAX;
        dfs(cul, price, special, needs, 0);
        return minnPrice;
    }
};