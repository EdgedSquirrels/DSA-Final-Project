#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define sum_N 10000
#define len_N 100 //hash function of expression: sum*len_N+len

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

typedef unsigned long long ull;
int n_mails, n_queries;
mail *mails;
query *queries;

char stack[2050];//variables for expression
int top = -1;
bool postfix[2050]; 
/*
int comp(const void* a, const void* b){
	//compare function for qsort
	return *(int*)a - *(int*)b;
}
*/

#define max(a,b) a > b ? a : b
void swap(int *a, int *b){
	int _ = *a;
	*a = *b;
	*b = _;
}
//functions for find_similar:

//functions for group_analyse:
//----------------------------------------------------------------
typedef struct DisjointSet{
	int n;
	int parent[10000];//parent is used to find the root with the path compression
	int size[10000];
	char* htable[10000];
	int group_num; //num of groups 
	int max_group_len; //size of largest group
} DisjointSet;

int hash(DisjointSet* djs,char* s){//with linear probing
	int hash_value = 0;
	for(int i=0; s[i]!='\0'; i++){
		hash_value += s[i];
	}
	hash_value %= 10000;
	while(djs->htable[hash_value] != NULL){
		if (strcmp(s,djs->htable[hash_value]) == 0) return hash_value;
		hash_value++;
		hash_value %= 10000;
	}
	djs->htable[hash_value] = s;
	return hash_value;
}

void djs_init(DisjointSet* djs){
	djs->group_num = 0;
	djs->max_group_len = 0;
	memset(djs->parent,-1,sizeof(int)*10000);
	for(int i=0;i<10000;i++) djs->htable[i] = NULL;
}

void djs_MakeSet(DisjointSet* djs,int x){
	djs->parent[x] = x;
	djs->size[x] = 1;
	++djs->group_num;
	djs->max_group_len = max(djs->max_group_len,djs->size[x]);
}

int djs_FindSet(DisjointSet* djs,int x){
	if(djs->parent[x] == x) return x;
	if(djs->parent[x] < 0) {
		djs_MakeSet(djs,x);
		return x;
	}
	return djs->parent[x] = djs_FindSet(djs, djs->parent[x]);
}

void djs_Union(DisjointSet* djs, int x, int y){
	x = djs_FindSet(djs, x);
	y = djs_FindSet(djs, y);
	if(x == y) return;
	if(djs->size[x] < djs->size[y]) swap(&x,&y);
	djs->parent[y] = x;
	djs->size[x] += djs->size[y];
	--djs->group_num;
	djs->max_group_len = max(djs->max_group_len, djs->size[x]);	
}
//----------------------------------------------------------------
//expression match
<<<<<<< HEAD
typedef struct hash_data
=======
//----------------------------------------------------------------
typedef struct hash_data//use chaining
>>>>>>> 9a5e20b6bfa18412561a4bc182c2ca589668532e
{
	char *string;
	int mail_index;
}hash_data;
<<<<<<< HEAD

