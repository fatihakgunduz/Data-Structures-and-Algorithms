#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct linkedList{
	
	char term[20];
	int txt;
	int dataset;
	int count;
	linkedList *next;
	
}linkedList;

int isThere(linkedList *control,char *word,int dataset){

	while (control->next!=NULL){

		if (strcmp(control->term,word)==0){
			
			if(control->dataset==dataset)
			control->count++;
			
			return 1;
		}

	control=control->next;

	}

	return 0;	

}

void printList(linkedList *list){
	
	while (list->next!=NULL){

		printf("%s %d\n",list->term,list->count);
		list=list->next;
	}
	printf("\n");
}

linkedList* addNode(FILE *fp,linkedList *iter,linkedList *control,int txt,int dataset){
	
	char c;
	char temp[20];
	
	if(fp!=NULL){
	
		while(!feof(fp)){

			fscanf(fp,"%s",temp);
			
			if(!isThere(control,temp,dataset)){
			
				strcpy(iter->term,temp);
				iter->txt=txt;
				iter->dataset=dataset;
				iter->count=1;
				iter->next=(linkedList *) malloc(sizeof(linkedList));
				iter=iter->next;
				iter->next=NULL;
				
			}

		}
		fclose(fp);

		}

	else
	printf("could not open file\n\n" );

	return iter;
	
	}

void printFirstOrder(linkedList *list){

	linkedList *print1=(linkedList *) malloc(sizeof(linkedList));
	print1=list;
	
	printf("first order term co-occurrence : ");

	while(print1->next!=NULL){
		
		list=list->next;
		
		while(list->next!=NULL){
			
			if(list->dataset==print1->dataset)
			printf("{%s,%s} ",print1->term,list->term);
			
		list=list->next;
		
		}
		print1=print1->next;
		list=print1;
	}
	printf("\n\n");
}

void printSecondOrder(linkedList *list){
	
	linkedList *print2=(linkedList *) malloc(sizeof(linkedList));
	print2=list;
	
	printf("second order term co-occurrence : ");

	while(print2->next!=NULL){
		
		list=list->next;
		
		while(list->next!=NULL){
			
			if(list->dataset!=print2->dataset&&list->dataset-print2->dataset==1)
			printf("{%s,%s} ",print2->term,list->term);
			
		list=list->next;
		
		}
		print2=print2->next;
		list=print2;
	}
	printf("\n\n");
}

void printThirdOrder(linkedList *list){
	
	linkedList *print3=(linkedList *) malloc(sizeof(linkedList));
	print3=list;
	
	printf("third order term co-occurrence : ");

	while(print3->next!=NULL){
		
		list=list->next;
		
		while(list->next!=NULL){
			
			if(list->dataset!=print3->dataset&&list->dataset-print3->dataset==2)
			printf("{%s,%s} ",print3->term,list->term);
			
		list=list->next;
		
		}
		print3=print3->next;
		list=print3;
	}
	printf("\n\n");
}

	
int main(){
	
	setlocale(LC_ALL,"TURKISH");
	
	linkedList *list=(linkedList *) malloc(sizeof(linkedList));
	list->next=NULL;
	
	linkedList *iter=(linkedList *) malloc(sizeof(linkedList));
	iter=list;
	
	linkedList *control=(linkedList *) malloc(sizeof(linkedList));
	control=list;
	
	list->count=0;
	list->txt=0;
		
	FILE *file11=fopen("dataset/econ/1.txt","r");
	iter=addNode(file11,iter,control,1,1);
	
	FILE *file12=fopen("dataset/econ/2.txt","r");
	iter=addNode(file12,iter,control,2,1);
	
	FILE *file13=fopen("dataset/econ/3.txt","r");
	iter=addNode(file13,iter,control,3,1);
	
	FILE *file14=fopen("dataset/econ/4.txt","r");
	iter=addNode(file14,iter,control,4,1);
	
	FILE *file21=fopen("dataset/health/1.txt","r");
	iter=addNode(file21,iter,control,1,2);
	
	FILE *file22=fopen("dataset/health/2.txt","r");
	iter=addNode(file22,iter,control,2,2);
	
	FILE *file23=fopen("dataset/health/3.txt","r");
	iter=addNode(file23,iter,control,3,2);
	
	FILE *file24=fopen("dataset/health/4.txt","r");
	iter=addNode(file24,iter,control,4,2);
	
	FILE *file25=fopen("dataset/health/5.txt","r");
	iter=addNode(file25,iter,control,5,2);
	
	FILE *file31=fopen("dataset/magazin/1.txt","r");
	iter=addNode(file31,iter,control,1,3);
	
	FILE *file32=fopen("dataset/magazin/2.txt","r");
	iter=addNode(file32,iter,control,2,3);
	
	FILE *file33=fopen("dataset/magazin/3.txt","r");
	iter=addNode(file33,iter,control,3,3);
	
	FILE *file34=fopen("dataset/magazin/4.txt","r");
	iter=addNode(file34,iter,control,4,3);

	printList(list);	
	printFirstOrder(list);
	printSecondOrder(list);
	printThirdOrder(list);
	
	printf("\nend of code");
}
