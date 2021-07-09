class Solution {
public:

    void reverseStr(string& s, int begin, int end){
        for (int i=begin, j=end;i < j;i++,j--) {
            swap(s[i], s[j]);
        }
    }

    void EatSpace(string &s) {
        int fastIndex = 0;
        int slowIndex = 0;
        int length = s.size();
        /* 去除字符串前面的空格 */
        while(length > 0 && fastIndex < length && s[fastIndex] == ' ')fastIndex++;
        
        for(;fastIndex < length;fastIndex++){
            if (fastIndex-1 > 0 &&s[fastIndex] == ' ' && s[fastIndex-1] == ' '){
                continue;
            } else {
                s[slowIndex++] = s[fastIndex];
            }
        }
        /* 去除字符串后面的空格 */
        if (slowIndex - 1 > 0 && s[slowIndex-1] == ' ')
            s.resize(slowIndex-1);
        else
            s.resize(slowIndex);
    }

    string reverseWords(string s) {
        /*-------method 1:字符串处理,先反转,吃空格,空间重定义,再反转----------*/
        int length = s.size();
        reverseStr(s, 0, length-1);
        EatSpace(s);
        length = s.size();
        int start = 0, end = 0;
        while(start < length){
            while(end < length && s[end] != ' ')end++;
            reverseStr(s, start, end-1);
            start = end+1;
            end = start;
        }
        return s;
    }
};