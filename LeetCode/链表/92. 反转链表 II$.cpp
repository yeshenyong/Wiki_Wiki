class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        /*---method 1:链表反转---*/
        ListNode* dummpy = new ListNode(-1);
        dummpy->next = head;
        ListNode* pre = dummpy;
        for (int i=1;i < m;i++) pre = pre->next;
        head = pre->next;
        for (int i=m;i < n;i++) {
            ListNode* tmp = head->next;
            head->next = tmp->next;
            tmp->next = pre->next;
            pre->next = tmp;
        }
        ListNode* result = dummpy->next;
        delete dummpy;
        return result;
    }
};