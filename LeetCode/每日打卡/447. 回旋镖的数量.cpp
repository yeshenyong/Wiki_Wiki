class Solution {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        /*---method 1:数学 + 模拟---*/
        int Boomerangs = 0;
        int n = points.size();
        for (int i=0;i < n;i++) {
            unordered_map<int, int> myset;
            int x = points[i][0], y = points[i][1];
            for (int j=0;j < n;j++) {
                int tx = points[j][0], ty = points[j][1];
                if (myset.find((tx-x)*(tx-x) +　(ty-y)*(ty-y)) == myset.end()) {
                    myset[(tx-x)*(tx-x) +　(ty-y)*(ty-y)]++;
                } else {
                    myset[(tx-x)*(tx-x) +　(ty-y)*(ty-y)]++;
                    int count = myset[(tx-x)*(tx-x) +　(ty-y)*(ty-y)];
                    int a = count, b = count-1;
                    Boomerangs -= b*(b-1);
                    Boomerangs += a*(a-1);
                }
            }
        }
        return Boomerangs;
    }
};