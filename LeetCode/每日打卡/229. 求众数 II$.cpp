class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        vector<int> majority;
        /*---method 1:hashmap计数*/
        /*
            O(n)
            O(n)
        */
        /*
        int size = nums.size()/3;
        unordered_map<int, int> mmap;
        unordered_set<int> myset;
        for (auto it:nums) {
            mmap[it]++;
            if (mmap[it] > size && myset.count(it) == 0) {
                majority.emplace_back(it);
                myset.emplace(it);
            }
        }
        return majority;
        */
        /*---method 2:排序*/
        /*
            O(nlogn)
            O(1)
        */
        /*
        sort(nums.begin(), nums.end());
        int Count = 1, pre = nums[0];
        for (int i=1;i < nums.size();i++) {
            if (nums[i] == pre) Count++;
            else {
                if (Count > nums.size()/3) majority.emplace_back(pre);
                Count = 1;
                pre = nums[i];
            }
        }
        if (Count > nums.size()/3) majority.emplace_back(pre);
        return majority;
        */
        /*---method 3:摩尔投票法*/
        /*
            O(n)
            O(1)
        */
        int size = nums.size()/3;
        int element1, element2;
        int vote1 = 0, vote2 = 0;
        for (auto it:nums) {
            if (vote1 != 0 && element1 == it) {
                vote1++;
            } else if (vote2 != 0 && element2 == it) {
                vote2++;
            } else if (vote1 == 0) {
                element1 = it;
                vote1++;
            } else if (vote2 == 0) {
                element2 = it;
                vote2++;
            } else {
                vote1--;
                vote2--;
            }
        }
        int cnt1 = 0;
        int cnt2 = 0;
        for (auto it:nums) {
            if (it == element1) cnt1++;
            else if (it == element2) cnt2++;
        }
        if (cnt1 > size) majority.emplace_back(element1);
        if (cnt2 > size) majority.emplace_back(element2);
        return majority;
    }
};