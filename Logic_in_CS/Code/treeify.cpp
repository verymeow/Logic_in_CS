// SUBMITTED BY:
// TARUN RAHEJA, 2015A7PS106H
// AHRAZ RIZVI, 2015A7PS012H

#include<cstdio>
#include<cstring>
#include<string>
#include<sstream>
#include<iostream>
#include<time.h>
using namespace std;

char infix[100], answer[100], reversed[100],prefix[100];
int seen[1000], values[1000];
int topElem=-1;//Global var representing size of stack.
char stack_arr[100];//Global var representing stack array.
char current;//Var representing current element being tested by for loop in give answer function.
int isOperand(char element);//Returns 1 if current character is not ~,+,*,>,) or(.
int isOperator(char element);//Returns 1 if current character is an operator, i.e. ~,+,* or>.
int isOpeningBracket(char element);//Returns 1 if current element is (.
int isClosingBracket(char element);//Returns 1 if current element is ).
void push(char element);//Pushes element onto stack.
void pop();//Pops top element from stack.
char top();// Shows top element of stack.
int emptys();//Returns 1 if stack is empty, 0 otherwise.
int precedence(char elem);//Returns precedence values of operators.
void reverses(char arr1[100], char arr2[100]);//Copies arr1 in reverse to arr2, and reverses parentheses.
int catches=0;
string cnf_exp;
int counter1=0,counter2=0;
int arr_for_op[300], arr_for_not[300];

void giveAnswer();
class Tree_node;
class Node_of_stack;
class Tree;
int isOperand(char element);
char temp_swap1,temp_swap2;

int topOfAns=-1;//Global variable representing top position of answer array.
char buffer;//Stores leftover characters if stack is nonempty.
int tot;
void impl_free(Tree_node* hav); //
void nnf(Tree_node* hav);       //  These four functions are involved in converting
void cnf(Tree_node* hav);       //  given expression to the more convenient cnf form.
void distr(Tree_node* has);     //
int check_validity(void);// Checks validity of CNF form of logic statement.

//Parse tree
//ALGORITHM:
//We first reverse the prefix expression.
//Then we traverse the string for each character.
//In case of an operand, we make a new node and add it to the stack.
//For an operator, we make a new node, then append two children to it by popping two nodes off the stack.
//We push this onto the stack, and recursively continue till we reach the end of the string.


class Tree_node{//This represents a single node of the expression tree.
public:
    char data;
    Tree_node *left_child;
    Tree_node *right_child;
    Tree_node(char elem){
        this->data= elem;
        this->left_child= NULL;
        this->right_child= NULL;
    }
};

class Node_of_stack{//This represents a single unit of the stack that'll be used to hold nodes of expression tree while construction.
public:
    Tree_node* node;
    Node_of_stack* next;
    Node_of_stack(Tree_node* node){
        this->node=node;
        this->next=NULL;
    }
};

class Tree{//This will be the stack where nodes parsed from input will be stacked. Will become final output tree.
private:
    Node_of_stack* top;
public:
    Tree(){
        top=NULL;
    }

    void push(Tree_node* new_node){//This pushes a new Node_of_stack onto the stack of nodes.
        if(top==NULL)
            top= new Node_of_stack(new_node);
        else{
            Node_of_stack* a= new Node_of_stack(new_node);
            a->next=top;
            top=a;
        }
    }

    Tree_node* pop(){//This pops out the top Node_of_stack from the stack.
        Tree_node* b= top->node;
        top= top->next;
        return b;
    }

    Tree_node* tops(){//Returns pointer to top node of current stack.
        return top->node;
    }

    void append(char elem){// Adds the parsed element from prefix string to the tree.
        if(isOperand(elem)==1){
            Tree_node * c= new Tree_node(elem);
            push(c);
        }
        else{
            Tree_node* c=new Tree_node(elem);
            if(elem=='~'){
                c->left_child=NULL;
                c->right_child=pop();
                push(c);
            }
            else{
                c->left_child=pop();
                c->right_child=pop();
                push(c);
            }
        }
    }

    int evaluate(){//Function for user to evaluate expression, no parameters for method.
        return evaluates(tops());
    }

