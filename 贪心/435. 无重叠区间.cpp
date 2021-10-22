class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        /*---method 1:排序 + 贪心---*/
        sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int> &b)->bool {
            return a[0]<b[0]?true:a[0]>b[0]?false:a[1]<b[1]?true:false;
        });
        int minDown = intervals[0][0], maxUp = intervals[0][1];
        int removeNum = 0;
        for (int i=1;i < intervals.size();i++) {
            if (intervals[i][1] <= maxUp) {
                removeNum++;
                minDown = intervals[i][0];
                maxUp = intervals[i][1];
            } else if (intervals[i][1] > maxUp && intervals[i][0] < maxUp) {
                removeNum++;
            } else {
                maxUp = intervals[i][1];
            }
        }
        return removeNum;
    }
};