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

int t2() {
  Q_UNREACHABLE();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use Q_UNREACHABLE_RETURN(0) [qt-use-unreachable-return]
  // CHECK-FIXES: {{^}}  Q_UNREACHABLE_RETURN(0);
  return 0;
  // CHECK-FIXES: {{^$}}
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
