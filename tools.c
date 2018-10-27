/* Heath Gerrald
 * MP5 - Binary Search Tree
 * tools.c - contains implementations for functions in lab5.c
 */

 #include "tools.h"

/******************************************************************************
 * Function bst_access
 * Find and return the tree element with the given key
 * Return NULL if not found
 */
data_t bst_access(bst_t *tree, bst_key_t key){

    bst_node_t *tree_rover = tree->root;
    while(tree_rover)
    {
      if (key < tree_rover->key)
        { tree_rover = tree_rover->left; }
      else if (key > tree_rover->key)
        { tree_rover = tree_rover->right; }
      else if (key == tree_rover->key)
        { return tree_rover; }
    }

 return NULL;
}

/******************************************************************************
 *  Function bst_construct
 * Creates a header block for our tree and saves the tree_policy in header
 * tree_size stores current number of keys in tree
 * num_recent_key_comparisons stores number of key comparisons during most
 * recent access, insert, or remove
 */
bst_t* bst_construct(int tree_policy){

    bst_t * tree_header = (bst_t*) malloc(sizeof(bst_t));
    tree_header->tree_policy = tree_policy;
    tree_header->tree_size = 0;
    tree_header->num_recent_key_comparisons = 0;
    tree_header->root = NULL;

    return tree_header;
}

void bst_destruct (bst_t * a){

}


/******************************************************************************
 *  Function bst_insert
 * Inserts the memory block pointed to by data_t new_node with given key into our BST
 * Returns 0 if the key is already in the BST and replaces that key with new_node
 * Returns 1 if new_node was added to the tree
 * Uses BST search policy
 */
int bst_insert (bst_t *tree, bst_key_t key, data_t new_node){

    int comparisons = 1;

    // search the tree to see if it already contains key
    bst_node_t* node_to_insert = bst_access(tree, key);
    if (node_to_insert)
      { node_to_insert->data_ptr = new_node; return 0; }

    // add node to tree
    // check if no current nodes
    if (tree->tree_size == 0)
    {
      node_to_insert->data_ptr = new_node;
      node_to_insert->key = key;
      node_to_insert->left = NULL;
      node_to_insert->right = NULL;
      tree->root = node_to_insert;

    } else { // find where to insert node
        bst_node_t *tree_rover = tree->root;
        bst_node_t* prev;
        while(tree_rover)
        {
          comparisons += 2;
          prev = tree_rover;
          if (key < tree_rover->key)  { tree_rover = tree_rover->left; }
          else if (key > tree_rover->key) {tree_rover = tree_rover->right; }
          else
          {
            if (prev->key < tree_rover->key)
              { prev->left = tree_rover;
                tree_rover->data_ptr = new_node;
                tree_rover->key = key;
              }
            else if (prev->key > tree_rover->key)
              {
                prev->right = tree_rover;
                tree_rover->data_ptr = new_node;
                tree_rover->key = key;
              }
          }
        }
    }

    tree->tree_size++;
    tree->num_recent_key_comparisons = comparisons;
    return 1;
}

/******************************************************************************
 * Function bst_remove
 * Removes the item from the tree with the matching key
 * Returns a pointer to the data_t memory block or NULL if key was not found
 */
data_t bst_remove(bst_t *tree, bst_key_t key){
      bst_node_t* node = bst_access(tree, key);

      if (node)
      {
     // establish a previous pointer
        bst_node_t* prev = tree->root;
        while (prev != node)
        {
          if (prev->left == node || prev->right == node){ }
          else if (node->key > prev->key) { prev = prev->right; }
          else if(node->key < prev->key) { prev = prev->left; }
        }

     // this node will either have 1) no children, 2) one child, 3) two children
     // or 4) two children and grandchildren
       if(!node->left && !node->right)  // no children
       {
         data_t temp = node->data_ptr;
         free(node);
         node = NULL;
         return temp;
     } else if(node->left && !node->right){ // only left child

       }


      }


      return NULL;
}

/******************************************************************************
 * Function bst_size
 * Returns the number of elements in the binary search tree
 */
int bst_size(bst_t *tree){
      return tree->tree_size;
}

/******************************************************************************
 * Function bst_stats
 * Returns the nymber of key comparisons for the most recent call to bst_access,
 * bst_insert, or bst_remove
 */
int bst_stats(bst_t *tree){
      return tree->num_recent_key_comparisons;
}

/******************************************************************************
 * Function bst_int_path_len
 * Returns the internal path length of the tree
 */
int bst_int_path_len(bst_t *tree){
     int n = bst_size(tree);
     return log2(n+1);
}


void bst_debug_print_tree(bst_t *a){

}


void bst_debug_validate(bst_t *a){

}
