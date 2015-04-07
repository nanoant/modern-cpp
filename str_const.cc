#include <cstdint>
#include <stdexcept>

template <std::size_t N> class const_string {
 public:
  const char(&array)[N];
  const std::size_t size;

  constexpr const_string(const char(&array)[N]) : array(array), size(N - 1) {}

  constexpr char operator[](std::size_t n) const {
    return n < size ? array[n] : throw std::out_of_range("");
  }

  const char* c_str() const { return &array[0]; }
};

template <std::size_t N> const_string<N> cstr(const char(&array)[N]) {
  return const_string<N>(array);
}

namespace detail {
  template <std::size_t...> struct index_seq {};

  template <std::size_t N, std::size_t... S>
  struct gen_index_seq : gen_index_seq<N - 1, N - 1, S...> {};

  template <std::size_t... S> struct gen_index_seq<0, S...> {
    typedef index_seq<S...> type;
  };

  template <std::size_t N,
            std::size_t... NIds,
            std::size_t M,
            std::size_t... MIds>
  constexpr const_string<N + M - 1> concat_const_string(
      const char(&lhs)[N],
      detail::index_seq<NIds...>,
      const char(&rhs)[M],
      detail::index_seq<MIds...>) {
    return const_string<N + M - 1>{ { lhs[NIds]..., rhs[MIds]..., '\0' } };
  }
}

template <std::size_t N, std::size_t M>
constexpr const_string<N + M - 1> operator+(const const_string<N>& lhs,
                                            const const_string<M>& rhs) {
  typedef typename detail::gen_index_seq<N - 1>::type LIndices;
  typedef typename detail::gen_index_seq<M - 1>::type RIndices;
  return detail::concat_const_string(lhs.array, LIndices(), rhs.array,
                                     RIndices());
}

int test1() {
  auto a = cstr("ala");
  auto b = cstr("kota");
  // const_string<4> a("ala");
  // const_string<5> b("kota");
  return (a + b).size;
}

#include <cstring>
#include <cstdio>

void test2(char *str) {
  auto a = cstr("ala");
  auto b = cstr("kota");
  auto c = a + b;
  strncpy(str, c.c_str(), c.size);
}
