#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 2.08
/* LIST */
typedef void* ELEMENT_TYPE;
typedef struct node {
  struct node* next;
  ELEMENT_TYPE element;
  int element_size;
} LIST_NODE;

typedef struct {
  LIST_NODE* head;
  LIST_NODE* tail;
  int list_size;
} LIST;

LIST* list_create();
int list_insert(LIST* list, int index, ELEMENT_TYPE elem, int element_size);
int list_append(LIST* list, ELEMENT_TYPE elem, int element_size);
int list_delete(LIST* list, int index);
int list_is_empty(LIST* list);
int list_traverse(LIST* list);
int list_get_element(LIST* list, int index, ELEMENT_TYPE* elem);
LIST_NODE* list_get_node(LIST* list, int index);
LIST_NODE* list_create_node(ELEMENT_TYPE elem, int element_size);
int list_test();

/* LIST */

/* Stack */
typedef struct {
  LIST* list;
} STACK;

STACK* stack_create();
int stack_pop(STACK* stack);
int stack_push(STACK* stack, ELEMENT_TYPE elem, int elem_size);
int stack_top(STACK* list, ELEMENT_TYPE* elem);
int stack_is_empty(STACK* stack);
int stack_traverse(STACK* stack);
int stack_test();

STACK* stack_create() {
  STACK* stack;
  stack = (STACK*)malloc(sizeof(STACK));
  if (!stack) {
    return 0;
  }
  stack->list = list_create();
  if (!stack->list) {
    return 0;
  }

  return stack;
}
int stack_pop(STACK* stack) {
  return list_delete(stack->list, stack->list->list_size - 1);
}
int stack_push(STACK* stack, ELEMENT_TYPE elem, int elem_size) {
  return list_append(stack->list, elem, elem_size);
}
int stack_top(STACK* stack, ELEMENT_TYPE* elem) {
  return list_get_element(stack->list, stack->list->list_size - 1, elem);
}
int stack_is_empty(STACK* stack) { return list_is_empty(stack->list); }
int stack_traverse(STACK* stack) { return list_traverse(stack->list); }

int stack_test() {
  STACK* stack;
  char* string = "0123456789abcdef";
  double d;
  int i;
  stack = stack_create();
  if (!stack) {
    return 0;
  }

  for (i = 0; string[i] != '\0'; i++) {
    stack_push(stack, &string[i], sizeof(char));
  }
  stack_traverse(stack);

  d = 1.0;
  stack_push(stack, &d, sizeof(double));
  stack_traverse(stack);

  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_traverse(stack);
}
/* Stack */

/* Calculate */
int is_operator(char character);
int operator_priority(char operator);
LIST* expression_to_list(char* expression);
LIST* infix_to_postfix(LIST* infix_expression);
double calculate_postfix_expression(LIST* postfix_expression);
/* Calculate */

int main() {
  char* expression = "(1+2)*10+2^2/2";
  LIST* infix_expression;
  LIST* postfix_expression;
  double result;

  setbuf(stdout, NULL);

  infix_expression = expression_to_list(expression);
  list_traverse(infix_expression);
  postfix_expression = infix_to_postfix(infix_expression);
  list_traverse(postfix_expression);
  // result = calculate_postfix_expression(postfix_expression);

  // list_test();
  // stack_test();
}

