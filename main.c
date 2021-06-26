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
int mail_index;
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
//----------------------------------------------------------------
typedef struct hash_data//use chaining
{
	char *string_start;
	int mail_index;
	hash_data *next;
}hash_data;
hash_data *hash_table[2][1000000];
int is_legal(char s)
{
	if((s-'0')<0||(s-'0')>9)
	{
		if((s-'A')<0||(s-'A')>25)
		{
			if((s-'a')<0||(s-'a')>25)
			{
				return 0;
			}
		}
	}
	return 1;
}
int hash_char(char s)
{
	if(is_legal(s)==0)
	{
		return 37;
	}
	if((s-'0')<10&&(s-'0')>=0)
	{
		return(s-'0')+26;
	}
	return (s-'A')%32;
}
int hash_token_mail(int start,int *len,char string[])
{
	int loop1=0;
	int sum=0;
	for(loop1=start;;loop1++)
	{
		if(is_legal(string[loop1]))
		{
			sum+=hash_char(string[loop1]);
		}
		else
		{
			break;
		}
		sum%=sum_N;
	}
	(*len)=loop1-start;
	return sum*len_N+(*len)%len_N;
}
int hash_token(int start,int len,char string[])
{
	int loop1=0;
	int sum=0;
	for(loop1=0;loop1<len;loop1++)
	{
		sum+=hash_char(string[loop1]);
	}
	sum%=sum_N;
	len%=len_N;
	return sum*len_N+len;
}
void put_into_hash_table(int hash_value,int string_index,int mail_index,int hash_table_id,int string[])//chaining
{
	hash_data *data=hash_table[hash_table_id][hash_value];
	while(1)
	{
		if(data->mail_index==mail_index)
		{
			if(data->next=NULL)
			{
				data->next=malloc(sizeof(hash_data));
				data=data->next;
				data->string_start=&string[string_index];
				data->mail_index=mail_index;
				return;
			}
			else
			{
				data=data->next;
			}
		}
		else
		{
			data->string_start=&string[string_index];
			data->mail_index=mail_index;
			return;
		}
	}
}
int string_compare(int len,char string1[],char string2[])
{
	for(int loop1=0;loop1<len;loop1++)
	{
		if(hash_char(string1[loop1])!=hash_char(string2[loop1]))
		{
			return 0;
		}
	}
	return 1;
}
int in_the_mail(int start,int len,int mail_index,int hash_table_id,char string[])//true is 1, false is 0
{
	hash_data *data=hash_table[hash_table_id][hash_token(start,len,string)];
	while(1)
	{
		if(data->mail_index!=mail_index)
		{
			return 0;
		}
		else//data->mail_index==mail_index
		{
			if(string_compare(len,&string[start],data->string_start))
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
//end of hashing funcs
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
bool toBool (char expression[],int *i,int mail_index){
    int start = *i,length=1,id;
    *i = *i + 1;
    while(isAlpha(expression[*i])){
        length++;
        *i = *i+1;
    }
    *i = *i - 1;
    return in_the_mail(start,length,mail_index,0,expression);//我不知道mail_id要怎麼處理，之後幫一下
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
bool transfer(char expression[],int mail_index){
    int i=0,pl=0; //pl is the length of postfix
    while(expression[i]!='\0'){
        if(isAlpha(expression[i])){
            postfix[pl] = toBool(expression,&i,mail_index);
            pl++;
        }
        else if(expression[i]=='!'){
            if(expression[i+1]=='(')
                push(expression[i]);
            else{
                i++;
                postfix[pl] = !toBool(expression,&i,mail_index);
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
int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	/* guessing no-match for all expression- match queries */
	int loop1,loop2,loop3,loop4;//loop1 means loop with depth 1,loop2 means loop with depth 2.......
	for(int loop1=0;loop1<1000000;loop1++)//initialize the hash_table(expression match)
	{
		hash_table[0][loop1]=malloc(sizeof(hash_table));
		hash_table[0][loop1]->string_start=NULL;
		hash_table[0][loop1]->mail_index=-1;
		hash_table[0][loop1]->next=NULL;
	}
	for(int loop1 = 0; loop1 < n_queries; loop1++){
		if(queries[loop1].type == expression_match){
			int *ans, n_ans = 0;
			ans = (int*)malloc(sizeof(int)*n_mails);
			char *expression = queries[loop1].data.expression_match_data.expression;
			int len,hash_value,ans_len;
			for(int loop2 = 0; loop2 < n_mails;loop2++)
			{
				// todo
				mail_index = mails[loop2].id;
				for(loop3=0;;loop3++)//hash the current email subject
				{
					hash_value=hash_token_mail(loop3,&len,mails[loop2].subject);
					put_into_hash_table(hash_value,loop3,loop2,0,mails[loop2].subject);
					loop3+=len;
					if(mails[loop2].content[loop3]=='\0')
					{
						break;
					}
				}
				for(loop3=0;;loop3++)//hash the current email content
				{
					hash_value=hash_token_mail(loop3,&len,mails[loop2].content);
					put_into_hash_table(hash_value,loop3,loop2,0,mails[loop2].content);
					loop3+=len;
					if(mails[loop2].content[loop3]=='\0')
					{
						break;
					}
				}
			}
			// fprintf(stderr,"id:%d\n",queries[i].id);
			// fprintf(stderr,"data:%d\n",queries[i].data);
			// qsort(ans, n_ans,sizeof(int),comp);
			api.answer(queries[loop1].id, ans, ans_len);
		}
		if(queries[loop1].type == find_similar){
			int *ans;
			ans = (int*)malloc(sizeof(int)*100);
			// fprintf(stderr,"id:%d\n",queries[i].id);
			// fprintf(stderr,"data:%d\n",queries[i].data);

			api.answer(queries[loop1].id, ans, 15);
			// getchar();
		}
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