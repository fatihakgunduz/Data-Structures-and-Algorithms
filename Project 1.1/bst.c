#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Fatih Akgündüz


typedef struct node{
	
	char word[20];
	int frequency;
	struct node *left;
	struct node *right;

}node; 


struct Queue
{
    int front, rear;
    int size;
    struct node* *array;
};


node* newNode(char *word, int frequency){
	
	node *new_node =(node*)malloc(sizeof(node));
    new_node->frequency=frequency;
    strcpy(new_node->word,word);
    new_node->left = NULL;
	new_node->right = NULL; 

	return new_node;
}


node* insert(node* vertice, char *word, int frequency){
	
	
	
	if (vertice == NULL) 
		return newNode(word, frequency);

	if (strcasecmp(word, vertice->word)<0){
        vertice->left  = insert(vertice->left, word, frequency);
		return vertice;
	}
	
	if (strcasecmp(word, vertice->word)>=0){
        vertice->right = insert(vertice->right, word, frequency);
		return vertice;
	}

}


// A utility function to create a new Queue
struct Queue* createQueue(int size)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof( struct Queue ));
 
    queue->front = queue->rear = -1;
    queue->size = size;
 
    queue->array = (struct node**) malloc
                   (queue->size * sizeof( struct node* ));
 
    int i;
    for (i = 0; i < size; ++i)
        queue->array[i] = NULL;
 
    return queue;
}
 
// Standard Queue Functions
int isEmpty(struct Queue* queue)
{
    return queue->front == -1;
}
 
int isFull(struct Queue* queue)
{  return queue->rear == queue->size - 1; }
 
int hasOnlyOneItem(struct Queue* queue)
{  return queue->front == queue->rear;  }
 
void Enqueue(node *root, struct Queue* queue)
{
    if (isFull(queue))
        return;
 
    queue->array[++queue->rear] = root;
 
    if (isEmpty(queue))
        ++queue->front;
}
 
node* Dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return NULL;
 
    node* temp = queue->array[queue->front];
 
    if (hasOnlyOneItem(queue))
        queue->front = queue->rear = -1;
    else
        ++queue->front;
 
    return temp;
}
 
node* getFront(struct Queue* queue)
{  return queue->array[queue->front]; }
 
// A utility function to check if a tree node
// has both left and right children
int hasBothChild(node* temp)
{
    return temp && temp->left && temp->right;
}
 
// Function to insert a new node in complete binary tree
void insertByOrder(node **root, int frequency,char *word, struct Queue* queue)
{
    // Create a new node for given frequency
    node *temp = newNode(word,frequency);
 
    // If the tree is empty, initialize the root with new node.
    if (!*root)
        *root = temp;
 
    else
    {
        // get the front node of the queue.
        node* front = getFront(queue);
 
        // If the left child of this front node doesn’t exist, set the
        // left child as the new node
        if (!front->left)
            front->left = temp;
 
        // If the right child of this front node doesn’t exist, set the
        // right child as the new node
        else if (!front->right)
            front->right = temp;
 
        // If the front node has both the left child and right child,
        // Dequeue() it.
        if (hasBothChild(front))
            Dequeue(queue);
    }
 
    // Enqueue() the new node for later insertions
    Enqueue(temp, queue);
}


void inorder(node* root) 
{ 
     if (root == NULL) 
          return; 
  
     /* first recur on left child */
     inorder(root->left); 
  
     /* then print the data of node */
     printf("%s ", root->word);   
  
     /* now recur on right child */
     inorder(root->right); 
}

int accessTime(node* root, int dephtLevel, int totalAccessTime){
	
	dephtLevel++;
	
	if (root == NULL) {
	   return totalAccessTime; 
	}
       
       
    totalAccessTime = accessTime(root->left,dephtLevel,totalAccessTime);
    totalAccessTime+=(dephtLevel)*root->frequency;
	
    totalAccessTime = accessTime(root->right,dephtLevel,totalAccessTime);
	
	
}



int main(){
	
	
	node *root=(node*)malloc(sizeof(node));
	root=NULL;

	
	int number;
	int frequency;
	char word[20];
	
	FILE *fp = fopen("input.txt","r"); 
	
	if(fp==NULL){
		printf("File could not be opened");
		exit(1);
	}

	fscanf(fp, "%d %s %d" ,&number,word,&frequency);
	
	root = insert(root, word, frequency);
	
	while(!feof(fp)){
		
		fscanf(fp, "%d %s %d" ,&number,word,&frequency);
		insert(root, word, frequency);
		
	}
	
	
	inorder(root);
	
	
	puts("\n");
	
	
	printf("\nTotal Access Time -> %d ",accessTime(root,0,0));
	
	puts("\n");
	
	root=NULL;

    struct Queue* queue = createQueue(number);
	
	rewind(fp);
	
	if(fp==NULL){
		printf("File could not be opened");
		exit(1);
	}

	
	while(!feof(fp)){
		
		fscanf(fp, "%d %s %d" ,&number,word,&frequency);
		insertByOrder(&root, frequency,word, queue);
		
		
	}
	
	fclose(fp);
	
	inorder(root);
	
	puts("\n");
	
	printf("\nTotal Access Time -> %d ",accessTime(root,0,0));
	
	puts("\n");
	
	
	
}
