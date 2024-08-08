#pragma once

/// <summary>
/// 수학 관련 클래스
/// Transform을 처리하기 위해 필수적으로 존재해야함
/// 
/// DirectXMath를 많이 참고해서 만들어봄
/// </summary>

namespace KunrealEngine
{
	namespace KunMath
	{
		constexpr float PI = 3.1415926535f;

		//float2
		struct KMFLOAT2
		{
			float x;
			float y;

			KMFLOAT2() = default;
			KMFLOAT2(const KMFLOAT2&) = default;
			KMFLOAT2& operator=(const KMFLOAT2&) = default;

			KMFLOAT2(KMFLOAT2&&) = default;
			KMFLOAT2& operator=(KMFLOAT2&&) = default;

			constexpr KMFLOAT2(float _x, float _y) : x(_x), y(_y) {}
		};

		//float3
		struct KMFLOAT3
		{
			float x;
			float y;
			float z;

			KMFLOAT3() = default;
			KMFLOAT3(const KMFLOAT3& kmfloat) = default;
			KMFLOAT3& operator=(const KMFLOAT3&) = default;

			KMFLOAT3(KMFLOAT3&& kmfloat) = default;
			KMFLOAT3& operator=(KMFLOAT3&&) = default;

			//이 부분이 있어야 밑에 연산자 오버로딩이 가능	//왜?
			//DXMath에서는 XM_CONSTEXPR이라는 매크로로 되어있음
			constexpr KMFLOAT3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}

			KMFLOAT3 operator+(const KMFLOAT3& kmfloat) const 
			{
				return KMFLOAT3(x + kmfloat.x, y + kmfloat.y, z + kmfloat.z);
			}

			KMFLOAT3 operator-(const KMFLOAT3& kmfloat) const 
			{
				return KMFLOAT3(x - kmfloat.x, y - kmfloat.y, z - kmfloat.z);
			}

			KMFLOAT3 operator-() const
			{
				return KMFLOAT3(-x, -y, -z);
			}

			KMFLOAT3 operator*(float scalar) const
			{
				return KMFLOAT3(x * scalar, y * scalar, z * scalar);
			}

			KMFLOAT3 operator*(const KMFLOAT3& kmfloat) const
			{
				return KMFLOAT3(x * kmfloat.x, y * kmfloat.y, z * kmfloat.z);
			}

		};

		//float4
		//단지 float3개 값을 받는게 아니라 float3과 float하나를 받는것도 생각해야함
		struct KMFLOAT4
		{
			float x;
			float y;
			float z;
			float w;

			KMFLOAT4() = default;
			KMFLOAT4(const KMFLOAT4&) = default;
			KMFLOAT4& operator=(const KMFLOAT4&) = default;

			KMFLOAT4(KMFLOAT4&&) = default;
			KMFLOAT4& operator=(KMFLOAT4&&) = default;

			constexpr KMFLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
		
			KMFLOAT4 operator+(const KMFLOAT4& kmfloat) const
			{
				return KMFLOAT4(x + kmfloat.x, y + kmfloat.y, z + kmfloat.z, w + kmfloat.w);
			}
		};

		//2x2 매트릭스
		struct KMFLOAT2x2
		{
			union
			{
				struct
				{
					float _11, _12;
					float _21, _22;
				};
				float matrix[2][2];
			};

			KMFLOAT2x2() = default;
			KMFLOAT2x2(const KMFLOAT2x2&) = default;
			KMFLOAT2x2& operator=(const KMFLOAT2x2&) = default;

			KMFLOAT2x2(KMFLOAT2x2&&) = default;
			KMFLOAT2x2& operator=(KMFLOAT2x2&&) = default;

			constexpr KMFLOAT2x2(float m00, float m01,
								 float m10, float m11)
						:		 _11(m00), _12(m01),
								 _21(m10), _22(m11) {}
		};

		//3x3 매트릭스
		struct KMFLOAT3x3
		{
			union
			{
				struct
				{
					float _11, _12, _13;
					float _21, _22, _23;
					float _31, _32, _33;
				};
				float matrix[3][3];
			};

