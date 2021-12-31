class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        /*---method 1:单调栈 + 哈希表---*/
        stack<int> st;
        unordered_map<int, int> mp;
        int n = nums1.size(), m = nums2.size();
        for (auto it : nums2) {
            while (!st.empty() && it > st.top()) {
                mp[st.top()] = it;
                st.pop();
            }
            st.emplace(it);
        }
        vector<int> res(n, -1);
        for (int i=0;i < n;i++) {
            if (mp.count(nums1[i])) res[i] = mp[nums1[i]];
        }
        return res;
    }
};