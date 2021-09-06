class Solution {
public:
    int wiggleMaxLength(vector<int>& nums) {
        /*---method 1:局部最优化最优---*/
        if (nums.size() <= 1) return nums.size();
        int Sequence = 1;
        int preCur = 0;
        int NowCur = 0;
        for (int i=0;i < nums.size()-1;i++) {
            NowCur = nums[i+1] - nums[i];
            if ((NowCur > 0 && preCur <= 0) || (preCur >= 0 && NowCur < 0)) {
                Sequence++;
                preCur = NowCur;
            }
        }
        return Sequence;
    }
};