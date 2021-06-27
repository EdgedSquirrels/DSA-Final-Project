#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define hash_N 1000000
#define len_N 100 //hash function of expression: sum*len_N+len
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.
int sum_N=hash_N/len_N;
typedef unsigned long long ull;
int n_mails, n_queries;
int mail_index, query_index;
mail *mails;
query *queries;

char stack[2050];//variables for expression
int top = -1;
bool postfix[2050]; 

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
	int query_index;
	struct hash_data *next;
}hash_data;
hash_data *hash_table[2][hash_N];
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

//string要是legal
int hash_token_mail(char *string,int *len) 
{
	int sum=0;
	*len = 0;
	for(;;(*len)+=1)
	{
		if(is_legal(string[*len]))
		{
			sum+=hash_char(string[*len]);
		}
		else
		{
			break;
		}
		sum%=sum_N;
	}
	return sum*len_N+(*len)%len_N;
}
int hash_token(char *string,int len) 
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
int string_compare(int len,char string1[],char string2[])
{
	for(int loop1=0;loop1<len;loop1++)
	{
		if(string1[loop1] != string2[loop1])
		{
			return 0;
		}
	}
	return 1;
}
void put_into_hash_table(int hash_value,char* string_start,int hash_table_id)//chaining
{
	hash_data *data=hash_table[hash_table_id][hash_value];
	// 考慮data是NULL的情況
	/*
	if (data == NULL)
	{
		data = (hash_data*) malloc(sizeof(hash_data));
		data->string_start = string_start;
		data->mail_index = mail_index;
		data->query_index = query_index;
		data->next = NULL;
		return;
	}
	*/
	while(1)
	{
		if(data->mail_index==mail_index && data->query_index==query_index)
		{
			if(data->next==NULL)
			{
				data->next=(hash_data*)malloc(sizeof(hash_data));
				data=data->next;
				data->string_start = string_start;
				data->mail_index = mail_index;
				data->query_index = query_index;
				data->next=NULL;
				return;
			}
			else
			{
				data=data->next;
			}
		}
		else
		{
			data->string_start=string_start;
			data->mail_index=mail_index;
			data->query_index=query_index;
			data->next = NULL;
			//garbage collection 省略
			return;
		}
	}
}

int in_the_mail(char* string,int len,int hash_table_id)//true is 1, false is 0
{
	hash_data *data=hash_table[hash_table_id][hash_token(string,len)];
	if(data == NULL) return 0;
	while(1)
	{
		if(data->mail_index!=mail_index||data->query_index!=query_index)
		{
			return 0;
		}
		else// mail_index and query_index are identical 
		{
			if(string_compare(len,string,data->string_start))
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
void Build_Hash_Table(int hash_table_id){
	if(hash_table_id == 0){
		// expression match
		char* text = mails[mail_index].subject;
		int len, hash_value;
		while(true){
			//fprintf(stderr,"\n%s\n",text);
			for(int index = 0; text[index]!='\0'; index++){
				if(!is_legal(text[index])) continue;
				//fprintf(stderr,"hi1");
				hash_value=hash_token_mail(text+index, &len);
				//fprintf(stderr," %d",hash_value);
				put_into_hash_table(hash_value,text+index,0);
				index += len;
				if(text[index] == '\0') break;
				//fprintf(stderr,"hi1");
			}
			if(text == mails[mail_index].subject) text = mails[mail_index].content;
			else break;
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
bool toBool (char expression[],int *i){
    int start = *i,length=1,id;
    *i = *i + 1;
    while(isAlpha(expression[*i])){
        length++;
        *i = *i+1;
    }
    *i = *i - 1;
    return in_the_mail(expression+start, length, 0);
													
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
int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	/* guessing no-match for all expression- match queries */
	int loop1,loop2,loop3,loop4;//loop1 means loop with depth 1,loop2 means loop with depth 2.......
	for(int loop0 = 0; loop0<2; loop0++){ //initialize the hash_table0 & hash_table1
		for (int loop1 = 0; loop1<hash_N; loop1++){
			hash_table[loop0][loop1]=(hash_data*)malloc(sizeof(hash_data));
			hash_table[loop0][loop1]->string_start=NULL;
			hash_table[loop0][loop1]->mail_index=-1;
			hash_table[loop0][loop1]->query_index=-1;
			hash_table[loop0][loop1]->next=NULL;
		}
	}
	n_queries = 100;
	for(int loop1 = 0; loop1 < n_queries; loop1++){
		
		if(queries[loop1].type == expression_match)
		{
			query_index = queries[loop1].id;
			int *ans, n_ans = 0;
			ans = (int*)malloc(sizeof(int)*n_mails);
			char *expression = queries[loop1].data.expression_match_data.expression;
			for(int loop2 = 0; loop2 < n_mails;loop2++)
			{
				// todo
				mail_index = mails[loop2].id;
				//fprintf(stderr,"hi1");
				Build_Hash_Table(0);
				//fprintf(stderr,"hi2");
				if(transfer(expression)){
					ans[n_ans] = mail_index;
					n_ans+=1;
				}
			}
			// qsort(ans, n_ans,sizeof(int),comp);
			api.answer(queries[loop1].id, ans, n_ans);
		}
		
		
		if(queries[loop1].type == find_similar)
		{
			int *ans;
			ans = (int*)malloc(sizeof(int)*n_mails);
			int mid=queries[loop1].data.find_similar_data.mid;
			double threshold=queries[loop1].data.find_similar_data.threshold;
			int hash_value,len;
			double mid_len=0,mail_len,intersect_len;
			double similarity;
			int ans_len=0;
			for(loop2=0;;loop2++)//hash the mid
			{
				hash_value=hash_token_mail(loop2,&len,mails[mid].content);
				if(!in_the_mail(loop2,len,mid,loop1,0,mails[mid].content))
				{
					put_into_hash_table(hash_value,loop2,mid,loop1,0,mails[mid].content);
					mid_len++;
				}
				loop2+=len;
				if(mails[mid].content[loop2]=='\0')
				{
					break;
				}
			}
			for(loop2=0;loop2<n_mails;loop2++)
			{
				mail_len=0,intersect_len=0;
				if(loop2==mid)
				{
					continue;
				}
				for(loop3=0;;loop3++)//hash the current email
				{
					hash_value=hash_token_mail(loop3,&len,mails[loop2].content);
					if(!in_the_mail(loop3,len,loop2,loop1,1,mails[loop2].content))
					{
						put_into_hash_table(hash_value,loop3,loop2,loop1,1,mails[loop2].content);
						mail_len++;
						if(in_the_mail(loop3,len,mid,loop1,0,mails[loop2].content))
						{
							intersect_len++;
						}
					}
					loop3+=len;
					if(mails[loop2].content[loop3]=='\0')
					{
						break;
					}
				}
				similarity=(intersect_len)/(mail_len+mid_len-intersect_len);
				if(similarity>threshold)
				{
					ans[ans_len]=loop2;
					ans_len++;
				}
			}
			api.answer(queries[loop1].id, ans, ans_len);
		}
		if(queries[loop1].type == group_analyse)
		{
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