#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define M_SIZE(__MAType__) sizeof(__MAType__)
#define V_STRUCT(__MASpecifier__) \
	struct __MASpecifier__##_T

#define FALSE 0
#define TRUE 1
#define BOOL int

struct ListNode_T
{
	char mData;
	struct ListNode_T* mPtrNextNode;
};
typedef struct ListNode_T ListNode;
typedef ListNode* ptrListNode;

struct List
{
	ptrListNode mPtrNode;
	
	void (*Insert)(struct List*, ptrListNode*, char);
	void (*Delete)(struct List*, ptrListNode*, char);
	void (*Clear) (struct List*, ptrListNode*);
	BOOL (*IsEmpty)(struct List*, ListNode*);
	void (*PrintList)(struct List* ListNode*);
};
typedef struct List List;
void CtorList(List* mList)
{
}

void Insert(ptrListNode*, char);
BOOL Delete(ptrListNode*, char);
void DeleteList(ptrListNode*);

BOOL IsEmpty(ListNode*);
void PrintList(ListNode*);
void Instructions(void);

#define COMM_INSERT 1
#define COMM_DELETE 2
#define COMM_EXIT 3

int main(int argc, char** argv)
{
	ListNode* ptrListNode = NULL;
	int	choice = 0;
	char item	 = 0;

	Instructions(); /* Display the menu */

	do {
		printf("Enter Command >> "); scanf("%d", &choice);

		switch (choice)
		{
		case COMM_INSERT:
		{
			for (;;)
			{
				printf("Enter a character : "); scanf("%c", &item);
				if (item == '\n') { break; }

				Insert(&ptrListNode, item);
				printf("List : "); PrintList(ptrListNode);
			}
		} break;
		case COMM_DELETE:
		{
			for (;;)
			{
				if (IsEmpty(ptrListNode) == TRUE)
				{
					printf("Linked list is empty.\n");
					break;
				}

				printf("Enter a character to delete : "); scanf("%c", &item);
				if (item == '\n') { break; }

				if (Delete(&ptrListNode, item) == TRUE) 
				{ printf("%c Deleted successfully.\n", item); }
				else 
				{ printf("Did not find %c in LinkedList.\n", item); }

				PrintList(ptrListNode);
			}
		} break;
		default: printf("Invalid Command.\n"); break;
		}
	}
	while (choice != COMM_EXIT);

	printf("End of Run.\n");
	DeleteList(&ptrListNode);
	return 0;
}

void Instructions(void)
{
	printf(
			"Enter your choice:\n"
			"1 to insert an element into the list.\n"
			"2 to delete an element from the list.\n"
			"3 to exit application.\n");
}

void Insert(ListNode** pptrListNode, char value)
{
	if (IsEmpty(*pptrListNode) == TRUE)
	{
		*pptrListNode = (ListNode*)malloc(sizeof(ListNode));
		(*pptrListNode)->mData = value;
		(*pptrListNode)->mPtrNextNode = NULL;
	}
	else
	{
		Insert(&(*pptrListNode)->mPtrNextNode, value);
	}
}

BOOL Delete(ListNode** pptrListNode, char value)
{
	if (IsEmpty(*pptrListNode) == TRUE)  { return FALSE; }
	if ((*pptrListNode)->mData != value) { return Delete(&(*pptrListNode)->mPtrNextNode, value); }

	ListNode* ptrListNode = *pptrListNode;
	*pptrListNode = (*pptrListNode)->mPtrNextNode;

	free(ptrListNode);
	return TRUE;
}

BOOL IsEmpty(ListNode* ptrListNode)
{
	return ptrListNode == NULL ? TRUE : FALSE;
}

void PrintList(ListNode* ptrListNode)
{
	if (IsEmpty(ptrListNode) == TRUE) { puts(""); return; }
	printf("%c ", ptrListNode->mData);
	PrintList(ptrListNode->mPtrNextNode);
}

void DeleteList(ListNode** pptrListNode)
{
	if (IsEmpty((*pptrListNode)->mPtrNextNode) == FALSE)
	{ 
		DeleteList(&(*pptrListNode)->mPtrNextNode); 
	}

	free(*pptrListNode);
	return;
}