			KMFLOAT3x3() = default;
			KMFLOAT3x3(const KMFLOAT3x3&) = default;
			KMFLOAT3x3& operator=(const KMFLOAT3x3&) = default;

			KMFLOAT3x3(KMFLOAT3x3&&) = default;
			KMFLOAT3x3& operator=(KMFLOAT3x3&&) = default;

			constexpr KMFLOAT3x3(float m00, float m01, float m02,
								float m10, float m11, float m12,
								float m20, float m21, float m22)
						:		_11(m00), _12(m01), _13(m02),
								_21(m10), _22(m11), _23(m12),
								_31(m20), _32(m21), _33(m22) {}
		};

		//4x4 매트릭스
		struct KMFLOAT4x4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float matrix[4][4];
			};

			KMFLOAT4x4() = default;
			KMFLOAT4x4(const KMFLOAT4x4&) = default;
			KMFLOAT4x4& operator=(const KMFLOAT4x4&) = default;

			KMFLOAT4x4(KMFLOAT4x4&&) = default;
			KMFLOAT4x4& operator=(KMFLOAT4x4&&) = default;

			constexpr KMFLOAT4x4(float m00, float m01, float m02, float m03,
								 float m10, float m11, float m12, float m13,
								 float m20, float m21, float m22, float m23,
								 float m30, float m31, float m32, float m33)
				:				_11(m00), _12(m01), _13(m02), _14(m03),
								_21(m10), _22(m11), _23(m12), _24(m13),
								_31(m20), _32(m21), _33(m22), _34(m23),
								_41(m30), _42(m31), _43(m32), _44(m33) {}

		};

		constexpr float Determinant3x3(float a, float b, float c,
			float d, float e, float f,
			float g, float h, float i)
		{
			return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
		}

		constexpr float MatrixMinor(const KMFLOAT4x4& matrix, int row, int col)
		{
			float a = matrix._11, b = matrix._12, c = matrix._13, d = matrix._14;
			float e = matrix._21, f = matrix._22, g = matrix._23, h = matrix._24;
			float i = matrix._31, j = matrix._32, k = matrix._33, l = matrix._34;
			float m = matrix._41, n = matrix._42, o = matrix._43, p = matrix._44;

			if (row == 0)
			{
				a = e; b = f; c = g; d = h;
			}
			else if (row == 1)
			{
				a = i; b = j; c = k; d = l;
			}
			else if (row == 2)
			{
				a = m; b = n; c = o; d = p;
			}

			if (col == 0)
			{
				a = f; e = g; i = h;
				b = j; f = k; j = l;
				c = n; g = o; k = p;
			}
			else if (col == 1)
			{
				a = e; b = g; c = h;
				d = i; f = k; d = l;
				m = n; n = o; o = p;
			}
			else if (col == 2)
			{
				a = e; b = f; c = h;
				d = i; j = k; d = l;
				m = n; o = p; p = l;
			}

			return Determinant3x3(a, b, c, d, e, f, g, h, i);
		}

		KMFLOAT2x2 Multiply2x2Matrix(const KMFLOAT2x2& matrix1, const KMFLOAT2x2& matrix2);
		KMFLOAT3x3 Multiply3x3Matrix(const KMFLOAT3x3& matrix1, const KMFLOAT3x3& matrix2);
		KMFLOAT4x4 Multiply4x4Matrix(const KMFLOAT4x4& matrix1, const KMFLOAT4x4& matrix2);

		KMFLOAT2x2 Inverse2x2Matrix(const KMFLOAT2x2& matrix);
		KMFLOAT3x3 Inverse3x3Matrix(const KMFLOAT3x3& matrix);
		KMFLOAT4x4 Inverse4x4Matrix(const KMFLOAT4x4& matrix);

		//어떤 매트릭스를 인자로 넣던 대응하게 하고 싶은데
		//이건 너무 어렵다 시간날 때 해보자
		template <typename MatrixT>
		MatrixT MultiplyMatrix(const MatrixT& matrix1, const MatrixT& matrix2)
		{
			
		}
		
	};
}