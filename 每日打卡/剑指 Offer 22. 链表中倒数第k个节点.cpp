class Solution {
public:
    ListNode* getKthFromEnd(ListNode* head, int k) {
        /*---method 1:双指针---*/
        ListNode* khead = head;
        while(khead != NULL && k--) khead = khead->next;
        while(head != NULL && khead != NULL) {
            khead = khead->next;
            head = head->next;
        }
        return head;
    }
};