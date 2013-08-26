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
	void* obj;
	struct node *left;
	struct node *right;
  struct node *parent;
  struct node *next;
  struct node *pre;
  size_t level, nChild;
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

struct node *newNode(size_t objSize, void *pObj, size_t keySize, void *pKey, struct node *pParent)
{
  struct node *new = malloc(sizeof(struct node));
  new->parent = pParent;
  new->level = 1;
  new->nChild = 1;
  new->obj = malloc(objSize);
  new->key = malloc(keySize);
  memcpy(new->obj, pObj, objSize);
  memcpy(new->key, pKey, keySize);
  new->left = new->right = NULL;
  new->pre = new->next = NULL;
  return new;
}
int mapInit(struct map_t *pThis, size_t keySize, size_t objSize, size_t cap,
		int (*cmp)(const void*, const void*))
{
 	if(cap != 0){
    pThis->cap = cap;
    pThis->dynamic = 0;
  }
  else
  {
    pThis->cap = 4;
    pThis->dynamic = 1;
  }
  pThis->head = NULL;
  pThis->objSize = objSize;
  pThis->keySize = keySize;
  pThis->cmp = cmp;
  pThis->size = 0;

	return __DS__MAP__NORMAL__;
}

int nodeFree(struct node *cur)
{
  if (cur->left != NULL)
    nodeFree(cur->left);
  else if (cur->right != NULL)
    nodeFree(cur->right);
  else
  {
    free(cur->obj);
    free(cur);
  }
  return 0;
}
int mapFree(struct map_t *pThis)
{
	struct node* cur = pThis->head;
  nodeFree(cur);
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
	if(pThis->size == 0)//skip dummy node
		return __DS__MAP__EMPTY__;
	return __DS__MAP__NOT_EMPTY__;
}

/*Rotate*/
struct node * R_Rotate(struct node *cur)
{
  if (cur->parent->parent == NULL) ;
  else if (cur->parent->parent->left == cur->parent)
    cur->parent->parent->left = cur;
  else
    cur->parent->parent->right = cur;
  cur->parent->left = cur->right;
  if (cur->right)
    cur->right->parent = cur->parent;
  cur->right = cur->parent;
  cur->parent = cur->parent->parent;
  cur->right->parent = cur;
  /*cur->right->level -= (cur->right->level > 2) ? 2 : (cur->right->level == 2) ? 1 : 0;*/
  /*
  cur->level = (cur->left == NULL) ? cur->right->level+1 :
  (cur->right->level > cur->left->level) ? cur->right->level+1 : cur->left->level+1;*/
  if (cur->right->left == NULL && cur->right->right == NULL)
    cur->right->level = 1;
  else if (cur->right->right == NULL)
  {
    cur->right->level = cur->right->left->level+1;
  }
  else if (cur->right->left == NULL)
  {
    cur->right->level = cur->right->right->level+1;
  }
  else
  {
    cur->right->level = (cur->right->right->level > cur->right->left->level) ? cur->right->right->level+1: cur->right->left->level+1;
  }
  if (cur->left == NULL)
  {
    cur->level = cur->right->level+1;
  }
  else
  {
    cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
  }
  cur->nChild += (cur->right->right == NULL)? 1 : (cur->right->right->nChild + 1);
  cur->right->nChild -= (cur->left == NULL) ? 1 : (cur->left->nChild + 1);
  return cur;
}
struct node * L_Rotate(struct node *cur)
{
  if (cur->parent->parent == NULL) ;
  else if (cur->parent->parent->right == cur->parent)
    cur->parent->parent->right = cur;
  else
    cur->parent->parent->left = cur;
  cur->parent->right = cur->left;
  if (cur->left)
    cur->left->parent = cur->parent;
  cur->left = cur->parent;
  cur->parent = cur->parent->parent;
  cur->left->parent = cur;
  /*cur->left->level -= (cur->left->level > 2 ) ? 2 : (cur->left->level == 2) ? 1 : 0;*/
  /*cur->level = (cur->right == NULL) ? cur->left->level+1 :
    ((cur->right->level > cur->left->level) ? (cur->right->level+1) : (cur->left->level+1));*/
  if (cur->left->left == NULL && cur->left->right == NULL)
    cur->left->level = 1;
  else if (cur->left->right == NULL)
  {
    cur->left->level = cur->left->left->level+1;
  }
  else if (cur->left->left == NULL)
  {
    cur->left->level = cur->left->right->level+1;
  }
  else
  {
    cur->left->level = (cur->left->right->level > cur->left->left->level) ? cur->left->right->level+1: cur->left->left->level+1;
  }
  if (cur->right == NULL)
  {
    cur->level = cur->left->level+1;
  }
  else
  {
    cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
  }
  cur->nChild += (cur->left->left == NULL) ? 1 : (cur->left->left->nChild + 1);
  cur->left->nChild -= (cur->right == NULL) ? 1 : (cur->right->nChild + 1);
  return cur;
}

