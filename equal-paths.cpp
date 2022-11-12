#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

/**
 * @brief Recursive function 
 * 
 * @param root Root node
 * @param leafHeight Height that should be equal for all leaves
 * @param currentHeight Height of current node
 */
bool helper(Node* root, int* leafHeight, int currentHeight);


bool equalPaths(Node * root)
{
    int leafHeight = 0;
    int currentHeight = 0;

    return helper(root, &leafHeight, currentHeight);
}

bool helper(Node* root, int* leafHeight, int currentHeight) {
    if(root == nullptr) return true;

    // check if current node is a leaf node
    if(root->left == nullptr && root->right == nullptr) {
        // first leaf visited
        if(*leafHeight == 0) {
            *leafHeight = currentHeight;
            return true;
        }
        
        // following leaf nodes height (currentHeight) should be equal to leafHeight
        if(currentHeight == *leafHeight) return true;
        else if(currentHeight != *leafHeight) return false;
    }
    // if its not a leaf, recurse until we find one
    return helper(root->right, leafHeight, currentHeight + 1) && helper(root->left, leafHeight, currentHeight + 1);
}

