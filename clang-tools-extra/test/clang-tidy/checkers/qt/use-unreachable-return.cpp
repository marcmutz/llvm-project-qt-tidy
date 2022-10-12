// RUN: %check_clang_tidy %s qt-use-unreachable-return %t

#define Q_UNREACHABLE() __builtin_unreachable()
#define Q_UNREACHABLE_RETURN(...) do { Q_UNREACHABLE(); return __VA_ARGS__; } while (0)

void t1() {
  Q_UNREACHABLE();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN() [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN();
  return;
  // CHECK-FIXES: {{^$}}
}

void t1b() {
  Q_UNREACHABLE(); return;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN() [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN();
}

int t2() {
  Q_UNREACHABLE();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN(0) [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN(0);
  return 0;
  // CHECK-FIXES: {{^$}}
}

int t2b() {
  Q_UNREACHABLE(); return 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN(0) [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN(0);
}

int t3() {
  Q_UNREACHABLE();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN((42)) [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN((42));
  return(42);
  // CHECK-FIXES: {{^$}}
}

int t4(int(*fun)()) {
  Q_UNREACHABLE();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN(fun()) [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN(fun());
  return fun();
  // CHECK-FIXES: {{^$}}
}

int t5(int n) {
  switch (n) {
  case 0: return 10;
  default:
    Q_UNREACHABLE();
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: use Q_UNREACHABLE_RETURN(24) [qt-use-unreachable-return]
    // CHECK-FIXES: {{^}}    Q_UNREACHABLE_RETURN(24);
    return 24;
    // CHECK-FIXES: {{^$}}
  }
}

int t5_2(int n) {
  switch (n) {
  case 0: return 10;
  default: {
    Q_UNREACHABLE();
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: use Q_UNREACHABLE_RETURN(42) [qt-use-unreachable-return]
    // CHECK-FIXES: {{^}}    Q_UNREACHABLE_RETURN(42);
    return 42;
    // CHECK-FIXES: {{^$}}
  }
  }
}

int t6(int n) {
  switch (n) {
  case 0: return 10;
  case 1:
    Q_UNREACHABLE();
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: use Q_UNREACHABLE_RETURN(242) [qt-use-unreachable-return]
    // CHECK-FIXES: {{^}}    Q_UNREACHABLE_RETURN(242);
    return 242;
    // CHECK-FIXES: {{^$}}
  }
  return 14;
}

int t6b(int n) {
  switch (n) {
  case 0: return 10;
  case 1: {
    Q_UNREACHABLE();
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: use Q_UNREACHABLE_RETURN(42) [qt-use-unreachable-return]
    // CHECK-FIXES: {{^}}    Q_UNREACHABLE_RETURN(42);
    return 42;
    // CHECK-FIXES: {{^$}}
  }
  }
  return 14;
}

int t7(int n) {
  if (n < 0) goto unreachable;
  return 42;
 unreachable:
  Q_UNREACHABLE();
  return -1;
}

int n1(int n) {
  switch (n) {
  case 0: return 10;
  case 1: Q_UNREACHABLE(); // doesn't replace this one
    // fall through
  case 2: return 42;
  }
  return 0;
}
