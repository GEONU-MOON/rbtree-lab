#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

// 빈 Red-Black 트리를 생성하여 반환
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));  // NIL 노드 생성 및 설정
  NIL->color = RBTREE_BLACK;
  
  
  p->root = p->nil = NIL;  // 루트와 NIL 노드 설정

  return p;
}

// 각 노드와 그 자식 노드들의 메모리를 반환하는 함수
void delete(rbtree *t, node_t *p) {
  if (p->left != t->nil)
      delete(t, p->left);
  if (p->right != t->nil)
      delete(t, p->right);

  free(p);
  p = NULL;
}

// 트리를 순회하면서 각 노드의 메모리를 반환하는 함수
void delete_rbtree(rbtree *t) {
  if (t->root != t->nil)
    delete(t, t->root);

  free(t->nil);
  t->nil = NULL;
  free(t);
  t = NULL;
}

/*
 * rbtree_travel 함수는 주어진 노드 p를 기준으로 중위 순회를 수행하여 다음 노드를 반환.
 * 중위 순회는 트리를 왼쪽 자식 노드, 자기 자신, 오른쪽 자식 노드 순서대로 방문하는 방식.
 *
 * 매개변수:
 *   - const rbtree *t: 중위 순회를 수행할 Red-Black 트리
 *   - node_t *p: 중위 순회를 시작할 노드
 *
 * 반환 값:
 *   - node_t *: 중위 순회를 통해 방문한 다음 노드를 반환.
 *
 * 동작:
 *   1. 우선 p 노드의 오른쪽 자식 노드를 node 변수에 할당.
 *   2. 만약 node가 트리의 nil 노드와 같다면, 이는 p 노드가 가장 오른쪽 노드이거나 오른쪽 자식이 없음을 의미함.
 *      따라서 이전 노드로 되돌아가며 가장 가까운 부모 노드를 찾는다.
 *   3. 그 후, 이전 노드의 부모 노드의 오른쪽 자식이 현재 노드인 경우에는 부모 노드를 계속해서 이동.
 *      그렇지 않은 경우에는 이전 노드의 부모 노드를 반환하고 함수를 종료.
 *   4. node의 왼쪽 자식 노드가 nil 노드가 아닐 때까지 계속해서 왼쪽으로 이동하며 가장 작은 값을 가진 노드를 찾는다.
 *   5. 가장 작은 값을 가진 노드를 반환.
 *
 * 이 함수는 Red-Black 트리에서 중위 순회를 수행하면서 다음 노드를 찾는 역할을 한다.
 */
node_t *rbtree_travel(const rbtree *t, node_t *p) {
  node_t *node = p->right;

  if (node == t->nil) {
    node = p;
    while (node != t->nil) {
      if (node->parent->right == node)
        node = node->parent;
      else
        return node->parent;                      
    }
  }
  while (node->left != t->nil)
    node = node->left;

  return node;
}

/*
 * left_rotate 함수는 주어진 노드를 기준으로 좌회전을 수행.
 *
 * 매개변수:
 *   - rbtree *t: 좌회전을 수행할 Red-Black 트리
 *   - node_t *node: 좌회전을 수행할 노드
 *
 * 동작:
 *   1. 주어진 노드의 부모와 조부모를 찾는다.
 *   2. 노드와 조부모를 양방향으로 연결.
 *   3. 부모가 루트인 경우, 노드를 새로운 루트로 설정.
 *   4. 그렇지 않고 조부모의 왼쪽 자식인 경우, 노드를 조부모의 왼쪽 자식으로 설정.
 *   5. 그렇지 않으면, 노드를 조부모의 오른쪽 자식으로 설정.
 *   6. 부모의 부모를 노드로 설정.
 *   7. 노드의 오른쪽 자식을 부모의 왼쪽 자식으로 설정.
 *   8. 노드의 왼쪽 자식의 부모를 부모로 설정.
 *   9. 노드의 왼쪽 자식으로 부모를 설정.
 *
 * 이 함수는 Red-Black 트리에서 주어진 노드를 기준으로 좌회전을 수행.
 */
