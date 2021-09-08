class Solution {
public:
    int jump(vector<int>& nums) {
        /* method 1:动态规划 +　记录最小值 */
        /*
        vector<int> minJump(nums.size(), INT_MAX);
        int length = nums.size();
        int cover = 0;
        minJump[0] = 0;
        for (int i=0;i <= cover && i < length-1;i++) {
            if (i + nums[i] >= cover) {
                for (int j=i+1;j <= i+nums[i] && j < length;j++) {
                    minJump[j] = min(minJump[j], minJump[i] + 1);
                }
                cover = i + nums[i];
            }
        }
        return minJump[length-1];
        */
        /* method 2:贪心 */
        int CurCover = 0;
        int NextCover = 0;
        int ans = 0;
        int length = nums.size();
        for (int i=0;i < length-1;i++) {
            NextCover = max(NextCover, i +　nums[i]);
            if (i == CurCover) {
                ans++;
                CurCover = NextCover;
            }
        }
        return ans;
    }
};