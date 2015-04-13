// use: c++ -std=c++11 const_string.cc

template <std::size_t N> auto tag(const char(&array)[N]) {
  return c("<") + c(array) + c(">");
}

template <std::size_t N, std::size_t M>
auto tag(const char(&a1)[N], const char(&a2)[M]) {
  return c("<") + c(a1) + c(" ") + c(a2) + c(">");
}

#if 0
int test() { return (c("ala") + c("kota") + c("kota") + c("kota")).size(); }
#endif

#if 1
#include <cstdio>
int main(int argc, char const* argv[]) {
  auto r = tag("html", "id=1");
  printf("%s\n", r.c_str());
}
#endif
