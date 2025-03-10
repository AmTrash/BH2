#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "avl.h"


int FindMinPath(struct AVLTree *tree, TYPE *path);
void printBreadthFirstTree(struct AVLTree *tree);
void _findMinCostPath(struct AVLnode *node, int currentCost, int *minCost, 
	TYPE *currentPath, TYPE *minPath, int depth, int *minDepth);
/* -----------------------
The main function
  param: argv = pointer to the name (and path) of a file that the program reads for adding elements to the AVL tree
*/
int main(int argc, char** argv) {

	FILE *file;
	int len, i;
	TYPE num; /* value to add to the tree from a file */
	struct timeval stop, start; /* variables for measuring execution time */
	int pathArray[100];  /* static array with values of nodes along the min-cost path of the AVL tree. The means that the depth of the AVL tree cannot be greater than 100 which is  sufficient for our purposes*/

	struct AVLTree *tree;
	
	tree = newAVLTree(); /*initialize and return an empty tree */
	
	file = fopen(argv[1], "r"); 	/* filename is passed in argv[1] */
	assert(file != 0);

	/* Read input file and add numbers to the AVL tree */
	while((fscanf(file, "%i", &num)) != EOF){
		addAVLTree(tree, num);		
	}
	/* Close the file  */
	fclose(file);

        printf("\nThe AVL tree has %d nodes.\n",tree->cnt);
	
	printf("\nPrinting the AVL tree breadth-first : \n");
	printBreadthFirstTree(tree);

	gettimeofday(&start, NULL);

	/* Find the minimum-cost path in the AVL tree*/
	len = FindMinPath(tree, pathArray);
	
	gettimeofday(&stop, NULL);

	/* Print out all numbers on the minimum-cost path */
	printf("\n\nThe minimum-cost path has %d nodes printed top-down from the root to the leaf: \n", len);
	for(i = 0; i < len; i++)
		printf("%d ", pathArray[i]);
	printf("\n");

	printf("\nYour execution time to find the mincost path is %f microseconds\n", (double) (stop.tv_usec - start.tv_usec));

        /* Free memory allocated to the tree */
	deleteAVLTree(tree); 
	
	return 0;
}
/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments: 
        tree = pointer to the tree,
        path = pointer to array that stores values of nodes along the min-cost path, 
   Output: return the min-cost path length 

   pre: assume that
       path is already allocated sufficient memory space 
       tree exists and is not NULL
*/
int FindMinPath(struct AVLTree *tree, TYPE *path)
{
    if (tree == NULL || tree->root == NULL) return 0;

    int minCost = INT_MAX;
    int minDepth = 0;
    TYPE currentPath[100];

    _findMinCostPath(tree->root, 0, &minCost, currentPath, path, 0, &minDepth);

    return minDepth;
}

/* Helper function to find the minimum-cost path recursively */
void _findMinCostPath(struct AVLnode *node, int currentCost, int *minCost, 
    TYPE *currentPath, TYPE *minPath, int depth, int *minDepth) {

    if (node == NULL) return;

    currentPath[depth] = node->val;

    if (depth > 0) {
        currentCost += abs(node->val - currentPath[depth - 1]);
    }

    if (node->left == NULL && node->right == NULL) {
        if (currentCost < *minCost) {
            *minCost = currentCost;
            *minDepth = depth + 1;
            int i;
            for (i = 0; i <= depth; i++) {
                minPath[i] = currentPath[i];
            }
        }
        return;
    }

    _findMinCostPath(node->left, currentCost, minCost, currentPath, minPath, depth + 1, minDepth);
    _findMinCostPath(node->right, currentCost, minCost, currentPath, minPath, depth + 1, minDepth);
}



/* -----------------------
Printing the contents of an AVL tree in breadth-first fashion
  param: pointer to a tree
  pre: assume that tree was initialized well before calling this function*/

void printBreadthFirstTree(struct AVLTree *tree) {
    if (tree == NULL || tree->root == NULL) return;

    struct AVLnode **queue = (struct AVLnode **)malloc(tree->cnt * sizeof(struct AVLnode *));
    assert(queue != NULL);

    int front = 0, rear = 0;
    queue[rear++] = tree->root;

    while (front < rear) {
        struct AVLnode *current = queue[front++];
        printf("%d ", current->val);

        if (current->left != NULL) queue[rear++] = current->left;
        if (current->right != NULL) queue[rear++] = current->right;
    }

    free(queue);
    printf("\n");
}



