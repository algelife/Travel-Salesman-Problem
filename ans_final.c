#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define printDegree
#define printConnect 
#define printFile
//#define printDist
//#define testGetRoot

typedef struct s_node{
	int id, x, y;
	struct s_node *pre;
	struct s_node *next;
} node;

typedef struct s_dist{
	int idl, idr;
	double dist;
} dist;

int cnm(int n, int m){return m==0?1:cnm(n-1,m-1)*n/m;}
double distance(node a, node b){return sqrt(pow(a.x-b.x, 2)+pow(a.y-b.y, 2));}
node getRoot(node *nPtr, dist *distSet, node *nodeSet);
node getLeaf(node *nptr, dist *distSet, node *nodeSet);
//void connectNode(node *a, node *b, int mode);//Note: 順序很重要  1.右連接 2.左連接 3.右雙向連接 4.左雙向連接 
void connectNode(node *a, node *b);
void inverseAdd(node *a);

void bubbleSort_dist(dist *d, int n){
	int i, j;
	dist temp;
	for(i=0;i<n;i++){
		for(j=0;j<n-1;j++){
			if(d[i].dist<d[j].dist){
				temp=d[i];
				d[i]=d[j];
				d[j]=temp;
			}
		}
	}
}


int main(int argc, char *argv[]){
	/*define variables*/
	int r1, r2, root;
	int i, j;
	int num_city, city_i=0, num_dist, dist_i=0, num_edge=0;
	int *degree;
	double total_dist=0;
	node *nodeSet, *nPtr;
	dist *distSet;
	FILE *fptr;
	fptr = fopen(argv[1], "r");
	if(!fptr){
		printf("ERROR!!\n");
		exit(1);
	}
	
	/*read file*/
	fscanf(fptr, "%d", &num_city);
	nodeSet = (node *)malloc(sizeof(node)*num_city);
	degree = (int *)malloc(sizeof(int)*num_city);
	while(fscanf(fptr, "%d %d %d", &(nodeSet[city_i].id), &(nodeSet[city_i].x), &(nodeSet[city_i].y))!=EOF){
		degree[city_i]=0; 
		nodeSet[city_i].id--;
		nodeSet[city_i].pre=NULL;
		nodeSet[city_i++].next=NULL;
	}
	fclose(fptr);
	
	#ifdef printFile
	printf("Content of file\n-\n");
	for(i=0;i<num_city;i++){
		printf("%c %d %d %d %d\n", nodeSet[i].id+65, nodeSet[i].x, nodeSet[i].y, nodeSet[i].pre, nodeSet[i].next);
	}
	printf("\n");
	#endif
	
	
	/*establish the dist*/
	dist_i=0;
	num_dist=cnm(num_city, 2);
	distSet = (dist *)malloc(sizeof(dist)*num_dist);
	for(i=0;i<num_city;i++){
		for(j=0;j<i;j++){
			distSet[dist_i].idl=j;
			distSet[dist_i].idr=i;
			distSet[dist_i++].dist=distance(nodeSet[i], nodeSet[j]);
		}
	}
	bubbleSort_dist(distSet, num_dist);
	
	#ifdef printDist
	printf("Distance Map:\n-\n");
	for(i=0;i<num_dist;i++){
		printf("%c %c %lf\n", distSet[i].idl+65, distSet[i].idr+65, distSet[i].dist);
	}
	printf("\n");
	#endif
	
	#ifdef testGetRoot
	nodeSet[distSet[0].idl].next=&(nodeSet[distSet[0].idr]);
	printf("the root of %c : %c\n\n", distSet[0].idl+65, getRoot(&(nodeSet[distSet[0].idl]), distSet, nodeSet).id+65);
	#endif
	
	#ifdef printConnect
	printf("Connect Process:\n-\n");
	#endif
	for(i=0;(num_edge<num_city) && (i<num_dist);i++){
		r1=getRoot(&(nodeSet[distSet[i].idl]), distSet, nodeSet).id;
		r2=getRoot(&(nodeSet[distSet[i].idr]), distSet, nodeSet).id;
		//printf("r1: %c r2: %c\n", r1+65, r2+65);
		#ifdef printDegree
		for(j=0;j<num_city;j++) printf("%d ", degree[j]);printf("\n");
		#endif
		
		if(r1!=r2 && degree[distSet[i].idl]<2 && degree[distSet[i].idr]<2){ //進入前必須要先小於2 才能滿足小於等於2 
			
			/*
			nodeSet[distSet[i].idl].next=&(nodeSet[distSet[i].idr]);
			nodeSet[distSet[i].idr].pre=&(nodeSet[distSet[i].idl]);
			*/
			
			/*
			if(nodeSet[distSet[i].idr].pre!=NULL){
				if(nodeSet[distSet[i].idl].pre!=NULL) inverseAdd(&nodeSet[distSet[i].idl]);
				connectNode(&nodeSet[distSet[i].idr], &nodeSet[distSet[i].idl]);
			}
			else if(nodeSet[distSet[i].idl].pre!=NULL){
				if(nodeSet[distSet[i].idr].pre!=NULL) inverseAdd(&nodeSet[distSet[i].idr]);
				connectNode(&nodeSet[distSet[i].idl], &nodeSet[distSet[i].idr]);
			}else connectNode(&nodeSet[distSet[i].idl], &nodeSet[distSet[i].idr]);
			*/
			
			/*
			if(nodeSet[distSet[i].idr].pre!=NULL){
				printf("條件一\n"); 
				if(nodeSet[distSet[i].idl].pre!=NULL) inverseAdd(&nodeSet[distSet[i].idl]);
				connectNode(&nodeSet[distSet[i].idr], &nodeSet[distSet[i].idl]);
			}else{
				printf("條件二\n");
				if(nodeSet[distSet[i].idl].next!=NULL){
					root=getRoot()
					inverseAdd(&nodeSet[distSet[i].idr]);
				}
				connectNode(&nodeSet[distSet[i].idl], &nodeSet[distSet[i].idr]);
			}*/
			
			
			if(nodeSet[distSet[i].idr].pre==NULL){
				//root = getRoot(&nodeSet[distSet[i].idl], distSet, nodeSet).id;
				root = nodeSet[distSet[i].idl].id;
				#ifdef printConnect
				printf("條件一\n");
				printf("root=%c\n", root+65);
				#endif
				inverseAdd(&nodeSet[root]);
				connectNode(&nodeSet[distSet[i].idl], &nodeSet[distSet[i].idr]);
			}else{
				root = getLeaf(&nodeSet[distSet[i].idl], distSet, nodeSet).id;
				#ifdef printConnect
				printf("條件二\n");
				printf("leaf=%c\n", root+65);
				#endif
				inverseAdd(&nodeSet[root]);
				connectNode(&nodeSet[distSet[i].idr], &nodeSet[distSet[i].idl]);
			}
			
			num_edge++;
			degree[distSet[i].idl]++;
			degree[distSet[i].idr]++;
			total_dist+=distSet[i].dist;
		}
		//system("pause");
	}

	/*Summary: 找尋與尋訪每個Node*/
	nPtr=&(nodeSet[r2]);
	while(nPtr->pre!=NULL){
		//printf("%c->", (nPtr->id)+65);
		nPtr=nPtr->pre;
	}
	//printf("%c->NULL", (nPtr->id)+65);
	//printf("r2=%c nPtr=%c\n", r2+65, nPtr->id+65);
	
	#ifdef printConnect
	printf("final : root: %c leaf: %c\n", r1+65, r2+65);
	#endif
	r1=getRoot(&(nodeSet[0]), distSet, nodeSet).id;
	r2=getLeaf(&(nodeSet[0]), distSet, nodeSet).id;
	if(nodeSet[r1].next==NULL){
		connectNode(&nodeSet[r1], &nodeSet[r2]);
	}else{
		connectNode(&nodeSet[r2], &nodeSet[r1]);
	}

	total_dist+=distance(nodeSet[r1], nodeSet[r2]);
	printf("total distance: %lf\n", total_dist);
	
	
	
	#ifdef printFile
	printf("Content of file\n-\n");
	for(i=0;i<num_city;i++){
		printf("%c %d %d %d %d %d\n", nodeSet[i].id+65, nodeSet[i].x, nodeSet[i].y, &nodeSet[i], nodeSet[i].pre, nodeSet[i].next);
	}
	printf("\n");
	#endif
	
	
	/*生成 output file*/ 
	fptr=fopen(argv[2], "w");
	if(!fptr){
		printf("ERROR\n");
		exit(1);
	}
	
	fprintf(fptr, "%lf\n", total_dist);
	nPtr=&nodeSet[0];
	for(i=0;i<num_city;i++){
		fprintf(fptr, "%d\n", nPtr->id+1);
		printf("%c->", nPtr->id+65);
		nPtr=nPtr->next;
	}
	printf("%c", nPtr->id+65);
	
	
	
	/*回收資源垃圾*/
	free(nodeSet);
	free(distSet);
	free(degree);
	fclose(fptr);
	
	return 0;
}

