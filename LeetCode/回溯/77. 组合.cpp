class Solution {
public:
    vector<vector<int>> MyVec;

    void dfs(const int n, const int k,int cur, int level, vector<int>& vec) {
        if (level == k) {
            MyVec.push_back(vec);
            return;
        }
        for (int i=cur;i <= n-(k-vec.size())+1;i++) {
            vec.push_back(i);
            dfs(n, k, i+1, level+1, vec);
            vec.pop_back();
        }
    }

    vector<vector<int>> combine(int n, int k) {
        /*---------method 1:纯dfs+剪枝(回溯部分到后面是没有意义的)---------*/
        vector<int> vec;
        dfs(n, k, 1, 0, vec);
        return MyVec;
    }
};