    int evaluates(Tree_node* k){// Method for internal computation, has a node as a method.
        if(k->left_child==NULL&&k->right_child==NULL)
            return convert_to_int(k->data);

            int result=0;
            int left, right;
            char oper;
            if(k->data=='~'){
                right=evaluates(k->right_child);
                oper=k->data;
            }
            else{
                left=evaluates(k->left_child);
                right=evaluates(k->right_child);
                oper=k->data;
            }
            switch(oper){
                case '*':{
                    if(left==1&&right==1)
                        result=1;
                    else
                        result=0;
                }
                break;
                case '+':{
                    if(left==0&&right==0)
                        result=0;
                    else
                        result=1;
                }
                break;
                case '>':{
                    if(left==1&&right==0)
                        result=0;
                    else
                        result=1;
                }
                break;
                case '~':{
                    if(right==0)
                        result=1;
                    else
                        result=0;
                }
                break;
            }
            return result;

    }

    int convert_to_int(char elem){//converts digit character to integer.
        int a;
        a=elem-'0';
        return a;
    }

    void make_tree(string prefix){//Parses the string and makes the tree.
        for(int i=prefix.length()-1; i>=0;i--)
            append(prefix[i]);
    }

    void infix(){//Gets the infix expression.
        in_ord_trav(tops());
    }

    void in_ord_trav(Tree_node* m){//Traverses the tree in order to get the infix expression.
        if(m!=NULL){
            cout<<"(";
//            if(catches==1)
//                cnf_exp+="(";
            in_ord_trav(m->left_child);
            cout<<m->data;
            if(catches==1)
                cnf_exp+=m->data;
            in_ord_trav(m->right_child);
            cout<<")";
//            if(catches==1)
//                cnf_exp+=")";
        }
    }

    int height(){//User friendly method for height.
        return heights(tops());
    }

    int heights(Tree_node*a){//calculates height of tree recursively.
        if(a->left_child==NULL&&a->right_child==NULL)
            return 1;
        if(a->left_child==NULL && a->right_child!=NULL){
            tot=1+heights(a->right_child);
        }
        else
            tot=1+max(heights(a->left_child),heights(a->right_child));
        return tot;
    }

};

 //Tree_node *temporary_t;//will be used to append "not"
 //Tree_node *temporary_2;//will be used to append "not"

int main(){
    clock_t start;

    //start=clock();
    double time_taken;
    cout<<"Type your infix logical expression: "<<endl;
    char temporary;
    gets(infix);
    giveAnswer();
    string expression;
    ostringstream os;
    for(int i=0;prefix[i]!='\0';i++){
        os<<""<<prefix[i];
    }
    expression=os.str();
    string back_up=expression;
    cout<<endl;
    cout<<"Required prefix expression is:\n";
    puts((prefix));                                          //TASK 1
    cout<<endl;
    cout<<"Now making the tree:"<<endl;     //TASK 2
    Tree t;
    t.make_tree(expression);
    cout<<"Tree is made."<<endl;
    cout<<endl;
    cout<<"Outputting infix using in-order traversal of parse tree:"<<endl;     //TASK 3
    t.infix();
    cout<<endl;
    cout<<endl;
    cout<<"Height of the tree is: "<<t.height()<<"."<<endl;         //TASK 4
    cout<<endl;
    cout<<"Now,for evaluating expression, give inputs:"<<endl;      //TASK 5
    cout<<endl;
    for(int i=0;i<expression.length();i++){
        if(isOperand(expression[i])==1){
            if(seen[(int)expression[i]]==0){
                seen[(int)expression[i]]=1;
                cout<<"Enter 1 or 0 value for "<<expression[i]<<" :"<<endl;
                cin>>temporary;
                values[i]=temporary;
                expression[i]=temporary;
            }
            else{
                expression[i]=values[i];
            }
        }
    }
    t.make_tree(expression);
    cout<<"Value is: "<<t.evaluate()<<"."<<endl;
    cout<<endl<<endl;
    cout<<"----------------------------------";
    cout<<endl;
    cout<<"Making tree for CNF form."<<endl;
    Tree cnf_tree;
    cnf_tree.make_tree(back_up);
    impl_free(cnf_tree.tops());
    //cout<<endl;
    //cnf_tree.infix();
    //cout<<endl;
    nnf(cnf_tree.tops());
    //cout<<endl;
    //cnf_tree.infix();
    //cout<<endl;
    cnf(cnf_tree.tops());
    //cout<<endl;
    //cnf_tree.infix();
    //cout<<endl;
    cout<<endl;
    catches=1;
    cout<<"CNF expression for given logic statement is:"<<endl;
    cnf_tree.infix();                     //TASK 6
    cout<<endl;
    cout<<endl;
    cout<<"Height of original tree is "<<t.height()<<" and that of CNF form is "<<cnf_tree.height()<<"."<<endl; //TASK 8
    catches=0;
    //cout<<cnf_exp;
    if(check_validity()==1)                                           //TASK 7
        cout<<"It is a valid expression."<<endl;
    else
        cout<<"It is not a valid expression."<<endl;
    return 0;
}

