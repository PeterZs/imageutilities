#include "./../src/ndarray/ndarray_ref.host.h"

void call_my_kernel(ndarray_ref<float, 2> result, const ndarray_ref<float, 3> & data);

void test_thrust_iterator_1();
void test_thrust_iterator_2();
void test_cuda_constructors();
