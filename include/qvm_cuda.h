#pragma once

#include <boost/qvm/mat_traits.hpp>
#include <boost/qvm/vec_traits.hpp>

namespace boost
{
	namespace qvm
	{
		namespace detail
		{
			template <typename scalar_type>
			struct type_picker{ };

			template<>
			struct type_picker<float> { using type = float3; };

			template<>
			struct type_picker<double> { using type = double3; };

			template<typename _scalar_type>
			struct cuda_vec3_traits
			{
				static int const dim = 3;

				using scalar_type = _scalar_type;

				using Vec = typename type_picker<scalar_type>::type;

				template<int I>
				static inline scalar_type & write_element(Vec & v) 
				{
					switch (I)
					{
						case 0: return v.x;
						case 1: return v.y;
						case 2: return v.z;
						default: throw std::runtime_error("Wrong dim.");
					}
				}

				template <int I>
				static inline scalar_type read_element(Vec const & v)
				{
					switch (I)
					{
						case 0: return v.x;
						case 1: return v.y;
						case 2: return v.z;
						default: throw std::runtime_error("Wrong dim.");
					}
				}

				static inline scalar_type & write_element_idx(int i, Vec & v) 
				{ 
					switch(i)
					{
						case 0: return v.x;
						case 1: return v.y;
						case 2: return v.z;
						default: throw std::runtime_error("Wrong dim.");
					}
				} //optional
				static inline scalar_type read_element_idx(int i, Vec const & v)
				{
					switch (i)
					{
						case 0: return v.x;
						case 1: return v.y;
						case 2: return v.z;
						default: throw std::runtime_error("Wrong dim.");
					}
				} //optional

			};
		}

		template<>
		struct vec_traits<float3> : detail::cuda_vec3_traits<float>
		{
		
		};

		template<>
		struct vec_traits<double3> : detail::cuda_vec3_traits<double>
		{

		};
	}
}