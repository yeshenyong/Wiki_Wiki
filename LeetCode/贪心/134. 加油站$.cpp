class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        /*---method 1:贪心规律---*/
        int n = gas.size();
        int startSum = 0, allSum = 0;
        int start = 0;
        for (int i=0;i < n;i++) {
            startSum += gas[i]-cost[i];
            allSum += gas[i]-cost[i];
            if (startSum < 0) {
                startSum = 0;
                start = i+1;
            }
        }
        if (allSum < 0) return -1;
        return start;
    }
};