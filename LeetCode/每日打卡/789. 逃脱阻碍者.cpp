class Solution {
public:

    int manhatunDis(const vector<int>& dis1, const vector<int>& dis2) {
        return abs(dis1[0] - dis2[0]) + abs(dis1[1] - dis2[1]);
    }

    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        /*---method 1: math 曼哈顿距离---*/
        int minnDis = manhatunDis(vector<int>({0, 0}), target);
        for (auto &it : ghosts) {
            if (manhatunDis(it, target) <= minnDis)return false;
        }
        return true;
    }
};