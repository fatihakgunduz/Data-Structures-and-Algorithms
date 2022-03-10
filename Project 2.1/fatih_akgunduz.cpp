#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	
	int data;
	int sequence;
	node *left;
	node *right;

}node; 

node* newNode(int data){
	
	node *root =(node *)malloc(sizeof(node)); 
    root->data = data;
	root->sequence=0; 
    root->left = root->right = NULL; 
    
	return root;
}

node* insert(node* node,int data){

	if (node == NULL) 
		return newNode(data);

	if (data < node->data) 
        node->left  = insert(node->left, data);

	if (data > node->data)
        node->right = insert(node->right, data);

	return node; 
}

node* buildTree(int *arr,int size){
	
	node *root=NULL;
	
	for(int i=0;i<size;i++){
		
		if(i==0)
			root=insert(root,arr[i]);

		insert(root,arr[i]);
	
	}
	
	return root;
	
}


void inorder(node* root,int last){
   
    if(root == NULL) 
		return;

	inorder(root->left,last);
	
	if(root->data!=last)
    	printf("%d -> ", root->data);
	
	else
		printf("%d",root->data);
		
	inorder(root->right,last);
	
	
}

node* rORl(node* root,int numOfSeq){
	
	//a random number for right and left
	int n = rand() % 2;
	
	if(root->sequence==numOfSeq+1){
		
		//if n is 0 it is left
		if(n==0){
			if(root->left!=NULL){
				root=root->left;

				return rORl(root,numOfSeq);
			}
		
		}
		//if n is 1 it is right
		if(n==1){
			if(root->right!=NULL){

				root=root->right;
				
				return rORl(root,numOfSeq);
			
			}
			
		}
		if (root->left==NULL&&root->right==NULL){

			return root;
		}
	}
	if(root->sequence==0){

		return root;
	}


}

void findSequence(node* root,int size,int numOfSeq,node *temp){
	

		if(root->sequence==numOfSeq){
			
			printf("%d ",root->data);
			
			root->sequence++;
			root=temp;
			
			return findSequence(rORl(root,numOfSeq),size,numOfSeq,temp);
		
		}	
	

		if(root->left==NULL&&root->right==NULL&&root->sequence==numOfSeq){

			printf("%d ",root->data);
	
			root=temp;
	
			return findSequence(rORl(root,numOfSeq),size,numOfSeq,temp);
	
		}
	
}

int main(){
	
	printf("How many numbers you want to enter : ");
	
	int size;
	
	scanf("%d",&size);
	
	printf("\nEnter your sequence : ");
	
	int b[size];
	
	//it takes the sequence from the user
	for(int i=0;i<size;i++){
		scanf("%d",&b[i]);
	}
	
	node *root=buildTree(b,size);
	
	node *temp=(node*)malloc(sizeof(node));
	temp=root;
	
	printf("\n\n");

	inorder(root,b[size-1]);
	
	printf("\n\n");
	
	findSequence(root,size,0,temp);

	printf("\n\n");

	printf("\n\nend of code");

}
