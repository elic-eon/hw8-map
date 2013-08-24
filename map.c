#include "map.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * This is a very slow linked-list implementation
 * Please modify to tree-based implementation
 */
struct node
{  
	void* key;
	void* object;
	struct node* next;
	struct node* pre;
	int dummyNode;
 };

struct map_t
 {   
	struct node* head;
	size_t keySize, objSize,  size, cap;
	int (*cmp)(const void*, const void*);
	int dynamic;
 };

struct map_t *mapAlloc()
{ 
	return malloc(sizeof(struct map_t));
}  

int mapInit(struct map_t *pThis, size_t keySize, size_t objSize, size_t cap,
		int (*cmp)(const void*, const void*))
{ 
	if(cap != 0){
		pThis->head = NULL;
		pThis->keySize = keySize;
		pThis->objSize = objSize;
		pThis->cmp = cmp;
		pThis->cap = cap;
		pThis->dynamic = 0;
		pThis->size = 0;

		struct node* dummy = (struct node*)malloc(sizeof(struct node));
		dummy->pre = dummy->next = NULL;
		dummy->dummyNode = 1;
		dummy->key = calloc(sizeof(pThis->keySize),0);
		dummy->object = calloc(sizeof(pThis->objSize),0);
		pThis->head = dummy;
		if(dummy==NULL || dummy->key==NULL || dummy->object==NULL)
			return __DS__MAP__OUT_OF_MEM__;
 	}
	return __DS__MAP__NORMAL__;
} 

int mapFree(struct map_t *pThis)
{  
	struct node* cur = pThis->head;
	while(cur!=NULL)
  	{
		struct node* next = cur->next;
		free(cur->key);
		free(cur->object);
		free(cur);
		cur=next;
 	}
	free(pThis);
	return __DS__MAP__NORMAL__;
}  

size_t mapSize(struct map_t *pThis)
{  
	return pThis->size;
}  

size_t mapCap(struct map_t *pThis)
{ 
	return pThis->cap;
}

size_t mapObjSize(struct map_t *pThis)
{  
	return pThis->objSize;
} 

size_t mapKeySize(struct map_t *pThis)
{
	return pThis->keySize;
}

int mapEmpty(struct map_t *pThis)
{ 
	if(pThis->head->next==NULL)//skip dummy node
		return __DS__MAP__EMPTY__;
	return __DS__MAP__NOT_EMPTY__;
}


/*Public Operator*/
int mapInsert(struct map_t *pThis, void *pKey, void *pObj)
{ 
	if(pThis->size == pThis->cap)
		return __DS__MAP__FULL__;
	struct node* cur = pThis->head;
	while(1)
 	{ 
		if(pThis->cmp(pKey,cur->key)==0 && cur->dummyNode==0)
 	 	{
			return __DS__MAP__OBJ_EXIST__;
 	 	}
		if(cur->next==NULL)
			break;
		cur = cur->next;
 	}  
	struct node* tmp = (struct node*)malloc(sizeof(struct node));
	tmp->key = malloc(sizeof(pThis->keySize));
	tmp->object = malloc(sizeof(pThis->objSize));
	tmp->next = NULL;
	tmp->pre = cur;
	tmp->dummyNode = 0;
	memcpy(tmp->key,pKey,pThis->keySize);
	memcpy(tmp->object,pObj,pThis->objSize);
	cur->next = tmp;

	if(tmp->key == NULL || tmp->object == NULL )
		return __DS__MAP__OUT_OF_MEM__;
	pThis->size ++;
	return __DS__MAP__NORMAL__;
} 

int mapDelete(struct map_t *pThis, void *pKey)
{ 
	struct node* cur = pThis->head;
	while(cur!=NULL)
 	{
		if(pThis->cmp(pKey,cur->key)==0 && 
				cur->dummyNode==0)
		{
			if(cur->next!=NULL)cur->next->pre = cur->pre;
			cur->pre->next = cur->next;
			struct node* tmp = cur->next;
			free(cur->key);
			free(cur->object);
			free(cur);
			pThis->size --;
			return __DS__MAP__NORMAL__;
 		}
		cur = cur->next;
	}
	return __DS__MAP__OBJ_NOT_EXIST__;
}

int mapGet(struct map_t *pThis, void *pKey, void *pRetObj)
{  
	struct node* cur = pThis->head;
	while(cur!=NULL)
  	{
		if(pThis->cmp(pKey,cur->key)==0 && cur->dummyNode==0)
  		{
			memcpy(pRetObj, cur->object, pThis->objSize);
			return __DS__MAP__OBJ_EXIST__;
		}
		cur = cur->next;
	}
	return __DS__MAP__OBJ_NOT_EXIST__;
}

int mapSet(struct map_t *pThis, void *pKey, void *pObj)
{ 
	struct node* cur = pThis->head;
	while(1)
 	{ 
		if(pThis->cmp(pKey,cur->key)==0 && cur->dummyNode==0)
 	 	{
            memcpy(cur->object, pObj, pThis->objSize);
			return __DS__MAP__NORMAL__;
 	 	}
		if(cur->next==NULL)
			break;
		cur = cur->next;
 	}  

	return __DS__MAP__OBJ_NOT_EXIST__;
}

/*Iterrator Support DS*/
/*
 * This structure is used to maintain current iterator state
 */
