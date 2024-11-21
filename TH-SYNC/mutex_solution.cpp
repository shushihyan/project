#include <iostream>
#include <pthread.h>
#include <chrono>

class Integer {
private:
  int inner_value = 0;
  pthread_mutex_t mutex;
public:
  Integer(int value) :inner_value(value) {
    pthread_mutex_init(&mutex, NULL);
  }
  ~Integer() {
    pthread_mutex_destroy(&mutex);
  }
  void inc() {
    pthread_mutex_lock(&mutex);
    inner_value++;
    pthread_mutex_unlock(&mutex);
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
  std::cout << "Final value (mutex): " << num->get_inner_value() << std::endl;
  std::cout << "Time (mutex): " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
            << " ms" << std::endl;

  delete num;
  return 0;
}
