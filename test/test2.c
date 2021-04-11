#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LIST BEGIN */

typedef void *element_type;

typedef struct ListNode {
  struct ListNode *next;
  element_type data;
  int elem_size;
} ListNode;

typedef struct {
  ListNode *head;
  int list_size;
} List;

List *create_list();
int list_insert(List *list, int index, element_type data, int elem_size);
int list_append(List *list, element_type data, int elem_size);
int list_delete(List *list, int index);
ListNode *list_get_address(List *list, int index);
int list_get_elem(List *list, int index, element_type *data);
int list_travers(List *list);
ListNode *list_create_node(element_type data, int elem_size);
int list_is_empty(List *list);
int list_test();

List *create_list() {
  List *list;
  element_type elem = 0;

  list = (List *)malloc(sizeof(List));

  if (!list) {
    puts("create list error");
  }

  list->head = list_create_node(elem, sizeof(double));
  if (!list->head) {
    puts("create list error");
    free(list);
    return 0;
  }

  list->head->next = 0;
  list->list_size = 0;

  return list;
}
int list_insert(List *list, int index, element_type data, int elem_size) {
  ListNode *new_node;
  ListNode *berfore_node;
  element_type elem;

  if (!(index >= 0 && index <= list->list_size)) {
    puts("index out range");
    return 0;
  }

  berfore_node = list_get_address(list, index - 1);
  if (!berfore_node) {
    puts("can not insert elem");
    return 0;
  }

  if (elem_size == sizeof(double)) {
    elem = (double *)malloc(sizeof(double));
  } else if (elem_size == sizeof(char)) {
    elem = (char *)malloc(sizeof(char));
  } else {
    puts("elem not surpport");
    return 0;
  }

  if (!elem) {
    return 0;
  }

  memcpy(elem, data, elem_size);

  new_node = list_create_node(elem, elem_size);

  if (!new_node) {
    puts("create new node error");
    return 0;
  }

  new_node->next = berfore_node->next;
  berfore_node->next = new_node;

  list->list_size++;

  return 1;
}
int list_append(List *list, element_type data, int elem_size) {
  int res;
  res = list_insert(list, list->list_size, data, elem_size);
  return res;
}

int list_delete(List *list, int index) {
  ListNode *before_node;
  ListNode *deleted_node;

  before_node = list_get_address(list, index - 1);
  if (!before_node) {
    puts("delete node not exist");
    return 0;
  }

  deleted_node = before_node->next;
  before_node->next = deleted_node->next;

  list->list_size--;

  free(deleted_node);

  return 1;
}

/* [-1,0...list_size) */
ListNode *list_get_address(List *list, int index) {
  ListNode *node;
  int a;

  if (!(index >= -1 && index < list->list_size) && list->list_size <= 0) {
    puts("index error");
    return 0;
  }

  a = 0;
  node = list->head;

  while (a <= index && node) {
    node = node->next;
    a++;
  }

  return node;
}

/* [0...list_size) */
int list_get_elem(List *list, int index, element_type *data) {
  ListNode *temp;

  if (!(index >= 0 && index < list->list_size)) {
    puts("index out range");
    return 0;
  }

  temp = list_get_address(list, index);
  if (!temp) {
    return 0;
  }

  *data = temp->data;

  return temp->elem_size;
}

int list_travers(List *list) {
  ListNode *temp;
  int a = 0;

  if (list->list_size <= 0) {
    puts("list is empty!!!");
    return 0;
  }

  for (temp = list->head->next; temp; temp = temp->next) {
    if (temp->elem_size == sizeof(double)) {
      printf("%lf ", *(double *)temp->data);
    } else if (temp->elem_size == sizeof(char)) {
      printf("%c ", *(char *)temp->data);
    } else {
      puts("error elem");
      return a;
    }
    a++;
  }

  puts("");
  return a;
}

ListNode *list_create_node(element_type data, int elem_size) {
  ListNode *new_node;

  new_node = (ListNode *)malloc(sizeof(ListNode));

  if (!new_node) {
    return 0;
  }

  new_node->data = data;
  new_node->elem_size = elem_size;
  new_node->next = 0;

  return new_node;
}

