class Solution {
public

    void getNext (int next, const string& s) {
        next[0] = -1;
        int j = -1;
        for (int i=1;i  s.size();i++){
            while(j = 0 && s[i] != s[j+1]){
                j = next[j];
            }
            if (s[i] == s[j+1])j++;
            next[i] = j;
        }
    }

    bool repeatedSubstringPattern(string s) {
        -------method 1自闭kmp---------
        if (s.size() == 0)
            return false;
        int next[s.size()];
        getNext(next, s);
        int len = s.size();
        if (next[len-1] != -1 && len%(len-(next[len-1] + 1))==0)
            return true;
        return false;
    }
};