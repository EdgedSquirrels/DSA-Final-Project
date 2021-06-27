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