node getRoot(node *nPtr, dist *distSet, node *nodeSet){
	while(nPtr->next!=NULL){
		nPtr=nPtr->next;
		//printf("*");
	}
	return *nPtr;
}

node getLeaf(node *nPtr, dist *distSet, node *nodeSet){
	while(nPtr->pre!=NULL){
		nPtr=nPtr->pre;
		//printf("*");
	}
	return *nPtr;
}


/*
void inverseAdd(node *a){ //將a到b的方向 相反過來 
	node *node_b=NULL, *node_f;
	node_f=a->pre;
	while(a->pre!=NULL){
		printf("inverseAdd: %c->", a->id+65);
		a->next=node_f;
		a->pre=node_b;
		if(node_b!=NULL) node_b->next=a;
		node_b=a;
		a=node_f->pre;
		node_f->pre=node_b;
		system("pause");
	}
}*/

void inverseAdd(node *cur){ //將a到b的方向 相反過來 
	node *back=NULL, *forward=cur->next;
	while(forward!=NULL){
		#ifdef printConnect
		printf("%c<-", cur->id+65);
		#endif
		cur->next=back;
		cur->pre=forward;
		
		back=cur;
		cur=forward;
		forward=forward->next;
	}
	cur->next=back;
	cur->pre=forward;
	//printf("\n");
}



void connectNode(node *a, node *b){
	#ifdef printConnect
	printf("%c->%c\n", a->id+65, b->id+65);
	#endif
	a->next=b;
	b->pre=a;
}

/*
void connectNode(node *a, node *b, int mode){//Note: 順序很重要  1.右連接 2.左連接 3.右雙向連接 4.左雙向連接 
	switch(mode){
		case 1:
			a->next=b;
			break;
		case 2:
			b->pre=a;
			break;
		case 3:
			a->next=b;
			b->pre=a;
			break;
		case 4:
			b->next=a;
			a->pre=b;
		
	}
}*/

/*
void checkConnect(node *nodeSet){
	node *nPtr=nodeSet->next, *nPtr_pre=nodeSet; //指標指在nodeSet開頭
	
	
}*/











