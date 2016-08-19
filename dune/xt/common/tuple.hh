// This file is part of the dune-xt-common project:
//   https://github.com/dune-community/dune-xt-common
// The copyright lies with the authors of this file (see below).
// License: Dual licensed as  BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2012, 2014 - 2016)
//   Rene Milk       (2010 - 2013, 2015)
//   Stefan Girke    (2012)
//   Tobias Leibner  (2014)

#ifndef DUNE_XT_COMMON_TUPLE_HH
#define DUNE_XT_COMMON_TUPLE_HH

#include <dune/common/tuples.hh>
#include <dune/common/typetraits.hh>

#include <boost/mpl/if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>

#define TMAX(t_, no_) (Dune::tuple_size<t_>::value >= (no_ + 1) ? no_ : 0)
#define TELE(t_, s_, no_) typename Dune::tuple_element<TMAX(t_, no_), t_>::type::s_

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, size_t Size>
struct RightTrimTuple
{
  typedef Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 9>
{
  typedef Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 8>
{
  typedef Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T8> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 7>
{
  typedef Dune::tuple<T1, T2, T3, T4, T5, T6, T7> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 6>
{
  typedef Dune::tuple<T1, T2, T3, T4, T5, T6> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 5>
{
  typedef Dune::tuple<T1, T2, T3, T4, T5> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 4>
{
  typedef Dune::tuple<T1, T2, T3, T4> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 3>
{
  typedef Dune::tuple<T1, T2, T3> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 2>
{
  typedef Dune::tuple<T1, T2> type;
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct RightTrimTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, 1>
{
  typedef Dune::tuple<T1> type;
};

#define TUPLE_TYPEDEFS_2_TUPLE(t_, s_)                                                                                 \
  typename RightTrimTuple<TELE(t_, s_, 0),                                                                             \
                          TELE(t_, s_, 1),                                                                             \
                          TELE(t_, s_, 2),                                                                             \
                          TELE(t_, s_, 3),                                                                             \
                          TELE(t_, s_, 4),                                                                             \
                          TELE(t_, s_, 5),                                                                             \
                          TELE(t_, s_, 6),                                                                             \
                          TELE(t_, s_, 7),                                                                             \
                          TELE(t_, s_, 8),                                                                             \
                          Dune::tuple_size<t_>::value>::type

/**
 * @def TUPLE_TYPEDEFS_2_TUPLE( t_, s_ )
 *
 * @brief extracts types of the elements of a Dune::tuple and
 * creates a new Dune::tuple with these extracted types.
 *
 * first argument: the tuple type
 * second argument: the typedef of the elements of the tuple
 *
 * example:
 * Let two classes be given:
 *
 * @code
 * class A
 * {
 *   public:
 *   typedef int MyFancyType;
 * };
 *
 * class B
 * {
 *   public:
 *   typedef int MyFancyType;
 * };
 * @endcode
 *
 * Define a tuple, i.e.
 *
 * @code
 * typedef Dune::tuple< A, B, B >
 *   MyTuple;
 * @endcode
 *
 * Instead of writing
 *
 * @code
 * typedef Dune::tuple< typename tuple_element<0,MyTuple>::type::MyFancyType,
 *                      typename tuple_element<1,MyTuple>::type::MyFancyType,
 *                      typename tuple_element<2,MyTuple>::type::MyFancyType >
 *   MySubTupleType;
 * @endcode
 *
 * just write
 *
 * @code
 * typedef TUPLE_TYPEDEFS_2_TUPLE( MyTuple, MyFancyType )
 *   MySubTupleType;
 * @endcode
 *
 * without (general) restriction to the size
 * of the tuple @c MyTuple. Enjoy it!
 */

namespace Dune {
namespace XT {
namespace Common {

// reduced from
// http://stackoverflow.com/questions/1492204/is-it-possible-to-generate-types-with-all-combinations-of-template-arguments
namespace TupleProduct {
using boost::is_same;
using boost::mpl::begin;
using boost::mpl::end;
using boost::mpl::next;
using boost::mpl::if_;
using boost::mpl::deref;

struct end_of_recursion_tag
{
  template <class... Args>
  static void Run(Args&&... /*args*/)
  {
  }
};

template <class UTypes, // Forward Sequence, e.g. boost::mpl::vector
          class VTypes, // Forward Sequence, e.g. boost::mpl::vector
          class TestFunc // class type that has a nested templated run() member function
          >
struct Combine
{
  // forward declaration
  template <class UIterator, class VIterator>
  struct Generate;

  // this class implements recursion body
  template <class UIterator, class VIterator>
  struct Next
  {
    // u_begin is not necessary ;)
    // it would be cheaper not to pre-declare all of them since we force evaluation
    // however this dramatically increase the readability
    typedef typename begin<VTypes>::type v_begin;

    typedef typename end<UTypes>::type u_end;
    typedef typename end<VTypes>::type v_end;

    typedef typename next<UIterator>::type u_next;
    typedef typename next<VIterator>::type v_next;

    typedef
        typename if_<boost::is_same<v_next, v_end>,
                     typename if_<boost::is_same<u_next, u_end>, end_of_recursion_tag, Generate<u_next, v_begin>>::type,
                     Generate<UIterator, v_next>>::type type;
  };

  //  this class run test on generated types in thos round and go to next*/
  template <class UIterator = typename begin<UTypes>::type, class VIterator = typename begin<VTypes>::type>
  struct Generate
  {
    //  generate <<next>> target type
    typedef typename Next<UIterator, VIterator>::type next_type;

    static void Run()
    {
      // test on the generated types of this round of recursion
      TestFunc::template run<typename deref<UIterator>::type, typename deref<VIterator>::type>();

      // go to the next round of recursion
      next_type::Run();
    }

    template <class... Args>
    static void Run(Args&&... args)
    {
      // test on the generated types of this round of recursion
      TestFunc::template run<typename deref<UIterator>::type, typename deref<VIterator>::type>(
          std::forward<Args>(args)...);

      // go to the next round of recursion
      next_type::Run(std::forward<Args>(args)...);
    }
  };
};

} // namespace TupleProduct

//! from
//! https://stackoverflow.com/questions/16853552/how-to-create-a-type-list-for-variadic-templates-that-contains-n-times-the-sam
template <std::size_t...>
struct indices
{
};

//! call this without Indices (i.e. create_indices< N >::type)
template <std::size_t N, std::size_t... Indices>
struct create_indices : create_indices<N - 1, N - 1, Indices...>
{
};

//! terminating template
template <std::size_t... Indices>
struct create_indices<0, Indices...>
{
  typedef indices<Indices...> type;
};

//! T_aliased< T, Index > is always the type T, no matter what index is
template <typename T, std::size_t index>
using T_aliased = T;

//! make_identical_tuple< T, N >::type is a std::tuple< T, ... , T > with a length of N
template <typename T, std::size_t N, typename I = typename create_indices<N>::type>
struct make_identical_tuple;

template <typename T, std::size_t N, std::size_t... Indices>
struct make_identical_tuple<T, N, indices<Indices...>>
{
  using type = std::tuple<T_aliased<T, Indices>...>;

  static type create(const T& t)
  {
    return type(T_aliased<T, Indices>{t}...);
  }
};

} // namespace Common
} // namespace XT
} // namespace Dune

#endif // DUNE_XT_COMMON_TUPLE_HH
