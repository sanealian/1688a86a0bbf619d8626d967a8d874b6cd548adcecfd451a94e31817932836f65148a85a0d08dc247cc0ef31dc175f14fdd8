#include "hashtable.h"
#include <stdio.h>

int main (void)
{
    uint_t test_items_count = 5, current = 0;
    HashTable * ht = newHashTable(test_items_count);

    char tmp_name[20];


    while ( current++ < test_items_count )
    {
        sprintf(tmp_name, "name_%d", current);
        ht->put(ht, tmp_name, &current);
        // printf("written value [%d] : %d\n", current, *(&(int){current}));
    }
    
    
    
    // // Delete second key from the hashtable
    // // ''''''''''''''''''''''''''''''''''''
    // ht->del(ht, "name_2");

    // // Replace data for a specific key
    // // '''''''''''''''''''''''''''''''
    // ht->put(ht, "name_5", &(int){43});
    // printf(" ---> %d\n", *(int*)ht->get(ht, "name_5"));
    // ht->get(ht, "name_5");
    // printf("\n\n %s\n", ht->__table__[4]->name);

    int i = 0;
    printf("%d |\n", *(int*)(ht->__table__[3]->data));
    // ht->del(ht, "name_5");

    // current = 0;
    // while ( current++ < test_items_count )
    // {
    //     printf(" ---------------------------------------------------- \n");
    //     // printf("ITEM: name_%d", current); 
    //     sprintf( tmp_name, "name_%d", current );
    //     int data = *(int*)ht->get(ht, tmp_name);
    //     // if ( ht->get(ht, tmp_name) == 0 )
    //     // {
    //     //     printf(" [ IS EMPTY ]\n");
    //     //     continue;
    //     // }
    //     printf(" ---> %d\n", data);
        
    // }


    return 0;
}