void left_rotate(rbtree *t, node_t *node) {
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;

  node->parent = grand_parent;
  if (parent == t->root)
    t->root = node;
  else if (grand_parent->left == parent)
    grand_parent->left = node;
  else
    grand_parent->right = node;
  
  parent->parent = node;
  parent->right = node->left;
  node->left->parent = parent;
  node->left = parent;
}

/*
 * right_rotate 함수는 주어진 노드를 기준으로 우회전을 수행.
 *
 * 매개변수:
 *   - rbtree *t: 우회전을 수행할 Red-Black 트리
 *   - node_t *node: 우회전을 수행할 노드
 *
 * 동작:
 *   1. 주어진 노드의 부모와 조부모를 찾는다.
 *   2. 노드와 조부모를 양방향으로 연결.
 *   3. 부모가 루트인 경우, 노드를 새로운 루트로 설정.
 *   4. 그렇지 않고 조부모의 왼쪽 자식인 경우, 노드를 조부모의 왼쪽 자식으로 설정.
 *   5. 그렇지 않으면, 노드를 조부모의 오른쪽 자식으로 설정.
 *   6. 부모의 부모를 노드로 설정.
 *   7. 노드의 왼쪽 자식을 부모의 오른쪽 자식으로 설정.
 *   8. 노드의 오른쪽 자식의 부모를 부모로 설정.
 *   9. 노드의 오른쪽 자식으로 부모를 설정.
 *
 * 이 함수는 Red-Black 트리에서 주어진 노드를 기준으로 우회전을 수행.
 */

void right_rotate(rbtree *t, node_t *node) {
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;

  node->parent = grand_parent;
  if (parent == t->root)
    t->root = node;
  else if (grand_parent->left == parent)
    grand_parent->left = node;
  else
    grand_parent->right = node;

  parent->parent = node;
  parent->left = node->right;
  node->right->parent = parent;
  node->right = parent;
}

// 두 노드의 색깔을 교환하는 함수
void exchange_color(node_t *a, node_t *b) {
    int temp_a_color = a->color;
    int temp_b_color = b->color;

    a->color = temp_b_color;

    if (temp_a_color == RBTREE_BLACK) {
        b->color = RBTREE_BLACK;
    } else {
        b->color = RBTREE_RED;
    }
}

/*
 * rbtree_insert_fixup 함수는 Red-Black 트리에 새로운 노드를 삽입한 후 규칙을 유지하기 위해 수행.
 *
 * 매개변수:
 *   - rbtree *t: 규칙을 유지해야 할 Red-Black 트리
 *   - node_t *node: 삽입된 새로운 노드
 *
 * 동작:
 *   1. 새로 삽입된 노드가 루트인 경우, 해당 노드의 색을 검은색으로 설정하고 종료.
 *   2. 삽입된 노드의 부모 노드의 색이 검은색인 경우에는 규칙이 이미 유지되므로 종료.
 *   3. 삽입된 노드의 부모가 빨간색인 경우, 삼촌 노드의 색을 확인.
 *   4. CASE 1: 삼촌 노드가 빨간색인 경우, 부모와 삼촌의 색을 검은색으로 변경하고, 부모의 부모(즉, 할아버지)의 색을 빨간색으로 변경.
 *             그 후 할아버지 노드를 삽입된 노드로 설정하고 재귀적으로 rbtree_insert_fixup 함수를 호출.
 *   5. CASE 2, 3: 삼촌 노드가 검은색인 경우, 삽입된 노드, 부모 노드, 할아버지 노드의 위치와 색을 기준으로 회전 및 색 변경을 수행.
 *                이 과정에서 CASE 2와 CASE 3을 구분하여 처리.
 *
 * 이 함수는 Red-Black 트리에 노드를 삽입한 후 규칙을 유지하는 역할.
 */
