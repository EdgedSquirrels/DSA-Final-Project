#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
			bool contain;
		}token;
		bool result;
	} data;
} exp_TreeNode;

/*
typedef struct exp_Tree{ // expression tree
	struct exp_TreeNode* root;
} exp_Tree;
*/
/*
typedef struct exp_StackNode{
	exp_TreeNode* node;
	exp_TreeNode* nxt;
} exp_StackNode;

typedef struct exp_Stack{
	int num;
	exp_StackNode* head;
} exp_Stack;
*/

exp_TreeNode *stk1[2050], *stk2[2050], tokens[2050];
int n_stk1, n_stk2, n_token;

exp_TreeNode* stk_Move(){
	// pop two nodes in stk1 and their parent is stk2->top
	exp_TreeNode *pnode =stk2[n_stk2-1];
	pnode->left = stk1[n_stk1-1];
	pnode->right = stk1[n_stk1-2];
	--n_stk2;
	--n_stk1;
	stk1[n_stk1-1] = pnode;
}

void tree_Build(exp_TreeNode* root, char* expression){
	n_stk1 = 0;
    n_stk2 = 0;
	n_token = 0;
	exp_TreeNode* tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
	tnode->type = upper_bracket; // put a '(' in stk2 first
    stk2[n_stk2++] = tnode; // push tnode to stk2
	int index=0;
	while(true){
		if(expression[index] == '('){
			if(expression[index+1] == '('){
				tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
				tnode->type = upper_bracket;
				stk2[n_stk2++] = tnode;
				++index;
				continue;
			}
			tnode = &tokens[n_token++];
			fprintf(stderr,"%d\n",n_token);
			tnode->type = token;
			tnode->data.token.not = false;
			if(expression[++index] == '!'){
				tnode->data.token.not = true;
				++index;
			}
			int index2 = index;
			while(expression[++index2] != ')');
			tnode->data.token.s = expression + index;
			tnode->data.token.len = index2-index;
			tnode->data.token.contain = false;
			index = index2+1;
			continue;
		}
		if(expression[index] == '|'){
			tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
			tnode->type = operator;
			tnode->data.operator = or;
			while(n_stk2 > 0 && stk2[n_stk2-1]->type == operator && stk2[n_stk2-1]->data.operator == and){
				stk_Move(); // move the supreme node from stk2 to stk1 and do other operations such as child handling 
			}
			stk2[n_stk2++] = tnode;
			++index;
			continue;
		}
		if(expression[index] == '&'){
			tnode = (exp_TreeNode*)malloc(sizeof(exp_TreeNode));
			tnode->type = operator;
			tnode->data.operator = and;
			while(n_stk2 > 0 && stk2[n_stk2-1]->type == operator){
				stk_Move(); // move the supreme node from stk2 to stk1 and do other operations such as child handling 
			}
			stk2[n_stk2++] = tnode;
			++index;
			continue;
		}	
		if(expression[index] == ')' || expression[index] == '\0'){
			while(n_stk2 > 0 && stk2[n_stk2-1]->type != upper_bracket){
				stk_Move(); // move the supreme node from stk2 to stk1 and do other operations such as child handling 
			}
			--n_stk2;
			if(n_stk2 == 0) break;
			++index;
			continue;
		}
	}
	root = stk1[0];
}

void tree_Delete(exp_TreeNode* root){ // garbage collection
	if(root->type == token) return;
	else{
		tree_Delete(root->left);
		tree_Delete(root->right);
		free(root);
	}
}


bool tree_Eval(exp_TreeNode* root){ // using expression tree to evaluate true or false
	if(root->type == token) return root->data.result;
	else{
		if(root->data.operator == or) return (tree_Eval(root->left) || tree_Eval(root->right));
		return (tree_Eval(root->left) && tree_Eval(root->right));
	}
}

int main(){
	char expression[] = "(00)&(!01)&(00)\0\0";
	exp_TreeNode* Tree = NULL;
	printf("hi\n");
	tree_Build(Tree, expression);
	printf("hi\n");
	for(int i=0;i<n_token;i++){
		tokens[i].data.result = true;
		if(tokens[i].data.token.s[0] == '1') tokens[i].data.token.contain = true;
		// if the subject or the content contains the token, modify contain to true
		tokens[i].data.result = tokens[i].data.token.contain ^ tokens[i].data.token.not;
	}
	printf("%d",tree_Eval(Tree));
	tree_Delete(Tree);
}