/* LIST Impl */
LIST* list_create() {
  LIST* list;
  ELEMENT_TYPE element;
  list = (LIST*)malloc(sizeof(LIST));
  if (!list) {
    return 0;
  }
  element = (double*)malloc(sizeof(double));
  if (!element) {
    return 0;
  }

  list->head = list_create_node(element, sizeof(double));
  list->tail = list->head;
  list->head->next = NULL;
  list->list_size = 0;

  return list;
}
int list_insert(LIST* list, int index, ELEMENT_TYPE elem, int element_size) {
  LIST_NODE* insert_node;
  LIST_NODE* before_node;

  if (!(index >= 0 && index <= list->list_size)) {
    return 0;
  }

  before_node = list_get_node(list, index - 1);
  if (!before_node) {
    return 0;
  }

  insert_node = list_create_node(elem, element_size);
  if (!insert_node) {
    return 0;
  }

  if (list_is_empty(list) && index == 0) {
    insert_node->next = list->tail->next;
    list->tail->next = insert_node;
    list->tail = insert_node;
  } else if (!list_is_empty(list)) {
    insert_node->next = before_node->next;
    before_node->next = insert_node;
  } else {
    return 0;
  }

  list->list_size++;

  return 1;
}
int list_append(LIST* list, ELEMENT_TYPE elem, int element_size) {
  LIST_NODE* append_node;

  append_node = list_create_node(elem, element_size);
  if (!append_node) {
    return 0;
  }

  append_node->next = list->tail->next;
  list->tail->next = append_node;
  list->tail = append_node;

  list->list_size++;

  return 1;
}
int list_delete(LIST* list, int index) {
  LIST_NODE* deleted_node = NULL;
  LIST_NODE* before_node = NULL;

  if (list_is_empty(list)) {
    return 0;
  }

  if (!(index >= 0 && index < list->list_size)) {
    return 0;
  }

  before_node = list_get_node(list, index - 1);
  if (!before_node) {
    return 0;
  }

  deleted_node = before_node->next;
  if (deleted_node) {
    before_node->next = deleted_node->next;
  } else {
    before_node->next = NULL;
  }
  list->list_size--;

  if (list->list_size == 0) {
    list->tail = list->head;
  }
  return 1;
}

int list_is_empty(LIST* list) { return !list->list_size; }
int list_traverse(LIST* list) {
  LIST_NODE* temp;
  for (temp = list->head->next; temp; temp = temp->next) {
    switch (temp->element_size) {
      case sizeof(double):
        printf("%lf ", *(double*)temp->element);
        break;
      case sizeof(char):
        printf("%c ", *(char*)temp->element);
        break;
      default:
        puts("error");
        return 0;
    }
  }
  puts("");
  return 1;
}
int list_get_element(LIST* list, int index, ELEMENT_TYPE* elem) {
  LIST_NODE* node;

  if (list_is_empty(list)) {
    return 0;
  }

  if (!(index >= 0 && index < list->list_size)) {
    return 0;
  }

  node = list_get_node(list, index);
  if (!node) {
    return 0;
  }

  *elem = node->element;

  return node->element_size;
}
/**
 * @brief get node address
 *
 * @param list list
 * @param index [0,list_size)
 * @return LIST_NODE* node
 */
LIST_NODE* list_get_node(LIST* list, int index) {
  LIST_NODE* node;
  int a;

  if (list_is_empty(list)) {
    return 0;
  }

  if (!(index >= -1 && index < list->list_size)) {
    return 0;
  }

  node = list->head;
  a = 0;

  while (a <= index && node) {
    node = node->next;
    a++;
  }

  return node;
}
LIST_NODE* list_create_node(ELEMENT_TYPE elem, int element_size) {
  LIST_NODE* new_node = NULL;
  ELEMENT_TYPE element;
  new_node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
  if (!new_node) {
    return 0;
  }

  switch (element_size) {
    case sizeof(double):
      element = (double*)malloc(sizeof(double));
      break;
    case sizeof(char):
      element = (char*)malloc(sizeof(char));
      break;
    default:
      puts("error");
      return 0;
  }
  if (elem) {
    memcpy(element, elem, element_size);
  }

  new_node->element = element;
  new_node->element_size = element_size;
  new_node->next = NULL;

  return new_node;
}
int list_test() {
  LIST* list;
  int i;
  double element;

  char* string = "0123456789abcdef";

  list = list_create();
  if (!list) {
    return 0;
  }

  for (i = 0; string[i] != '\0'; i++) {
    list_append(list, &string[i], sizeof(char));
  }
  list_traverse(list);

  element = 2.0;
  list_insert(list, 0, &element, sizeof(double));
  list_insert(list, 0, &element, sizeof(double));
  list_insert(list, 0, &element, sizeof(double));
  list_insert(list, 0, &element, sizeof(double));
  list_insert(list, list->list_size, &element, sizeof(double));
  list_insert(list, list->list_size, &element, sizeof(double));
  list_insert(list, list->list_size, &element, sizeof(double));
  list_insert(list, list->list_size, &element, sizeof(double));
  list_traverse(list);

  list_delete(list, 0);
  list_delete(list, 0);
  list_delete(list, 0);
  list_delete(list, 0);
  list_delete(list, 0);
  list_delete(list, 0);
  list_delete(list, list->list_size - 1);
  list_delete(list, list->list_size - 1);
  list_delete(list, list->list_size - 1);
  list_delete(list, list->list_size - 1);
  list_delete(list, list->list_size - 1);
  list_traverse(list);
}
/* LIST Impl */