struct map_inner_t
{ 
	struct node* node;
	size_t objSize;
	size_t keySize;
};

/*Function used to support iterator*/
int mapGetIt(struct map_t *pThis, void *pKey, mapit *pRetIt)
{
	struct node* cur = pThis->head;
	while(cur!=NULL)
 	{
		if(pThis->cmp(pKey,cur->key)==0 && cur->dummyNode==0)
 		{
			mapit it = malloc(sizeof(struct map_inner_t));
			if(it==NULL)return __DS__MAP__OUT_OF_MEM__;
			it->node = cur;
			it->objSize = pThis->objSize;
			it->keySize = pThis->keySize;
			*pRetIt = it;
			return __DS__MAP__OBJ_EXIST__;
		}
		cur = cur->next;
	}
	return __DS__MAP__OBJ_NOT_EXIST__;
}

int mapGetByIt(mapit it, void *pRetKey, void *pRetObj)
{
	if(it==NULL) return __DS__MAP__INVALID_ITOR__;
	memcpy(pRetKey, it->node->key, it->keySize);
	memcpy(pRetObj, it->node->object, it->objSize);
	return __DS__MAP__NORMAL__;
}

int mapSetByIt(mapit it, void *pObj)
{
	if(it==NULL) return __DS__MAP__INVALID_ITOR__;
	memcpy( it->node->object, pObj, it->objSize);
	return __DS__MAP__NORMAL__;
}

int mapUpdateByIt(mapit it, void *pKey, void *pObj)
{
	if(it==NULL) return __DS__MAP__INVALID_ITOR__;
	memcpy( it->node->object, pObj, it->objSize);
	memcpy( it->node->key, pKey, it->keySize);
	return __DS__MAP__NORMAL__;
}

mapit mapNextIt(mapit it)
{
	if(it==NULL || it->node->next==NULL)
	{
		return NULL;
	}
	mapit rit = malloc(sizeof(struct map_inner_t));
	if(rit==NULL)return NULL;
	rit->node = it->node->next;
	rit->keySize = it->keySize;
	rit->objSize = it->objSize;
	return rit;
}  

mapit mapPrevIt(mapit it)
{  
	if(it==NULL || it->node->pre==NULL)
 	{
		return NULL;
	}
	mapit rit = malloc(sizeof(struct map_inner_t));
	if(rit==NULL)return NULL;
	rit->node = it->node->pre;
	rit->keySize = it->keySize;
	rit->objSize = it->objSize;
	return rit;
} 

int mapFreeIt(mapit it)
{
	free(it);
}

//Bonus 3

int mapGetOrder(struct map_t *pThis, int index, void *pRetKey, void *pRetObj)
{ 
	struct node* cur = NULL;

	int* tmpArr = (int*)malloc( (pThis->size+1)*sizeof(int) );
	memset(tmpArr, 0, (pThis->size+1)*sizeof(int));
	int round = index;
	int minIndex;
	struct node* tmpNode;
	while(round-->0)
	{
		int index = 0;
		minIndex = 0;
		tmpNode = NULL;
		cur = pThis->head;
		while(cur!=NULL)
		{
			if( (tmpArr[index]==0 && minIndex == 0) ||
				(tmpArr[index]==0 && pThis->cmp(tmpNode->key,cur->key)>0 )
			  )
			{
				minIndex = index;
				tmpNode = cur;
 			}
			cur = cur->next;
			index ++;
 		}
		tmpArr[minIndex] = 1;
 	}
	free(tmpArr);
	if(minIndex==0) return __DS__MAP__OBJ_NOT_EXIST__;
	memcpy(pRetKey, tmpNode->key, pThis->keySize);
	memcpy(pRetObj, tmpNode->object, pThis->objSize);
	return __DS__MAP__NORMAL__;
} 

int mapGetOrderIt(struct map_t *pThis, int index, void *pRetKey, void *pRetObj, mapit *pRetIt)
{
	struct node* cur = NULL;

	int* tmpArr = (int*)malloc( (pThis->size+1)*sizeof(int) );
	memset(tmpArr, 0, (pThis->size+1)*sizeof(int));
	int round = index;
	int minIndex;
	struct node* tmpNode;
	while(round-->0)
	{
		int index = 0;
		minIndex = 0;
		tmpNode = NULL;
		cur = pThis->head;
		while(cur!=NULL)
		{
			if( (tmpArr[index]==0 && minIndex == 0) ||
				(tmpArr[index]==0 && pThis->cmp(tmpNode->key,cur->key)>0 )
			  )
			{
				minIndex = index;
				tmpNode = cur;
 			}
			cur = cur->next;
			index ++;
 		}
		tmpArr[minIndex] = 1;
 	}
	free(tmpArr);
	if(minIndex==0) return __DS__MAP__OBJ_NOT_EXIST__;
	memcpy(pRetKey, tmpNode->key, pThis->keySize);
	memcpy(pRetObj, tmpNode->object, pThis->objSize);
	//new iterator
	mapit it = malloc(sizeof(struct map_inner_t));
	if(it==NULL)return __DS__MAP__OUT_OF_MEM__;
	it->node = tmpNode;
	it->keySize = pThis->keySize;
	it->objSize = pThis->objSize;
	return __DS__MAP__NORMAL__;
}
