#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
////
bool isAlpha (char c){ 
    if((c>=97&&c<=122)||(c>=65&&c<=90)||(c>=48&&c<=57))
        return true;
    else
        return false;
}
/// 
typedef struct exp_TreeNode{ // expression tree node
	enum{
		operator,
		token, 
		upper_bracket,
		not
	} type;
	struct exp_TreeNode* left;
	struct exp_TreeNode* right;
	union{
		enum{
			or,
			and
		} operator;
		struct{
			char *s;
			int len;
			bool result;
		}token;
	} data;
} exp_TreeNode;

exp_TreeNode *stk1[2050], *stk2[2050], tokens[2050];
int n_stk1, n_stk2, n_token;

exp_TreeNode* stk_Move(int type){ //type1: not type2: other
	// pop two nodes in stk1 and their parent is stk2->top
	exp_TreeNode *pnode =stk2[n_stk2-1];
	--n_stk2;
	pnode->left = stk1[n_stk1-1];
	if(type != 1){
		--n_stk1;
		pnode->right = stk1[n_stk1-1];
	}
	stk1[n_stk1-1] = pnode;
}

void tree_Build(exp_TreeNode** root, char* expression){
	n_stk1 = 0;
    n_stk2 = 0;
	n_token = 0;
	exp_TreeNode* tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
	tnode->type = upper_bracket; // put a '(' in stk2 first
    stk2[n_stk2++] = tnode; // push tnode to stk2
	int index=0;
	while(true){
		//fprintf(stderr,"%d %d\n", n_stk1, n_stk2);
		if(expression[index] == ')' || expression[index] == '\0'){
			while(n_stk2 > 0 && stk2[n_stk2-1]->type != upper_bracket){
				if (stk2[n_stk2-1]->type == not) stk_Move(1);
				else stk_Move(2); // move the supreme node from stk2 to stk1 and do other operations such as child handling 
			}
			free(stk2[--n_stk2]);
			if(n_stk2 == 0) break;
			++index;
			continue;
		}
		if(isAlpha(expression[index])){
			tnode = &tokens[n_token++];
			//fprintf(stderr,"%d\n",n_token);
			tnode->type = token;
			int index2 = index;
			while(expression[++index2] != ')');
			tnode->data.token.s = expression + index;
			tnode->data.token.len = index2-index;
			index = index2;
			stk1[n_stk1++] = tnode;
			continue;
		}

		tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
		if(expression[index] == '('){
			tnode->type = upper_bracket;
		}
		else if(expression[index] == '!'){
			tnode->type = not;
			while(n_stk2 > 0){
				if(stk2[n_stk2-1]->type == not) stk_Move(1);
				else break;
			}
		}
		else if(expression[index] == '|'){
			tnode->type = operator;
			tnode->data.operator = or;
			while(n_stk2 > 0){
				if(stk2[n_stk2-1]->type == operator ) stk_Move(2);
				else if(stk2[n_stk2-1]->type == not) stk_Move(1);
				else break;
			}
		}
		else if(expression[index] == '&'){
			tnode->type = operator;
			tnode->data.operator = and;
			while(n_stk2 > 0){
				if(stk2[n_stk2-1]->type == operator && stk2[n_stk2-1]->data.operator == or) stk_Move(2);
				else if(stk2[n_stk2-1]->type == not) stk_Move(1);
				else break;
			}
		}
		stk2[n_stk2++] = tnode;
		++index;
	}
	*root = stk1[0];
}

void tree_Delete(exp_TreeNode* root){ // garbage collection
	if(root->type == token) return;
	tree_Delete(root->left);
	if(root->type == operator){
		tree_Delete(root->right);
	}
	free(root);
}


bool tree_Eval(exp_TreeNode* root){ // using expression tree to evaluate true or false
	//fprintf(stderr,"he");
	if(root == NULL) fprintf(stderr,"JNOOOF");
	if(root->type == token) {
		//root->data.token.result = in_the_mail(0,root->data.token.len   /*,int mail_index*/,root->data.token.s)
		//fprintf(stderr,"%d",root->data.token.result);
		return root->data.token.result;
		//return root->data.result;
	}
	if(root->type == not) {
		//fprintf(stderr,"!");
		return !tree_Eval(root->left);
	}
	else{
		if(root->data.operator == or) {
			//fprintf(stderr,"|");
			return (tree_Eval(root->left) || tree_Eval(root->right));
		}
		//fprintf(stderr,"&");
		return (tree_Eval(root->left) && tree_Eval(root->right));
	}
}

int main(){
	char expression[] = "(00)&((111)|(10))&!(99)";
	exp_TreeNode* Tree = NULL;
	tree_Build(&Tree, expression);
	for(int i=0;i<n_token;i++){
		//for test
		if(tokens[i].data.token.s[0] == '1') {
			tokens[i].data.token.result = true;
		}
		else tokens[i].data.token.result = false;
	}
	printf("%d",tree_Eval(Tree));
	tree_Delete(Tree);
}