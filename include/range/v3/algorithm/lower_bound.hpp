/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//  Copyright Casey Carter 2016
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_LOWER_BOUND_HPP
#define RANGES_V3_ALGORITHM_LOWER_BOUND_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/distance.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/algorithm/partition_point.hpp>
#include <range/v3/algorithm/aux_/lower_bound_n.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct lower_bound_fn
        {
            template<typename I, typename S, typename V, typename C = ordered_less, typename P = ident,
                CONCEPT_REQUIRES_(Sentinel<S, I>() && BinarySearchable<I, V, C, P>())>
            I operator()(I begin, S end, V const &val, C pred_ = C{}, P proj = P{}) const
            {
                auto&& pred = as_function(pred_);
                return partition_point(std::move(begin), std::move(end),
                    detail::make_lower_bound_predicate(pred, val), std::move(proj));
            }

            template<typename Rng, typename V, typename C = ordered_less, typename P = ident,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(Range<Rng>() && BinarySearchable<I, V, C, P>())>
            range_safe_iterator_t<Rng> operator()(Rng &&rng, V const &val, C pred_ = C{}, P proj = P{}) const
            {
                auto&& pred = as_function(pred_);
                return partition_point(rng,
                    detail::make_lower_bound_predicate(pred, val), std::move(proj));
            }
        };

        /// \sa `lower_bound_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& lower_bound = static_const<with_braced_init_args<lower_bound_fn>>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
