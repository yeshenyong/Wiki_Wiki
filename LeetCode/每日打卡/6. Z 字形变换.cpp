class Solution {
public:
    string convert(string s, int numRows) {
        /* z-transformer */
        string result = "";
        int length = s.size();
        // some rules
        /*
            1.numRows
            2.total length
            3.some rules
         */
        int cul = numRows + (numRows - 2);
        if (!cul) return s;
        int cul_num = numRows - 2;
        int num = length / cul;
        for (int i=0;i < numRows;i++) {
            for (int j=0;j <= num;j++) {
                int index = i + (j * cul);
                if ((i == 0 || i == numRows - 1) && index < length) result.push_back(s[index]);
                else {
                    if (index < length) result.push_back(s[index]);
                    index += 2 * (numRows - 1 - i);
                    if (index < length) result.push_back(s[index]);
                }
            }
        }
        return result;
    }
};
// 时间复杂度O(numRows * z_nums)    空间复杂度O(s.size())