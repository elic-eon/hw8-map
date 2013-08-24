#include <stdlib.h>
#ifndef __ds__map__header__
#define __ds__map__header__

#define __DS__MAP__NORMAL__        0x00000000
#define __DS__MAP__FULL__          0x00000001
#define __DS__MAP__EMPTY__         0x00000002
#define __DS__MAP__NOT_EMPTY__     0x00000004
#define __DS__MAP__OUT_OF_MEM__    0x00000008
#define __DS__MAP__OBJ_EXIST__     0x00000010
#define __DS__MAP__OBJ_NOT_EXIST__ 0x00000020
#define __DS__MAP__INVALID_ITOR__  0x00000040

struct map_t;
/* An ordered map contains muliple (key,value) pairs, where key is unique in the map */
/* Key type must support customized comparison */

/* If it is OOM( out of memory) in the memory allocation,
 * Please return __DS__MAP__OUT_OF_MEM__ for each function which supports to 
 * return __DS__MAP__NORMAL__ when normal execution. 
 */

struct map_t *mapAlloc();
/* Allocate memory for map. Return NULL if not enough memory */

int mapInit(struct map_t *pThis, size_t keySize, size_t objSize, size_t cap,  
           int (*cmp)(const void*, const void*));
/* Ini tialize pThis: 
 * set the size of key as keySize,
 * set the size of value as objSize
 * and the capacity as cap.
 * the comparison function as cmp: 
 *     cmp(keyA, keyB) returns -1 if keyA < keyB
 *     cmp(keyA, keyB) returns 0 if keyA = keyB
 *     cmp(keyA, keyB) returns 1 if keyA > keyB
 * If cmp(keyA, keyB)==0 then keyA and keyB are identical.
 * A valid map does not contain two key-value pairs whose keys are identical
 * After the initialization, there's nothing. So size is 0.
 * Return __DS__MAP__NORMAL__ for normal execution.
 * Bonus1: Using dynamic capacity when cap=0.
 */

int mapFree(struct map_t *pThis);
/* Release the memory.  
 * Deep release: You should release the memory allocated by any operation.
 * Return __DS__MAP__NORMAL__ for normal execution.
 */

size_t mapSize(struct map_t *pThis);
/* Return number of elements in the ordered map */

size_t mapCap(struct map_t *pThis);
/* Return current capacity */

size_t mapObjSize(struct map_t *pThis);
/* Return the size of value in the ordered map */

size_t mapKeySize(struct map_t *pThis);
/* Return the size of key in the object */

int mapEmpty(struct map_t *pThis);
/* Return zero if ordered map is not empty */

int mapInsert(struct map_t *pThis, void *pKey, void *pObj);
/* Insert a new (key,value) pair into ordered map *pThis
 * Input: pThis, an pointer to ordered map
 *        pKey, pointer to the key
 *        pObj, pointer to object of pKey
 * Output: __DS__MAP__NORMAL__ for normal execution
 *         __DS__MAP__OBJ_EXIST__  if the key to insert is already in th map
 *         __DS__MAP__OUT_OF_MEM__ for memory unavailable
 *         __DS__MAP__FULL__ if size is equal to capacity
 * Note 1: mapInsert inserts the key-value pair into *pThis only when key is not in *pThis
 * Note 2: mapInsert should be implement in O(log n)
 */          

int mapDelete(struct map_t *pThis, void *pKey);
/* Delete a object in the set  
 * Input: pThis, an pointer to order msp
 *        pKey, pointer to key
 * Output: __DS__MAP__OBJ_NOT_EXIST__ if object to delete is not in the map
 *         __DS__MAP__NORMAL__ for normal execution
 * Note 1: mapDelete should do nothing if key is not in *pThis
 * Note 2: mapDelete should be implement in O(log n)
 */

int mapGet(struct map_t *pThis, void *pKey, void *pRetObj);
/* Check if certain key is in the map, and get corresponding value
 * Input: pThis, an pointer to ordered map
 *        pKey, pointer to the key
 *        pRetObj, pointer to the value of pKey
 * Output: __DS__MAP__OBJ_NOT_EXIST__ if object is not in the map
 *         __DS__MAP__OBJ_EXIST__ if object is inside the map
 * Note 1: *RetObj should be NULL if key is not in *pThis
 * Note 2: mapGet should be implement in O(log n)
 */ 