void rbtree_insert_fixup(rbtree *t, node_t *node) {
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *uncle;

  // 루트일 경우 루트를 검은색으로 설정
  if (node == t->root) {
    node->color = RBTREE_BLACK;
    return;
  }

  // 부모가 검은색인 경우에는 규칙이 유지되므로 종료
  if (parent->color == RBTREE_BLACK)
    return;

  int is_right = (node == parent->right);
  int is_parent_left = (parent == grand_parent->left);

  // 삼촌 노드 설정
  if (is_parent_left)
    uncle = grand_parent->right;
  else
    uncle = grand_parent->left;

  // CASE 1: 삼촌이 빨간색인 경우
  if (uncle->color == RBTREE_RED) {
    parent->color = uncle->color = RBTREE_BLACK;
    grand_parent->color = RBTREE_RED;
    rbtree_insert_fixup(t, grand_parent);
    return;
  }

  // CASE 2, 3: 삼촌이 검은색인 경우
  if (is_right) { // 현재 노드가 오른쪽
    if (is_parent_left) {
      left_rotate(t, node);
      right_rotate(t, node);
      exchange_color(node, node->right); 
    } else {
      left_rotate(t, parent);
      exchange_color(parent, parent->left);
    }
  } else {  // 현재 노드가 왼쪽
    if (is_parent_left) {
      right_rotate(t, parent);
      exchange_color(parent, parent->right);  // 우회전 후 색 변경
    } else {
      right_rotate(t, node);
      left_rotate(t, node);
      exchange_color(node, node->left);
    }
  }
}

/*
 * rbtree_insert 함수는 Red-Black 트리에 새로운 키를 삽입하는 함수.
 *
 * 매개변수:
 *   - rbtree *t: 새로운 키를 삽입할 Red-Black 트리
 *   - const key_t key: 삽입할 새로운 키 값
 *
 * 반환값:
 *   - node_t *: 삽입된 노드를 가리키는 포인터
 *
 * 동작:
 *   1. 새로운 노드를 할당하고 트리를 순회하여 삽입할 위치를 찾는다.
 *   2. 삽입할 위치를 찾으면 해당 위치에 새로운 노드를 연결.
 *   3. 새로운 노드의 색을 빨간색으로 설정.
 *   4. 새로운 노드의 부모를 지정하고, 자식 노드를 트리의 NIL 노드로 초기화.
 *   5. 삽입된 노드가 루트인 경우, 색을 검은색으로 변경하고 함수를 종료.
 *   6. 그렇지 않은 경우, rbtree_insert_fixup 함수를 호출하여 Red-Black 트리의 규칙을 유지.
 *   7. 삽입된 노드를 가리키는 포인터를 반환.
 *
 * 이 함수는 Red-Black 트리에 새로운 키를 삽입하고 규칙을 유지하는 역할.
 */
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *temp = (node_t *)calloc(1, sizeof(node_t));
  node_t *node = t->root;

  // 트리를 순회하며 삽입할 위치를 찾음
  while (node != t->nil) {  // 노드의 자식이 빌 때 까지
    if (key < node->key) {
      if (node->left == t->nil) {
        node->left = temp;
        break;
      }
      node = node->left;
    } else {
      if (node->right == t->nil) {
        node->right = temp;
        break;
      }
      node = node->right;
    }
  }

  temp->color = RBTREE_RED;
  temp->key = key;
  temp->parent = node;
  temp->left = temp->right = t->nil;

  // 루트일 경우 색을 검은색으로 변경하고 종료
  if (node == t->nil) {
    t->root = temp;
    temp->color = RBTREE_BLACK;
    return t->root;
  }

  rbtree_insert_fixup(t, temp);
  
  return t->root;
}

