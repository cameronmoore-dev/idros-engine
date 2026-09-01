#include "math.hpp"

#include <cstring>
#include <cmath>

namespace idrs
{
    namespace matrix
    {
        const Mat4 perspective(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar)
        {
            Mat4 result = {};

            f32 _fov = math::degToRad(fov);
            f32 f = 1.f / std::tan(_fov * 0.5f);
            result[0][0] = f / aspect;
            result[1][1] = f;
            result[2][2] = -(zFar + zNear) / (zFar - zNear);
            result[2][3] = -(2.f * zFar * zNear) / (zFar - zNear);
            result[3][2] = -1.f;

            return result;
        }

        const Mat4 orthographic(const s32 left, const s32 right, const s32 bottom, const s32 top, const f32 zNear, const f32 zFar)
        {
            Mat4 result = {};

            result[0][0] = 2.f / (right - left);
            result[0][3] = -(right + left) / (right - left);
            result[1][1] = 2.f / (top - bottom);
            result[1][3] = -(top + bottom) / (top - bottom);
            result[2][2] = -2.f / (zFar - zNear);
            result[2][3] = -(zFar + zNear) / (zFar - zNear);
            result[3][3] = 1.f;

            return result;
        }

        const Mat4 lookAt(const Vec3f& pos, const Vec3f& forward, const Vec3f& up)
        {
            const Vec3f f = math::normalize(pos - forward);            /* Local Forward */
            const Vec3f s = math::normalize(math::cross(f, up)); /* Local Side */
            const Vec3f u = math::cross(s, f);                        /* Local Up */

            Mat4 result = {};
            result[0][0] = s.x;
            result[0][1] = s.y;
            result[0][2] = s.z;
            result[0][3] = -math::dot(s, pos);

            result[1][0] = u.x;
            result[1][1] = u.y;
            result[1][2] = u.z;
            result[1][3] = -math::dot(u, pos);

            result[2][0] = -f.x;
            result[2][1] = -f.y;
            result[2][2] = -f.z;
            result[2][3] = math::dot(f, pos);

            result[3][3] = 1.f;

            return result;
        }

        const Mat4 translate(const Vec3f &vec)
        {
            Mat4 t = identity();

            t[0][3] = vec.x;
            t[1][3] = vec.y;
            t[2][3] = vec.z;

            return t;
        }

        const Mat4 rotate(const f32 angle, const Vec3f &axis)
        {
            const f32 _angle = math::degToRad(angle);
            const Vec3f _axis = math::normalize(axis);

            float c = std::cos(_angle);
            float s = std::sin(_angle);
            float t = 1 - c;

            Mat4 r = identity();

            r[0][0] = t * _axis.x * _axis.x + c;
            r[0][1] = t * _axis.x * _axis.y - s * _axis.z;
            r[0][2] = t * _axis.x * _axis.z + s * _axis.y;

            r[1][0] = t * _axis.x * _axis.y + s * _axis.z;
            r[1][1] = t * _axis.y * _axis.y + c;
            r[1][2] = t * _axis.y * _axis.z - s * _axis.x;

            r[2][0] = t * _axis.x * _axis.z - s * _axis.y;
            r[2][1] = t * _axis.y * _axis.z + s * _axis.x;
            r[2][2] = t * _axis.z * _axis.z + c;

            return r;
        }

        const Mat4 scale(const Vec3f &vec)
        {
            Mat4 s = identity();

            s[0][0] = vec.x;
            s[1][1] = vec.y;
            s[2][2] = vec.z;
            s[3][3] = 1.f;

            return s;
        }

        const Mat4 identity()
        {
            Mat4 result = {};
            result[0][0] = 1.f;
            result[1][1] = 1.f;
            result[2][2] = 1.f;
            result[3][3] = 1.f;

            return result;
        }