struct node * RL_Rotate(struct node *cur)
{
  cur = R_Rotate(cur);
  cur = L_Rotate(cur);
  return cur;
}
struct node * LR_Rotate(struct node *cur)
{
  cur = L_Rotate(cur);
  cur = R_Rotate(cur);
  return cur;
}
/*Buggy Handler*/
struct node * buggy(struct node *cur, int type)
{
  /*type 1 means right > left*/
  /*type 2 means left > right*/
  if (type == 1)
  {
    if (cur->right->left == NULL)
    {
      return L_Rotate(cur->right);
    }
    else if (cur->right->right == NULL)
    {
      return RL_Rotate(cur->right->left);
    }
    else if (cur->right->right->level > cur->right->left->level)
    {
      return L_Rotate(cur->right);
    }
    else
    {
      return RL_Rotate(cur->right->left);
    }
  }
  else
  {
    if (cur->left->left == NULL)
      return LR_Rotate(cur->left->right);
    else if (cur->left->right == NULL)
      return R_Rotate(cur->left);
    else if (cur->left->left->level > cur->left->right->level)
    {
      return R_Rotate(cur->left);
    }
    else
    {
      return LR_Rotate(cur->left->right);
    }
  }
}
/*Public Operator*/
int mapInsert(struct map_t *pThis, void *pKey, void *pObj)
{
	if(pThis->size == pThis->cap)
    if (pThis->dynamic)
      pThis->cap *= 2;
    else
		  return __DS__MAP__FULL__;
  if (pThis->size == 0)
  {
    pThis->head = newNode(pThis->keySize, pKey, pThis->objSize, pObj, NULL);
    pThis->size++;
    return __DS__MAP__NORMAL__;
  }
	struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(cur->obj, pObj) == 0)
      return __DS__MAP__OBJ_EXIST__;
    else if (pThis->cmp(cur->obj, pObj) < 0)
    {
      if (cur->right == NULL)
      {
        cur->right = newNode(pThis->keySize, pKey, pThis->objSize, pObj, cur);
        if (cur->next == NULL)
        {
          cur->next = cur->right;
          cur->right->pre = cur;
        }
        else
        {
          cur->right->pre = cur;
          cur->right->next = cur->next;
          cur->next->pre = cur->right;
          cur->next = cur->right;
        }
        cur = cur->right;
        break;
      }
      else
        cur = cur->right;
    }
    else
    {
      if (cur->left == NULL)
      {
        cur->left = newNode(pThis->keySize, pKey, pThis->objSize, pObj, cur);
        if (cur->pre == NULL)
        {
          cur->pre = cur->left;
          cur->left->next = cur;
        }
        else
        {
          cur->left->next = cur;
          cur->left->pre = cur->pre;
          cur->pre->next = cur->left;
          cur->pre = cur->left;
        }
        cur = cur->left;
        break;
      }
      else
      {
        cur = cur->left;
      }
    }
  }
  /*update level*/
  while (1)
  {
    if (cur->left == NULL && cur->right == NULL)
      cur->level = 1;
    else if (cur->right == NULL)
    {
      cur->level = cur->left->level+1;
    }
    else if (cur->left == NULL)
    {
      cur->level = cur->right->level+1;
    }
    else
    {
      cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
    }
    if (cur->left == NULL || cur->right == NULL)
    {
      if (cur->left == NULL && cur->right == NULL)
        ;
      else if (cur->left == NULL)
      {
        if (cur->right->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 1);
          else
            buggy(cur, 1);
          break;
        }
        /*left = 0 right = 2*/
      }
      else if (cur->right == NULL)
      {
        if (cur->left->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 2);
          else
            buggy(cur, 2);
          break;
        }
        /*left = 2 right = NULL*/
      }
    }
    else if (cur->right->level - cur->left->level == 2)
    {
      if (cur == pThis->head)
      {
        pThis->head = buggy(cur, 1);
      }
      else
        buggy(cur, 1);
      /*right > left*/
      break;
    }
    else if (cur->right->level - cur->left->level == -2)
    {
      if (cur == pThis->head)
      {
        pThis->head = buggy(cur, 2);
      }
      else
        buggy(cur, 2);
      /*right < left*/
      break;
    }
    if (cur == pThis->head)
      break;
    cur = cur->parent;
  }
	pThis->size++;
	return __DS__MAP__NORMAL__;
}