/*
 * rbtree_find 함수는 Red-Black 트리에서 주어진 키 값을 가진 노드를 찾는 함수.
 *
 * 매개변수:
 *   - const rbtree *t: 노드를 찾을 Red-Black 트리
 *   - const key_t key: 찾고자 하는 키 값
 *
 * 반환값:
 *   - node_t *: 주어진 키 값을 가진 노드를 가리키는 포인터
 *              (키 값이 존재하지 않는 경우 NULL 반환)
 *
 * 동작:
 *   1. 주어진 키 값을 가진 노드를 찾기 위해 트리를 순회.
 *   2. 현재 노드의 키 값이 찾고자 하는 키 값과 같으면 해당 노드를 반환.
 *   3. 현재 노드의 키 값이 찾고자 하는 키 값보다 작으면 왼쪽 자식 노드로 이동.
 *   4. 현재 노드의 키 값이 찾고자 하는 키 값보다 크면 오른쪽 자식 노드로 이동.
 *   5. 트리를 순회하면서 해당 키 값을 가진 노드를 찾을 때까지 반복.
 *   6. 찾지 못한 경우 NULL을 반환.
 *
 * 이 함수는 Red-Black 트리에서 주어진 키 값을 가진 노드를 찾아 반환.
 */
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *node = t->root;

  while (node != t->nil) {
    if (key == node->key) {
      return node;
    } else if (key < node->key)
      node = node->left;
    else
      node = node->right;
  }

  return NULL;
}

// Red-Black 트리에서 가장 작은 값을 가진 노드를 찾는 함수
node_t *rbtree_min(const rbtree *t) {
  node_t *node = t->root;

  while (node->left != t->nil)
    node = node->left;

  return node;
}

// Red-Black 트리에서 가장 큰 값을 가진 노드를 찾는 함수
node_t *rbtree_max(const rbtree *t) {
  node_t *node = t->root;

  while (node->right != t->nil)
    node = node->right;

  return node;
}

/*
 * rbtree_erase_fixup 함수는 Red-Black 트리에서 노드를 삭제한 후 규칙을 유지하는 함수.
 *
 * 매개변수:
 *   - rbtree *t: 노드가 삭제된 Red-Black 트리
 *   - node_t *p: 삭제된 노드를 대체한 노드의 부모 노드
 *   - int is_remove_left: 삭제된 노드가 부모 노드의 왼쪽 자식인지 여부를 나타내는 플래그
 *
 * 동작:
 *   1. 대체된 노드의 색이 빨간색인 경우 해당 노드의 색을 검은색으로 변경하고 종료.
 *   2. 대체된 노드의 색이 검은색인 경우, 해당 노드의 형제 노드를 확인.
 *   3. 형제 노드가 빨간색인 경우, 부모 노드와 형제 노드의 색을 교환하고 회전을 수행.
 *   4. 형제 노드와 형제 노드의 자식 노드의 색을 확인하여 규칙에 따라 회전 및 색 변경을 수행.
 *   5. 규칙을 만족할 때까지 부모 노드를 기준으로 재귀적으로 함수를 호출.
 *
 * 이 함수는 Red-Black 트리에서 노드를 삭제한 후 규칙을 유지.
 */

void rbtree_erase_fixup(rbtree *t, node_t *p, int is_remove_left) {
  node_t *replaced;
  if (is_remove_left) {
    replaced = p->left;
  } else {
    replaced = p->right;
  }

  if (replaced->color == RBTREE_RED) {
    replaced->color = RBTREE_BLACK;
    return;
  }

  node_t *sibling;
  if (is_remove_left) {
    sibling = p->right;
  } else {
    sibling = p->left;
  }

  if (sibling->color == RBTREE_RED) {
    exchange_color(p, sibling);
    if (is_remove_left)
      left_rotate(t, sibling);
    else
      right_rotate(t, sibling);

    rbtree_erase_fixup(t, p, is_remove_left);
    return;
  }

  node_t *left, *right;
  
  if (is_remove_left) {
    left = sibling->left;
    right = sibling->right;
  } else {
    left = sibling->right;
    right = sibling->left;
  }

  if (left->color == RBTREE_BLACK && right->color == RBTREE_BLACK) {
    sibling->color = RBTREE_RED;

    if (p != t->root) {
        if (p == p->parent->left) {
            rbtree_erase_fixup(t, p->parent, 1);
        } else {
            rbtree_erase_fixup(t, p->parent, 0);
        }
    }
  } else if (left->color == RBTREE_RED && right->color == RBTREE_BLACK) {
    exchange_color(sibling, left);
    if (is_remove_left) {
        right_rotate(t, left);
    } else {
        left_rotate(t, left);
    }
    rbtree_erase_fixup(t, p, is_remove_left);
    return;
  } else if (right->color == RBTREE_RED) {
    exchange_color(p, sibling);
    if (is_remove_left) {
        left_rotate(t, sibling);
    } else 
    {
        right_rotate(t, sibling);
    }
    right->color = RBTREE_BLACK;
    return;
  }
}

