/*
 ============================================================================
 Name        : LinearOrderTree.c
 Author      : Jinay Mehta
 Description : Implements a structure that maintains a linear order
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define LEFT 0
#define RIGHT 1
typedef int key_st;


typedef int object_t;

typedef struct tr_n_t {key_st      key;
                struct tr_n_t   *left;
                struct tr_n_t  	*right;
                struct tr_n_t 	*back;
                int height;} o_t;


#define HASHSIZE 300010
o_t* hash[HASHSIZE];

void store_leaf(o_t* leaf)
{
	int i = leaf->key;
	hash[i] = leaf;
}

o_t *get_leaf(key_st key)
{
	return(hash[key]);
}

void remove_leaf(key_st key)
{
	hash[key] = 0;
}

#define BLOCKSIZE 256

o_t *currentblock = NULL;
int    size_left;
o_t *free_list = NULL;
int nodes_taken = 0;
int nodes_returned = 0;



/*	Height Stack Implementation : Starts */
#define MAXSIZE 200
int indx;
o_t* stack[MAXSIZE];

void create_stack()
{
	indx = 0;
}

void remove_stack()
{
  int s=0;
	for(s = 0;s<MAXSIZE;s++)
	stack[s]=NULL;
  indx = 0;
}

int stack_empty()
{
	return (indx==0);
}

int push(o_t *x)
{
	if(indx<MAXSIZE)
	{
 		stack[indx++] = x;
		return (0);
	}
	else
	return (-1);
}

o_t* pop()
{
	return(stack[--indx]);
}

/*	Height Stack Implementation : ENDS	*/


