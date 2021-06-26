
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char stack[2050];
int top = -1;
bool postfix[2050];

int in_the_mail(int start,int len,int mail_index,char expression[]); 
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
// bool toBool (char expression[],int *i){
//     int start = *i,length=1,id;
//     *i = *i + 1;
//     while(isAlpha(expression[*i])){
//         length++;
//         *i = *i+1;
//     }
//     *i = *i - 1;
//     return in_the_mail(start,length,id,expression[]);
// }
bool toBool(char expression[],int *i){ //for test purpose
    if(expression[*i]=='1')
        return true;
    else 
        return false;
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
void transfer(char expression[]){
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
}
int main (void){
    char test[1000];
    scanf("%s",test);
    transfer(test);
    if(postfix[0])
        printf("true");
    else
        printf("false");
    return 0;
}
  270  main.c 
@@ -14,6 +14,9 @@ int n_mails, n_queries;
mail *mails;
query *queries;

char stack[2050];//variables for expression
int top = -1;
bool postfix[2050]; 
/*
int comp(const void* a, const void* b){
	//compare function for qsort
@@ -27,15 +30,10 @@ void swap(int *a, int *b){
	*a = *b;
	*b = _;
}

//functions for expression_match:

//functions for find_similar:

//functions for group_analyse:



//----------------------------------------------------------------
typedef struct DisjointSet{
	int n;
	int parent[10000];//parent is used to find the root with the path compression
@@ -93,43 +91,136 @@ void djs_Union(DisjointSet* djs, int x, int y){
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
int hash_token(char token[])
int hash_token_no_len(int start,int *len,char expression[])
{
	int loop1=0;
	int sum=0,len;
	for(loop1=0;;loop1++)
	int sum=0;
	for(loop1=start;;loop1++)
	{
		if(token[loop1]=='\0')
		if(is_legal(expression[loop1]))
		{
			sum+=hash_char(expression[loop1]);
		}
		else
		{
			len=loop1+1;
			break;
		}
		sum+=hash_char(token[loop1]);
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
@@ -267,25 +358,178 @@ bool tree_Eval(){ // using expression tree to evaluate true or false
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
0 comments on commit c232c7c
@EdgedSquirrels
 
 
Leave a comment
未選擇任何檔案
Attach files by dragging & dropping, selecting or pasting them.
 You’re receiving notifications because you’re watching this repository.
© 2021 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
Loading complete