int mapSet(struct map_t *pThis, void *pKey, void *pObj);
/* Set the value of certain key
 * Input: pThis, an pointer to ordered map
 *        pKey, the key which we want to reset the value
 *        pObj, value we want to set
 * Output: __DS__MAP__NORMAL__ for normal execution
 *         __DS__MAP__OBJ_NOT_EXIST__ if the key is not found
 * Note 1: If *pKey is not in *pThis, then do nothing to *pThis
 * Note 2: mapSet should be implement in O(log n)
 */

/* Bonus 2: iterator support! complete the following structures and 6 functions
 * Goal: Get previous/next iterator in O(1) time */

struct map_inner_t;
/* the inner cell stores an element and its structural information */

typedef struct map_inner_t *mapit;
/* Ordered set iterator: a pointer to os_inner_t */

int mapGetIt(struct map_t *pThis, void *pKey, mapit *pRetIt);
/* Get the iterator of certain pair is in the map
 * Input: pThis, pointer to the ordered set
 *        pKey, pointer to the object
 *        *pRetIt, pointer to the iterator storing return value
 * Output: __DS__MAP__OBJ_NOT_EXIST__ if *pKey is not in *pThis
 *         __DS__MAP__OBJ_EXIST__ if *pKey is in *pThis
 * Note 1: mapGetIt should be implement in O(log n)
 * Note 2: *pRetIt should be NULL if *pKey is not in *pThis
 *         otherwise *pRetIt should be the iterator indicating the inner
 *		   cell which contains *pKey
 */

int mapGetByIt(mapit it, void *pRetKey, void *pRetObj);
/* Get element by iterator
 * Input: pThis, pointer to the ordered set
 *        pRetKey, pointer to the result key
 *        pRetObj, pointer to the result object
 * Output: __DS__MAP__INVALID_ITOR__ if it is invalid
 *         __DS__MAP__NORMAL__ for normal execution
 * Note that mapGetByIt should be implement in O(1)
 */

int mapSetByIt(mapit it, void *pObj);
/* Set element by iterator
 * Input: pThis, pointer to the ordered set
 *        pRetObj, pointer to the object storing result
 * Output: __DS__OS__INVALID_ITOR__ if *pObj is not in *pThis
 *         __DS__OS__NORMAL__ for normal execution
 * Note that MapSetByIt should be implement in O(1)
 */

int mapUpdateByIt(mapit it, void *pKey, void *pObj);
/* Get element by iterator
 * Input: pThis, pointer to the ordered set
 *        pKey, pointer to key
 *        pObj, pointer to object
 * Output: __DS__MAP__INVALID_ITOR__ if it is invalid
 *         __DS__MAP__NORMAL__ for normal execution
 * Note that mapUpdateByIt should be implement in O(log n)
 */

mapit mapNextIt(mapit it);
/* Get the iterator of the next element of the one indicated by it
 * Input: it, iterator
 * Output: NULL if it is invalid or it indicates the last element
 * Note that osPrevIt should be implement in O(1)
 */

mapit mapPrevIt(mapit it);
/* Get the iterator of the previous element of the one indicated by it
 * Input: it, iterator
 * Output: NULL if it is invalid or it indicates the first element
 * Note that osPrevIt should be implement in O(1)
 */

int mapFreeIt(mapit it);

int mapGetOrder(struct map_t *pThis, int k, void *pRetKey, void *pRetObj);
/* Bonus 3: Find the kth-smallest key and it's value in the map
 * Input: pThis, an pointer to ordered map
 *        k, an int number represent that we need to find k-th smallest key in map
 *        pRetKey,  the pointer to k-th smallest key
 *        pRetObj, poiter to the value of pRetKey
 * Output: __DS__MAP__NORMAL__ for normal execution
 *         __DS__MAP__OBJ_NOT_EXIST__ if no key found
 *  Note that mapGetOrder should be implement in O(log n)
 */ 

int mapGetOrderIt(struct map_t *pThis, int k, void *pRetKey, void *pRetObj, mapit *pRetIt);
/* Bonus 3: Find the iterator indicating the kth-smallest key and it's value in the map
 * Input: pThis, an pointer to ordered map
 *        k, an int number represent that we need to find k-th smallest key in map
 *        pRetKey,  the pointer to k-th smallest key
 *        pRetObj, poiter to the value of pRetKey
 *        pRetIt, pointer to the iterator storing return value
 * Output: __DS__MAP__NORMAL__ for normal execution
 *         __DS__MAP__OBJ_NOT_EXIST__ if no key found
 *  Note that mapGetOrderIt should be implement in O(log n)
 */

#endif
