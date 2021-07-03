class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        
        /*
            map<key,value>
            key:代表数组中的value值
            value:代表数组中的index值,因为map默认空值为0,和下标为0冲突
            解决方法：下标全部+1
        */
        unordered_map<int32_t, int32_t> mmap;
        int length = nums.size();
        for(int i = 0;i < length;i++){
            if (mmap[target-nums[i]] != 0){
                return vector<int>({mmap[target-nums[i]]-1, i});
            }
            mmap[nums[i]] = i+1;
        }
        return vector<int>();
    }
};