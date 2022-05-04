#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  
  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  p->root = nilNode;
  
  return p;
}


void left_rotation(rbtree *t, node_t *x){   
    node_t *y = x->right;
    x->right = y->left;
    if(y->left != t->nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil){
        t->root = y;
    }
    else if (x== x->parent->left){
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;

} 

void right_rotation(rbtree *t, node_t *y) {

    node_t *x = y->left;
    y->left = x->right;
    if(x->right != t->nil){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == t->nil){
        t->root = x;
    }
    else if (y== y->parent->left){
        y->parent->left = x;
    }
    else{
        y->parent->right = x;

    }
    x->right = y;
    y->parent = x;
}



void free_node(rbtree *t, node_t *x){
    // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
    if (x->left != t->nil)
      free_node(t, x->left);
    if (x->right != t->nil)
      free_node(t, x->right);
    free(x);
    x = NULL;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    free_node(t, t->root);
  free(t->nil);
  free(t);
}



void rbtree_insert_fixup(rbtree* t, node_t *z){
    while(z->parent->color == RBTREE_RED){
        if(z->parent == z->parent->parent->left){ //z의 parent가 왼쪽 자식 노드이면
          node_t *uncle = z->parent->parent->right;
          if (uncle->color == RBTREE_RED){ // 경우 1 : 삼촌의 색이 RED이면
            z->parent->color = RBTREE_BLACK; // 부모의 색을 검은색
            uncle->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED; // 할아버지의 색깔을 빨간색으로
            z= z->parent->parent;// ??? 왜 할아버지를 가리키지? : 할아버지 밑으로 업데이트 끝났고 다시 while문이 돌아야함
            } 
          else {// uncle color가 블랙
              if (z == z->parent->right){ // 경우 2, 3 : uncle color가 black일 때,
                z = z->parent;
                left_rotation(t,z);
              }
              z->parent->color = RBTREE_BLACK;
              z->parent->parent->color = RBTREE_RED;
              right_rotation(t, z->parent->parent);
          }  


          
        }
        // left => right로 방향바꿈
        else {
            node_t *uncle = z->parent->parent->left;
            if (uncle->color == RBTREE_RED){
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z= z->parent->parent;
            }
            else {
              if (z== z->parent->left){
                  z = z->parent;
                  right_rotation(t,z);

              }  
              z->parent->color = RBTREE_BLACK;
              z->parent->parent->color = RBTREE_RED;
              left_rotation(t, z->parent->parent);
            }

        }
    }
    t->root->color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *y = t->nil;
    node_t *x = t->root;

    while (x != t->nil){
        y = x;
        if (x->key == key){
            printf("같은 키가 있습니다.\n");
            x = x->right;
            // 포인터가 들어가면 값이고, 아니면 주소가 아닌가
        }
        else if (key < x->key){
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    //x가 null을 가리키면 그때가 z를 삽입할 때!!

    node_t *z = (node_t *)calloc(1, sizeof(node_t));
    z->key = key;
    z->parent = y; 
    if (y == t->nil)
        t->root = z;

    else if (z->key < y->key)
       y->left = z;

    else
        y->right = z;

    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_RED;
    rbtree_insert_fixup(t,z);
    return z;
} 



node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t * current = t->root; // current라는 포인터 변수를 선언하는 것이고, 
                              //current에는 주소가 담겨야하니 root의 주소가 담겨야 맞다
  while (current != t->nil) {
      if (current->key == key)
        return current;
      
      if (current->key < key)
        current = current->right;
      else
        current = current->left;
  }

  return NULL;                 // 값이 없을 수도 있기 때문에 0을 return 해야한다.
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t * current = t->root;
  if (current == t->nil){
      return NULL;
  }
  while (current->left != t->nil){
      current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t * current = t->root;
  if (current == t->nil){
      return NULL;
  }
  while (current->right != t->nil){
      current = current->right;
  }
  return current;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){ 
    if (u->parent == t->nil){
        t->root = v;

    }
    else if (u == u->parent->left){
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
    node_t *w;  //경우 1 : x의 형제 w가 RED일 경우
    while (x != t->root && x->color == RBTREE_BLACK){
        if(x== x->parent->left){    
            w = x->parent->right;   // w를 정의해준다
            if (w->color == RBTREE_RED){    // w의 색깔이 RED라면
                w->color = RBTREE_BLACK;        //w와 x의 부모 색깔을 변경하고
                x->parent->color = RBTREE_RED;
                left_rotation(t, x->parent);   //좌회전
                w = x->parent->right; // 새로운 w를 정의해준다
            }
        
            // 경우 2 : x의 형제 w는 흑색 w의 자식 둘 다 BLACK일 경우
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
                w->color = RBTREE_RED; // w의 색깔을 RED로 변경
                x = x->parent;     //x.p를 새로운 x에두고 while문을 반복한다
            }
            // 경우 3 : x의 형제 w는 흑색 w의 왼쪽 자식 RED, 오른쪽 자식 BLACK일 경우
            else 
            {
                if (w->right->color == RBTREE_BLACK){
                     w->left->color = RBTREE_BLACK;  //회전을 하기 위한 색깔 변경(균형을 맞추기 위해)
                     w->color = RBTREE_RED;
                     right_rotation(t, w);            // 우회전
                     w = x->parent->right;
            }
                // 경우 4 : x의 형제 w는 흑색 w의 오른쪽 자식 RED    
                w->color = x->parent->color;     // 회전하기 위해서 색깔을 바꿔준다, w는 부모의 색깔
                x->parent->color = RBTREE_BLACK; // 부모 색깔 BLACK
                w->right->color = RBTREE_BLACK;  // w의 오른쪽 자식 색깔 BLACK
                left_rotation(t, x->parent);     // 좌회전
                x = t->root;  // x를 루트로 정의해서 while문 끝나도록 해준다.(레드블랙트리 특성 만족)
            }       
        }                     
        else{       //경우 5 ~ 8 : 반대로 시행
            w = x->parent->left;
            if (w->color == RBTREE_RED){
                w->color =  RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotation(t, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else {
                if (w->left->color == RBTREE_BLACK){
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotation(t, w);
                    w = x->parent->left;
                    }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotation(t, x->parent);
                x = t->root;
            }
        }

    }

     x->color = RBTREE_BLACK;
}



int rbtree_erase(rbtree *t, node_t *z){
    node_t *y = z;         // z의 빈자리를 대체할 수를 가진 노드
    node_t *x;             // 살려야 될 z의 자식 노드 (많아봐야 1개, 있을 수도 없을 수도 있다.)
    color_t y_ori_color = y->color; // y의 원래 색깔(즉, 삭제될 색깔, y는 나중에 z의 색깔을 물려받음)
    
    if (z->left == t->nil){ // 오른쪽 자식밖에 없는 경우 and 자식이 둘 다 없는 경우
        x = z->right;       //오른쪽 자식을 x라 놓고
        rbtree_transplant(t, z, z->right);  // z값을 x값으로 바꿔준다
    }
    else if (z->right == t->nil){   // 왼쪽 자식밖에 없는 경우
        x = z->left;                // 왼쪽 자식을 x라 놓고
        rbtree_transplant(t, z, z->left); // z값을 x값으로 바꿔준다
    }

    else{   // 자식이 둘 있는 경우
        y = z->right;      // z의 오른쪽 자식 중 최솟값을 찾는 루틴
        while(y->left != t->nil){   //y의 왼쪽 자식이 nil이 될 때까지 찾는다.
            y = y -> left;
        }            
        y_ori_color = y->color;     // y의 원래 색깔(즉, 삭제될 색깔)
        x = y->right;               // y의 오른쪽 자식의 값(있을수도 있고, 없을 수도 있다.)
        if (y->parent == z)         // y의 부모가 z라면 
          x->parent = y;            // x의 부모가 y이다 --> 경우의 수 있을 수 있다.

        else{                       
            rbtree_transplant(t, y, y->right); // y의 부모가 z가 아니라면, y값을 x값으로 바꿔준다.      
            y->right = z->right;               // z의 right 값은 x의 값이다.
            y->right->parent = y;              // x의 부모는 y이다.
        }
        rbtree_transplant(t,z,y);              // z와 y를 바꾼다
        y->left = z->left;                     // z 왼쪽 자식 = y의 왼쪽 자식
        y->left->parent = y;                   // y의 왼쪽 자식의 부모는 y이다
        y->color = z->color;                   // z의 색깔을 y가 물려받음

    }

    free(z);                                   // z를 아예 삭제
    if (y_ori_color == RBTREE_BLACK)           // y의 색깔이 검은색이면
    rbtree_delete_fixup(t, x);                // RBTREE 조건에 맞게 고쳐줘야함

    return 0;
}



void inorder_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count){
    // 
    if(curr == t->nil){
        return;
    }
    inorder_to_array(t, curr->left, arr, n, count);
    if (*count < n) {
        arr[(*count)++] = curr->key;
    }
    else return;
    inorder_to_array(t, curr->right, arr, n, count);    
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  
  if (t->root == t->nil){
      return 0;
  }
  
  size_t cnt = 0;
  inorder_to_array(t, t->root, arr, n, &cnt);
  
  return 0;
}