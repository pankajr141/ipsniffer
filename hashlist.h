struct hash_t
{
	/*File Name . x,y,z coordinates*/
	char* key_name;		
	int x;			
	int y;
	int z;
	int valid;
};
typedef struct hash_t hash;

struct hashlist_t
{
	int size;
	int alloted;
	hash* Hash;
};
typedef struct hashlist_t hashlist;

struct hashlist_ops_n
{
	void (*hashlist_initialize)(hashlist* list1);
	int (*hashlist_add)(hashlist* list1, char* key_name,int x, int y, int z);
	void (*hashlist_display)(hashlist* list1);
	hash* (*hashlist_findhash)(hashlist* list1, char* key_name);
	void (*hashlist_destroy)(hashlist* list1);
};
static const struct hashlist_ops_n hashlist_ops;

void hashlist_reallot(hashlist* list1);
int hashlist_add_implemented(hashlist* list1,char* key_name,int x,int y,int z);
void hashlist_display_implemented(hashlist* list1);
void hashlist_initialize_implemented(hashlist* list1);
void hashlist_destroy_implemented(hashlist* list1);
hash* hashlist_findhash_implemented(hashlist* list1, char* key_name);

static const struct hashlist_ops_n hashlist_ops = {		//Redefining Functions to operate on node
	.hashlist_initialize=hashlist_initialize_implemented,
	.hashlist_add=hashlist_add_implemented,
	.hashlist_display=hashlist_display_implemented,
	.hashlist_findhash=hashlist_findhash_implemented,
	.hashlist_destroy=hashlist_destroy_implemented
};


/*Usage
hashlist hashlist1;
hashlist_ops.hashlist_initialize(&hashlist1);
hashlist_ops.hashlist_add(&hashlist1,"A",3,4,4);
hashlist_ops.hashlist_add(&hashlist1,"N",3,4,4);
hashlist_ops.hashlist_add(&hashlist1,"N",3,4,4);
hashlist_ops.hashlist_display(&hashlist1);	
hash* h1 = hashlist_ops.hashlist_findhash(&hashlist1,"key");	
*/
