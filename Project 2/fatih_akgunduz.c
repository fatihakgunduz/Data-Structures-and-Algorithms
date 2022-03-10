#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#define MaxTrees (12)
#define Capacity (4095)


//// Fatih Akgündüz 150117032

        
typedef struct BinNode{	  
	int priority; 
	char file_name[50];
    struct BinNode *LeftChild;
    struct BinNode *NextSibling;
}*Position,*BinTree,Keyword;

const int Infinity = INT_MAX;

typedef struct Collection{
	int CurrentSize;
    BinTree TheTrees[ MaxTrees ];
}*BinQueue;

struct results{
	char file[40];
	int count;
};


BinQueue Initialize( void ){
    BinQueue H;
    int i;

    H = (BinQueue)malloc( sizeof( struct Collection ) );
    if( H == NULL )
        printf( "Out of space!!!" );
    H->CurrentSize = 0;
    for( i = 0; i < MaxTrees; i++ ){
        H->TheTrees[ i ] = NULL;
    }
            
    return H;
}


static void DestroyTree( BinTree T ){
    
	if( T != NULL ){
        DestroyTree( T->LeftChild );
        DestroyTree( T->NextSibling );
        free( T );
    }
}


void Destroy( BinQueue H ){
    int i;

    for( i = 0; i < MaxTrees; i++ )
        DestroyTree( H->TheTrees[ i ] );
}


BinQueue MakeEmpty( BinQueue H ){
    int i;

    Destroy( H );
    for( i = 0; i < MaxTrees; i++ )
        H->TheTrees[ i ] = NULL;
    H->CurrentSize = 0;

    return H;
}


BinTree CombineTrees( BinTree T1, BinTree T2 ){
        	
    if( T1->priority < T2->priority )
        return CombineTrees( T2, T1 );
            
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
}


void Merge( BinQueue H1, BinQueue H2 ){	
    
	BinTree T1, T2, Carry = NULL;
    int i, j;
    if( H1->CurrentSize + H2->CurrentSize > Capacity )
        printf( "Merge would exceed capacity" );

    H1->CurrentSize += H2->CurrentSize;
    for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 ){
        T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

    	switch( !!T1 + 2 * !!T2 + 4 * !!Carry ){
            case 0: /* No trees */
                	break;
            case 1: /* Only H1 */
                    break;
            case 2: /* Only H2 */
                    H1->TheTrees[ i ] = T2;
                    H2->TheTrees[ i ] = NULL;
                    break;
            case 4: /* Only Carry */
                    H1->TheTrees[ i ] = Carry;
                    Carry = NULL;
                    break;
            case 3: /* H1 and H2 */
            		Carry = CombineTrees( T1, T2 );
                    H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                    break;
            case 5: /* H1 and Carry */
                    Carry = CombineTrees( T1, Carry );
                    H1->TheTrees[ i ] = NULL;
                    break;
            case 6: /* H2 and Carry */
                    Carry = CombineTrees( T2, Carry );
                    H2->TheTrees[ i ] = NULL;
                    break;
            case 7: /* All three */
                    H1->TheTrees[ i ] = Carry;
                    Carry = CombineTrees( T1, T2 );
                    H2->TheTrees[ i ] = NULL;
                    break;
            }
    }
            
}

     
void insert(char *filename, int priority, BinQueue H ){
    
	BinTree new_node;
    BinQueue Item;
	int i;

    new_node = (BinTree)malloc( sizeof( struct BinNode ) );
            
	if( new_node == NULL )
        printf( "Out of space!!!" );
                
    new_node->LeftChild = new_node->NextSibling = NULL;
    new_node->priority = priority;
    strcpy(new_node->file_name,filename);

    Item = Initialize( );
    Item->CurrentSize = 1;
    Item->TheTrees[0] = new_node;
            
	Merge( H, Item);
            
}


double DeleteMax( BinQueue H ){
    int i, j;
    int MaxTree;
    BinQueue del_queue;
    Position del_tree, old_root;
    double MaxItem;

    if( H->CurrentSize == 0 ){
        puts( "Empty binomial queue" );
		MaxItem=-Infinity;
				
    	return MaxItem;
	}

    MaxItem = -Infinity;
    for( i = 0; i < MaxTrees; i++ ){
    	
        if( H->TheTrees[ i ] && H->TheTrees[ i ]->priority >= MaxItem ){
            MaxItem = H->TheTrees[ i ]->priority;		
            MaxTree = i;
        }
    }

    del_tree = H->TheTrees[ MaxTree ];
    old_root = del_tree;
    del_tree = del_tree->LeftChild;
    free( old_root );

    del_queue = Initialize( );
    del_queue->CurrentSize = ( 1 << MaxTree ) - 1;
    for( j = MaxTree - 1; j >= 0; j-- ){
        del_queue->TheTrees[ j ] = del_tree;
        del_tree = del_tree->NextSibling;
        del_queue->TheTrees[ j ]->NextSibling = NULL;
    }

    H->TheTrees[ MaxTree ] = NULL;
    H->CurrentSize -= del_queue->CurrentSize + 1;

    Merge( H, del_queue );
    return MaxItem;
}


