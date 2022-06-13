#include <math.h>
#include <pthread.h>

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;
static double ref_ent = 0;


void put_refTemp(double ref) {
  pthread_mutex_lock(&exclusao_mutua);
  ref_ent = ref;
  pthread_mutex_unlock(&exclusao_mutua);
}

double get_refTemp() {
  double aux;

  pthread_mutex_lock(&exclusao_mutua);
  aux = ref_ent;
  pthread_mutex_unlock(&exclusao_mutua);

  return aux;
}