int list_is_empty(List *list) { return list->list_size <= 0 ? 1 : 0; }
int list_test() {
  /* char *str = "01"; */
  double d_arr[10] = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  };
  int a;

  List *list = create_list();
  if (!list) {
    return 0;
  }

  /* for (a = 0; str[a] != '\0'; a++) {
    list_append(list, &str[a], sizeof(char));
  } */
  for (a = 0; a < 10; a++) {
    list_append(list, &d_arr[a], sizeof(double));
  }
  list_travers(list);

  list_delete(list, list->list_size - 1);
  list_travers(list);
  list_delete(list, list->list_size - 1);
  list_travers(list);
}
/* LIST END */

/* STACK BEGIN */
typedef struct {
  List *list;
} Stack;

Stack *create_stack();
int stack_pop(Stack *stack);
int stack_top(Stack *stack, element_type *data);
int stack_push(Stack *stack, element_type elem, int elem_size);
int stack_travers(Stack *stack);
int stack_is_empty(Stack *stack);
int stack_test();

Stack *create_stack() {
  Stack *stack;
  stack = (Stack *)malloc(sizeof(Stack));
  if (!stack) {
    puts("create stack error");
    return 0;
  }
  stack->list = create_list();
  if (!stack->list) {
    puts("create stack error");
    return 0;
  }

  return stack;
}
int stack_pop(Stack *stack) {
  int res = 0;
  if (!stack_is_empty(stack)) {
    res = list_delete(stack->list, stack->list->list_size - 1);
  }
  return res;
}
int stack_top(Stack *stack, element_type *data) {
  int res = 0;
  if (!stack_is_empty(stack)) {
    res = list_get_elem(stack->list, stack->list->list_size - 1, data);
  }
  return res;
}
int stack_push(Stack *stack, element_type elem, int elem_size) {
  int res = 0;
  res = list_append(stack->list, elem, elem_size);
  return res;
}
int stack_travers(Stack *stack) {
  int res = 0;
  if (!stack_is_empty(stack)) {
    res = list_travers(stack->list);
  }
  return res;
}
int stack_is_empty(Stack *stack) {
  int res = 0;
  res = list_is_empty(stack->list);
  return res;
}
int stack_test() {
  element_type c_top;
  char *str = "0123";
  int a = 0;
  int res;
  Stack *stack;

  stack = create_stack();

  if (!stack) {
    exit(0);
  }

  for (a = 0; str[a] != '\0'; a++) {
    stack_push(stack, &str[a], sizeof(char));
  }
  stack_travers(stack);

  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);
  stack_pop(stack);

  c_top = (char *)malloc(sizeof(char));
  res = stack_top(stack, &c_top);
  if (res) {
    printf("%c \n", *(char *)c_top);
  }
}
/* STACK END */

/* Calculate BEGIN */
int is_operator(char op);
int operator_priority(char op);
List *string_to_list(char *str);
List *infix_to_postfix(List *infix_expression);
double calculate_postfix_expression(List *postfix_expression);
int calculate_test();

