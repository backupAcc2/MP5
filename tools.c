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

    bst_node_t *tree_node = tree->root;
    while(tree_node)
    {
      if (key < tree_node->key)
        { tree_node = tree_node->left; }
      else if (key > tree_node->key)
        { tree_node = tree_node->right; }
      else if (key == tree_node->key)
        { return tree_node->data_ptr; }

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


/*
 * Function bst_destruct
 * Frees all nodes in the tree including the header block for the tree
 */
void bst_destruct (bst_t * tree){
    bst_node_t *node = tree->root;
    if (node) { destruct_helper(node); }
    free(tree);
}

/*
 * Helper function to recursively free all nodes in the tree
 */
void destruct_helper(bst_node_t *node){

    if(node->left) { destruct_helper(node->left); }
    if(node->right) { destruct_helper(node->right); }

    free(node->data_ptr);
    free(node);
}


/******************************************************************************
 *  Function bst_insert
 * Inserts the memory block pointed to by data_t new_data with given key into our BST
 * Returns 0 if the key is already in the BST and replaces that key with new_data
 * Returns 1 if new_data was added to the tree in a new node
 * Uses BST search policy
 */
int bst_insert(bst_t *tree, bst_key_t key, data_t new_data){

    int comparisons = 1;

    // search the tree to see if it already contains key
    data_t dataPtr = bst_access(tree, key);
    if (dataPtr) // there is already a node with this key
      {
        //need to free this data and replace with new one
        // find the node with this key and set its data_ptr to new_data
        bst_node_t *rover = tree->root;
        while (rover->key != key){
            if (rover->key < key)
              {  rover = rover->right;}
            else if (rover->key > key)
              { rover = rover->left; }
        }
        data_t temp = rover->data_ptr;
        rover->data_ptr = new_data;
        free(temp);
        return 0;
      }

    // add node to tree
    bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
    // check if no current nodes
    if (tree->tree_size == 0)
    {
      new_node->data_ptr = new_data;
      new_node->key = key;
      new_node->left = NULL;
      new_node->right = NULL;
      tree->root = new_node;

    } else { // find where to insert node
        bst_node_t *tree_node = tree->root;
        while(tree_node)
        {
          comparisons += 2;
          if (key < tree_node->key && tree_node->left)
               { tree_node = tree_node->left; }
          else if (key > tree_node->key && tree_node->right)
               { tree_node = tree_node->right; }

          else  // insert in the new node
          {
            if (key < tree_node->key){
                tree_node->left = new_node;
                new_node->data_ptr = new_data;
                new_node->key = key;
                new_node->left = NULL;
                new_node->right = NULL;
              }
            else if (key > tree_node->key)
              {
                tree_node->right = new_node;
                new_node->data_ptr = new_data;
                new_node->key = key;
                new_node->left = NULL;
                new_node->right = NULL;
              }
            tree_node = NULL; // break out of while loop
          }

        } // ends while loop
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
      bst_node_t *swap;
      data_t temp;

  // make sure we are removing a legitimate node
      data_t data = bst_access(tree, key);
      if (data)
      {
          // find the bst_node_t with this data
          bst_node_t *node = tree->root;
          bst_node_t *prev = node;
          while (node->data_ptr != data){
            if(node->key < key) {
              prev = node;
              node = node->right;
            } else if (node->key > key) {
                 prev = node;
                  node = node->left;
            }
          }
      // "node" should now equal the node which holds our data
      // "prev" should point to the previous node

         if (tree->tree_size == 1){
            temp = node->data_ptr;
            tree->tree_size--;
            free(node);
            tree->root = NULL;
            return temp;
         }


     // this node will either have 1) no children, 2) one child, 3) two children
     // or 4) two children and grandchildren
         if(!node->left && !node->right)  // no children
         {
              temp = node->data_ptr;
              if (prev->key < node->key) { prev->right = NULL; }
              else if (prev->key > node->key) {prev->left = NULL; }
              free(node);
              node = NULL;
              tree->tree_size--;
              return temp;

         } else if(node->left && !node->right){ // only left child
          // we are going to swap the nodes' data and remove lower node
              swap = node->left;
              node->left = swap->left;
              node->right = swap->right;

         } else if (node->right && !node->left){ // only right child
              swap = node->right;
              node->left = swap->left;
              node->right = swap->right;

         } else {
              swap = node;
              if (node->left->right) {
                   swap = node->left;
                  // find the greatest grandchild
                   while (swap->right){
                      prev = swap;
                      swap = swap->right;
                   }
                   prev->right = swap->left;

              } else if(node->right->left){
                   swap = node->right;
                 // find the smallest grandchild
                   while(swap->left){
                      prev = swap;
                      swap = swap->left;
                   }
                   prev->left = swap->right;

              } else {  // must be swapped with a child
                   swap = node->left;
                   node->left = swap->left; // should be NULL
                // do not alter right child
              }

         }

       temp = node->data_ptr;
       node->data_ptr = swap->data_ptr;
       node->key = swap->key;
       free(swap);
       tree->tree_size--;
       return temp;

   } // ends if (data)


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

     int I = 0;
     bst_node_t *node = tree->root;
     find_int_path(node, &I, 0);
     return I;
}

/*
 * Helper function for bst_int_path_len
 * Helps recursively calculate the internal path length
 */
 void find_int_path(bst_node_t *node, int *I, int level){
    if (node->left){
      level++;
      *I += level;
      find_int_path(node->left, I, level);
      level--;
    }

    if (node->right){
      level++;
      *I += level;
      find_int_path(node->right, I, level);
    }

}

/*
 * Function bst_debug_print_tree
 * Prints out the values of the binary search tree
 */
void bst_debug_print_tree(bst_t *tree){
    int distance = 0;
    print_tree(tree->root, distance);
}

/*
 * Helper function to print out binary tree
 */
void print_tree(bst_node_t *node, int distance){
    if (node == NULL) { return; }
    int increment = 5;
    distance += increment;

    print_tree(node->right, distance);
    puts("");
    for (int i = increment; i < distance; i++){  printf(" ");  }
    printf("%d\n", node->key);
    print_tree(node->left, distance);
}

void bst_debug_validate(bst_t *tree){
    assert(tree);

  // make sure the number of null leafs equals number of nodes + 1
    int leaf_count = 0;
    bst_count_leaves(tree->root, &leaf_count);
    assert(leaf_count == tree->tree_size  + 1);

}

/*
 * Helper function for bst_debug_validate to count the number of leaves
 * Leaves are the null pointers at the end of each child
 */
void bst_count_leaves(bst_node_t *node, int *count){
    if (node == NULL) { *count += 1; return; }
    bst_count_leaves(node->right, count);
    bst_count_leaves(node->left, count);
}
