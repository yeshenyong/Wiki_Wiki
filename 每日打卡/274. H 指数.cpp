class Solution {
public:
    int hIndex(vector<int>& citations) {
        /*-------method 1:sort + 遍历-----------*/
        /*
            时间复杂度O(nlogn)  空间复杂度O(n)即存储空间复杂度
         */
        sort(citations.begin(), citations.end());
        int h_hop=0, i = citations.size()-1;
        while (i >= 0 && citations[i] > h_hop) {
            h_hop++;
            i--;
        }
        return h_hop;

    }
};