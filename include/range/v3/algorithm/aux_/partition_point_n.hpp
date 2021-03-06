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
#ifndef RANGES_V3_ALGORITHM_AUX_PARTITION_POINT_N_HPP
#define RANGES_V3_ALGORITHM_AUX_PARTITION_POINT_N_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \ingroup group-concepts
        template<typename I, typename C, typename P = ident,
            typename V = iterator_common_reference_t<I>,
            typename = concepts::Callable::result_t<P, V>>
        using PartitionPointable = meta::strict_and<
            ForwardIterator<I>,
            IndirectCallablePredicate<C, projected<I, P>>>;

        namespace aux
        {
            struct partition_point_n_fn
            {
                template<typename I, typename C, typename P = ident,
                    CONCEPT_REQUIRES_(PartitionPointable<I, C, P>())>
                I operator()(I begin, iterator_difference_t<I> d, C pred_, P proj_ = P{}) const
                {
                    if(0 < d)
                    {
                        auto &&pred = as_function(pred_);
                        auto &&proj = as_function(proj_);
                        do
                        {
                            auto half = d / 2;
                            auto middle = next(uncounted(begin), half);
                            if(pred(proj(*middle)))
                            {
                                begin = recounted(begin, std::move(++middle), half + 1);
                                d -= half + 1;
                            }
                            else
                                d = half;
                        } while(0 != d);
                    }
                    return begin;
                }
            };

            namespace
            {
                constexpr auto&& partition_point_n = static_const<partition_point_n_fn>::value;
            }
        }
    } // namespace v3
} // namespace ranges

#endif // include guard