/* Calculate Impl */
int is_operator(char character) {
  switch (character) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
      return 1;
  }
  return 0;
}
int operator_priority(char operator) {
  switch (operator) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    case '^':
      return 3;
    default:
      return 0;
  }
}
LIST* expression_to_list(char* expression) {
  int i, j, k;
  char c_i, c_j;
  char c_k[256];
  double number;
  LIST* infix_expression;

  infix_expression = list_create();
  if (!infix_expression) {
    return 0;
  }

  for (i = 0; expression[i] != '\0'; i++) {
    c_i = expression[i];
    if (c_i >= '0' && c_i <= '9') {
      for (j = i + 1; expression[j] != '\0'; j++) {
        c_j = expression[j];
        if ((c_j >= '0' && c_j <= '9') || c_j == '.') {
          continue;
        } else {
          break;
        }
      }

      for (k = 0; k < j - i; k++) {
        c_k[k] = expression[i + k];
      }
      c_k[k] = '\0';

      number = atof(c_k);

      list_append(infix_expression, &number, sizeof(double));

      i = j - 1;

    } else {
      list_append(infix_expression, &c_i, sizeof(char));
    }
  }

  return infix_expression;
}
LIST* infix_to_postfix(LIST* infix_expression) {
  STACK* character_stack;
  LIST* postfix_expression;

  ELEMENT_TYPE l_element;
  ELEMENT_TYPE s_element;
  int l_element_size;
  int s_element_size;

  character_stack = stack_create();
  postfix_expression = list_create();
  l_element = (double*)malloc(sizeof(double));
  s_element = (double*)malloc(sizeof(double));

  int i;
  for (i = 0; i < infix_expression->list_size; i++) {
    l_element_size = list_get_element(infix_expression, i, &l_element);
    if (l_element_size == sizeof(double)) {
      list_append(postfix_expression, l_element, sizeof(double));
    } else if (l_element_size == sizeof(char)) {
      if (*(char*)l_element == '(') {
        stack_push(character_stack, l_element, sizeof(char));
      } else if (*(char*)l_element == ')') {
        s_element_size = stack_top(character_stack, &s_element);

        while (*(char*)s_element != '(' && s_element_size == sizeof(char)) {
          list_append(postfix_expression, s_element, sizeof(char));
          stack_pop(character_stack);
          s_element_size = stack_top(character_stack, &s_element);
        }

        stack_pop(character_stack);
      } else if (is_operator(*(char*)l_element)) {
        s_element_size = stack_top(character_stack, &s_element);

        while (s_element_size == sizeof(char) &&
               !stack_is_empty(character_stack) && *(char*)s_element != '(' &&
               operator_priority(*(char*)l_element) <=
                   operator_priority(*(char*)s_element)) {
          list_append(postfix_expression, s_element, sizeof(char));
          stack_pop(character_stack);
          s_element_size = stack_top(character_stack, &s_element);
        }

        stack_push(character_stack, l_element, sizeof(char));
      } else {
        puts("error");
        return NULL;
      }
    }
    puts("-----");
    stack_traverse(character_stack);
    list_traverse(postfix_expression);
    puts("-----");
    puts("");
  }
  while (!stack_is_empty(character_stack)) {
    s_element_size = stack_top(character_stack, &s_element);
    list_append(postfix_expression, s_element, sizeof(char));
    stack_pop(character_stack);
  }

  return postfix_expression;
}

double calculate_postfix_expression(LIST* postfix_expression);
/* Calculate Impl */