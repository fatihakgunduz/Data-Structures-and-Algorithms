       #include "binomial.h"
       #include "fatal.h"
	   #include <math.h>
        
    typedef struct read{

       	int pid,ei,tarr,wt;
        	
		}read;
		
		
	typedef struct BinNode *Position;
        
	struct BinNode
        {
		    double priority;
           	int pid,ei,tarr,wt;
            Position LeftChild;
            Position NextSibling;
        };
        
    typedef struct BinNode node;

        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };

    BinQueue Initialize( void )
        {
            BinQueue H;
            int i;

            H = (BinQueue)malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ ){
                H->TheTrees[ i ] = NULL;
            }
            
            return H;
        }

    static void DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

    void Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

    BinQueue MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }

        /* Not optimized for O(1) amortized performance */
        
    void Insert( double priority,int pid,int ei,int tarr, BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = (BinTree)malloc( sizeof( struct BinNode ) );
            
			if( NewNode == NULL )
                FatalError( "Out of space!!!" );
                
            NewNode->LeftChild = NewNode->NextSibling = NULL;
            NewNode->priority = priority;
            NewNode->pid = pid;
			NewNode->ei = ei;
			NewNode->tarr = tarr;
			NewNode->wt = 0;
			
			for (i=0; i < MaxTrees; i++){
				
				if(H->TheTrees[i]&&H->TheTrees[i]->priority==NewNode->priority)
					NewNode->priority+=0.005;
				
			}
			
            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[0] = NewNode;
            
			Merge( H, OneItem);
            
        }

    double DeleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;   // The tree with the minimum item 
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            double MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=-Infinity;
				
			//  for (i=0; i < max_act_const; i++) {
			//    MinItem.from[i].s=-1;
			//    MinItem->mn.from[i].a=m.from[i].a;
			//  }
                return MinItem;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->priority < MinItem )
                {
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->priority;
					
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }

     node* FindMin( BinQueue H )
        {	
        	
        	node *Min=(node*)malloc( sizeof( node ) );
            int i;

            if( IsEmpty( H ) )
            {	
                Error( "Empty binomial queue" );
                exit(1);
            }
            
			Min->priority= Infinity;
			
            for( i = 0; i < MaxTrees; i++ )
            {		
                	
				if( H->TheTrees[ i ] && H->TheTrees[ i ]->priority < Min->priority ) 
                    Min = H->TheTrees[ i ];
                    
            }
            
			return Min;
        }

    int IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

    int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

        /* Return the result of merging equal-sized T1 and T2 */
    BinTree CombineTrees( BinTree T1, BinTree T2 )
        {	
        	
            if( T1->priority > T2->priority )
                return CombineTrees( T2, T1 );
            
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }

        /* Merge two binomial queues */
        /* Not optimized for early termination */
        /* H1 contains merged result */

	void Merge( BinQueue H1, BinQueue H2 )
        {	
            BinTree T1, T2, Carry = NULL;
            int i, j;
            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
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
		

	double newPriority(int ei,int tarr,int emax)
		{
		
			double nPriority;
			
			nPriority=(double)(1/(exp(-pow(((2.0*ei)/(3.0*emax)),3))))*ei;
			
			return nPriority;
		
		}
	
	void searchDatas(read* datas,int size,int passedTime,int time,BinQueue H){

		int datasLenght=0;
		
		for(datasLenght=0 ; datasLenght < size ; datasLenght++){
			
			if(datas[datasLenght].tarr>-1){
				
				int comparison=datas[datasLenght].tarr-time;
				
				if(comparison==0){
					Insert(datas[datasLenght].ei,datas[datasLenght].pid,datas[datasLenght].ei,passedTime+time,H);
					datas[datasLenght].tarr=-1;
				}
				
				else if(comparison<0){
					Insert(datas[datasLenght].ei,datas[datasLenght].pid,datas[datasLenght].ei,passedTime+datas[datasLenght].tarr,H);
					datas[datasLenght].tarr=-1;
				}
				
				else{
					//printf("\ndatas %d\n",datas[datasLenght].tarr);
					datas[datasLenght].tarr=comparison;
					//printf("\ndatas %d\n",datas[datasLenght].tarr);
				}
			}
		}
				
	}
	

	int main(){
		
		int q;
		
		node *temp;
		
		for(q=1;q<11;q++){
			
			FILE *fp=fopen("input.txt","r");
		
			int processes=0;
			
			if(fp==NULL){
				printf("could not open the file!!!");
				exit(1);
			
			}
				
			while (!feof(fp)){
	
				if(fgetc(fp)=='P'){
					processes++;
				}
			}
			
			rewind(fp);
			
			read datas[processes];
			
			int i=0;
			
			int emax=0;
			
			int tmax=0;
			
			while (!feof(fp)){
	
				if(fgetc(fp)=='P'){
	
					fscanf(fp,"%d",&datas[i].pid);
					
					fscanf(fp,"%d",&datas[i].ei);
					
					fscanf(fp,"%d",&datas[i].tarr);
					
					if(datas[i].ei>emax)
						emax=datas[i].ei;
	
					if(datas[i].tarr>tmax)
						tmax=datas[i].tarr;
									
					i++;
				}
			
			}
			
			BinQueue H;
			H=Initialize();
		
			Insert(datas[0].ei, datas[0].pid, datas[0].ei, 0, H);
			datas[0].tarr=-1;
			
			int size=processes;
			int wtNumber=0;
			int time=q;
			int passedTime=0;
			int wts[processes][2];
			double awt=0.0;
		
			while(processes>0){
				
				time=q;
				
				while(time!=0){
					
					temp=FindMin(H);
					
					if(temp->ei>time){	
						temp->ei=temp->ei-time;
						temp->priority=newPriority(temp->ei,temp->tarr,emax);
						temp->wt+=time;
						searchDatas(datas,size,passedTime,time,H);
						passedTime+=time;
						time=0;
										
					}
							
					else if(temp->ei==time){
						temp->wt+=time;
						
						passedTime+=time;
						wts[wtNumber][0]=temp->pid;  wts[wtNumber][1]=passedTime-temp->tarr-temp->wt; 
						printf("P%d waiting time %d\n",wts[wtNumber][0],wts[wtNumber][1]);
						awt=awt+wts[wtNumber][1];
						wtNumber++;
						
						DeleteMin(H);
						processes--;
						searchDatas(datas,size,passedTime-time,time,H);
						time=0;
					}	
					
					else{
						
						
						time=time-temp->ei;
						temp->wt+=time;
						
						passedTime+=temp->ei;
						wts[wtNumber][0]=temp->pid; wts[wtNumber][1]=passedTime-temp->tarr-temp->wt; 
						printf("P%d waiting time %d\n",wts[wtNumber][0],wts[wtNumber][1]);
						awt=awt+wts[wtNumber][1];
						wtNumber++;
						DeleteMin(H);
						processes--;
						searchDatas(datas,size,passedTime-(q-time),time,H);
					}
				}
				
				
			}	
				
			awt=awt/size;
				
			printf("\nAWT for q => %d is %.2lf \n",q,awt);
						
		}

		printf("\n\nend of code");
		
	}		
