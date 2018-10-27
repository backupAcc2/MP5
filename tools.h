/* Heath Gerrald
 * MP5 - tools.h
 * Contains function prototypes for tools.c
 */
#include "bst.h"


 data_t bst_access (bst_t *, bst_key_t);
 bst_t* bst_construct(int tree_policy);
 void bst_destruct (bst_t *);
 int bst_insert (bst_t *, bst_key_t, data_t);
 data_t bst_remove (bst_t *, bst_key_t);
 int bst_size(bst_t *);
 int bst_stats(bst_t *);
 int bst_int_path_len(bst_t *);

 void bst_debug_print_tree(bst_t *);
 void bst_debug_validate(bst_t *);