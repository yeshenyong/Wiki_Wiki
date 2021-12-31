class Solution {
public:
    bool isCovered(vector<vector<int>>& ranges, int left, int right) {
        /*-------method 1:打卡---------*/
        vector<int> w(52);
        for (auto x : ranges){
            w[x[0]]++,w[x[1] + 1]--;
        }
        vector<int> v(52);
        for (int i = 1; i < 52; ++i){
            v[i] = v[i - 1] + w[i];
        }
        for (int i = left; i <= right; ++i){
            if (!v[i]) return false;
        }
        return true;
    }   
};