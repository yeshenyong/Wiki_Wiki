class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> myset;
        unordered_set<int> resultset(nums2.begin(), nums2.end());
        for(int &it:nums1){
            if　(resultset.find(it) != resultset.end()){
                myset.insert(it);
            }
        }
        return vector<int>(myset.begin(), myset.end());
    }
};