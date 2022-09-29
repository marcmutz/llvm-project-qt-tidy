// RUN: %check_clang_tidy %s qt-port-to-std-compatible-api %t

template <typename T>
class QSharedPointer {
public:
  void clear(); // old
  void reset(); // new

  T *data() const; // old
  T *get() const;  // new
};

template <typename T, typename Deleter = void>
class QScopedPointer {
public:
  T *data() const; // old
  T *get() const;  // new
};

template <typename T>
class QPointer {
public:
  T *data() const; // old
  T *get() const;  // new
};

namespace QtTestNameSpace {
template <typename T>
class QWeakPointer {
public:
  QSharedPointer<T> toStrongRef(); // old
  QSharedPointer<T> lock();        // new
};
}
using namespace QtTestNameSpace;

class QChar;
class QString {
public:
  QChar *data();             // shouldn't be replaced
  const QChar *data() const; // ditto
};

void w1() {
  QWeakPointer<int> w;
  QSharedPointer<int> p = w.toStrongRef();
  // CHECK-MESSAGES: :[[@LINE-1]]:27: warning: use 'lock' instead of 'toStrongRef' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  QSharedPointer<int> p = w.lock();
}

void c1() {
  QSharedPointer<int> p;
  p.clear();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use 'reset' instead of 'clear' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  p.reset();
}

void dm1() {
  QSharedPointer<int> p;
  int* ptr = p.data();
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: use 'get' instead of 'data' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  int* ptr = p.get();
}

void dc1() {
  const QSharedPointer<QChar> cp;
  QChar* cptr = cp.data();
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: use 'get' instead of 'data' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  QChar* cptr = cp.get();
}

void dm2() {
  QScopedPointer<int> p;
  int* ptr = p.data();
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: use 'get' instead of 'data' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  int* ptr = p.get();
}

void dc2() {
  const QScopedPointer<QChar> cp;
  QChar* cptr = cp.data();
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: use 'get' instead of 'data' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  QChar* cptr = cp.get();
}

void dm3() {
  QPointer<int> p;
  int* ptr = p.data();
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: use 'get' instead of 'data' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  int* ptr = p.get();
}

void dc3() {
  const QPointer<QChar> cp;
  QChar* cptr = cp.data();
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: use 'get' instead of 'data' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  QChar* cptr = cp.get();
}

void dn1() {
  QString str;
  QChar* ptr = str.data();
  const QString cstr;
  const QChar* cptr = cstr.data();
}


template <typename T>
class QList {
public:
  int count() const;  // old
  int length() const; // also old
  int size() const;   // new
  int count(const T&); // shouldn't be touched
};

template <typename T, int N = 255>
class QVarLengthArray {
public:
  int count() const;  // old
  int length() const; // also old
  int size() const;   // new
  int count(const T&); // shouldn't be touched
};

template <typename T>
class QHash {
public:
  int count() const;  // old
  int length() const; // also old
  int size() const;   // new
  int count(const T&); // shouldn't be touched
};

void l1() {
  QList<int> l;
  auto c = l.count();
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use 'size' instead of 'count' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  auto c = l.size();
}

void l2() {
  QList<QChar> l;
  auto c = l.length();
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use 'size' instead of 'length' [qt-port-to-std-compatible-api]
  // CHECK-FIXES: {{^}}  auto c = l.size();
}
