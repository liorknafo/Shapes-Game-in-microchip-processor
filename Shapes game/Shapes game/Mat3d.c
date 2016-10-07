#include "Mat3d.h"

mat3 identityMat3()
{
	mat3 mat = { {{1 , 0 , 0},
				  {0 , 1 , 0},
				  {0 , 0 , 1} } };
	return mat;
}
mat3 multyMat3Mat(const mat3 mat1, const mat3 mat2)
{
	mat3 mat;
	int row, col, index;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			mat.args[row][col] = 0;
			for (index = 0; index < 3; index++)
			{
				mat.args[row][col] += mat1.args[row][index] * mat2.args[index][col];
			}
		}
	}
	return mat;
}
mat3 minusMat3(const mat3 mat)
{
	mat3 newMat;
	int row, col;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			newMat.args[row][col] = -mat.args[row][col];
		}
	}
	return newMat;
}
mat3 plusMat3(const mat3 mat1, const mat3 mat2)
{
	mat3 mat;
	int row, col;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			mat.args[row][col] = mat1.args[row][col] + mat2.args[row][col];
		}
	}
	return mat;
}
mat3 rotateMat3(const vec3 axis, const float angle)
{
	vec3 a = vec3Normalize(axis);
	float cosV = (float)cos(angle);
	float cosM1 = 1 - cosV;
	float sinV = (float)sin(angle);
	mat3 mat = { { { cosV + a.X_ * a.X_ * cosM1        , a.X_ * a.Y_ * cosM1 - a.Z_ * sinV , a.X_ * a.Z_ * cosM1 + a.Y_ * sinV },
				   { a.Y_ * a.X_ * cosM1 + a.Z_ * sinV , cosV + a.Y_ * a.Y_ * cosM1       , a.Y_ * a.Z_ * cosM1 - a.X_ * sinV },
				   { a.Z_ * a.X_ * cosM1 - a.Y_ * sinV , a.Z_ * a.Y_ * cosM1 + a.X_ * sinV , cosV + a.Z_ * a.Z_ * cosM1       } } };
	return mat;
}
mat3 mat4to3(const mat4 mat)
{
	mat3 newMat;
	int row, col;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			newMat.args[row][col] = mat.args[row][col];
		}
	}
	return newMat;
}
mat3 mat3Scale(const vec3 scale)
{
	mat3 mat = identityMat3();
	int i;
	for (i = 0; i < 3; i++)
	{
		mat.args[i][i] = scale.args[i];
	}
	return mat;
}

mat4 identityMat4()
{
	mat4 mat = { { { 1 , 0 , 0 , 0},
				   { 0 , 1 , 0 , 0},
				   { 0 , 0 , 1 , 0},
				   { 0 , 0 , 0 , 1}} };
	return mat;
}
mat4 multyMat4Mat(const mat4 mat1, const mat4 mat2)
{
	mat4 mat;
	int row, col, index;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			mat.args[row][col] = 0;
			for (index = 0; index < 4; index++)
			{
				mat.args[row][col] += mat1.args[row][index] * mat2.args[index][col];
			}
		}
	}
	return mat;
}

float ToRadian(float degree)
{
	return (degree / 180) * M_PI;
}

mat4 setProjectionMatrix(const float screenRatio, const float angleOfView, const float near, const float far)
{
	mat4 mat = identityMat4();
	const float zRange = near - far;
	const float tanHalfFOV = tanf(ToRadian(angleOfView / 2.0));

	mat.args[0][0] = 1.0f / (tanHalfFOV * screenRatio);
	mat.args[0][1] = 0.0f;
	mat.args[0][2] = 0.0f;
	mat.args[0][3] = 0.0f;

	mat.args[1][0] = 0.0f;
	mat.args[1][1] = 1.0f / tanHalfFOV;
	mat.args[1][2] = 0.0f;
	mat.args[1][3] = 0.0f;

	mat.args[2][0] = 0.0f;
	mat.args[2][1] = 0.0f;
	mat.args[2][2] = (-near - far) / zRange;
	mat.args[2][3] = 2.0f * far * near / zRange;

	mat.args[3][0] = 0.0f;
	mat.args[3][1] = 0.0f;
	mat.args[3][2] = 1.0f;
	mat.args[3][3] = 0.0f;
	return mat;
}
mat4 minusMat4(const mat4 mat)
{
	mat4 newMat;
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			newMat.args[row][col] = -mat.args[row][col];
		}
	}
	return newMat;
}
mat4 plusMat4(const mat4 mat1, const mat4 mat2)
{
	mat4 mat;
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			mat.args[row][col] = mat1.args[row][col] + mat2.args[row][col];
		}
	}
	return mat;
}
mat4 mat3to4(const mat3 mat)
{
	mat4 newMat = ZERO_MAT;
	int row, col;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			newMat.args[row][col] = mat.args[row][col];
		}
	}
	return newMat;
}
mat4 rotateMat4(const vec3 axis, const float angle)
{
	mat3 mat3Rot = rotateMat3(axis, angle);
	mat4 newMat = mat3to4(mat3Rot);
	newMat.args[3][3] = 1;
	return newMat;
}
mat4 translate(vec3 delta)
{
	mat4 mat = identityMat4();
	int i;
	for (i = 0; i < 3; i++)
		mat.args[3][i] = delta.args[i];
	return mat;
}
mat4 mat4Scale(const vec3 scale)
{
	mat4 mat = identityMat4();
	int i;
	for (i = 0; i < 3; i++)
	{
		mat.args[i][i] = scale.args[i];
	}
	return mat;
}

