class Solution {
public:
    int minMoves(vector<int>& nums) {
        /*---method 1:贪心,角度反转---*/
        int minn = *min_element(nums.begin(), nums.end());
        int cul = 0;
        for (auto it:nums) {
            cul += it - minn;
        }
        return cul;
    }
};