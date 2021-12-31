class Solution {
public:
    vector<vector<int>> Myvec;
    vector<int> vec;
    void dfs(const int k, const int n, int cur, int level, int sum) {
        if (level == k-1) {
            if (n-sum>=cur && n-sum<10) {
                vec.push_back(n-sum);
                Myvec.push_back(vec);
                vec.pop_back();
            }
            return;
        }
        for (int i=cur;i <= 9-(k-vec.size())+1;i++) {
            if (sum + i > n)return;
            vec.push_back(i);
            dfs(k, n, i+1, level+1, sum+i);
            vec.pop_back();
        }
    }
    vector<vector<int>> combinationSum3(int k, int n) {
        /*---------method 1:dfs+剪枝-----------*/
        /*
            三种剪
            1.中途超了剪
            2.k-1层没必要进行下去
            3.循环剪枝
            极限双百！
        */
        dfs(k, n, 1, 0, 0);
        return Myvec;
    }
};