int is_operator(char op) {
  return op == '+' || op == '-' || op == '*' || op == '/' || op == '^';
}
int operator_priority(char op) {
  switch (op) {
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
List *string_to_list(char *str) {
  int i, j, k;
  char c_i, c_j;
  char temp[256];
  double number;

  List *infix_expression;
  infix_expression = create_list();
  if (!infix_expression) {
    return 0;
  }

  for (i = 0; str[i] != '\0'; i++) {
    c_i = str[i];

    if (c_i >= '0' && c_i <= '9') {
      for (j = i + 1; str[j] != '\0'; j++) {
        c_j = str[j];
        if ((c_j >= '0' && c_j <= '9') || c_j == '.') {
          continue;
        } else {
          break;
        }
      }

      for (k = 0; k < j - i; k++) {
        temp[k] = str[k + i];
      }
      temp[k] = '\0';

      number = atof(temp);
      list_append(infix_expression, &number, sizeof(double));

      i = j - 1;
    } else {
      list_append(infix_expression, &c_i, sizeof(char));
    }
  }
  return infix_expression;
}
List *infix_to_postfix(List *infix_expression) {
  int i;
  int l_elem_size;
  int s_elem_size;

  element_type elem;
  element_type s_top_elem;

  double d_elem;
  char c_elem;

  List *postfix_expression;
  Stack *character_stack;

  postfix_expression = create_list();
  character_stack = create_stack();

  elem = (double *)malloc(sizeof(double));
  s_top_elem = (char *)malloc(sizeof(char));

  for (i = 0; i < infix_expression->list_size; i++) {
    l_elem_size = list_get_elem(infix_expression, i, &elem);
    if (l_elem_size <= 0) {
      puts("error ");
      exit(0);
    }

    d_elem = *(double *)elem;
    c_elem = *(char *)elem;

    if (l_elem_size == sizeof(double)) {
      list_append(postfix_expression, elem, l_elem_size);
    } else if (l_elem_size == sizeof(char)) {
      if (c_elem == '(') {
        stack_push(character_stack, elem, l_elem_size);
      } else if (c_elem == ')') {
        s_elem_size = stack_top(character_stack, &s_top_elem);
        if (s_elem_size <= 0) {
          puts("error");
          exit(0);
        }
        while (*(char *)s_top_elem != '(') {
          list_append(postfix_expression, s_top_elem, sizeof(char));
          stack_pop(character_stack);
          stack_top(character_stack, &s_top_elem);
        }
        stack_pop(character_stack);
      } else if (is_operator(c_elem)) {
        s_elem_size = stack_top(character_stack, &s_top_elem);

        while (*(char *)s_top_elem != '(' && !stack_is_empty(character_stack) &&
               operator_priority(*(char *)s_top_elem) >=
                   operator_priority(c_elem)) {
          list_append(postfix_expression, s_top_elem, sizeof(char));
          stack_pop(character_stack);
          stack_top(character_stack, &s_top_elem);
        }

        stack_push(character_stack, elem, sizeof(char));
      }
    } else {
      puts("error");
      exit(0);
    }
  }
  while (!stack_is_empty(character_stack)) {
    stack_top(character_stack, &elem);
    list_append(postfix_expression, elem, sizeof(char));
    stack_pop(character_stack);
  }

  return postfix_expression;
}
double calculate_postfix_expression(List *postfix_expression) {
  element_type elem;
  int i = 0;
  int elem_size;

  char c_elem;
  double d_elem;

  Stack *digital_stack;
  element_type number_1;
  element_type number_2;
  double res;

  number_1 = (double *)malloc(sizeof(double));
  number_2 = (double *)malloc(sizeof(double));
  res = 0;

  digital_stack = create_stack();

  for (i = 0; i < postfix_expression->list_size; i++) {
    elem_size = list_get_elem(postfix_expression, i, &elem);

    c_elem = *(char *)elem;
    d_elem = *(double *)elem;

    if (elem_size == sizeof(double)) {
      stack_push(digital_stack, elem, elem_size);
    } else if (elem_size == sizeof(char)) {
      stack_top(digital_stack, &number_1);
      stack_pop(digital_stack);
      stack_top(digital_stack, &number_2);
      stack_pop(digital_stack);

      switch (c_elem) {
        case '+':
          res = *(double *)number_2 + *(double *)number_1;
          break;
        case '-':
          res = *(double *)number_2 - *(double *)number_1;
          break;
        case '*':
          res = *(double *)number_2 * *(double *)number_1;
          break;
        case '/':
          res = *(double *)number_2 / *(double *)number_1;
          break;
        case '^':
          res = pow(*(double *)number_2, *(double *)number_1);
          break;
        default:
          puts("error: not surpport");
          exit(0);
      }
      stack_push(digital_stack, &res, sizeof(double));
    } else {
      puts("error: not operator");
      exit(0);
    }
  }
  stack_top(digital_stack, &number_1);
  res = *(double *)number_1;

  return res;
}
int calculate_test() {
  char *str = "(1.5+1)*10+2^3/2+20";
  List *infix_expression = 0;
  List *postfix_expression = 0;
  double res = 0;

  puts("Original String");
  puts(str);

  infix_expression = string_to_list(str);
  puts("infix_expression");
  list_travers(infix_expression);
  postfix_expression = infix_to_postfix(infix_expression);
  puts("postfix_expression");
  list_travers(postfix_expression);
  puts("Result");
  res = calculate_postfix_expression(postfix_expression);

  printf("res: %lf\n", res);
}
/* Calculate END */

int main() {
  setbuf(stdout, NULL);

  /* list_test(); */
  /* stack_test(); */
  calculate_test();

  return 0;
}