vec4 identityVec4()
{
	vec4 vec = { {1,1,1,1} };
	return vec;
}
vec4 vec4Plus(const vec4 vec1, const vec4 vec2)
{
	vec4 vec = { vec1.X_ + vec2.X_ , vec1.Y_ + vec2.Y_ , vec1.Z_ + vec2.Z_ , vec1.W_ + vec2.W_ };
	return vec;
}
vec4 multyMat4vec(const mat4 mat, const vec4 vec)
{
	vec4 newVec;
	int row, col;
	for (row = 0; row < 4; row++)
	{
		newVec.args[row] = 0;
		for (col = 0; col < 4; col++)
		{
			newVec.args[row] += mat.args[row][col] * vec.args[col];
		}
	}
	return newVec;
}
vec4 vec3to4(const vec3 vec, const float arg)
{
	vec4 newVec = { {vec.X_ , vec.Y_ ,vec.Z_ , arg} };
	return newVec;
}
float dotVev4(const vec4 vec1, const vec4 vec2)
{
	return vec1.X_ * vec1.X_ + vec1.Y_ * vec1.Y_ + vec1.Z_ * vec1.Z_ + vec1.W_ * vec1.W_;
}
vec4 crossVec4(const vec4 vec1, const vec4 vec2)
{
		vec3 vec31 = vec4to3(vec1);
		vec3 vec32 = vec4to3(vec2);
		vec3 res = crossVec3(vec31, vec32);
		return vec3to4(res, 1);
}
vec4 vec4Normalize(const vec4 vec)
{
	float length = vec4length(vec);
	vec4 newVec = { {vec.X_ / length , vec.Y_ / length ,vec.Z_ / length ,vec.W_ / length } };
	return newVec;
}
float vec4length(const vec4 vec)
{
	return (float)sqrt(vec.X_ * vec.X_ + vec.Y_ * vec.Y_ + vec.Z_ * vec.Z_ + vec.W_ * vec.W_);
}

vec4 initVec4(const float x , const float y , const float z , const float w)
{
	vec4 out;
	out.X_ = x;
	out.Y_ = y;
	out.Z_ = z;
	out.W_ = w;
	return out;
}

vec3 identityVec3()
{
	vec3 vec = { {1,1,1} };
	return vec;
}
vec3 multyMat3vec(const mat3 mat, const vec3 vec)
{
	vec3 newVec;
	int row, col;
	for (row = 0; row < 3; row++)
	{
		newVec.args[row] = 0;
		for (col = 0; col < 3; col++)
		{
			newVec.args[row] += mat.args[row][col] * vec.args[col];
		}
	}
	return newVec;
}
float dotVec3(const vec3 vec1, const vec3 vec2)
{
	return vec1.X_ * vec2.X_ + vec1.Y_ * vec2.Y_ + vec1.Z_ * vec2.Z_;
}
vec3 crossVec3(const vec3 vec1, const vec3 vec2)
{
	vec3 vec = { {vec1.Y_ * vec2.Z_ - vec1.Z_ * vec2.Y_
				,vec1.Z_ * vec2.X_ - vec1.X_ * vec2.Z_
				,vec1.X_ * vec2.Y_ - vec1.Y_ * vec2.X_} };
			
	return vec;
}
vec3 vec4to3(const vec4 vec)
{
	vec3 newVec = { { vec.X_ , vec.Y_ ,vec.Z_} };
	return newVec;
}
vec3 vec3Plus(const vec3 vec1, const vec3 vec2)
{
	vec3 vec = { vec1.X_ + vec2.X_ , vec1.Y_ + vec2.Y_ , vec1.Z_ + vec2.Z_};
	return vec;
}
vec3 vec3Minus(const vec3 vec1, const vec3 vec2)
{
	vec3 vec = { vec1.X_ - vec2.X_ , vec1.Y_ - vec2.Y_ , vec1.Z_ - vec2.Z_};
	return vec;
}
vec3 vec3multyScalar(const vec3 vec1, const float arg)
{
	vec3 vec = {vec1.X_ * arg , vec1.Y_ * arg , vec1.Z_ * arg};
	return vec;
}
vec3 vec3Normalize(const vec3 vec)
{
	float length = vec3length(vec);
	vec3 newVec = { { vec.X_ / length , vec.Y_ / length ,vec.Z_ / length} };
	return newVec;
}
float vec3length(const vec3 vec)
{
	return (float)sqrt(vec.X_ * vec.X_ + vec.Y_ * vec.Y_ + vec.Z_ * vec.Z_);
}

int vec3Equals(const vec3 vec1, const vec3 vec2)
{
	if (vec1.X_ == vec2.X_ && vec1.Y_ == vec2.Y_ && vec1.Z_ == vec2.Z_)
		return TRUE;
	return FALSE;
}

vec3 initVec3(const float x , const float y , const float z)
{
	vec3 out;
	out.X_ = x;
	out.Y_ = y;
	out.Z_ = z;
	return out;
}

vec3 initVec2(const float x , const float y)
{
	vec3 out;
	out.X_ = x;
	out.Y_ = y;
	out.Z_ = 0;
	return out;
}





