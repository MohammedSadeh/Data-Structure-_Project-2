/*
Name : Mohammed Jamil Saada
ID : 1221972
Sec. NO. : 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1000
#define LINE 100

struct stack;
struct queue;

typedef struct stack* pts;
typedef struct queue* ptq;

struct stack{
char* token;
int operation;
int index;
pts next;
};

struct queue{
char* token;
ptq next;
ptq front;
ptq rear;
};

//functions Prototype
void display_menu();
void print_loaded_text(char myString[SIZE]);
pts createStack();
ptq createQueue();
void push(pts, char*, int, int);
pts pop(pts);
void EnQueue(ptq, char*);
ptq DeQueue(ptq);
void loadText(char myString[SIZE], char str[SIZE]);
void insert(char str[SIZE],ptq,pts,pts);
int FindIndex(char word[LINE],char str[SIZE]);
void shiftRight(char str[SIZE], int from, int x);
void removeFromText(char str[SIZE], pts, pts);
void shiftLeft(char str[SIZE], int from, int x);
void undo_op(char str[SIZE],pts Undo, pts Redo);
void redo_op(char str[SIZE],pts Undo, pts Redo);
void print_undo_redo(pts undo, pts redo);
void saveToFile(char str[SIZE], pts, pts);
void clearStackContent(pts stack);


int main()
{
    int choice;
    char myString[SIZE]; //string that we want to modify on it
    char str[SIZE];      //string to save the loaded text without modification
    ptq Q = createQueue();
    pts Undo = createStack();
    pts Redo = createStack();
    display_menu();
    scanf("%d",&choice);
    while(choice != 1 && choice != 9){
        printf("You can't do any operation before loaded the initial text.\n");
        printf("Please try again..\n");
        printf("\n");
        display_menu();
        scanf("%d",&choice);
    }
    while(choice != 9){
        switch(choice){
            case 1 : loadText(myString,str);
                     break;
            case 2 : print_loaded_text(str);
                     break;
            case 3 : insert(myString,Q,Undo,Redo);
                     break;
            case 4 : removeFromText(myString,Undo,Redo);
                     break;
            case 5 : undo_op(myString,Undo,Redo);
                     break;
            case 6 : redo_op(myString,Undo,Redo);
                     break;
            case 7 : print_undo_redo(Undo,Redo);
                     break;
            case 8 : saveToFile(myString,Undo,Redo);
                     break;
            default : printf("Invalid operation, Try again..\n");
        }
        printf("\n");
        display_menu();
        scanf("%d",&choice);
    }
    printf("Thank you for using my text editor..\n");
    printf("Exit from the program...\n");
    return 0;
}

//function to display the menu
void display_menu(){
    printf("chose the operation you want..\n");
    printf("Menu :\n");
    printf("1- Load the input file which contains the initial text.\n");
    printf("2- Print the loaded text.\n");
    printf("3- Insert strings to the text.\n");
    printf("4- Remove strings from the text.\n");
    printf("5- Perform Undo operation.\n");
    printf("6- Perform Redo operation.\n");
    printf("7- Print the Undo Stack and the Redo Stack.\n");
    printf("8- Save the updated text to the output file.\n");
    printf("9- Exit.\n");
}

//function to print the loaded text
void print_loaded_text(char myString[SIZE]){
    printf("The loaded text : \n%s\n",myString);
}

//function to create stack
pts createStack(){  //Done
    pts T = (pts)malloc(sizeof(struct stack));
    if(T==NULL){
        printf("Stack can not be defined..\n");
        return NULL;
        }
    else {
        T->next = NULL;
        return T;
        }
}

//function to create Queue
ptq createQueue(){
    ptq T = (ptq)malloc(sizeof(struct queue));
    if(T==NULL){
        printf("Queue can not be defined..\n");
        return NULL;
        }
    else {
        T->next = NULL;
        T->front = T->next;
        T->rear = T->next;
        return T;
        }
}

//function to push node to stack
void push(pts S, char* a, int op, int in){   //Done
    if(S == NULL)
        printf("Stack is not found..\n");
    else {
        pts k = (pts)malloc(sizeof(struct stack));
        k->token = strdup(a);
        k->operation = op;
        k->index = in;
        k->next = S->next;
        S->next = k;
    }
}

//function to pop node from stack
pts pop(pts S){         //Done
    if(S == NULL){
        printf("Stack is not found..\n");
        return NULL;
    }
    else {
        if(S->next != NULL){
            pts temp = S->next;
            S->next = temp->next;
            temp->next = NULL;
            return temp;
        }
        else return NULL;
    }
}

//function to enqueue node to queue
void EnQueue(ptq Q, char* tok){
    if(Q == NULL)
        printf("Queue not exist..\n");
    else {
            ptq New = (ptq)malloc(sizeof(struct queue));
            New->token = tok;
            New->front = NULL;
            New->rear = NULL;
            ptq temp = Q->rear;
            if(temp != NULL){
                temp->next = New;
                New->next = NULL;
                Q->rear = New;
                Q->front = Q->next;
            }
            else {
                Q->next = New;
                New->next = NULL;
                Q->front = New;
                Q->rear = New;
            }
    }

}

//function to dequeue node from queue
ptq DeQueue(ptq Q){
    if(Q == NULL){
        printf("Queue not exist..\n");
        return NULL;
    }
    else {
        if(Q->front == NULL){
            printf("Queue is Empty..\n");
            return NULL;
        }
        else {
            ptq temp = Q->front;
            Q->next = temp->next;
            Q->front = Q->next;
            if(Q->front==NULL)
                Q->rear = NULL;
            temp->next = NULL;
            return temp;
        }
    }
}

//function to load data to stack
void loadText(char myString[SIZE], char str[SIZE]){
    FILE* input = fopen("input.txt","r");
    //check if the file exist or not
    if(input == NULL){
        printf("Error! the file can't be opened..\n");
        return;
    }
    //if the file exist load the data
    else{
        myString[0] = '\0';
        char s[LINE];
        while(fgets(s,sizeof(s),input)){
            strcat(myString,s);
        }
        fclose(input);
        strcpy(str,myString);
    }
    printf("The initial text loaded successfully..\n");

}

//function to insert to the original text
void insert(char str[SIZE],ptq Q, pts Undo, pts redo){
    int x;
    int index;
    char s[SIZE];
    char newS[SIZE];
    char before[LINE];
    newS[0] ='\0';
    clearStackContent(redo);
    printf("Insert word or sentence to this Text..\n");
    printf("%s\n",str);
    //cases that we can insert in it
    printf("What you want ?\n");
    printf("1- Insert at the beginning of Text.\n");
    printf("2- Insert at specific place in Text.\n");
    printf("3- Insert at the end of Text.\n");
    scanf("%d",&x);
    printf("Enter what you want to insert to the text..\n");
    getchar();
    scanf("%[^\n]%*c",s);
    int v = strlen(str);
    //load what we want to insert to queue word by word
    char* token = strtok(s," ");
    while(token != NULL){
        EnQueue(Q,token);
        token = strtok(NULL," ");
    }
    int b = 0;
    //switch to determine what case we chose to insert
    switch(x){
    case 1 :    while(Q->front != NULL){
                ptq temp = DeQueue(Q);
                index = b;
                push(Undo,temp->token,1,index);
                strcat(newS,temp->token);
                strcat(newS," ");
                b += strlen(temp->token)+1;
                free(temp);
             }
             strcat(newS,str);
             strcpy(str,newS);
             break;
    case 2 : printf("Enter the word that you want to insert before it..\n");
             scanf("%[^\n]%*c",before);
             char temp[SIZE], bef[LINE];
             strcpy(temp,str);
             strlwr(temp);
             strcpy(bef,before);
             strlwr(bef);
             if(strstr(temp,bef)!=NULL){
                 index = FindIndex(before,str);
                 while(Q->front !=NULL){
                    ptq q = DeQueue(Q);
                    push(Undo,q->token,1,index);
                    shiftRight(str,index,strlen(q->token)+1);
                    int temp1 = index;
                    for(int i = 0; i<strlen(q->token); i++){
                        str[temp1] = q->token[i];
                        temp1++;
                    }
                 str[temp1] =' ';
                 index += (strlen(q->token)+1);
                 free(q);
                 }
             }
             else
                printf("%s word not found, try again..\n",before);
             break;
    case 3 :    while(Q->front != NULL){
                ptq temp = DeQueue(Q);
                index = v+1;
                push(Undo,temp->token,1,index);
                strcat(str," ");
                strcat(str,temp->token);
                v += (strlen(temp->token)+1);
                free(temp);
             }
             break;
    default : printf("Invalid option, try again..\n");
    }
    printf("The modified text :\n%s\n",str);

}

//function to find index for a substring in string
int FindIndex(char word[LINE],char str[SIZE]){
    int index = 0;
    char temp[SIZE],w[LINE];
    strcpy(temp,str);
    strlwr(temp);
    strcpy(w,word);
    strlwr(w);
    char* result = strstr(temp,w); //strstr is function in string.h library that search for substring and return a string that
    index = result - temp;        // beginning from the fist index in substring from the original string
    return index;
}

//function to right shift character in string (to insert thing in specific place in string)
void shiftRight(char str[SIZE], int from, int x){
    while(x > 0){
        int a = strlen(str);
        str[a+1] = '\0';
        for (int i = a; i > from; i--){
            str[i] = str[i-1];
        }
        x--;
   }
}

//function to remove substring from the original string
void removeFromText(char str[SIZE], pts Undo, pts redo){
    int index;
    char s[SIZE];
    char newS[LINE];
    printf("remove word or sentence from this Text..\n");
    printf("%s\n",str);
    printf("Enter what you want to remove from the text..\n");
    getchar();
    scanf("%[^\n]%*c",s);
    char temp[SIZE],w[LINE];
    //change the substring that we want to remove and the original text to lower case
    // to make remove operation not case sensitive
    strcpy(temp,str);
    strlwr(temp);
    strcpy(w,s);
    strlwr(w);
    char* result = strstr(temp,w);
    index = result - temp;
    if(result != NULL){
        int x = strlen(s);
        int j = 0;
        for(int i = index; i < x+index; i++){
            newS[j] = str[i];
            j++;
        }
        if(index != 0){
            if(str[index-1]=='\n' && (str[x+index]=='\n' || str[x+index]== '\0')){
                //to check if the substring that will remove is a full line - remove substring and '\n'
                shiftLeft(str,index-1,1);
                index--;
                shiftRight(newS,0,1);
                newS[0]='\n';
                j++;
            }
        }
        else {
            if((str[x+index]=='\n' || str[x+index]== '\0')){
                //to check if the substring that will remove is a full line - remove substring and '\n' [in case that line is first line]
                shiftLeft(str,index,1);
                newS[j]='\n';
                j++;
            }
        }
        newS[j] = '\0';
        push(Undo,newS,0,index);
        shiftLeft(str,index,x);
        clearStackContent(redo);
    }
    else
        printf("The word %s doesn't exist..\n",s);
    printf("The modified text :\n%s\n",str);

}

//function to left shift character in string (to remove thing in specific place in string)
void shiftLeft(char str[SIZE], int from, int x){
    while(x > 0){
        int i;
        for (i = from; i <strlen(str); i++){
            str[i] = str[i+1];
        }
        str[i] ='\0';
        x--;
   }
}

//undo function
void undo_op(char str[SIZE],pts Undo, pts Redo){
    if(Undo->next != NULL){
        pts temp = pop(Undo);
        push(Redo,temp->token,temp->operation,temp->index);
        if(temp->operation == 0){                   //operation = 0, mean that operation is remove
            int length = strlen(temp->token);
            shiftRight(str,temp->index,length);
            int j = temp->index;
            for(int i = 0; i < strlen(temp->token); i++){
                str[j] = temp->token[i];
                j++;
            }
        }
        else if(temp->operation == 1){              //operation = 1, mean that operation is insert
            int length = strlen(temp->token)+1;
            shiftLeft(str,temp->index,length);
        }
        printf("The modified text :\n%s\n",str);
        free(temp);
    }
    else
        printf("sorry, there is no operation in UNDO Stack..\n");
}

//redo function
void redo_op(char str[SIZE],pts Undo, pts Redo){
    if(Redo->next != NULL){
        pts temp = pop(Redo);
        push(Undo,temp->token,temp->operation,temp->index);
        if(temp->operation == 1){                   //operation = 1, mean that operation is insert
            int length = strlen(temp->token);
            shiftRight(str,temp->index,length+1);
            int j = temp->index;
            for(int i = 0; i < strlen(temp->token); i++){
                str[j] = temp->token[i];
                j++;
            }
            str[j] = ' ';

        }
        else if(temp->operation == 0){              //operation = 0, mean that operation is remove
            int length = strlen(temp->token);
            shiftLeft(str,temp->index,length);
        }
        printf("The modified text :\n%s\n",str);
        free(temp);
    }
    else
        printf("sorry, there is no operation in REDO Stack..\n");
}

//print stacks without any change on its content
void print_undo_redo(pts undo, pts redo){
    //temp stack to save data temporarily
    pts tempStack = createStack();
    printf("The UNDO Stack :\n");
    if(undo == NULL)
        printf("The UNDO Stack not found..\n");
    else{
        printf("%-20s %-20s %-20s\n","Token","Operation","Index");
        while(undo->next != NULL){
            pts temp = pop(undo);
            char* z;
            if(temp->operation == 1)
                z = "Insert";
            else if(temp->operation == 0)
                z = "remove";
            printf("%-20s %-20s %-20d\n",temp->token,z,temp->index);
            push(tempStack,temp->token,temp->operation,temp->index);
            free(temp);
        }
        //return data from temp stack to original stack
        while(tempStack->next != NULL){
            pts temp = pop(tempStack);
            push(undo,temp->token,temp->operation,temp->index);
            free(temp);
        }
    }
    printf("\n");
    printf("The REDO Stack :\n");
    if(redo == NULL)
        printf("The REDO Stack is not found..\n");
    else{
        printf("%-20s %-20s %-20s\n","Token","Operation","Index");
        while(redo->next != NULL){
            pts temp = pop(redo);
            char* z;
            if(temp->operation == 1)
                z = "Insert";
            else if(temp->operation == 0)
                z = "remove";
            printf("%-20s %-20s %-20d\n",temp->token,z,temp->index);
            push(tempStack,temp->token,temp->operation,temp->index);
            free(temp);
        }
        //return data from temp stack to original stack
        while(tempStack->next != NULL){
            pts temp = pop(tempStack);
            push(redo,temp->token,temp->operation,temp->index);
            free(temp);
        }
    }
    printf("\n");
    free(tempStack);
}

//function to save the modified text to the output file
void saveToFile(char str[SIZE], pts undo, pts redo){
    FILE* output = fopen("output.txt","w");
    //check if the file exist or not
    if(output == NULL)
        printf("The file can't be open..\n");
    else{
        fprintf(output,"%s",str);
        //clear content of the stacks
        clearStackContent(undo);
        clearStackContent(redo);
        printf("Updated text Saved to the output file successfully..\n");
        fclose(output);
    }
}

//function to clear the content of stacks
void clearStackContent(pts stack){
    //check if the stack exist or not
    if(stack == NULL){
        printf("The stack not exist to be clear..\n");
        return;
    }
    //if the stack exist clear its content
    else{
        while(stack->next != NULL){
            pts temp = pop(stack);
            free(temp);
        }
    }
}
