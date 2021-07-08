class Solution {
public:
    string replaceSpace(string s) {
        /*--------method 0:额外空格，略--------------------------------*/
        /*--------method 1:计算空格数量再进行扩容操作再进行重复制---------*/
        int old_length = s.size();
        int count = 0;
        for(auto &it:s){
            if (it == ' ')count++;
        }
        s.resize(old_length + 2 * count);
        for (int i = old_length-1, j = s.size()-1;i >= 0;i--,j--) {
            if (s[i] != ' '){
                s[j] = s[i];
            } else {
                s[j] = '0';
                s[j-1] = '2';
                s[j-2] = '%';
                j -= 2;
            }
        }
        return s;
    }
};