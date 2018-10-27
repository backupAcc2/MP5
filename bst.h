/* bst.h
 * MP5 - Binary Search Tree
 * ECE 2230 - Fall 2018
 */

 enum balanceoptions {BST, AVL, TWOTHREET};

 typedef void *data_t;
 typedef int bst_key_t;

 typedef struct bst_node_tag {
   data_t data_ptr;
   bst_key_t key;
   struct bst_node_tag *left;
   struct bst_node_tag *right;
 } bst_node_t;

 typedef struct bst_tag {
   bst_node_t *root;
   int tree_policy;  // must be a balanceoptions
   int tree_size;
   int num_recent_key_comparisons;
 } bst_t;
