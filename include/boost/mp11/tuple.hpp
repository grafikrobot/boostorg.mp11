#ifndef BOOST_MP11_TUPLE_HPP_INCLUDED
#define BOOST_MP11_TUPLE_HPP_INCLUDED

//  Copyright 2015, 2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/mp11/integer_sequence.hpp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <tuple>
#include <utility>
#include <type_traits>
#include <cstddef>

namespace boost
{
namespace mp11
{

// tuple_apply
namespace detail
{

template<class F, class Tp, std::size_t... J> BOOST_CONSTEXPR auto tuple_apply_impl( F && f, Tp && tp, integer_sequence<std::size_t, J...> )
    -> decltype( std::forward<F>(f)( std::get<J>(std::forward<Tp>(tp))... ) )
{
    return std::forward<F>(f)( std::get<J>(std::forward<Tp>(tp))... );
}

} // namespace detail

template<class F, class Tp,
    class Seq = make_index_sequence<std::tuple_size<typename std::remove_reference<Tp>::type>::value>>
BOOST_CONSTEXPR auto tuple_apply( F && f, Tp && tp )
    -> decltype( detail::tuple_apply_impl( std::forward<F>(f), std::forward<Tp>(tp), Seq() ) )
{
    return detail::tuple_apply_impl( std::forward<F>(f), std::forward<Tp>(tp), Seq() );
}

// tuple_for_each
namespace detail
{

template<class Tp, std::size_t... J, class F> BOOST_CONSTEXPR F tuple_for_each_impl( Tp && tp, integer_sequence<std::size_t, J...>, F && f )
{
    using A = int[sizeof...(J)];
    return (void)A{ ((void)f(std::get<J>(std::forward<Tp>(tp))), 0)... }, std::forward<F>(f);
}

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1800 )

template<class Tp, class F> BOOST_CONSTEXPR F tuple_for_each_impl( Tp && tp, integer_sequence<std::size_t>, F && f )
{
    return std::forward<F>(f);
}

#endif

} // namespace detail

template<class Tp, class F> BOOST_CONSTEXPR F tuple_for_each( Tp && tp, F && f )
{
    using seq = make_index_sequence<std::tuple_size<typename std::remove_reference<Tp>::type>::value>;
    return detail::tuple_for_each_impl( std::forward<Tp>(tp), seq(), std::forward<F>(f) );
}

} // namespace mp11
} // namespace boost

#endif // #ifndef BOOST_TUPLE_HPP_INCLUDED