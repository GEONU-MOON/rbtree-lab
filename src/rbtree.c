#include "rbtree.h"

#include <stdlib.h>

void recursively_free_node(rbtree *t, node_t *node);
void fixup_insert(rbtree *t, node_t *node);
void left_rotate(rbtree *t, node_t *node);
void right_rotate(rbtree *t, node_t *node);
void exchange_color(node_t *a, node_t *b);

// 새로운 Red-Black Tree를 생성하는 함수
rbtree *new_rbtree(void) {
    // rbtree 구조체 메모리 할당 및 초기화
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

    // NIL 노드 생성 및 초기화
    node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
    NIL -> color = RBTREE_BLACK; // NIL 노드는 항상 검은색
    p -> root = p -> nil = NIL; // rbtree의 root와 nil 포인터 설정

    return p; 
}

// Red-Black Tree를 삭제하는 함수
void delete_rbtree(rbtree *t)
{
    node_t *node = t -> root; // 시작 노드는 트리의 루트 노드
    if (node != t -> nil) // 루트 노드가 nil이 아닌 경우에만 삭제 함수 호출
        recursively_free_node(t, node);

    // nil 노드와 rbtree 구조체의 메모리 반환
    free(t->nil);
    free(t);
}

// 재귀적으로 노드 및 그 자식 노드들의 메모리를 해제하는 함수
void recursively_free_node(rbtree *t, node_t *node)
{
    if (node->left != t->nil) // 왼쪽 자식 노드가 nil이 아닌 경우
        recursively_free_node(t, node->left); // 왼쪽 자식 노드 재귀적 해제
    if (node->right != t->nil) // 오른쪽 자식 노드가 nil이 아닌 경우
        recursively_free_node(t, node->right); // 오른쪽 자식 노드 재귀적 해제
    free(node); // 현재 노드의 메모리 해제
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
    new_node->key = key;
    new_node->color = RBTREE_RED;              // 항상 레드로 추가
    new_node->left = new_node->right = t->nil; // 추가한 노드의 자식들을 nil 노드로 설정

    // 새 노드를 삽입할 위치 탐색
    node_t *current = t->root;
    while (current != t->nil)
    {
        if (key < current->key)
        {
            if (current->left == t->nil)
            {
                current->left = new_node; // 새 노드를 왼쪽 자식으로 추가
                break;
            }
            current = current->left;
        }
        else
        {
            if (current->right == t->nil)
            {
                current->right = new_node; // 새 노드를 오른쪽 자식으로 추가
                break;
            }
            current = current->right;
        }
    }

    new_node->parent = current; // 새 노드의 부모 지정

    // root가 nil이면(트리가 비어있으면) 새 노드를 트리의 루트로 지정
    if (current == t->nil)
        t->root = new_node;

    // 불균형 복구
    fixup_insert(t, new_node);

    return new_node;
}

// 삽입 후 불균형 복구 함수
void fixup_insert(rbtree *t, node_t *node)
{
    node_t *parent = node -> parent;
    node_t *grand_parent = parent -> parent;
    node_t *uncle;
    int is_left = node == parent -> left;
    int is_parent_is_left;

    if ( node == t -> root){
        node -> color = RBTREE_BLACK;
        return;
    }

    if( parent -> color == RBTREE_BLACK){
        return;
    }

    is_parent_is_left = grand_parent -> left == parent;
    if (is_parent_is_left) {
        uncle = grand_parent->right;
    } else {
        uncle = grand_parent->left;
    }

    // [CASE 1]: 부모와 부모의 형제가 모두 RED인 경우
    if (uncle->color == RBTREE_RED)
    {
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        fixup_insert(t, grand_parent);
        return;
    }

    if (is_parent_is_left)
    {
        if (is_left)
        // [CASE 2] : 부모의 형제가 BLACK & 부모가 왼쪽 자식 & 현재 노드가 왼쪽 자식인 경우
        {
            right_rotate(t, parent);
            exchange_color(parent, parent->right);
            return;
        }

        // [CASE 3] : 부모의 형제가 BLACK & 부모가 왼쪽 자식 & 현재 노드가 오른쪽 자식인 경우
        left_rotate(t, node);
        right_rotate(t, node);
        exchange_color(node, node -> right);
        return;
    }

    if (is_left)
    {
        // [CASE 3]: 부모의 형제가 BLACK & 부모가 오른쪽 자식 & 현재 노드가 왼쪽 자식인 경우
        right_rotate(t, node);
        left_rotate(t, node);
        exchange_color(node, node->left);
        return;
    }

    // [CASE 2]: 부모의 형제가 BLACK & 부모가 오른쪽 자식 & 현재 노드가 오른쪽 자식인 경우
    left_rotate(t, parent);
    exchange_color(parent, parent->left);
}

// 오른쪽으로 회전하는 함수
void right_rotate(rbtree *t, node_t *node)
{
    node_t *parent = node->parent;
    node_t *grand_parent = parent->parent;
    node_t *node_right = node->right;
    node_t *node_left = node->left;

    if (parent == t->root)
        t->root = node;
    else if (grand_parent->left == parent)
        grand_parent->left = node;
    else
        grand_parent->right = node;

    node->parent = grand_parent;
    parent->parent = node;
    parent->right = node_left;
    node_right->parent = parent;
    node->right = parent;
}

// 왼쪽으로 회전하는 함수
void left_rotate(rbtree *t, node_t *node)
{
    node_t *parent = node->parent;
    node_t *grand_parent = parent->parent;
    node_t *node_left = node->left;
    node_t *node_right = node->right;

    if (parent == t->root)
        t->root = node;
    else if (grand_parent->left == parent)
        grand_parent->left = node;
    else
        grand_parent->right = node;

    node->parent = grand_parent;
    parent->parent = node;
    parent->left = node_left;
    node_left->parent = parent;
    node->left = parent;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
    // TODO: implement erase
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}

// 두 노드의 색을 교환하는 함수
// Parameters:
// - a: 첫 번째 노드 포인터
// - b: 두 번째 노드 포인터
void exchange_color(node_t *a, node_t *b) {
    int tmp = a->color;
    a->color = b->color;
    b->color = tmp;
}
