#include <iostream>
#include <pthread.h>
#include <chrono>

class Integer {
private:
  int inner_value = 0;
  pthread_spinlock_t spinlock;
public:
  Integer(int value) :inner_value(value) {
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
  }
  ~Integer() {
    pthread_spin_destroy(&spinlock);
  }
  void inc() {
    pthread_spin_lock(&spinlock);
    inner_value++;
    pthread_spin_unlock(&spinlock);
  }
  int get_inner_value() {
    return inner_value;
  }
};

void* increment(void* arg) {
  Integer* num = (Integer*)arg;
  for (int i = 0; i < 10000; ++i) {
    num->inc();
  }
  return nullptr;
}

int main() {
  Integer* num = new Integer(0);
  const int thread_count = 50;
  pthread_t tids[thread_count];

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < thread_count; ++i) {
    pthread_create(&tids[i], NULL, increment, num);
  }
  for (int i = 0; i < thread_count; ++i) {
    pthread_join(tids[i], NULL);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Final value (spinlock): " << num->get_inner_value() << std::endl;
  std::cout << "Time (spinlock): " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
            << " ms" << std::endl;

  delete num;
  return 0;
}
