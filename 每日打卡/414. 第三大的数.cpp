class Solution {
public:
    int thirdMax(vector<int>& nums) {
        /*---method 1:单调队列/优先队列/去重---*/
        /*
        priority_queue<int, vector<int>, greater<int>> myqueue;
        unordered_set<int> myset;
        for (auto & it : nums) {
            if (!myset.count(it)) {
                myqueue.emplace(it);
                myset.insert(it);
            }
            if (myqueue.size() > 3) myqueue.pop();
        }
        if (myqueue.size() < 3) {
            int ret = myqueue.top();
            while(!myqueue.empty()) {
                ret = myqueue.top();
                myqueue.pop();
            }
            return ret;
        }
        return myqueue.top();
        */
        /*---method 2:有序集合---*/
        set<int> myset;
        for (auto & it : nums) {
            myset.insert(it);
            if (myset.size() > 3) {
                myset.erase(myset.begin());
            }
        }
        return myset.size() == 3?*myset.begin():*myset.rbegin();
    }
};