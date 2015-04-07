#include <cstdint>
#include <stdexcept>
#include <string>
#include <array>

namespace {
  template <std::size_t...> struct index_seq {};

  template <std::size_t N, std::size_t... S>
  struct gen_index_seq : gen_index_seq<N - 1, N - 1, S...> {};

  template <std::size_t... S> struct gen_index_seq<0, S...> {
    typedef index_seq<S...> type;
  };
}

template <std::size_t N> class const_string {
  std::array<char, N> array;

  template <std::size_t M> friend class const_string;

 public:
  constexpr const_string(const char(&arg)[N])
      : array(reinterpret_cast<std::array<char, N> const&>(arg)) {}

  constexpr char operator[](std::size_t n) const {
    return n < N ? array[n] : throw std::out_of_range("");
  }

  constexpr std::size_t size() const { return N - 1; }

  operator char*() { return array.data(); }
  const char* c_str() const { return array.data(); }
  char* c_str() { return array.data(); }

  template <std::size_t M>
  constexpr const_string<N + M - 1> operator+(
      const const_string<M>& rhs) const {
    typedef typename gen_index_seq<N - 1>::type LIndices;
    typedef typename gen_index_seq<M - 1>::type RIndices;
    return concat_impl(array, LIndices(), rhs.array, RIndices());
  }

 private:
  template <std::size_t... NIds, std::size_t M, std::size_t... MIds>
  static constexpr const_string<N + M - 1> concat_impl(
      const std::array<char, N>& lhs,
      index_seq<NIds...>,
      const std::array<char, M>& rhs,
      index_seq<MIds...>) {
    return const_string<N + M - 1>({ lhs[NIds]..., rhs[MIds]..., '\0' });
  }
};

template <std::size_t N> const_string<N> c(const char(&array)[N]) {
  return const_string<N>(array);
}

#if 0
int test() { return (c("ala") + c("kota") + c("kota") + c("kota")).size(); }
#endif

#if 1
#include <cstdio>
int main(int argc, char const* argv[]) {
  auto r = c("ala") + c("kota") + c("psa") + c("psa") + c("psa") + c("psa") +
           c("kota") + c("psa") + c("psa") + c("psa") + c("psa") + c("kota") +
           c("psa") + c("psa") + c("psa") + c("psa");
  printf("%s\n", r.c_str());
}
#endif
