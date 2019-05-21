#pragma once
#include <boost/qvm/mat_traits.hpp>
#include <boost/qvm/vec_traits.hpp>

#include <osg/Vec3f>
#include <osg/Vec3d>

namespace boost
{
	namespace qvm
	{
		template<>
		struct vec_traits<osg::Vec3f>
		{
			static int const dim = 3;

			using scalar_type = float;

			template <int I>
			static inline scalar_type & write_element(osg::Vec3f & v) { return v[I]; }

			template <int I>
			static inline scalar_type read_element(osg::Vec3f const & v) { return v[I]; }

			static inline scalar_type & write_element_idx(int i, osg::Vec3f & v) { return v[i]; } //optional
			static inline scalar_type read_element_idx(int i, osg::Vec3f const & v) { return v[i]; } //optional
		};

		template<>
		struct vec_traits<osg::Vec3d>
		{
			static int const dim = 3;

			using scalar_type = double;

			template <int I>
			static inline scalar_type & write_element(osg::Vec3d & v) { return v[I]; }

			template <int I>
			static inline scalar_type read_element(osg::Vec3d const & v) { return v[I]; }

			static inline scalar_type & write_element_idx(int i, osg::Vec3d & v) { return v[i]; } //optional
			static inline scalar_type read_element_idx(int i, osg::Vec3d const & v) { return v[i]; } //optional
		};


	}
}