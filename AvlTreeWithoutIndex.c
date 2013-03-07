#include <stdio.h>
#include <stdlib.h>


typedef struct line_text_t {
                int ls;
                int rs;
                int height;
                struct line_text_t   *left;
                struct line_text_t  *right;
                } text_t;


/*	Stack Implementation : STARTS	*/

#define MAXSIZE 200

/* Height Stack */
int st_indx_ht;
text_t* stack_ht[MAXSIZE];
int st_indx_chld;
text_t* stack_chld[MAXSIZE];

void create_stack_ht()
{
	st_indx_ht = 0;
}

void remove_stack_ht()
{
  int s=0;
	for(s = 0;s<MAXSIZE;s++)
	stack_ht[s]=NULL;
  st_indx_ht = 0;
}

int stack_empty_ht()
{
	return (st_indx_ht==0);
}

int push_ht(text_t *x)
{
	if(st_indx_ht<MAXSIZE)
	{
 		stack_ht[st_indx_ht++] = x;
		return (0);
	}
	else
	return (-1);
}

text_t* pop_ht()
{
	return(stack_ht[--st_indx_ht]);
}

/*  Child stack*/

void create_stack_chld()
{
	st_indx_chld = 0;
}

void remove_stack_chld()
{
  int s=0;
	for(s = 0;s<MAXSIZE;s++)
	stack_chld[s]=NULL;
  st_indx_chld = 0;
}

int stack_empty_chld()
{
	return (st_indx_chld==0);
}

int push_chld(text_t *x)
{
	if(st_indx_chld<MAXSIZE)
	{
 		stack_chld[st_indx_chld++] = x;
		return (0);
	}
	else
	return (-1);
}

text_t* pop_chld()
{
	return(stack_chld[--st_indx_chld]);
}

/*	Stack Implementation : ENDS	*/


#define BLOCKSIZE 256   

/*  Function declarations*/
void insert_line(text_t*,int,char*);

text_t *currentblock = NULL;
int    size_left;
text_t *free_list = NULL;
int nodes_taken = 0;
int nodes_returned = 0;