int mapDelete(struct map_t *pThis, void *pKey)
{
  if (pThis->size == 0)
    return __DS__MAP__OBJ_NOT_EXIST__;
	struct node* cur = pThis->head;
  if (pThis->size <= 3)
  {
    if (pThis->size == 0)
      return __DS__MAP__EMPTY__;
    if (pThis->size == 1)
    {
      free(cur->obj);
      free(cur);
      pThis->size--;
      return __DS__MAP__NORMAL__;
    }
    if (pThis->cmp(cur->obj, pKey) == 0)
    {
      if (cur->right)
      {
        pThis->head = cur->right;
        pThis->head->pre = cur->pre;
        pThis->head->parent = NULL;
        pThis->head->left = cur->left;
        if (cur->left)
          cur->left->parent = pThis->head;
        free(cur->obj);
        free(cur);
        pThis->size--;
        return __DS__MAP__NORMAL__;
      }
      else
      {
        pThis->head = cur->left;
        pThis->head->next = cur->next;
        pThis->head->parent = NULL;
        pThis->head->right = cur->right;
        if (cur->right)
          cur->right->parent = pThis->head;
        free(cur->obj);
        free(cur);
        pThis->size--;
        return __DS__MAP__NORMAL__;
      }
    }
    if (pThis->cmp(cur->obj, pKey) > 0)
    {
      if (cur->left == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      if (pThis->cmp(cur->left->obj, pKey) != 0)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
      {
        free(cur->left->obj);
        free(cur->left);
        pThis->head->left = NULL;
        pThis->head->pre = NULL;
        pThis->size--;
        return __DS__MAP__NORMAL__;
      }
    }
    else
    {
      if (cur->right == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      if (pThis->cmp(cur->right->obj, pKey) != 0)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
      {
        free(cur->right->obj);
        free(cur->right);
        pThis->head->right = NULL;
        pThis->head->next = NULL;
        pThis->size--;
        return __DS__MAP__NORMAL__;
      }
    }
  }
  /*find obj*/
  while (1)
  {
    if (pThis->cmp(pKey, cur->obj) == 0)
      break;
    else if (pThis->cmp(pKey, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
        cur = cur->right;
    }
    else
    {
      if (cur->left == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
        cur = cur->left;
    }
  }
  /*delete obj*/
  struct node *re;
  struct node *temp;
  if (cur->next != NULL && cur->next->level < cur->level)
  {
    re = cur->next;
    if (cur == pThis->head)
    {
      pThis->head = re;
      pThis->head->parent = NULL;
    }
    temp = re->parent;
    if (temp == cur)
      temp = re;
    /*fix level nChild*/
    re->parent->nChild--;
    /*fix linked list*/
    if (cur->pre)
      cur->pre->next = re;
    re->pre = cur->pre;
    /*fix tree structure*/
    if (re->parent == cur)
    {
      re->left = cur->left;
      if (re->left)
        re->left->parent = re;
      re->parent = cur->parent;
      if (cur->parent)
        if (cur->parent->right == cur)
          cur->parent->right = re;
        else
          cur->parent->left = re;
    }
    else
    {
      if (re->right != NULL)
      {
        re->parent->left = re->right;
        re->right->parent = re->parent;
      }
      else
      {
        re->parent->left = NULL;
      }
      /*fix connection*/
      re->parent = cur->parent;
      re->left = cur->left;
      if (re->left != NULL)
        re->left->parent = re;
      re->right = cur->right;
      if (re->right != NULL)
        re->right->parent = re;
    }
    free(cur->obj);
    free(cur);
  }
  else if (cur->pre != NULL && cur->pre->level < cur->level)
  {
    re = cur->pre;
    if (cur == pThis->head)
    {
      pThis->head = re;
      pThis->head->parent;
    }
    temp = re->parent;
    if (temp == cur)
      temp = re;
    /*fix level nChild*/
    re->parent->nChild--;
    /*fix linked list*/
    if (cur->next)
      cur->next->pre = re;
    re->next = cur->next;
    /*fix tree structure*/
    if (re->parent == cur)
    {
      re->right = cur->right;
      if (re->right)
        re->right->parent = re;
      re->parent = cur->parent;
      if (cur->parent)
        if (cur->parent->left == cur)
          cur->parent->left = re;
        else
          cur->parent->right = re;
    }
    else
    {
      if (re->left != NULL)
      {
        re->parent->right = re->left;
        re->left->parent = re->parent;
      }
      else
      {
        re->parent->right = NULL;
      }
      /*fix connection*/
      re->parent = cur->parent;
      re->left = cur->left;
      if (re->left != NULL)
        re->left->parent = re;
      re->right = cur->right;
      if (re->right != NULL)
        re->right->parent = re;
    }
    free(cur->obj);
    free(cur);
  }
  else
  {
    temp = cur->parent;
    if (cur->pre)
      cur->pre->next = cur->next;
    if (cur->next)
      cur->next->pre = cur->pre;
    if (cur->parent == NULL)
      ;
    else if (cur->parent->right == cur)
    {
      cur->parent->right = NULL;
      if (cur->parent)
      {
        cur->parent->level = (cur->parent->left) ? cur->parent->left->level+1 : 1;
      }
    }
    else if (cur->parent->left == cur)
    {
      cur->parent->left = NULL;
      if (cur->parent)
      {
        cur->parent->level = (cur->parent->right) ? cur->parent->right->level+1 : 1;
      }
    }
    free(cur->obj);
    free(cur);
    if (pThis->size == 1)
    {
      pThis->size--;
      return __DS__MAP__NORMAL__;
    }
  }
  /*balance*/
  cur = temp;
  while (1)
  {
    if (cur->left == NULL && cur->right == NULL)
      cur->level = 1;
    else if (cur->right == NULL)
    {
      cur->level = cur->left->level+1;
    }
    else if (cur->left == NULL)
    {
      cur->level = cur->right->level+1;
    }
    else
    {
      cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
    }
    if (cur->left == NULL || cur->right == NULL)
    {
      if (cur->left == NULL && cur->right == NULL)
        ;
      else if (cur->left == NULL)
      {
        if (cur->right->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 1);
          else
            buggy(cur, 1);
        }
        /*left = 0 right = 2*/
      }
      else if (cur->right == NULL)
      {
        if (cur->left->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 2);
          else
            buggy(cur, 2);
        }
        /*left = 2 right = NULL*/
      }
    }
    else if (cur->right->level - cur->left->level == 2)
    {
      if (cur == pThis->head)
        pThis->head = buggy(cur, 1);
      else
        buggy(cur, 1);
      /*right > left*/
    }
    else if (cur->left->level - cur->right->level == 2)
    {
      if (cur == pThis->head)
        pThis->head = buggy(cur, 2);
      else
        buggy(cur, 2);
      /*right < left*/
    }
    if (cur->parent == NULL)
      break;
    cur = cur->parent;
  }
  pThis->size--;
	return __DS__MAP__NORMAL__;
}

int mapGet(struct map_t *pThis, void *pKey, void *pRetObj)
{
	struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(pKey, cur->obj) == 0)
    {
      memcpy(pRetObj, cur->key, pThis->objSize);
      return __DS__MAP__OBJ_EXIST__;
    }
    else if (pThis->cmp(pKey, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
        cur = cur->right;
    }
    else
    {
      if (cur->left == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
        cur = cur->left;
    }
  }
}

int mapSet(struct map_t *pThis, void *pKey, void *pObj)
{
	struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(pKey, cur->obj) == 0)
    {
      memcpy(cur->key, pObj, pThis->objSize);
      return __DS__MAP__OBJ_EXIST__;
    }
    else if (pThis->cmp(pKey, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
        cur = cur->right;
    }
    else
    {
      if (cur->left == NULL)
        return __DS__MAP__OBJ_NOT_EXIST__;
      else
        cur = cur->left;
    }
  }
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
	/*struct node* cur = pThis->head;*/
	/*while(cur!=NULL)*/
   /*{*/
		/*if(pThis->cmp(pKey,cur->key)==0 && cur->dummyNode==0)*/
     /*{*/
			/*mapit it = malloc(sizeof(struct map_inner_t));*/
			/*if(it==NULL)return __DS__MAP__OUT_OF_MEM__;*/
			/*it->node = cur;*/
			/*it->objSize = pThis->objSize;*/
			/*it->keySize = pThis->keySize;*/
			/**pRetIt = it;*/
			/*return __DS__MAP__OBJ_EXIST__;*/
		/*}*/
		/*cur = cur->next;*/
	/*}*/
	return __DS__MAP__OBJ_NOT_EXIST__;
}

int mapGetByIt(mapit it, void *pRetKey, void *pRetObj)
{
	/*if(it==NULL) return __DS__MAP__INVALID_ITOR__;*/
	/*memcpy(pRetKey, it->node->key, it->keySize);*/
	/*memcpy(pRetObj, it->node->object, it->objSize);*/
	return __DS__MAP__NORMAL__;
}

int mapSetByIt(mapit it, void *pObj)
{
	/*if(it==NULL) return __DS__MAP__INVALID_ITOR__;*/
	/*memcpy( it->node->object, pObj, it->objSize);*/
	return __DS__MAP__NORMAL__;
}

int mapUpdateByIt(mapit it, void *pKey, void *pObj)
{
	/*if(it==NULL) return __DS__MAP__INVALID_ITOR__;*/
	/*memcpy( it->node->object, pObj, it->objSize);*/
	/*memcpy( it->node->key, pKey, it->keySize);*/
	return __DS__MAP__NORMAL__;
}

mapit mapNextIt(mapit it)
{
	/*if(it==NULL || it->node->next==NULL)*/
	/*{*/
		/*return NULL;*/
	/*}*/
	/*mapit rit = malloc(sizeof(struct map_inner_t));*/
	/*if(rit==NULL)return NULL;*/
	/*rit->node = it->node->next;*/
	/*rit->keySize = it->keySize;*/
	/*rit->objSize = it->objSize;*/
	/*return rit;*/
}

mapit mapPrevIt(mapit it)
{
	/*if(it==NULL || it->node->pre==NULL)*/
   /*{*/
		/*return NULL;*/
	/*}*/
	/*mapit rit = malloc(sizeof(struct map_inner_t));*/
	/*if(rit==NULL)return NULL;*/
	/*rit->node = it->node->pre;*/
	/*rit->keySize = it->keySize;*/
	/*rit->objSize = it->objSize;*/
	/*return rit;*/
}

int mapFreeIt(mapit it)
{
	/*free(it);*/
}

//Bonus 3

int mapGetOrder(struct map_t *pThis, int index, void *pRetKey, void *pRetObj)
{
	/*struct node* cur = NULL;*/

	/*int* tmpArr = (int*)malloc( (pThis->size+1)*sizeof(int) );*/
	/*memset(tmpArr, 0, (pThis->size+1)*sizeof(int));*/
	/*int round = index;*/
	/*int minIndex;*/
	/*struct node* tmpNode;*/
	/*while(round-->0)*/
	/*{*/
		/*int index = 0;*/
		/*minIndex = 0;*/
		/*tmpNode = NULL;*/
		/*cur = pThis->head;*/
		/*while(cur!=NULL)*/
		/*{*/
			/*if( (tmpArr[index]==0 && minIndex == 0) ||*/
				/*(tmpArr[index]==0 && pThis->cmp(tmpNode->key,cur->key)>0 )*/
				/*)*/
			/*{*/
				/*minIndex = index;*/
				/*tmpNode = cur;*/
       /*}*/
			/*cur = cur->next;*/
			/*index ++;*/
     /*}*/
		/*tmpArr[minIndex] = 1;*/
   /*}*/
	/*free(tmpArr);*/
	/*if(minIndex==0) return __DS__MAP__OBJ_NOT_EXIST__;*/
	/*memcpy(pRetKey, tmpNode->key, pThis->keySize);*/
	/*memcpy(pRetObj, tmpNode->object, pThis->objSize);*/
	return __DS__MAP__NORMAL__;
}

int mapGetOrderIt(struct map_t *pThis, int index, void *pRetKey, void *pRetObj, mapit *pRetIt)
{
	/*struct node* cur = NULL;*/

	/*int* tmpArr = (int*)malloc( (pThis->size+1)*sizeof(int) );*/
	/*memset(tmpArr, 0, (pThis->size+1)*sizeof(int));*/
	/*int round = index;*/
	/*int minIndex;*/
	/*struct node* tmpNode;*/
	/*while(round-->0)*/
	/*{*/
		/*int index = 0;*/
		/*minIndex = 0;*/
		/*tmpNode = NULL;*/
		/*cur = pThis->head;*/
		/*while(cur!=NULL)*/
		/*{*/
			/*if( (tmpArr[index]==0 && minIndex == 0) ||*/
				/*(tmpArr[index]==0 && pThis->cmp(tmpNode->key,cur->key)>0 )*/
				/*)*/
			/*{*/
				/*minIndex = index;*/
				/*tmpNode = cur;*/
       /*}*/
			/*cur = cur->next;*/
			/*index ++;*/
     /*}*/
		/*tmpArr[minIndex] = 1;*/
   /*}*/
	/*free(tmpArr);*/
	/*if(minIndex==0) return __DS__MAP__OBJ_NOT_EXIST__;*/
	/*memcpy(pRetKey, tmpNode->key, pThis->keySize);*/
	/*memcpy(pRetObj, tmpNode->object, pThis->objSize);*/
	/*//new iterator*/
	/*mapit it = malloc(sizeof(struct map_inner_t));*/
	/*if(it==NULL)return __DS__MAP__OUT_OF_MEM__;*/
	/*it->node = tmpNode;*/
	/*it->keySize = pThis->keySize;*/
	/*it->objSize = pThis->objSize;*/
	return __DS__MAP__NORMAL__;
}
