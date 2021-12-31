class Solution {
public:
    string SearchMaxPalidRome(const string& s, int left, int right) {
        while(left >= 0 && right < s.size()) {
            if (s[left] == s[right]) {
                left--;
                right++;
            } else {
                break;
            }
        }
        // printf("left = %d, right = %d\n", left+1, right-1);
        return s.substr(left+1, right-left-1);
    }

    string longestPalindrome(string s) {
        int maxnLength = 0;
        string result;
        /*-----method 1:中心扩展法-----*/
        for (int i=0;i < s.size();i++) {
            string str1 = SearchMaxPalidRome(s, i, i);
            string str2 = SearchMaxPalidRome(s, i, i+1);
            // cout << "str1 = " << str1<< endl;
            // cout << "str2 = " << str2<< endl;
            
            if (str1.size() < str2.size())str1 = str2;
            if (maxnLength < str1.size()) {
                maxnLength = str1.size();
                result = str1;
                // cout << result << endl;
            }
        }
        /*-----method 2:动态规划-----*/
    

        return result;
    }
};