void giveAnswer(){
    reverses(infix,reversed);//Input is now reversed and with correct parentheses and stored in reversed.
    int i;
    for(i=0; reversed[i]!='\0';i++){
        current=reversed[i];
        if(isOperand(current)==1){
            //Add to String
            topOfAns++;
            answer[topOfAns]=current;
        }
        else if(isOperator(current)==1){
            //While Loop
            while(emptys()==0 && precedence(current)< precedence(top())){
                topOfAns++;
                answer[topOfAns]=top();
                pop();
            }
            push(current);
        }
        else if(isOpeningBracket(current)==1){
            //Add to stack
            push(current);
        }
        else if(isClosingBracket(current)==1){
            //While loop
            while(emptys()==0 && isOpeningBracket(top())==0){
                topOfAns++;
                answer[topOfAns]=top();
                pop();
            }
            pop();
        }
    }
    while(emptys()==0){
        //Add remaining to expression
        buffer=top();
        topOfAns++;
        answer[topOfAns]=buffer;
        pop();
    }
    topOfAns++;
    answer[topOfAns]='\0';
    reverses(answer,prefix);
}

int isOperand(char element){
    switch(element){
    case '+':
    case '*':
    case '~':
    case '>':
    case '(':
    case ')':
        return 0;
    break;
    default:
        return 1;
    }
}

int isOperator(char element){
    switch(element){
    case '+':
    case '*':
    case '~':
    case '>':
        return 1;
    break;
    default:
        return 0;
    }
}

int isOpeningBracket(char element){
    if(element=='(')
        return 1;
    else
        return 0;
}

int isClosingBracket(char element){
    if(element==')')
        return 1;
    else
        return 0;
}

void push(char element){
    topElem++;
    stack_arr[topElem]=element;
}

void pop(){
    topElem--;
}

char top(){
    char ret;
    ret=stack_arr[topElem];
    return ret;
}

void reverses(char arr1[100], char arr2[100]){
    int length=0;
    int j,k;
    for(j=0;arr1[j]!='\0';j++){
        length++;
    }
    for(k=0;k<length;k++){
        arr2[k]=arr1[length-1-k];
        if(arr2[k]=='(')
            arr2[k]=')';
        else if(arr2[k]==')')
            arr2[k]='(';
    }
    arr2[length]='\0';
}

int emptys(){
    if(topElem==-1)
        return 1;
    else
        return 0;
}

int precedence(char elem){
    switch(elem){
        case '>':
            return 2;
		break;
		case '*':
        case '+':
            return 3;
		break;
		case '~':
            return 4;
		break;
        case '(':
        case ')':
            return 1;
		break;
    }
}

void impl_free(Tree_node* hav){
    if(isOperand(hav->data)==1)
        //return;
    if(hav->data=='+'){
        impl_free(hav->left_child);
        impl_free(hav->right_child);
    }
    if(hav->data=='*'){
        impl_free(hav->left_child);
        impl_free(hav->right_child);
    }
    if(hav->data=='~'){
        impl_free(hav->right_child);
    }
    if(hav->data=='>'){
        char temp_swap1=(hav->left_child)->data;
        Tree_node* to_add= new Tree_node(temp_swap1);
        to_add->left_child=(hav->left_child)->left_child;
        to_add->right_child=(hav->left_child)->right_child;
        (hav->left_child)->data='~';
        (hav->left_child)->left_child=NULL;
        (hav->left_child)->right_child=to_add;
        hav->data='+';
        impl_free(hav->left_child);
        impl_free(hav->right_child);
    }
}

