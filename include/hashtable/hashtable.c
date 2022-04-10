
// These headers are specific to this module
// they are not seen outside
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hashtable.h"


uint_t HASHD_PRIME = 1309401007;



/* Function: keyhash
 * -------------------------------------------------
 * > This function generates hashtable index from string and given range.
 * > The range is specified as hashtable size property.
 * > HowWorks:
 * > - until the end of the keystring is met
 * > - starting from value of thero hash number is modified in this way:
 * > --- hash = ( hash << 5 | hash >> 27
 * > --- hash += (unsigned int) current_key_char
 * > - after all this a compression function is applied to the hash
 * > - with a specific purpose to coerce resulting hash
 * > - to a more pseudo-randomized fixed value
 * > - and to reduce number of futher possible collisions
 * > - then the result is coerced to hashtable's defined range (size)
 * -------------------------------------------------
 * @param { HashTable ** } ht
 *  | pointer --> to to a specific hashtable
 * @param { char * } namekey
 *  | the desired string key to hash 
 * -------------------------------------------------
 * @RETURNS { unsigned int }
 *  | generated within the range of hashtable index 
 * -------------------------------------------------
 */
uint_t keyhash(HashTable * ht, char * namekey)
{
    uchar_t * s = (unsigned char *) namekey;
    uint_t char_counter = 0,
           hash = 0;

    // HashTable for a little
    while ( *s != 0 )
    {
        hash  = (hash << 5) | (hash >> 27);
        hash += (uint_t) *s;
        ++s;
    }

    // Use compress function to compress a little
    // ------------------------------------------
    uint_t compressed = ( ht->__salt_a * hash + ht->__salt_b ) % HASHD_PRIME;
    
    // Reduce to the given range (size of array)
    // -----------------------------------------
    return compressed % ht->size;
}

/* Function: hashtable_put
 * -------------------------------------------------
 * > This function stores given pointer to user's data
 * > handling collisions using Separate Chaining method
 * -------------------------------------------------
 * @param { HashTable ** } ht
 *  | a pointer to a specific hashtable structure
 * @param { char * } name
 *  | Named key of the hashtable (it is then implicitly translated to index)
 * @param ( void * } data
 *  | this is generic pointer to whatever the data user wants to store
 * -------------------------------------------------
 * @RETURNS void
 * -------------------------------------------------
 */
void hashtable_put(HashTable * ht, char name[], void * data)
{
    // hash index from name
    uint_t hindex = keyhash(ht, name);

    // temporary shortcut pointer to the table element
    // -----------------------------------------------
    Node * bucket = ht->table[hindex];
    
    // if no items were inserted before
    // --------------------------------
    if ( ! bucket->data && ! bucket->next )
    {
        strcpy(bucket->name, name);
        bucket->data = (void *) data;
        bucket->next = 0;

        printf("[SET]: table[%d] => %s\n", hindex, bucket->name);
        return;
    }

    bool_t name_match; // check if node name and provided name match
    Node * node = bucket; // points to last node before null

    // iterate through chained nodes in bucket
    do {
        name_match = ( strcmp(bucket->name, name) == 0 );
        if ( name_match )
        {
            bucket->data = (void *) data;
            printf("[CHANGE]: table[%d] => %s\n", hindex, bucket->name);
            return;
        }
        node = bucket; // this is needed to save
        bucket = bucket->next; // this can be null so above is needed
    }
    while ( bucket != 0 );
    
    // if no matches were found
    // then just create and append a new node to the chain
    // ---------------------------------------------------
    node->next = malloc ( sizeof (Node) );
    strcpy (node->next->name, name);
    node->next->data = (void *) data;
    node->next->next = 0;
    printf("[INSERT]: table[%d] => %s\n", hindex, node->next->name);
}


/* Function: hashtable_get_ptr
 * -------------------------------------------------
 * > Obrain the pointer to a specific (target hash pointer) placed at index
 * -------------------------------------------------
 * @param { HashTable * ht } ht
 *  | the pointer to a hashtable itsef
 * @param { char * } name
 *  | the string that holds the name of the in table
 * -------------------------------------------------
 * @RETURNS { HashNode ** }
 *  | pointer --> to a pointer of hashtable item
 * -------------------------------------------------
 */
Node * hashtable_get_ptr(HashTable * ht, char name[])
{
    uint_t hashindex = keyhash(ht, name);
    Node * node = ht->table[ hashindex ];
    // In case of collision use Separate Chaining method
    // '''''''''''''''''''''''''''''''''''''''''''''''''
    if ( node->next == 0 )
        return node;

    while ( node->next != 0 )
    {
        if ( strcmp ( node->name, name ) == 0 ) break;
        node = node->next;
    }
    return node;
}



/* Function: hashtable_get
 * -------------------------------------------------
 * > This one retrieves an item data from hashtable
 * > so to say
 * > -----------------------------------------------
 * @param { HashTable * } ht
 * @param { char []     } name
 * -------------------------------------------------
 * @RETURNS { void * }
 *  | pointer to the data
 */
void * hashtable_get(HashTable * ht, char name[])
{
    return hashtable_get_ptr(ht, name)->data;
}


/* Function: hashtable_del
 * -------------------------------------------------
 * > This one removes the pointer to the item
 * > it is responsibility of the user
 * > to handle memory management
 * > -----------------------------------------------
 * @param { HashTable * } ht
 * @param { char []     } name
 * -------------------------------------------------
 * @RETURNS { void }
 */
void hashtable_rem(HashTable * ht, char name[])
{
    ( hashtable_get_ptr(ht, name)->data ) = 0;
}


HashTable * newHash (int elements)
{
    HashTable * ht = malloc ( sizeof (HashTable) );
    ht->size = elements;
    // Allocate array of pointers to structures
    // ----------------------------------------
    Node ** tbl = malloc ( sizeof ( Node * ) * elements);
    while (elements--)
    {
        tbl[elements] = malloc ( sizeof ( Node ) );
    }
    ht->table = tbl;

    // Assign functions as hashtable asmethods
    // '''''''''''''''''''''''''''''''''''''''
    ht->put = (void   (*)(HashTable *, char *, void*)) hashtable_put;
    ht->get = (void * (*)(HashTable *, char *       )) hashtable_get;
    ht->rem = (void   (*)(HashTable *, char *       )) hashtable_rem;


    // Set pseudo-random *salt* values for a hash function
    // '''''''''''''''''''''''''''''''''''''''''''''''''''
    ht->__salt_a = rand() % (HASHD_PRIME - 1);
    ht->__salt_b = rand() % (HASHD_PRIME - 1);


    return ht;
}

void dropHash(HashTable * ht)
{
    int els = ht->size;

    while (--els)
        free ( ht->table[els] );
    free ( ht->table );
}