Keyword* FindMax( BinQueue H ){	
        	
    Keyword *Max=(Keyword*)malloc( sizeof( Keyword ) );
    int i;

    if( H->CurrentSize == 0 ){
        printf( "Empty binomial queue" );
                
		Max->priority=Infinity;
				
		return Max;
    }
            
	Max->priority = -Infinity;
			
    for( i = 0; i < MaxTrees; i++ ){		
                	
		if( H->TheTrees[ i ] && H->TheTrees[ i ]->priority >= Max->priority )
            Max = H->TheTrees[ i ];
		
    }
            
	return Max;
}


int scan(FILE *fp,char *word){
	char str[50]="";
	char ch;
	int count=0;
	while((ch=fgetc(fp))!=EOF){
		
		if(ch=='.'||ch==' '||ch=='\n'||ch=='?'||ch=='!'){
			if(strcmp(str,word)==0)
				count++;
			
			strcpy(str,"");
			
			continue;
		}
	
		else if(ch==','){
			ch=fgetc(fp);
			if(isdigit(ch)){
				fseek(fp, -2, SEEK_CUR);
				ch=fgetc(fp);
				strncat(str, &ch, 1);
			}
			else{
				fseek(fp, -1, SEEK_CUR);
				continue;
			}
				
		}
		
		else{
			strncat(str, &ch, 1);
		}
	}
	
	return count;
	
	
}

void documents(struct results files[],int i){
	int j=0;
	char file_path[40];
	FILE *fp;
	char ch;
	while(j<i){
		printf("\n\n%s (%d) : ",files[j].file,files[j].count);
		sprintf(file_path,"files\\\\%s", files[j].file);
		fp = fopen(file_path,"r");
		while((ch=fgetc(fp))!=EOF){
			printf("%c",ch);
		}
		
		fclose(fp);
		printf("\n");
		j++;
		
	}
	
}

int main(){
	
	
	printf("Enter the keyword you want to search\n");
	char word[50];
	scanf("%s",word);
	
	
	//////READING DIRECTORY
	
    DIR *d;
    struct dirent *dir;
    FILE *fp;
    d = opendir("files");
    int no = 1;
    int count = 0;
    
    BinQueue H;
	H=Initialize();
			
    
	if (d){
    	
        while ((dir = readdir(d))!=NULL){
        	
        	if (!strcmp (dir->d_name, ".") || !strcmp (dir->d_name, ".."))
                continue;
           
           
           	char file_path[40];            
			sprintf(file_path,"files\\\\%s", dir->d_name);
            
            fp = fopen(file_path,"r");

			if(fp==NULL){
				puts("\n!!!Could not open file");
				exit(EXIT_FAILURE);	
			}
			
			
			count=scan(fp,word);
			
			insert(dir->d_name,count,H);
			
			
			no++;
			
			fclose(fp);
				
            
        }
        
        closedir(d);
        
        printf("Enter the number of relevant documents\n");
		int relevant_doc;
		scanf("%d",&relevant_doc);
		
		if (relevant_doc>no){
			printf("\nThere is only %d documents.Please enter again\n",no);
			scanf("%d",&relevant_doc);
		}
	
        
        ////PRINTING RESULTS
        
		printf("\nSearched for \"%s\" \n",word);
        
		puts("\n*** RESULTS ***\n");
        printf("The relevance order is: ");
        int i;
        Keyword *temp;
        struct results files[relevant_doc];
        for(i=0;i<no;i++){
        	
        	temp = FindMax(H);
        	
        	if(i<relevant_doc){
        		strcpy(files[i].file,temp->file_name);
        		files[i].count=temp->priority;
        	}
        		
        	//If the person would want to continue it continues to show results that 
			//until showed results number mode 14 is equal zero
			if( i==relevant_doc || i==14 || i==28 || i==42 ){
        		
        		if(i==relevant_doc)
        			documents(files,i);
        		
				puts("\n\nEnter \"more\" to see more results or enter \"exit\" to quit");
				char pressed[5];
				scanf("%s",pressed);
        		
        		if(strcmp(pressed,"more")==0){
				}
        		else if(strcmp(pressed,"exit")==0){
        			exit(EXIT_SUCCESS);
				}
				else{
					puts("You entered wrong. The program ends");
					exit(EXIT_FAILURE);
				}
			}
        	
        	if(temp->priority==0||temp->priority==Infinity){
        		if(i<=relevant_doc)
					documents(files,i);
        		puts("\nThere is no more/no word like you entered");
        		exit(EXIT_SUCCESS);
			}
        		
        	
        	printf("%s (%d) ,",temp->file_name,temp->priority);
			
			DeleteMax(H);	
		
		}
        
        
        printf("\n\n!!!program ends!!!\n\n");
	
	
	}
    
    else{
    	puts("\n!!!Could not open directory");
		exit(EXIT_FAILURE);	
	}
    
    return 0;
}
	
	

