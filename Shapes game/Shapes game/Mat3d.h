#ifndef __MAT3D__H
#define __MAT3D__H
#include <math.h>

#define M_PI 3.14159265358979323846

#define ZERO_VEC {{0}}
#define ZERO_MAT {{{0}}}

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define X_ args[0]
#define Y_ args[1]
#define Z_ args[2]
#define W_ args[3]

typedef struct { float args[3]; } vec3;
typedef struct { float args[4]; } vec4;
typedef struct { float args[3][3]; } mat3;
typedef struct { float args[4][4]; } mat4;

mat3 identityMat3();
mat3 multyMat3Mat(const mat3 mat1, const mat3 mat2);
mat3 minusMat3(const mat3 mat);
mat3 plusMat3(const mat3 mat1, const mat3 mat2);
mat3 rotateMat3(const vec3 axis, const float angle);
mat3 mat4to3(const mat4 mat);
mat3 mat3Scale(const vec3 scale);

mat4 identityMat4();
mat4 multyMat4Mat(const mat4 mat1, const mat4 mat2);
mat4 setProjectionMatrix(const float screenRatio, const float angleOfView, const float near, const float far);
mat4 minusMat4(const mat4 mat);
mat4 plusMat4(const mat4 mat1, const mat4 mat2);
mat4 rotateMat4(const vec3 axis, const float angle);
mat4 translate(vec3 delta);
mat4 mat3to4(const mat3 mat);
mat4 mat4Scale(const vec3 scale);

vec4 identityVec4();
vec4 vec4Plus(const vec4 vec1, const vec4 vec2);
vec4 multyMat4vec(const mat4 mat, const vec4 vec);
vec4 vec3to4(const vec3, const float arg);
float dotVev4(const vec4 vec1, const vec4 vec2);
vec4 crossVec4(const vec4 vec1, const vec4 vec2);
vec4 vec4Normalize(const vec4 vec);
float vec4length(const vec4 vec);
vec4 initVec4(const float x , const float y , const float z , const float w);

vec3 identityVec3();
vec3 multyMat3vec(const mat3 mat, const vec3 vec);
float dotVec3(const vec3 vec1, const vec3 vec2);
vec3 crossVec3(const vec3 vec1, const vec3 vec2);
vec3 vec4to3(const vec4 vec);
vec3 vec3Plus(const vec3 vec1, const vec3 vec2);
vec3 vec3Minus(const vec3 vec1, const vec3 vec2);
vec3 vec3multyScalar(const vec3 vec1, const float arg);
vec3 vec3Normalize(const vec3 vec);
float vec3length(const vec3 vec);
int vec3Equals(const vec3 vec1, const vec3 vec2);
vec3 initVec3(const float x , const float y , const float z);
vec3 initVec2(const float x , const float y);

#endif
