/* Heath Gerrald
 * MP5 - Binary Search Tree
 * tools.c - contains implementations for functions in lab5.c
 */

 #include "tools.h"
 int item_count = 0;

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


/*
 * Function bst_destruct
 * Frees all nodes in the tree including the header block for the tree
 */
void bst_destruct (bst_t * tree){
    bst_node_t *rover = tree->root;
    destruct_helper(rover);

}

/*
 * Helper function to recursively free all nodes in the tree
 */
void destruct_helper(bst_node_t *rover){

    if(rover->left) { destruct_helper(rover->left); }
    if(rover->right) { destruct_helper(rover->right); }

    free(rover);
}


/******************************************************************************
 *  Function bst_insert
 * Inserts the memory block pointed to by data_t new_node with given key into our BST
 * Returns 0 if the key is already in the BST and replaces that key with new_node
 * Returns 1 if new_node was added to the tree
 * Uses BST search policy
 */
int bst_insert(bst_t *tree, bst_key_t key, data_t new_node){

    int comparisons = 1;

    // search the tree to see if it already contains key
    bst_node_t* node_to_insert = bst_access(tree, key);
    if (node_to_insert) // there is already a node with this key
      { node_to_insert->data_ptr = new_node; return 0; }

    // add node to tree
    node_to_insert = (bst_node_t*)malloc(sizeof(bst_node_t));
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
        while(tree_rover)
        {
          comparisons += 2;
          if (key < tree_rover->key && tree_rover->left)
               { tree_rover = tree_rover->left; }
          else if (key > tree_rover->key && tree_rover->right)
               { tree_rover = tree_rover->right; }

          else  // insert in the new node
          {
            if (key < tree_rover->key)
              { tree_rover->left = node_to_insert;
                node_to_insert->data_ptr = new_node;
                node_to_insert->key = key;
              }
            else if (key > tree_rover->key)
              {
                tree_rover->right = node_to_insert;
                node_to_insert->data_ptr = new_node;
                node_to_insert->key = key;
              }
            tree_rover = NULL; // break out of while loop
          }

        } // ends while loop
    }

    tree->tree_size++;
    tree->num_recent_key_comparisons = comparisons;
    item_count++;
    return 1;
}

/******************************************************************************
 * Function bst_remove
 * Removes the item from the tree with the matching key
 * Returns a pointer to the data_t memory block or NULL if key was not found
 */
data_t bst_remove(bst_t *tree, bst_key_t key){
      bst_node_t *swap, *prev;
      data_t temp;

      bst_node_t *node = (bst_node_t*)bst_access(tree, key);
      if (node)
      {
        if (node != tree->root){
          // find the previous pointer
          prev = tree->root;
          while(prev->left != node && prev->right != node)
          {
             if (prev->key > node->key)
                { prev = prev->left; }
             else if (prev->key < node->key)
                { prev = prev->right; }
          }
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

   } // ends if (node)


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
     bst_node_t *rover = tree->root;
     find_int_path(rover, &I, 0);
     return I;
}

/*
 * Helper function for bst_int_path_len
 * Helps recursively calculate the internal path length
 */
 void find_int_path(bst_node_t *rover, int *I, int level){
    if (rover->left){
      level++;
      *I += level;
      find_int_path(rover->left, I, level);
      level--;
    }

    if (rover->right){
      level++;
      *I += level;
      find_int_path(rover->right, I, level);
    }

}

/*
 * Function bst_debug_print_tree
 * Prints out the values of the binary search tree
 */
void bst_debug_print_tree(bst_t *tree){
    int distance = 0;
    item_count = 0;  // item_count will count the number of nodes in the tree
    print_tree(tree->root, distance);
}

/*
 * Helper function to print out binary tree
 */
void print_tree(bst_node_t *rover, int distance){
    if (rover == NULL) { item_count++; return; }
    int increment = 5;
    distance += increment;

    print_tree(rover->right, distance);
    puts("");
    for (int i = increment; i < distance; i++){  printf(" ");  }
    printf("%d\n", rover->key);
    print_tree(rover->left, distance);
}

void bst_debug_validate(bst_t *tree){
    assert(tree);
    assert(tree->root);
    assert(item_count-1 == tree->tree_size);
  // make sure the keys are all in correct order
  // cant really do this if we can't make this function recursively

}
