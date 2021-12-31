class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        /*--------method 1:sort排序---------*/
        if (is_sorted(nums.begin(), nums.end())) {
            return 0;
        }
        int length = nums.size();
        vector<pair<int, int>> vec(length);
        for (int i=0;i < length;i++) {
            vec[i] = make_pair(nums[i], i);
        }
        sort(vec.begin(), vec.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.first < b.first;
        });
        int left=0, right=0;
        for (int i=0,j=length-1;i<length || j >=0;) {
            if (i < length && vec[i].second == i)i++;
            else if (i < length && vec[i].second != i) {
                left = i;
                i = length;
            }
            if (j >=0 && vec[j].second == j)j--;
            else if (j >= 0 && vec[j].second != j) {
                right = j;
                j = -1;
            }
        }
        return right==left?0:right-left+1;
        /*---------method 2:单调栈----------*/
    }
};