void nnf(Tree_node* hav){
    if(isOperand(hav->data)==1){
        //return;
    }
    if(hav->data=='+'){
        nnf(hav->left_child);
        nnf(hav->right_child);
    }
    if(hav->data=='*'){
        nnf(hav->left_child);
        nnf(hav->right_child);
    }
    if(hav->data=='~'){
        if((hav->right_child)->data=='~'){
            hav->data=((hav->right_child)->right_child)->data;
            //cout<<"s";
            hav->left_child=((hav->right_child)->right_child)->left_child;
            //cout<<"s";
            hav->right_child=((hav->right_child)->right_child)->right_child;
            //cout<<"s";
            nnf(hav);
            return;
            //cout<<"bbb";
        }
        if((hav->right_child)->data=='+'){
            //temp_swap1=((hav->right_child)->left_child)->data;
            //temp_swap2=((hav->right_child)->right_child)->data;
            hav->data='*';
            (hav->right_child)->data='~';
            Tree_node* num1 = new Tree_node('~');
            num1->left_child=NULL;
            num1->right_child=(hav->right_child)->left_child;
            (hav->right_child)->left_child=NULL;
            hav->left_child=num1;
            nnf(hav->left_child);
            nnf(hav->right_child);
        }
        if((hav->right_child)->data=='*'){
            //temp_swap1=((hav->right_child)->left_child)->data;
            //temp_swap2=((hav->right_child)->right_child)->data;
            hav->data='+';
            (hav->right_child)->data='~';
            Tree_node* num1 = new Tree_node('~');
            num1->left_child=NULL;
            num1->right_child=(hav->right_child)->left_child;
            (hav->right_child)->left_child=NULL;
            hav->left_child=num1;
            nnf(hav->left_child);
            nnf(hav->right_child);
        }
//        if(isOperand((hav->right_child)->data)==1){
//            return;
//        }
    }
}

void distr(Tree_node* has){
    if((has->left_child)->data=='*'){
        has->data='*';
        (has->left_child)->data='+';
        temp_swap1=((has->left_child)->right_child)->data;
        temp_swap2=(has->right_child)->data;
        Tree_node* num1=new Tree_node(temp_swap1);
        num1->left_child=((has->left_child)->right_child)->left_child;
        num1->right_child=((has->left_child)->right_child)->right_child;
        ((has->left_child)->right_child)->data=temp_swap2;
        ((has->left_child)->right_child)->left_child=(has->right_child)->left_child;
        ((has->left_child)->right_child)->right_child=(has->right_child)->right_child;
        (has->right_child)->data='+';
        Tree_node* num2=new Tree_node(temp_swap2);
        num2->left_child=((has->left_child)->right_child)->left_child;
        num2->right_child=((has->left_child)->right_child)->right_child;
        (has->right_child)->left_child=num1;
        (has->right_child)->right_child=num2;
        distr(has->left_child);
        distr(has->right_child);
        return;
    }
    if((has->right_child)->data=='*'){
        has->data='*';
        (has->right_child)->data='+';
        temp_swap1=((has->right_child)->left_child)->data;//b
        temp_swap2=(has->left_child)->data;//a
        Tree_node* num1=new Tree_node(temp_swap1);
        num1->left_child=((has->right_child)->left_child)->left_child;
        num1->right_child=((has->right_child)->left_child)->right_child;
        ((has->right_child)->left_child)->data=temp_swap2;
        ((has->right_child)->left_child)->left_child=(has->left_child)->left_child;
        ((has->right_child)->left_child)->right_child=(has->left_child)->right_child;
        (has->left_child)->data='+';
        Tree_node* num2=new Tree_node(temp_swap2);
        num2->left_child=((has->right_child)->left_child)->left_child;
        num2->right_child=((has->right_child)->left_child)->right_child;
        (has->left_child)->left_child=num1;
        (has->left_child)->right_child=num2;
        distr(has->left_child);
        distr(has->right_child);
    }
    if(((has->left_child)->data!='*')&&((has->right_child)->data!='*')){
        return;
    }
}

void cnf(Tree_node* hav){
    if(isOperand(hav->data)==1)
        return;
    if(hav->data=='*'){
        cnf(hav->left_child);
        cnf(hav->right_child);
    }
    if(hav->data=='+'){
        cnf(hav->left_child);
        cnf(hav->right_child);
        distr(hav);
    }
}

int check_validity(void){
    for(int i=0;i<cnf_exp.length();i++){
        if(cnf_exp[i]=='*')
            counter1++;
    }
    counter1++;
    int j=0;
    while(cnf_exp[j]!='\0'){
        while(cnf_exp[j]!='\0'){
            if(cnf_exp[j]=='*'){
                j++;
                break;
            }
            if(isOperand(cnf_exp[j])==1){
                if(cnf_exp[j-1]=='~'){
                    arr_for_not[(int)cnf_exp[j]]=1;
                }
                else{
                    arr_for_op[(int)cnf_exp[j]]=1;
                }
            }
            j++;
        }
        for(int p=0;p<300;p++){
            if(arr_for_not[p]==1 && arr_for_op[p]==1){
                counter2++;
                break;
            }
        }
        for(int p=0;p<300;p++){
            arr_for_not[p]=0;
            arr_for_op[p]=0;
        }
    }
    if(counter2==counter1)
        return 1;
    else
        return 0;
}
