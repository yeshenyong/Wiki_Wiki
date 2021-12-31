class Solution {
public:

    int compress(vector<char>& chars) {
        /*---method 1:字符串模拟---*/
        int length = chars.size();
        char pre = chars[0];
        if (length == 1)return 1;
        int preCount = 1;
        int beginStep = 0;
        for (int i=1;i < chars.size() || chars.size() == 1;i++) {
            if (pre == chars[i]) {
                preCount++;
            } else {
                if (preCount == 1) {
                    chars[beginStep++]=pre;
                }
                else {
                    chars[beginStep++]=pre;
                    string cnt = to_string(preCount);
                    for (auto& it:cnt) {
                        chars[beginStep++]=it;
                    }
                }
                pre = chars[i];
                preCount = 1;
            }
            if (i == length-1) {
                chars.push_back(pre+1);
            }
        }
        return beginStep;
    }
};