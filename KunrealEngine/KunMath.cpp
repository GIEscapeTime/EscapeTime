#include "KunMath.h"

//4x4행렬의 곱셈
KunrealEngine::KunMath::KMFLOAT4x4 KunrealEngine::KunMath::Multiply4x4Matrix(const KMFLOAT4x4& matrix1, const KMFLOAT4x4& matrix2)
{
	KMFLOAT4x4 resultMatrix;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				resultMatrix.matrix[i][j] += matrix1.matrix[i][k] * matrix2.matrix[k][j];
			}
		}
	}

	return resultMatrix;
}

//3x3행렬의 곱셈
KunrealEngine::KunMath::KMFLOAT3x3 KunrealEngine::KunMath::Multiply3x3Matrix(const KMFLOAT3x3& matrix1, const KMFLOAT3x3& matrix2)
{
	KMFLOAT3x3 resultMatrix;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				resultMatrix.matrix[i][j] += matrix1.matrix[i][k] * matrix2.matrix[k][j];
			}
		}
	}

	return resultMatrix;
}

//2x2행렬의 곱셈
KunrealEngine::KunMath::KMFLOAT2x2 KunrealEngine::KunMath::Multiply2x2Matrix(const KMFLOAT2x2& matrix1, const KMFLOAT2x2& matrix2)
{
	KMFLOAT2x2 resultMatrix;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				resultMatrix.matrix[i][j] += matrix1.matrix[i][k] * matrix2.matrix[k][j];
			}
		}
	}

	return resultMatrix;
}

//2x2 역행렬
KunrealEngine::KunMath::KMFLOAT2x2 KunrealEngine::KunMath::Inverse2x2Matrix(const KMFLOAT2x2& pMatrix)
{
	float availCheck = (pMatrix.matrix[0][0] * pMatrix.matrix[1][1]) - (pMatrix.matrix[0][1] * pMatrix.matrix[1][0]);

	if (availCheck == 0)
	{
		//ad - bc가 0이면 역행렬은 없다
		//그대로 돌려주는 것보다 에러가 발생하는게 하는게 안전할까?
		return pMatrix;
	}

	KMFLOAT2x2 inverseMatrix;


}

//3x3 역행렬
KunrealEngine::KunMath::KMFLOAT3x3 KunrealEngine::KunMath::Inverse3x3Matrix(const KMFLOAT3x3& matrix)
{
	return matrix;
}

//4x4 역행렬
KunrealEngine::KunMath::KMFLOAT4x4 KunrealEngine::KunMath::Inverse4x4Matrix(const KMFLOAT4x4& pMatrix)
{
	KMFLOAT4x4 result;

	float det =
		pMatrix._11 * MatrixMinor(pMatrix, 0, 0) -
		pMatrix._12 * MatrixMinor(pMatrix, 0, 1) +
		pMatrix._13 * MatrixMinor(pMatrix, 0, 2) -
		pMatrix._14 * MatrixMinor(pMatrix, 0, 3);

	float invDet = 1.0f / det;

	result._11 = MatrixMinor(pMatrix, 0, 0) * invDet;
	result._12 = -MatrixMinor(pMatrix, 1, 0) * invDet;
	result._13 = MatrixMinor(pMatrix, 2, 0) * invDet;
	result._14 = -MatrixMinor(pMatrix, 3, 0) * invDet;

	result._21 = -MatrixMinor(pMatrix, 0, 1) * invDet;
	result._22 = MatrixMinor(pMatrix, 1, 1) * invDet;
	result._23 = -MatrixMinor(pMatrix, 2, 1) * invDet;
	result._24 = MatrixMinor(pMatrix, 3, 1) * invDet;

	result._31 = MatrixMinor(pMatrix, 0, 2) * invDet;
	result._32 = -MatrixMinor(pMatrix, 1, 2) * invDet;
	result._33 = MatrixMinor(pMatrix, 2, 2) * invDet;
	result._34 = -MatrixMinor(pMatrix, 3, 2) * invDet;

	result._41 = -MatrixMinor(pMatrix, 0, 3) * invDet;
	result._42 = MatrixMinor(pMatrix, 1, 3) * invDet;
	result._43 = -MatrixMinor(pMatrix, 2, 3) * invDet;
	result._44 = MatrixMinor(pMatrix, 3, 3) * invDet;

	return result;
}
