#pragma once

#include <Eigen/Core>

#include <boost/qvm/mat_traits.hpp>
#include <boost/qvm/vec_traits.hpp>


namespace boost
{
	namespace qvm
	{
		template <typename DataType, int Rows>
		struct vec_traits<Eigen::Matrix<DataType, Rows, 1>>
		{
			static int const dim = Rows;

			using Vector = Eigen::Matrix<DataType, Rows, 1>;
			using scalar_type = DataType;

			template <int I>
			static inline scalar_type & write_element(Vector & v) { return v(I); }

			template <int I>
			static inline scalar_type read_element(Vector const & v) { return v(I); }

			static inline scalar_type & write_element_idx(int i, Vector & v) { return v(i); } //optional
			static inline scalar_type read_element_idx(int i, Vector const & v) { return v(i); } //optional
		};


		template <typename DataType, int Rows, int Cols>
		struct mat_traits<Eigen::Matrix<DataType, Rows, Cols>>
		{
			static int const rows = Rows;
			static int const cols = Cols;

			using Matrix = Eigen::Matrix<DataType, Rows, Cols>;
			using scalar_type = DataType;

			template <int R, int C>
			static inline scalar_type & write_element(Matrix & m) { return m(R, C); }

			template <int R, int C>
			static inline scalar_type read_element(Matrix const & m) { return m(R, C); }

			static inline scalar_type & write_element_idx(int r, int c, Matrix & m) { return m(r, c); }
			static inline scalar_type read_element_idx(int r, int c, Matrix const & m) { return m(r, c); }
		};
	}
}