hash_data hash_table[1000000];
int hash_char(char s)
{
=======
hash_data *hash_table[1000000];
int is_legal(char s)
{
	if((s-'0')<0&&(s-'0')>9)
	{
		if((s-'A')<0&&(s-'A')>25)
		{
			if((s-'a')<0&&(s-'a')>25)
			{
				return 0;
			}
		}
	}
	return 1;
}
int hash_char(char s)
{
	if((s-'0')<10&&(s-'0')>=0)
	{
		return(s-'0')+26;
	}
>>>>>>> 9a5e20b6bfa18412561a4bc182c2ca589668532e
	return (s-'A')%32;
}
int hash_token_no_len(int start,int *len,char expression[])
{
	int loop1=0;
	int sum=0;
	for(loop1=start;;loop1++)
	{
		if(is_legal(expression[loop1]))
		{
			sum+=hash_char(expression[loop1]);
		}
		else
		{
			break;
		}
		(*len)=loop1-start;
		sum%=sum_N;
	}
	return sum*len_N+(*len)%len_N;
}
int hash_token(int start,int len,char expression[])
{
	int loop1=0;
	int sum=0,len;
	for(loop1=0;loop1<len;loop1++)
	{
		sum+=hash_char(expression[loop1]);
	}
	sum%=sum_N;
	len%=len_N;
	return sum*len_N+len;
}
<<<<<<< HEAD
int in_the_mail(char token[],int mail_index)//true is 1, false is 0
{
	hash_data data=hash_table[hash_token(token)];
	if(data.mail_index==mail_index&&data.string)
=======
void put_into_hash_table(int hash_value,int string_index,int mail_index)//chaining
{
	hash_data *data=hash_table[hash_value];
	int temp_mail_index;
	if(data->mail_index!=mail_index)
	{
		data->string_index=string_index;
		data->mail_index=mail_index;
	}
	else
	{
		while(1)
		{
			if(data->mail_index==mail_index)
			{
				if(data->next=NULL)
				{
					data->next=malloc(sizeof(hash_data));
					data=data->next;
					data->string_index=string_index;
					data->mail_index=mail_index;
					break;
				}
				else
				{
					data=data->next;
				}
			}
			else
			{
				data->string_index=string_index;
				data->mail_index=mail_index;
				break;
			}
		}
	}
	
}
int string_compare(int len,char string1[],char string2[])
{
	for(int loop1=0;loop1<len;loop1++)
>>>>>>> 9a5e20b6bfa18412561a4bc182c2ca589668532e
	{
		if(hash_char(string1[loop1])!=hash_char(string2[loop1]))
		{
			return 0;
		}
	}
	return 1;
}
<<<<<<< HEAD

//////////////////////////////////////////////////////////////////////
typedef struct exp_TreeNode{ // expression tree node
	enum{
		operator,
		token,
		upper_bracket
	} type;
	struct exp_TreeNode* left;
	struct exp_TreeNode* right;
	union{
		enum{
			or,
			and
		} operator;
		struct{
			bool not;
			char *s;
			int len;
		}token
	} data;
} exp_TreeNode;

typedef struct exp_Tree{ // expression tree
	struct exp_TreeNode* root;
} exp_Tree;

typedef struct exp_StackNode{
	exp_TreeNode* node;
	exp_TreeNode* nxt;
} exp_StackNode;

typedef struct exp_Stack{
	int num;
	exp_StackNode* head;
} exp_Stack;

void stk_init(exp_Stack* stk){
	stk->num = 0;
	stk->head = NULL;
}

exp_TreeNode* stk_Peep(exp_Stack* stk){
	if(stk->head != NULL) return stk->head->node;
	else return NULL;
}

exp_TreeNode* stk_Pop(exp_Stack* stk){
	if(stk->head == NULL) return NULL;
	exp_StackNode old = stk->head;
	exp_TreeNode* tnode = old->node;
	stk->head = old->nxt;
	free(old);
	--stk->num;
	return tnode;
}

exp_TreeNode* stk_Move(exp_Stack* stk1, exp_Stack stk2){
	exp_TreeNode* tnode1 = stk_Pop(stk1);
	exp_TreeNode* tnode2 = stk_Pop(stk2);
}

void stk_Push(exp_Stack* stk, exp_TreeNode* tnode){
	exp_StackNode* snode = (exp_StackNode*)malloc(sizeof(exp_StackNode));
	snode->node = tnode;
	snode->nxt = stk->head;
	stk->head = snode;
	++stk->num;
}

void tree_Build(exp_Tree* Tree, char* expression){
	exp_Stack stk1, stk2;
	stk_init(stk1);
	stk_init(stk2);
	exp_TreeNode* tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
	tnode->data = upper_bracket;
	stk_Push(stk2, tnode);
	int index=0;
	while(true){
		if(expression[index] == '('){
			tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
			if(expression[index+1] == '('){
				tnode->data = upper_bracket;
				stk_Push(stk1, tnode);
				++index;
				continue;
			}
			tnode->type = token;
			tnode->data.token.not = false;
			++index;
			if(expression[index] == '!'){
				tnode->data.token.not = true;
				++index;
			}
			int index2 = index;
			while(expression[++index2] != ')');
			tnode->data.token.s = expression + index;
			tnode->data.token.len = index2-index;
			index = index2+1;
			continue;
		}
		if(expression[index] == '|'){
			tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
			tnode->type = operator;
			tnode->data.operator = or;
			exp_StackNode* snode = stk_Peep(stk2);
			while(snode != NULL && snode->node->data.operator == and){
				stk_Move(stk1, stk2); // move the supreme node from stk2 to stk1 and do other operations such as child handling 
			}

			++index;
			continue;
		}
		if(expression[index] == '&'){
			tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
			tnode->type = operator;
			tnode->data.operator = and;
			++index;
			continue;
		}
	}

	//!() | & ()

}

void tree_Delete(){ // garbage collection

}


bool tree_Eval(){ // using expression tree to evaluate true or false

}
//////////////////////////////////////////////////////////////////////////


=======
int in_the_mail(int start,int len,int mail_index,char expression[],char mail[])//true is 1, false is 0
{
	hash_data *data=hash_table[hash_token(start,len,expression)];
	while(1)
	{
		if(data->mail_index!=mail_index)
		{
			return 0;
		}
		else//data->mail_index==mail_index
		{
			if(string_compare(len,&expression[start],&mail[data->string_index]))
			{
				return 1;
			}
			else
			{
				if(data->next==NULL)
				{
					return 0;
				}
				else
				{
					data=data->next;
				}
			}
		}
	}
}
void push(char oper){
    top++;
    stack[top] = oper;
}
char pop(){
    top--;
    return stack[top+1];
}
bool isAlpha (char c){
    if((c>=97&&c<=122)||(c>=65&&c<=90)||(c>=48&&c<=57))
        return true;
    else
        return false;
}
bool toBool (char expression[],int *i){
    int start = *i,length=1,id;
    *i = *i + 1;
    while(isAlpha(expression[*i])){
        length++;
        *i = *i+1;
    }
    *i = *i - 1;
    return in_the_mail(start,length,id,expression[]);//我不知道mail_id要怎麼處理，之後幫一下
													//然後mails是全域變數，不用當參數吧(你的in_the_mail)
}
bool operate (bool a,bool b,char oper){
    switch(oper){
        case '&':
            return a&b;
        case '|':
            return a|b;
        default:
            break;
    }
}
bool supreme (char a){
    if(top==-1)
        return true;
    else{
        char b = stack[top];
        if(a=='&'&&b=='|')
            return true;
        else 
            return false;
    }
}
bool transfer(char expression[]){
    int i=0,pl=0; //pl is the length of postfix
    while(expression[i]!='\0'){
        if(isAlpha(expression[i])){
            postfix[pl] = toBool(expression,&i);
            pl++;
        }
        else if(expression[i]=='!'){
            if(expression[i+1]=='(')
                push(expression[i]);
            else{
                i++;
                postfix[pl] = !toBool(expression,&i);
                pl++;
            }
        }
        else if(expression[i]=='('){
            push(expression[i]);
        }
        else if(expression[i]==')'){
            char op = pop();
            while(op!='('){
                postfix[pl-2] = operate(postfix[pl-2],postfix[pl-1],op);
                pl--;
                op = pop();
            }
            if(top!=-1&&stack[top]=='!')
                postfix[pl-1] = !postfix[pl-1];
        }
        else{
            if(supreme(expression[i]))
                push(expression[i]);
            else{
                while(top!=-1&&(!supreme(expression[i]))&&stack[top]!='('){
                    char op = pop();
                    postfix[pl-2] = operate(postfix[pl-2],postfix[pl-1],op);
                    pl--;
                }
                push(expression[i]);
            }
        }
        i++;
    }
    while(top!=-1){
        char y = pop();
        postfix[pl-2] = operate(postfix[pl-2],postfix[pl-1],y);
        pl--;        
    }
    return postfix[0];
}
//----------------------------------------------------------------
>>>>>>> 9a5e20b6bfa18412561a4bc182c2ca589668532e
int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	/* guessing no-match for all expression- match queries */
	int loop1,loop2,loop3,loop4;//loop1 means loop with depth 1,loop2 means loop with depth 2.......
	for(int loop1=0;loop1<1000000;loop1++)//initialize the hash_table(expression match)
	{
<<<<<<< HEAD
		hash_table[loop1].string=NULL;
		hash_table[loop1].mail_index=-1;
=======
		hash_table[loop1]=malloc(sizeof(hash_table));
		hash_table[loop1]->string_index=-1;
		hash_table[loop1]->mail_index=-1;
		hash_table[loop1]->next=NULL;
>>>>>>> 9a5e20b6bfa18412561a4bc182c2ca589668532e
	}
	for(int loop1 = 0; loop1 < n_queries; loop1++){
		if(queries[loop1].type == expression_match){
			int *ans, n_ans = 0;
			ans = (int*)malloc(sizeof(int)*n_mails);
			char *expression = queries[loop1].data.expression_match_data.expression;
<<<<<<< HEAD
			exp_Tree Tree;
			tree_Build(&Tree, expression);
			for(int loop2 = 0; loop2 < n_mails;loop2++){
				// todo
				mails[loop2].content;
=======
			int len,hash_value,ans_len;
			for(int loop2 = 0; loop2 < n_mails;loop2++)
			{
				// todo
				for(loop3=0;;loop3++)//hash the current email
				{
					hash_value=hash_token_no_len(loop3,&len,mails[loop2].content);
					put_into_hash_table(hash_value,loop3,loop2);
					loop3+=len;
					if(mails[loop2].content[loop3]=='\0')
					{
						break;
					}
				}
				//庭碩&squirrels�????�???????�????
>>>>>>> 9a5e20b6bfa18412561a4bc182c2ca589668532e
			}
			// fprintf(stderr,"id:%d\n",queries[i].id);
			// fprintf(stderr,"data:%d\n",queries[i].data);
			// qsort(ans, n_ans,sizeof(int),comp);
			api.answer(queries[loop1].id, ans, n_ans);
		}
		/*
		if(queries[i].type == find_similar){
			int *ans;
			ans = (int*)malloc(sizeof(int)*100);
			fprintf(stderr,"id:%d\n",queries[i].id);
			fprintf(stderr,"data:%d\n",queries[i].data);
			api.answer(queries[i].id, ans, 15);
			getchar();
		}*/
		if(queries[loop1].type == group_analyse){
			int ans[2];
			int len = queries[loop1].data.group_analyse_data.len;
			int* mids = queries[loop1].data.group_analyse_data.mids;
			DisjointSet djs;
			djs_init(&djs);
			for(int loop2=0; loop2<len; loop2++){
				//fprintf(stderr,"str:%s %s\n", mails[mids[j]].from, mails[mids[j]].to);
				//fprintf(stderr,"str:%d %d\n", hash(&djs,mails[mids[j]].from), hash(&djs,mails[mids[j]].to));
				djs_Union(&djs, hash(&djs,mails[mids[loop2]].from), hash(&djs,mails[mids[loop2]].to));
				//build a hash function for hash
			}
			

			ans[0] = djs.group_num;
			ans[1] = djs.max_group_len;
			/*
			fprintf(stderr,"id:%d\n",queries[i].id);
			fprintf(stderr,"data:%d\n",queries[i].data);
			fprintf(stderr,"ans:%d %d\n",ans[0],ans[1]);
			*/
			api.answer(queries[loop1].id, ans, 2);
			//return :[ng, lg] 
			//ng: numberofgroups 
			//lg: sizeoflargestgroup 
		}
	}
  return 0;
}



/*
gcc main.c -o main -O3 -std=c11 -w
g++ validator/validator.cpp -o validator/validator -O3 -std=c11 -w
./main < testdata/test.in | validator/validator
./main < testdata/test.in
*/