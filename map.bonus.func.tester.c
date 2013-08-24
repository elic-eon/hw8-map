#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "utility.h"
#include "map.h"

#define DOUBLE_NEG_INF (-1.0/0.0)
#define SHORT_NEG_INF SHRT_MIN
#define DOUBLE_INF (1.0/0.0) 
#define SHORT_INF SHRT_MAX

typedef struct map_t map;

int doubleGT(const void *a, const void *b)
{
	double da=*(double*)a;
	double db=*(double*)b;
	if(da==db)return 0;
	return (da>db)?1:-1;
}
int shortGT(const void *a, const void *b)
{
	short da=*(short*)a;
	short db=*(short*)b;
	if(da==db)return 0;
	return (da>db)?1:-1;
}

int main()
{ 
    map *p = NULL;
    size_t cap, tSize;
    int objType,keyType;
	int objTypeSize, keyTypeSize;
    void *data = malloc(sizeof(double)*10);
	void *data2 = malloc(sizeof(double)*10);
    double double_neg_inf = DOUBLE_NEG_INF;
    short short_neg_inf = SHORT_NEG_INF;
	mapit it = NULL, rit=NULL;	

     while(1){
        char cmd[10240] = {0};
        int ret;
         if(p == NULL){
            printf("The map needs to be initialized.\n");
            printf("Init step 1. Key Type: d for double, others for short.\n");
            scanf("%s", cmd);
            keyType = (cmd[0] == 'd')?__DS__DOUBLE__:__DS__SHORT__;
			printf("Init step 2. Value Type: d for double, others for short.\n");
			scanf("%s", cmd);
			objType = (cmd[0] == 'd')?__DS__DOUBLE__:__DS__SHORT__;
			printf("Init step 3. Capacity: \n");
			scanf("%zu", &cap);
            p = mapAlloc();
            if(p == NULL)
                ret = __DS__MAP__OUT_OF_MEM__;
            else{
				objTypeSize = (objType==__DS__DOUBLE__)? sizeof(double): sizeof(short);
                if(keyType == __DS__DOUBLE__)
                    ret = mapInit(p, sizeof(double), objTypeSize, cap, doubleGT);
                else
                    ret = mapInit(p, sizeof(short), objTypeSize, cap, shortGT);
            } 
                
            if(ret != __DS__MAP__NORMAL__){
                printf("Not enough memory.\n");
                if(p != NULL)
                    mapFree(p);
                p = NULL;
             } 
            else{
                printf("Initialization done.\n");
             }
		}else{
            int choice = 0;
            printf("size/capacity: %zu/%zu\n", mapSize(p), mapCap(p));
            printf("Valid operation: 1)insert, 2)delete, 3)get iterator, 4)set\n");
            printf("                 5)update by it, 6)get order, 7)next, 8)pre, 9)get by iterator\n");
			printf("				 10)empty, 11)free,  12)quit\n");
	
            while(choice <= 0 || choice > 12){
                scanf("%s", cmd);
                sscanf(cmd, "%d", &choice);
            }
            if (choice == 1){//insert 
                printf("input a %s-type key:", (keyType == __DS__DOUBLE__)?"double":"short"); 
                getData(keyType, data);
				printf("input a %s-type value:", (objType == __DS__DOUBLE__)?"double":"short");
                getData(objType, data2);
				ret = mapInsert(p, data, data2);
                if(ret==__DS__MAP__FULL__)printf("Map is full!\n");
				else if(ret==__DS__MAP__OBJ_EXIST__)printf("Element already exists!\n");
				else if(ret==__DS__MAP__OUT_OF_MEM__)printf("Run out of memory\n");
				else if(ret==__DS__MAP__NORMAL__)printf("Insert OK\n");
            } 
            else if(choice == 2){//delete  
				printf("input a %s value:", (keyType == __DS__DOUBLE__)?"double":"short");
				getData(keyType, data);
                ret = mapDelete(p, data);
                if(ret==__DS__MAP__OBJ_NOT_EXIST__){
                    printf("Element is not in the set.\n");
                }  
                else
					printf("Delete OK\n");
            }
            else if (choice == 3){//get it
                printf("input a %s value:", (keyType == __DS__DOUBLE__)?"double":"short"); 
                getData(keyType, data);
                ret = mapGetIt(p, data, &it);
                if(ret == __DS__MAP__OBJ_EXIST__){
                    printf("Get iterator OK!");
					printf(".\n");
			 	} 
			 	else{
					printf("The key ");
					printData(keyType, data);
					printf(" is not in the map.\n");
				} 
            }
            else if(choice == 4){//set by it
				printf("input the value to set:");
				getData( objType, data2);
                ret = mapSetByIt(it, data2);
                if(ret == __DS__MAP__NORMAL__){
                    printf("Set OK!\n");
                  } 
                else
                    printf("Invalid It\n");
            }
			else if(choice == 5){//update by it
				printf("input the key to set:");
				getData( objType, data);
				printf("input the value to set:");
				getData( objType, data2);
				ret = mapUpdateByIt(it, data, data2);
				if(ret == __DS__MAP__NORMAL__)
				{
					printf("Update OK!\n");
				}else
				{
					printf("Invalid It\n");
				}
			}
            else if (choice == 6){//get order
				int order;
                printf("input the order:");
				scanf("%zu", &order);
				ret = mapGetOrderIt(p, order, data, data2, &it);
        
                if(ret == __DS__MAP__NORMAL__){
                    printf("The result key is ");
                    printData( keyType, data);
                    printf(".\n");
					printf("The result value is ");
					printData( objType, data2);
					printf(".\n");
                 } 
                else
                    printf("No element found\n");
            }  
			else if(choice == 7){//next
				rit = mapNextIt(it);
				if(rit!=NULL)
					printf("Get Next OK!\n");
				else
					printf("Cannot get next\n");
				mapFreeIt(it);
				it = rit;
			} 
			else if(choice == 8){//pre
				rit = mapPrevIt(it);
				if(rit!=NULL)
					printf("Get Priv OK!\n");
				else
					printf("Cannot get priv\n");
				mapFreeIt(it);
				it = rit;
			}
			else if(choice == 9){//get element by it
				ret = mapGetByIt(it,data,data2);
				if(ret==__DS__MAP__NORMAL__)
				{
					printf("The key of curremt it :");
					printData( keyType, data);
					printf("\nThe value of curremt it :");
					printData( objType, data2);
					printf("\n");
				}else
				{
					printf("Invalid Iterator\n");
				}
			}
            else if(choice == 10){//empty
                if(mapEmpty(p)==__DS__MAP__EMPTY__)
                    printf("The map is empty.\n");
                else
                    printf("The map is not empty.\n");
            }  
            else if(choice == 11){
                mapFree(p);
                p = NULL;
            }  
            else if(choice == 12)
                break;
        } 
    } 
}

