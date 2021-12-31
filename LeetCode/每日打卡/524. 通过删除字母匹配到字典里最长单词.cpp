class Solution {
public:
    string findLongestWord(string s, vector<string>& dictionary) {
        /*---method 1:双指针---*/
        /*
        string maxStr = "";
        int maxLength = 0;
        for (int i=0;i < dictionary.size();i++) {
            int si = 0, sj = 0;
            while(si < dictionary[i].size() && sj < s.size()) {
                if (dictionary[i][si] == s[sj]) {
                    si++;
                    sj++;
                } else {
                    sj++;
                }
                if (si == dictionary[i].size()) {
                    if (dictionary[i].size() > maxLength || (maxStr > dictionary[i] && maxLength == dictionary[i].size())) {
                        maxLength = dictionary[i].size();
                        maxStr = dictionary[i];
                    }
                }
            }
        }
        return maxStr;
        */
        /*---method 2:双指针 +　sort---*/
        sort(dictionary.begin(), dictionary.end(), [](string s1, string s2){
            if (s2.size() > s1.size()) return false;
            if (s1.size() == s2.size()) return s1>s2?false:true;
            return true;
        });
        for (int i=0;i < dictionary.size();i++) {
            int si = 0, sj = 0;
            while(si < dictionary[i].size() && sj < s.size()) {
                if (dictionary[i][si] == s[sj]) {
                    si++;
                    sj++;
                } else {
                    sj++;
                }
                if (si == dictionary[i].size()) {
                    return dictionary[i];
                }
            }
        }
        return "";
    }
};