        /* Inversion by Cramer's rule.  Code taken from an Intel publication */
        /* https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html */
        void inverse4(Mat4 &out, const Mat4 &matrix)
        {
            f32 result[4][4];
            f32 tmp[12];
            f32 src[16];
            f32 det;
            for (u32 i = 0; i < 4; i++)
            {
                src[i + 0]  = matrix[i][0];
                src[i + 4]  = matrix[i][1];
                src[i + 8]  = matrix[i][2];
                src[i + 12] = matrix[i][3];
            }

            /* calculate pairs for first 8 elements (cofactors) */
            tmp[0] = src[10] * src[15];
            tmp[1] = src[11] * src[14];
            tmp[2] = src[9] * src[15];
            tmp[3] = src[11] * src[13];
            tmp[4] = src[9] * src[14];
            tmp[5] = src[10] * src[13];
            tmp[6] = src[8] * src[15];
            tmp[7] = src[11] * src[12];
            tmp[8] = src[8] * src[14];
            tmp[9] = src[10] * src[12];
            tmp[10] = src[8] * src[13];
            tmp[11] = src[9] * src[12];
            /* calculate first 8 elements (cofactors) */
            result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
            result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
            result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
            result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
            result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
            result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
            result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
            result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
            result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
            result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
            result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
            result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
            result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
            result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
            result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
            result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
            /* calculate pairs for second 8 elements (cofactors) */
            tmp[0] = src[2]*src[7];
            tmp[1] = src[3]*src[6];
            tmp[2] = src[1]*src[7];
            tmp[3] = src[3]*src[5];
            tmp[4] = src[1]*src[6];
            tmp[5] = src[2]*src[5];
        
            tmp[6] = src[0]*src[7];
            tmp[7] = src[3]*src[4];
            tmp[8] = src[0]*src[6];
            tmp[9] = src[2]*src[4];
            tmp[10] = src[0]*src[5];
            tmp[11] = src[1]*src[4];
            /* calculate second 8 elements (cofactors) */
            result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
            result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
            result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
            result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
            result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
            result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
            result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
            result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
            result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
            result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
            result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
            result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
            result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
            result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
            result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
            result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
            /* calculate determinant */
            det=src[0]*result[0][0]+src[1]*result[0][1]+src[2]*result[0][2]+src[3]*result[0][3];
            /* calculate matrix inverse */
            det = 1.0f / det;

            for (u32 i = 0; i < 4; i++)
            {
                for (u32 j = 0; j < 4; j++)
                {
                    result[i][j] *= det;
                }
            }

            std::memcpy(&out, &result, sizeof(result));
        }

        const Mat4 transpose(const Mat4 &matrix)
        {
            Mat4 result;
            for (u32 i = 0; i < 4; i++)
            {
                for (u32 j = 0; j < 4; j++)
                {
                    result[j][i] = matrix[i][j];
                }
            }

            return result;
        }

        const Mat4 multiply(const Mat4 &matA, const Mat4 &matB)
        {
            size_t ar = matA.size();
            size_t ac = matA[0].size();
            size_t bc = matB[0].size();

            Mat4 result = {};
            for (size_t row = 0; row < ar; row++)
            {
                for (size_t col = 0; col < bc; col++)
                {
                    for (size_t e = 0; e < ac; e++)
                    {
                        result[row][col] += matA[row][e] * matB[e][col];
                    }
                }
            }
            return result;
        }

        const Vec4f multiply(const Mat4 &mat, const Vec4f &vec)
        {
            Vec4f result = {};
            result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w;
            result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w;
            result.z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w;
            result.w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w;

            return result;
        }

        const Vec3f multiply(const Mat4 &mat, const Vec3f &vec)
        {
            Vec3f result = {};
            result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3];
            result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3];
            result.z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3];

            return result;
        }
    } // namespace matrix

    namespace math
    {
        const f32 degToRad(const f32 deg)
        {
            return deg * (pi / 180.f);
        }

        const f32 dot(const Vec3f &a, const Vec3f &b)
        {
            return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
        }

        const f32 dot(const Vec2f &a, const Vec2f &b)
        {
            return (a.x * b.x) + (a.y * b.y);
        }
    
        const Vec3f normalize(const Vec3f &vec)
        {
            f32 mag = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
            return { vec.x / mag, vec.y / mag, vec.z / mag };
        }

        const Vec2f normalize(const Vec2f &vec)
        {
            f32 mag = std::sqrt(vec.x * vec.x + vec.y * vec.y);
            return { vec.x / mag, vec.y / mag };
        }
    
        const Vec3f cross(const Vec3f &a, const Vec3f &b)
        {
            Vec3f c;
            c.x = a.y * b.z - a.z * b.y;
            c.y = a.z * b.x - a.x * b.z;
            c.z = a.x * b.y - a.y * b.x;
    
            return c;
        }

        const f32 cross(const Vec2f &a, const Vec2f &b)
        {
            return a.x * b.y - a.y * b.x;
        }
    } // namespace math
} // namespace idrs
