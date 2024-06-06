#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TreeNode* createNode(int data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct TreeNode* insert(struct TreeNode* root, int data) {
    if (root == NULL) {
        root = createNode(data);
    } else {
        if (data <= root->data) {
            root->left = insert(root->left, data);
        } else {
            root->right = insert(root->right, data);
        }
    }
    return root;
}

int isSame(struct TreeNode* root1, struct TreeNode* root2) {
    if (root1 == NULL && root2 == NULL){
        return 1;
    }
    if (root1 == NULL || root2 == NULL){
        return 0;
    }
    return isSame(root1->left, root2->left) && isSame(root1->right, root2->right);
}

void freeTree(struct TreeNode* node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int isUnique(struct TreeNode** nodeArr, int index){
    for(int i = 0; i < index; i++){
        if (isSame(nodeArr[i], nodeArr[index])) {
            return 0;
        }
    }
    return 1;
}

void isolation(){
    int proto, layers;
    scanf("%d %d", &proto, &layers);
    int arr[proto][layers];
    for(int i = 0; i < proto; i++){
        for(int j = 0; j < layers; j++){
            int random = (rand() % 1000000) + 1;
            arr[i][j] = random;
        }
    }
    //в каждом рядке массива находятся каждый отдельный прототип
    //нужно распихать значения одного ряда в бинарное дерево
    struct TreeNode** nodeArr = (struct TreeNode**)malloc(proto * sizeof(struct TreeNode*));
    for(int i = 0; i < proto; i++){
        nodeArr[i] = NULL; 
    }

    for(int i = 0; i < proto; i++){
        for(int k = 0; k < layers; k++){
            nodeArr[i] = insert(nodeArr[i], arr[i][k]);
        }
    }
    int unique = 0;
    for(int i = 0; i < proto; i++){
            if(isUnique(nodeArr, i) == 1) unique++;           
    }
    printf("%d\n", unique);
    for(int i = 0; i < proto; i++) freeTree(nodeArr[i]);
    free(nodeArr);
    //printf("Node and the left: %d %d", nodeArr[0]->data, nodeArr[0]->left->data);
    //printf("\n");
}
int main(){
    srand(time(NULL));
    isolation();
    return 0;
}