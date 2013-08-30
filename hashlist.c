#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include"hashlist.h"
#include"hashalgo.h"
void hashlist_reallot(hashlist* list1)
{
        int i;
        float alloted_percent =  ((float)list1->alloted/list1->size)*100 ;
        printf("Realloting Hashlist with allocation %f  New Size %d\n",alloted_percent,list1->size*2);
        hashlist hashlist2;

        hashlist2.Hash=(hash*)malloc(sizeof(hash)*list1->size*2);
        hashlist2.size=list1->size*2;
        for( i=0; i < hashlist2.size-1; i++)
                hashlist2.Hash[i].valid=0;
        hashlist2.alloted=0;
       
        for(i=0; i < list1->size; i++)
        {
                if(list1->Hash[i].valid == 1)
                {
                        //printf("\tCoords :- %d %d %d\n",list1->Hash[i].x,list1->Hash[i].y,list1->Hash[i].z);
                        hashlist_ops.hashlist_add(&hashlist2,list1->Hash[i].key_name,list1->Hash[i].x,list1->Hash[i].y,list1->Hash[i].z);
                }                      
        }

        hash* hash_tmp = list1->Hash;
        list1->Hash=hashlist2.Hash;
        list1->size=hashlist2.size;
        list1->alloted=hashlist2.alloted;
        free(hash_tmp);
}

int hashlist_add_implemented(hashlist* list1,char* key_name,int x,int y,int z)
{
        int hash_value;
        int i;
        if ( list1->Hash == NULL )
        {
                 list1->Hash = (hash*)malloc(sizeof(hash) * list1->size );
                 for( i=0; i<list1->size-1; i++)
                        list1->Hash[i].valid=0;
        }

        /* Reallot Hashlist if it iis 80% full */
        float alloted_percent =  ((float)list1->alloted/list1->size)*100 ;
        //printf("Adding hash :- %s\t Hash:- %d \tAlloted %f\%\n",key_name,hashalgo(key_name,list1->size),alloted_percent);            
        if ( alloted_percent >= 80 )
                hashlist_reallot(list1);

        hash_value = hashalgo(key_name,list1->size);
        //printf("Hash Value  :- %d\n",hash_value);

        /*Closed Hashing, In collision look for the next available space to fit the key*/

        while (list1->Hash[hash_value].valid == 1)
        {
                /*Check if same key exist. If yes return*/
                if (!strcmp(list1->Hash[hash_value].key_name,key_name)) 
			return -1;
                if ( hash_value == list1->size-1 )
                        hash_value=-1;
                hash_value++;
        //      printf("Hash %d  Valid %d\n",hash_value,list1->Hash[hash_value].valid);
        }

        list1->Hash[hash_value].key_name = (char*)malloc(sizeof(char)*strlen(key_name)+1);
        strcpy(list1->Hash[hash_value].key_name,key_name);      
        list1->Hash[hash_value].x = x;
        list1->Hash[hash_value].y = y;
        list1->Hash[hash_value].z = z;
        list1->Hash[hash_value].valid = 1;
        list1->alloted++;
        return 1; 
}

void hashlist_display_implemented(hashlist* list1)
{
        int i;
        printf("Displaying Hash list\n");
        if (list1->alloted == 0) return;
        for(i=0; i < list1->size; i++)
        {
                if(list1->Hash[i].valid == 1)
                {
                        printf("\tName   :- %s\n",list1->Hash[i].key_name);
                        printf("\tCoords :- %d %d %d\n",list1->Hash[i].x,list1->Hash[i].y,list1->Hash[i].z);
                }                      
        }
}

void hashlist_initialize_implemented(hashlist* list1)
{
        //printf("Initializing\n");
        list1->size=10;
        list1->alloted=0;
        list1->Hash=NULL;
}

hash* hashlist_findhash_implemented(hashlist* list1, char* key_name)
{
        int i;
        if (list1->alloted == 0) return NULL;
        for(i=0; i < list1->size; i++)
        {
                if(list1->Hash[i].valid == 1 && !strcmp(list1->Hash[i].key_name,key_name))
                        return &list1->Hash[i];
        }
        return NULL;
}

void hashlist_destroy_implemented(hashlist* list1)
{
        int i;
        if (list1->alloted == 0) return;
        for(i=0; i < list1->size; i++)
        {
                if(list1->Hash[i].valid == 1)
			free(list1->Hash[i].key_name);
        }
	free(list1->Hash);	
}