text_t *get_node()
{ text_t *tmp;
  nodes_taken += 1;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list -> right;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock =
                (text_t *) malloc( BLOCKSIZE * sizeof(text_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
  return( tmp );
}

void return_node(text_t *node)
{  node->right = free_list;
   free_list = node;
   nodes_returned +=1;
}

text_t *create_text()
{  
	text_t *tmp_node;
    //char *dummy = "DUMMY NODE";
    tmp_node = get_node();
    tmp_node->left = NULL;
	//tmp_node->right = NULL;
	//tmp_node->ls = 1;
	//tmp_node->rs = 0;
	//tmp_node->height = 0;
    return( tmp_node );
}


int length_text(text_t *txt)
{
	int len = 0;
  if(txt->left==NULL)
  return 0;
  else if(txt->right==NULL)
  return 1;
  else
  len = txt->ls+txt->rs;
	return (len);
}


char *get_line(text_t *txt, int index)
{  text_t *tmp_node;
   if( txt->left == NULL )
     return(NULL);
   else
   {  tmp_node = txt;
      while( tmp_node->right != NULL )
      {   
      if( index <= tmp_node->ls )
			tmp_node = tmp_node->left;
		  else
          {
          index = index - tmp_node->ls;
			tmp_node = tmp_node->right;
		  }
      }
      if( tmp_node->ls == 1 && index<=1)
         return( (char *) tmp_node->left );
      else
         return( NULL );
   }
}



void left_rotation(text_t *txt)
{
  
  text_t *tmp_node;
  tmp_node = txt->left;
  txt->left = txt->right;
  txt->right = txt->left->right;
  txt->left->right = txt->left->left;
  txt->left->left = tmp_node;
  push_chld(txt);
  push_chld(txt->left);

}


void right_rotation(text_t *txt)
{

  text_t *tmp_node;
  tmp_node = txt->right;
  txt->right = txt->left;
  txt->left = txt->right->left;
  txt->right->left = txt->right->right;
  txt->right->right = tmp_node;
  push_chld(txt);
  push_chld(txt->right);

}


void balance_children()
{ 

  text_t *tmp_node;
  while(!stack_empty_chld())
  {
    tmp_node = pop_chld(); 
    tmp_node->ls = tmp_node->left->ls + tmp_node->left->rs;
    tmp_node->rs = tmp_node->right->ls + tmp_node->right->rs;
  }
    remove_stack_chld();
}

void balance_height()
{
	int finished = 0;
	text_t *tmp_node;

 
  while(!stack_empty_ht() && !finished)
	      {
			  int tmp_height,old_height;
			  tmp_node = pop_ht();
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
		}

		remove_stack_ht();
 }


void append_line(text_t *txt, char *new_line)
{
   int index = length_text(txt)+1;
   insert_line(txt,index,new_line); 
}

char *set_line(text_t *txt, int index, char *new_line)
{
	text_t *tmp_node;
  if( txt->left == NULL )
	     return(NULL);
	   else
	   {  tmp_node = txt;
	      while( tmp_node->right != NULL )
	      {   
			if( index<= tmp_node->ls )
					   tmp_node = tmp_node->left;
			 else
			  {
				index = index - tmp_node->ls;
        tmp_node = tmp_node->right;
			  }
	      }
      if( tmp_node->ls == 1 && index<=2)
      {
      tmp_node->left = (text_t *) new_line;
  	  return( (char *) tmp_node->left );
  	  }
      else
      return(NULL);
    }
}

void insert_line(text_t *txt, int new_index, char *new_line)
{
   text_t *tmp_node;

   if( txt->left == NULL )
   {  txt->left = (text_t *) new_line;
      txt->ls  = 1;
      txt->rs = 0;
      txt->height = 0;
      txt->right  = NULL;
   }
   else
   {
	  create_stack_ht();
	  create_stack_chld();
	  tmp_node = txt;
      while( tmp_node->right != NULL )
      {
		  push_ht(tmp_node);
		  push_chld(tmp_node);
		  if( new_index <= tmp_node->ls )
               tmp_node = tmp_node->left;
          else
               {
				new_index = new_index - tmp_node->ls;
        tmp_node = tmp_node->right;	
				}
      }
      /* found the candidate leaf. Test whether key distinct */
      
      /* key is distinct, now perform the insert */
      {  text_t *old_leaf, *new_leaf;
         old_leaf = get_node();
         old_leaf->left = tmp_node->left;
         old_leaf->ls = tmp_node->ls;
         old_leaf->rs = tmp_node->rs;
         old_leaf->right  = NULL;
         old_leaf->height = 0;
         new_leaf = get_node();
         new_leaf->left = (text_t *) new_line;
         new_leaf->ls = 1;
         new_leaf->rs = 0;
         new_leaf->right  = NULL;
         new_leaf->height = 0;
         
         if( new_index > tmp_node->ls)
         {   tmp_node->left  = old_leaf;
             tmp_node->right = new_leaf;
         }
         else
         {   tmp_node->left  = new_leaf;
             tmp_node->right = old_leaf;
         }
         tmp_node->height = 1;
         tmp_node->rs = 1;
      }
      /* rebalance */
		balance_children();
		balance_height();
		balance_children();
	}
 }



char *delete_line(text_t *txt, int index)
{  text_t *tmp_node, *upper_node, *other_node;
   char *deleted_object;
  
   if( txt->left == NULL )
      return( NULL );
   else if( txt->right == NULL)
   {  if(  txt->ls == 1 && txt->rs==0)
      {  deleted_object = (char *) txt->left;
         txt->left = NULL;
         return( deleted_object );
      }
      else
         return( NULL );
   }
   else
   {
	  create_stack_ht();
	  create_stack_chld();
	  tmp_node = txt;
      while( tmp_node->right != NULL )
      {
		  push_ht(tmp_node);
		  push_chld(tmp_node);
		  upper_node = tmp_node;
          if( index <= tmp_node->ls )
          {  tmp_node   = upper_node->left;
             other_node = upper_node->right;
          }
          else
          {  
           index = index - tmp_node->ls;
           tmp_node   = upper_node->right;
           other_node = upper_node->left;
          }
      }
      if( tmp_node->ls !=1)
         return( NULL );
      else
      {  
         upper_node->ls   = other_node->ls;
         upper_node->rs   = other_node->rs;
         upper_node->height = other_node->height;
         upper_node->left  = other_node->left;
         upper_node->right = other_node->right;
         deleted_object = (char *) tmp_node->left;
         pop_ht();
         pop_chld();
         balance_children();
		     balance_height();
		     balance_children();
         return_node( tmp_node );
         return_node( other_node );
         return( deleted_object );
      }
   }
}

void remove_txt(text_t *txt)
{  text_t *current_node, *tmp;
   if( txt->left == NULL )
      return_node( txt );
   else
   {  current_node = txt;
      while(current_node->right != NULL )
      {  if( current_node->left->right == NULL )
         {  return_node( current_node->left );
            tmp = current_node->right;
            return_node( current_node );
            current_node = tmp;
         }
         else
         {  tmp = current_node->left;
            current_node->left = tmp->right;
            tmp->right = current_node;
            current_node = tmp;
         }
      }
      return_node( current_node );
   }
}



int main()
{

  int i, tmp; text_t *txt1, *txt2; char *c;
   printf("starting \n");
   txt1 = create_text();
   
   txt2 = create_text();
   append_line(txt1, "line one" );
   if( (tmp = length_text(txt1)) != 1)
   {  printf("Test 1: length should be 1, is %d\n", tmp); exit(-1);
   }
   append_line(txt1, "line hundred" );
   insert_line(txt1, 2, "line ninetynine" );
   insert_line(txt1, 2, "line ninetyeight" );
   insert_line(txt1, 2, "line ninetyseven" );
   insert_line(txt1, 2, "line ninetysix" );
   insert_line(txt1, 2, "line ninetyfive" );
   for( i = 2; i <95; i++ )
     insert_line(txt1, 2, "some filler line between 1 and 95" );
   if( (tmp = length_text(txt1)) != 100)
   {  printf("Test 2: length should be 100, is %d\n", tmp); exit(-1);
   }
   printf("found at line 1:   %s\n",get_line(txt1,  1));
   printf("found at line 2:   %s\n",get_line(txt1,  2));
   printf("found at line 99:  %s\n",get_line(txt1, 99));
   printf("found at line 100: %s\n",get_line(txt1,100));
   
   for(i=1; i<=10000; i++)
   {  if( i%2==1 )
        append_line(txt2, "A");
      else
        append_line(txt2, "B");
   }
   
   
   if( (tmp = length_text(txt2)) != 10000)
   {  printf("Test 3: length should be 10000, is %d\n", tmp); exit(-1);
   }
   
   c = get_line(txt2, 9876 );
   
   if( *c != 'B')
     {  printf("Test 4: line 9876 of txt2 should be B, found %s\n", c); exit(-1);
   }
   
   for( i= 10000; i > 1; i-=2 )
   {  c = delete_line(txt2, i);
      if( *c != 'B')
      {  printf("Test 5: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
      }
      append_line( txt2, c );
   }
   
   for( i=1; i<= 5000; i++ )
   {  c = get_line(txt2, i);
      if( *c != 'A')
      {  printf("Test 6: line %d of txt2 should be A, found %s\n", i, c); exit(-1);
      }
   }
   for( i=1; i<= 5000; i++ )
     delete_line(txt2, 1 );
   for( i=1; i<= 5000; i++ )
   {  c = get_line(txt2, i);
      if( *c != 'B')
      {  printf("Test 7: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
      }
   }
   
   
   set_line(txt1, 100, "the last line");
   
   for( i=99; i>=1; i-- )
  delete_line(txt1, i );
  
  printf("found at the last line:   %s\n",get_line(txt1,  1));
  remove_txt(txt1);
  remove_txt(txt2);
  return 0; 
}