/*
 * rbtree_erase 함수는 Red-Black 트리에서 노드를 삭제하는 함수.
 *
 * 매개변수:
 *   - rbtree *t: 노드가 삭제된 Red-Black 트리
 *   - node_t *p: 삭제할 노드를 가리키는 포인터
 *
 * 반환값:
 *   - int: 삭제가 성공한 경우 0을 반환.
 *
 * 동작:
 *   1. 삭제할 노드의 대체 노드를 설정.
 *   2. 삭제할 노드가 두 개의 자식을 모두 가진 경우 대체 노드를 오른쪽 서브트리에서 최소값으로 설정.
 *   3. 대체 노드를 삭제할 노드의 위치로 이동.
 *   4. 대체 노드가 새로운 자식을 가리키도록 부모 노드와 연결.
 *   5. 만약 삭제된 노드가 루트 노드인 경우, 대체된 노드를 새로운 루트로 설정하고 색을 검은색으로 변경.
 *   6. 만약 삭제된 노드의 색이 검은색인 경우, 삭제 후 규칙을 유지하기 위해 rbtree_erase_fixup 함수를 호출.
 *
 * 이 함수는 Red-Black 트리에서 노드를 삭제하고 규칙을 유지.
 */
int rbtree_erase(rbtree *t, node_t *p) {
  node_t *node;
  node_t *parent, *replace;
  int is_remove_left, is_remove_black;

  if (p->left != t->nil && p->right != t->nil) {
    node = rbtree_travel(t, p);
    replace = node->right;
    p->key = node->key;
  } else {
    node = p;
    if (node->right != t->nil) {
        replace = node->right;
    } else {
        replace = node->left;
    }
  }

  parent = node->parent;

  if (node == t->root) {
    t->root = replace;
    t->root->color = RBTREE_BLACK;
    free(node);
    node = NULL;
    return 0;
  }

  is_remove_black = node->color;
  if (parent->left == node) {
    is_remove_left = 1;
    parent->left = replace;
  } else {
    is_remove_left = 0;
    parent->right = replace;
  }
  replace->parent = parent;
  free(node);
  node = NULL;

  if (is_remove_black)
    rbtree_erase_fixup(t, parent, is_remove_left);

  return 0;
}

/*
 * rbtree_to_array 함수는 Red-Black 트리의 모든 노드를 배열로 변환하는 함수.
 *
 * 매개변수:
 *   - const rbtree *t: Red-Black 트리
 *   - key_t *arr: 노드 키값을 저장할 배열
 *   - const size_t n: 배열의 크기
 *
 * 반환값:
 *   - int: 배열로 변환하는 과정에서 발생한 오류가 없으면 0을 반환.
 *
 * 동작:
 *   1. Red-Black 트리의 가장 작은 값을 가진 노드를 찾는다.
 *   2. 가장 작은 값을 가진 노드의 키값을 배열의 첫 번째 요소에 저장.
 *   3. rbtree_travel 함수를 사용하여 다음으로 작은 값을 가진 노드를 찾는다.
 *   4. 노드가 더 이상 존재하지 않거나 배열의 크기에 도달할 때까지 3단계를 반복.
 *   5. 배열로 변환하는 과정에서 발생한 오류가 없으면 0을 반환.
 *
 * 이 함수는 Red-Black 트리의 모든 노드를 배열로 변환.
 */
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *node = rbtree_min(t);
  arr[0] = node->key;
 
  for (int i = 1; i < n; i++) {
    node = rbtree_travel(t, node);
    if (node == t->nil)
      break;

    arr[i] = node->key;
  }
  
  return 0;
}
