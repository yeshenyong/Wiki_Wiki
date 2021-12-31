class Solution {
public:
    int EatZero(const string& cnt) {
        int result = 0;
        int left = 0, right = cnt.size() - 1;
        while(left < cnt.size() && cnt[left] == '0')left++;
        // while(right >=0 && cnt[right] == '0')right--;        
        if (left >= cnt.size()) return 0;
        else if (left == right) return cnt[left]-'0';
        else {
            while(left <= right) {
                result = result * 10 + cnt[left]-'0';
                left++;
            }
        }
        return result;
    }

    int compareVersion(string version1, string version2) {
        /*----method 1:字符串模拟----*/
        vector<int> v1;
        vector<int> v2;
        int left = 0;
        for (int i=0;i < version1.size();i++) {
            if (version1[i] == '.') {
                string tmp = version1.substr(left, i-left);
                v1.emplace_back(EatZero(tmp));
                left = i+1;
            }
        }
        v1.emplace_back(EatZero(version1.substr(left)));
        left = 0;
        for (int i=0;i < version2.size();i++) {
            if (version2[i] == '.') {
                string tmp = version2.substr(left, i-left);
                int result = EatZero(tmp);
                v2.emplace_back(result);
                if (v2.size() > v1.size()) {
                    if (result != 0) return -1;
                } else {
                    if (result > v1[v2.size()-1]) return -1;
                    else if (result < v1[v2.size()-1])return 1;
                }
                left = i+1;
            }
        }
        string fin_al = version2.substr(left);
        int final_result = EatZero(fin_al);
        v2.emplace_back(final_result);
        if (v2.size() > v1.size()) {
            if (final_result != 0) return -1;
        } else if (v1.size() >= v2.size()) {
            if (final_result > v1[v2.size()-1]) return -1;
            else if (final_result < v1[v2.size()-1])return 1;
            for (int i=v2.size();i < v1.size();i++) if (v1[i] != 0) return 1;
        }
        return 0;
    }
};