o_t *get_node()
{ o_t *tmp;
  nodes_taken += 1;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list -> right;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock =
                (o_t *) malloc( BLOCKSIZE * sizeof(o_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
  return( tmp );
}

void return_node(o_t *node)
{  node->right = free_list;
   free_list = node;
   nodes_returned +=1;
}



o_t *create_order(void)
{  o_t *tmp_node;
   tmp_node = get_node();
   tmp_node->left = NULL;
   tmp_node->back = NULL;
   return( tmp_node );
}




o_t *search_node(o_t *tmp_node, key_st key)
{
	if(tmp_node->key == key && tmp_node->right == NULL)
		return tmp_node;
	else if(tmp_node->right != NULL)
	{
		search_node(tmp_node->left, key);
		search_node(tmp_node->right, key);
	}
	return NULL;
}


void left_rotation(o_t *n)
{

	o_t *tmp_node;
	key_st tmp_key;
	/* copying node in tmp */
	tmp_node = n->left;
	tmp_key = n->key;
	/* Rotation */
	n->left = n->right;
	n->key = n->right->key;
	n->right = n->left->right;
	n->right->back = n;
	n->left->right = n->left->left;
	/* Copying back the values*/
	n->left->left = tmp_node;
	n->left->key = tmp_key;
	n->left->left->back = n->left;

}


void right_rotation(o_t *n)
{

  o_t *tmp_node;
  key_st tmp_key;
  /* copying node into tmp */
  tmp_node = n->right;
  tmp_key = n->key;
  /* Rotation */
  n->right = n->left;
  n->key = n->left->key;
  n->left = n->right->left;
  n->left->back = n;
  n->right->left = n->right->right;
  /* copying back values */
  n->right->right = tmp_node;
  n->right->key = tmp_key;
  n->right->right->back = n->right;
}

void insert_leaf(o_t *ord , key_st new_key, int new_pos)
{
	o_t *old_leaf, *new_leaf, *tmp_node;
	tmp_node = ord;
	old_leaf = get_node();
	old_leaf->left = tmp_node->left;
	old_leaf->key = tmp_node->key;
	old_leaf->right  = NULL;
	old_leaf->height = 0;


	new_leaf = get_node();
	new_leaf->left = (o_t*)get_node();
	new_leaf->key = new_key;
	new_leaf->right  = NULL;
	new_leaf->height = 0;

	old_leaf->back = new_leaf->back = tmp_node;
	store_leaf(old_leaf);
	store_leaf(new_leaf);
	if( new_pos == RIGHT )
	{
		tmp_node->left  = old_leaf;
		tmp_node->right = new_leaf;
		tmp_node->key = new_key;
	}
	else
	{
		tmp_node->left  = new_leaf;
		tmp_node->right = old_leaf;
	}

	tmp_node->height = 1;
}


void rebalance_tree(o_t *ord)
{
	o_t *tmp_node;
	tmp_node = ord;
	int finished = 0;
    while(tmp_node!=NULL)
    {
    int tmp_height, old_height;

    old_height = tmp_node->height;

    if(tmp_node->left->height - tmp_node->right->height == 2)
    {
  	  if(tmp_node->left->left->height - tmp_node->right->height == 1)
  	  {
  		  right_rotation(tmp_node);
  		  tmp_node->right->height = tmp_node->right->left->height + 1;
  		  tmp_node->height = tmp_node->right->height + 1;
  	  }
  	  else
  	  {
  		  left_rotation(tmp_node->left);
  		  right_rotation(tmp_node);
  		  tmp_height = tmp_node->left->left->height;
  		  tmp_node->left->height = tmp_height+1;
  		  tmp_node->right->height = tmp_height+1;
  		  tmp_node->height = tmp_height +2;
  	  }
    }
    else if(tmp_node->left->height - tmp_node->right->height == -2)
    {
  	  if((tmp_node->right->right->height - tmp_node->left->height == 1))
  	  {
  		  left_rotation(tmp_node);
  		  tmp_node->left->height = tmp_node->left->right->height +1;
  		  tmp_node->height = tmp_node->left->height + 1;
  	  }
  	  else
  	  {

  		  right_rotation(tmp_node->right);
  		  left_rotation(tmp_node);
  		  tmp_height = tmp_node->right->right->height;
  		  tmp_node->left->height = tmp_height+1;
  		  tmp_node->right->height = tmp_height +1;
  		  tmp_node->height = tmp_node->height +2;
  	  }
    }
    else
  	  /* update height even if there was no rotation */

  	  if(tmp_node->left->height > tmp_node->right->height)
  		  tmp_node->height = tmp_node->left->height+1;
  	  else
  		  tmp_node->height = tmp_node->right->height+1;

    if(tmp_node->height == old_height)
  	  finished =1;

    tmp_node = tmp_node->back;
   }

}

void insert_top(o_t *ord, key_st new_key)
{
	o_t *tmp_node;
	if( ord->left == NULL )
	{  ord->left = (o_t*)get_node();
	ord->key  = new_key;
	ord->height = 0;
	ord->right  = NULL;
	ord->back = NULL;
	store_leaf(ord);
	}
	else
	{

	tmp_node = ord;
	while( tmp_node->right != NULL )
	{

		tmp_node = tmp_node->right;
	}
	insert_leaf(tmp_node, new_key, RIGHT);
	rebalance_tree(tmp_node->back);
	}
}


void insert_bottom(o_t *ord, key_st new_key)
{
	o_t *tmp_node;
	if( ord->left == NULL )
	{  ord->left = (o_t*)get_node();
	ord->key  = new_key;
	ord->height = 0;
	ord->right  = NULL;
	ord->back = NULL;
	store_leaf(ord);
	}
	else
	{
	tmp_node = ord;
	while( tmp_node->right != NULL )
	{
		tmp_node = tmp_node->left;
	}
	insert_leaf(tmp_node, new_key, LEFT);
	rebalance_tree(tmp_node->back);
	}
}


int insert_before(o_t *ord, key_st new_key, key_st finger)
{
	o_t *tmp_node;
	if( ord->left == NULL )
	{  ord->left = (o_t*)get_node();
	ord->key  = new_key;
	ord->height = 0;
	ord->right  = NULL;
	ord->back = NULL;
	store_leaf(ord);
	}
	else
	{
	tmp_node = get_leaf(finger);
	if(tmp_node == NULL) return -1;
	insert_leaf(tmp_node, new_key, LEFT);
	rebalance_tree(tmp_node->back);
	}
	return 0;
}


int insert_after(o_t *ord, key_st new_key, key_st finger)
{
	o_t *tmp_node;
	if( ord->left == NULL )
	{  ord->left = (o_t*)get_node();
	ord->key  = new_key;
	ord->height = 0;
	ord->right  = NULL;
	ord->back = NULL;
	store_leaf(ord);
	}
	else
	{
	tmp_node = get_leaf(finger);
	if(tmp_node==NULL) return -1;
	insert_leaf(tmp_node, new_key, RIGHT);

	rebalance_tree(tmp_node->back);
	}
	return 0;
}

object_t *delete_o(o_t *ord, key_st delete_key)
{
	o_t *tmp_node, *upper_node, *other_node;
	   object_t *deleted_object;
	   if( ord->left == NULL )
	      return( NULL );
	   else if( ord->right == NULL )
	   {  if(  ord->key == delete_key )
	      {  deleted_object = (object_t *) ord->left;
	      ord->left = NULL;
	         return( deleted_object );
	      }
	      else
	         return( NULL );
	   }
	   else
	   {
		   tmp_node = get_leaf(delete_key);
		   if(tmp_node == NULL)
			   return NULL;
		   upper_node = tmp_node->back;
		   if(upper_node->left == tmp_node)
		   other_node = upper_node->right;
		   else
		   other_node = upper_node->left;

		   upper_node->key   = other_node->key;
		   upper_node->left  = other_node->left;
		   upper_node->right = other_node->right;
		   upper_node->height = other_node->height;

		   deleted_object = (object_t *) tmp_node->left;
		   remove_leaf(delete_key);
		   store_leaf(upper_node);
		   rebalance_tree(upper_node->back);
		   //return_node( tmp_node );
		  // return_node( other_node );
		   return deleted_object;
	   }
return NULL;
}



int is_before(o_t *ord, key_st a, key_st b)
{
int i;
o_t *key_a, *key_b;
key_a = get_leaf(a);
key_b = get_leaf(b);
if(key_a == NULL || key_b == NULL) return 0;
remove_stack();
create_stack();

while(key_a!=NULL)
{
	push(key_a);
	key_a = key_a->back;
}

while(key_b!=NULL)
{
	for(i=0;i<indx;i++)
	if(key_b == stack[i])
	{
		if(key_b->left == stack[i-1])
		return 1;
		return 0;
	}
	key_b = key_b->back;
}
	return 0;
}


long p(long q)
{ return( (1247*q +104729) % 300007 );
}

int main()
{  long i; o_t *o;
   printf("starting \n");
   o = create_order();

      for(i=100000; i>=0; i-- )
      insert_bottom( o, p(i) );

   for(i=100001; i< 300007; i+=2 )
   {  insert_after(o, p(i+1), p(i-1) );
      insert_before( o, p(i), p(i+1) );
   }
   printf("inserted 300000 elements. ");

   for(i = 250000; i < 300007; i++ )
      delete_o( o, p(i) );
   printf("deleted 50000 elements. ");

   insert_top( o, p(300006) );
   for(i = 250000; i < 300006; i++ )
      insert_before( o, p(i) , p(300006) );
   printf("reinserted. now testing order\n");

   for( i=0; i < 299000; i +=42 )
   {  if( is_before( o, p(i), p(i+23) ) != 1 )
      {  printf(" found error (1) \n"); exit(0);
      }
   }
   for( i=300006; i >57; i -=119 )
   {
	   if( is_before( o, p(i), p(i-57) ) != 0 )
      {  printf(" found error (0) \n"); exit(0);
      }
   }
   printf("finished. no problem found.\n");

   return 0;
}

