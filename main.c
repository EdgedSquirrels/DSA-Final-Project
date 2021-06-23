#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

typedef unsigned long long ull;
int n_mails, n_queries;
mail *mails;
query *queries;

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

//functions for expression_match:

//functions for find_similar:

//functions for group_analyse:



typedef struct DisjointSet{
	int n;
	int p[10000];
	int sz[10000];
	char* htable[10000];
	int ng; //num of groups 
	int lg; //size of largest group
} DisjointSet;

int hash(DisjointSet* djs,char* s){
	int tmp = 0;
	for(int i=0; s[i]!='\0'; i++){
		tmp += s[i];
	}
	tmp %= 10000;
	while(djs->htable[tmp] != NULL){
		if (strcmp(s,djs->htable[tmp]) == 0) return tmp;
		tmp++;
		tmp %= 10000;
	}
	djs->htable[tmp] = s;
	return tmp;
}

void djs_init(DisjointSet* djs){
	djs->ng = 0;
	djs->lg = 0;
	memset(djs->p,-1,sizeof(int)*10000);
	for(int i=0;i<10000;i++) djs->htable[i] = NULL;
}

void djs_MakeSet(DisjointSet* djs,int x){
	djs->p[x] = x;
	djs->sz[x] = 1;
	++djs->ng;
	djs->lg = max(djs->lg,djs->sz[x]);
}

int djs_FindSet(DisjointSet* djs,int x){
	if(djs->p[x] == x) return x;
	if(djs->p[x] < 0) {
		djs_MakeSet(djs,x);
		return x;
	}
	return djs->p[x] = djs_FindSet(djs, djs->p[x]);
}

void djs_Union(DisjointSet* djs, int x, int y){
	x = djs_FindSet(djs, x);
	y = djs_FindSet(djs, y);
	if(x == y) return;
	if(djs->sz[x] < djs->sz[y]) swap(&x,&y);
	djs->p[y] = x;
	djs->sz[x] += djs->sz[y];
	--djs->ng;
	djs->lg = max(djs->lg, djs->sz[x]);	
}


int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	/* guessing no-match for all expression- match queries */
	for(int i = 0; i < n_queries; i++){
		/*if(queries[i].type == expression_match){
			int *ans, n_ans = 0;
			ans = (int*)malloc(sizeof(int)*n_mails);
			char *expression = queries[i].data.expression_match_data.expression;
			for(int j = 0; j < n_mails;j++){
				// todo
				mails[j].content;
			}
			fprintf(stderr,"id:%d\n",queries[i].id);
			fprintf(stderr,"data:%d\n",queries[i].data);
			qsort(ans, n_ans,sizeof(int),comp);
			api.answer(queries[i].id, ans, n_ans);
		}
		if(queries[i].type == find_similar){
			int *ans;
			ans = (int*)malloc(sizeof(int)*100);
			fprintf(stderr,"id:%d\n",queries[i].id);
			fprintf(stderr,"data:%d\n",queries[i].data);
			api.answer(queries[i].id, ans, 15);
			getchar();
		}*/
		if(queries[i].type == group_analyse){
			int ans[2];
			int len = queries[i].data.group_analyse_data.len;
			int* mids = queries[i].data.group_analyse_data.mids;
			DisjointSet djs;
			djs_init(&djs);
			for(int j=0; j<len; j++){
				//fprintf(stderr,"str:%s %s\n", mails[mids[j]].from, mails[mids[j]].to);
				//fprintf(stderr,"str:%d %d\n", hash(&djs,mails[mids[j]].from), hash(&djs,mails[mids[j]].to));
				djs_Union(&djs, hash(&djs,mails[mids[j]].from), hash(&djs,mails[mids[j]].to));
				//build a hash function for hash
			}
			

			ans[0] = djs.ng;
			ans[1] = djs.lg;
			/*
			fprintf(stderr,"id:%d\n",queries[i].id);
			fprintf(stderr,"data:%d\n",queries[i].data);
			fprintf(stderr,"ans:%d %d\n",ans[0],ans[1]);
			*/
			api.answer(queries[i].id, ans, 2);
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