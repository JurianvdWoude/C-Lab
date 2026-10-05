#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(hello, m) {
  m.def("say_hello", []() {
    return "hello, world!";
  });
}
