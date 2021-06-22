////////////////////
/// Egemen Engin ///
////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
typedef struct node{
    char name[11];
    struct node **children;
    int numberOfChildren;
    int depthChecker;
    int passed;

}node;
node * createNode(char * inp);
char * removeSpaces(char * inp);
void addChild(node **parentN, node **child);
node * buildTree(node **root, char * inp, int number, int* index, int* isThereExtraOpen,int*isThereExtraClose);
void print(node *root);
node * find(node **root,char* index, int number);
node * link(node **root,char* from,char* to);
void clearTree(node **root);
static int extraStarting=0;
static int extraClosing=0;
int main(int argc, char * argv[])
{


    char * input = (char *)malloc(512*sizeof(char));
   


    struct node *root = NULL;
    fgets(input,512,stdin);
    
    input = removeSpaces(input);
    
    int index = 0;
    int checkExtraStarting=0;
    int checkExtraFinish=0;
    root=buildTree(&root,input,0, &index, &checkExtraStarting,&checkExtraFinish);
    //print(root);
    while(fgets(input,512,stdin)){
        char * temp = strtok(input," ");

        if(strcmp("FIND",temp)==0){

            temp = strtok(NULL," ");
            node * tempNode=find(&root,temp,0);
            if(tempNode!=NULL){
                printf("%s\n",tempNode->name);
            }



        }
        else if(strcmp("LINK",temp)==0){

            temp = strtok(NULL," ");
            char *temp2=strtok(NULL," ");
            link(&root,temp,temp2);


        }
        else{
            printf("INVALID COMMAND\n");
        }




    }
    clearTree(&root);
    free(input);
}

char * removeSpaces(char * inp){
    int i,x;
    int checker=0;

    for(i=x=0;inp[i];++i){
        if((inp[i]=='\"') && checker==0){

            checker=1;
        }
        else if((inp[i]=='\"') && checker==1){
            checker=0;
        }
        if(checker == 1){

            inp[x++] = inp[i];
        }
        if(checker!=1 && (inp[i] != ' ' && inp[i] != '\n')){
            inp[x++] = inp[i];
        }
    }
    inp[x]='\0';

    return inp;
}

node *buildTree(node **root, char * inp, int number, int *index, int *isThereExtraStarting, int *isThereExtraClose){
    char *temp =NULL;
    if((*root)==NULL && inp[0]!='['){
        *root=createNode(inp);
        (*root)->depthChecker=0;

    }

    else{
        if(number == 0){
            temp = strtok(inp,"\"");
            (*index)++;
            number++;
        }
        else{
            temp = strtok(NULL,"\"");
            (*index)++;
            number++;
        }
        int starting=0;
        int ending=0;

        while(temp!=NULL){
            if((*index)%2!=0){
                if(temp[0]==']'){
                    ending=1;
                    for(int i=1;i<strlen(temp);i++){
                        if(temp[i]==']'){


                            (*isThereExtraClose)=(*isThereExtraClose)+1;

                        }
                    }
                    if(temp[strlen(temp)-1]=='['){


                        (*isThereExtraStarting)=1;

                    }


                    return *root;
                }
                else if(temp[strlen(temp)-1]=='[' || *isThereExtraStarting==1){
                    starting=1;
                    *isThereExtraStarting=0;
                }

            }
            else{
                if((*root)==NULL && starting==1){
                    starting =0;
                    *root=createNode(temp);
                    (*root)->depthChecker=0;

                }
                else if(starting==1){
                    node * temp2=createNode(temp);
                    addChild(root,&temp2);
                    temp2 = buildTree(&temp2,inp,number, index, isThereExtraStarting, isThereExtraClose);
                    starting=0;

                    if((*isThereExtraStarting)==1){
                        *isThereExtraStarting=0;
                        starting=1;
                    }
                    if(*isThereExtraClose>0){
                        (*isThereExtraClose)=(*isThereExtraClose)-1;
                        if(starting == 1){
                            *isThereExtraStarting=1;
                        }
                        return *root;
                    }
                }

                else{

                    node * temp2=createNode(temp);
                    addChild(root,&temp2);


                }
            }
            if(number == 0){
                temp = strtok(inp,"\"");
                (*index)++;
                number++;
            }
            else{
                temp = strtok(NULL,"\"");
                (*index)++;
                number++;
            }
        }
    }
    return *root;
}

node * createNode(char * inp){
    node * temp = (node *)malloc(sizeof(node));
    strcpy(temp->name,inp);
    temp->children=NULL;
    temp->numberOfChildren=0;
    temp->depthChecker=-1;
    temp->passed=0;
    return temp;
}
void addChild( node **parentN, node **child){
    if((*parentN)==NULL){
        return;
    }
    if ((*parentN)->children == NULL) {
        (*parentN)->children = (node**)malloc(sizeof(node*));
        (*parentN)->children[(*parentN)->numberOfChildren]=NULL;
        (*parentN)->numberOfChildren=(*parentN)->numberOfChildren+1;
    }
    else{
        (*parentN)->children = (node**)realloc((*parentN)->children,sizeof(node*)*((*parentN)->numberOfChildren+1));
        (*parentN)->children[(*parentN)->numberOfChildren]=NULL;
        (*parentN)->numberOfChildren=(*parentN)->numberOfChildren+1;
    }
    //node *temp = createNode(c);
    if( (*child)->depthChecker==-1){
        (*child)->depthChecker=(*parentN)->depthChecker +1;
    }
    (*parentN)->children[(*parentN)->numberOfChildren-1]=*child;
}
node * find(node **root,char* index, int number){
    char* temp = strtok(index,".");;

    node * findedOne=*root;
    if(atoi(temp)==0){
        return *root;
    }
    while(temp!=NULL){
        if(atoi(temp)>findedOne->numberOfChildren){
            printf("There is no child.\n");
            return NULL;
        }
        else{
            findedOne= findedOne->children[atoi(temp)-1];
            temp = strtok(NULL,".");
        }


    }
    return findedOne;

}
node * link(node **root,char* from,char* to){
    node * fromNode=find(root,from,0);
    node * toNode=find(root,to,0);
    addChild(&fromNode,&toNode);
    printf("Linking %s -> %s\n",fromNode->name,fromNode->children[fromNode->numberOfChildren-1]->name);


}
void print(node *root){
    if(root==NULL){
        return;
    }
    printf("%s %d\n",root->name,root->numberOfChildren);
    for(int i =0; i<root->numberOfChildren;i++){
        printf("child of %s\n",root->name);
        for(int k=0 ; k< root->numberOfChildren;k++){
            printf("%s\n", root->children[k]->name);
        }
        print(root->children[i]);
    }
}
void clearTree(node **root){
    (*root)->passed=1;
    if((*root)->numberOfChildren > 0){

        for(int i = 0; i<(*root)->numberOfChildren;i++ ){

            if((*root)->depthChecker < ((*root)->children[i])->depthChecker && (*root)->children[i]->passed!=1){

                clearTree(&((*root)->children[i]));

            }
        }
        (*root)->numberOfChildren=0;
    }

    free(*root);

}
