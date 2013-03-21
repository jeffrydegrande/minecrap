#include "minecrap.h"
#include "Matrix.h"
#include <cmath>

/////////////////////////////////////////////////////////////////
/// Matrix3 implementation
/////////////////////////////////////////////////////////////////

Matrix3::Matrix3() {
    identity();
}

Matrix3::Matrix3(float v[9]) {
    memcpy(&m, v, sizeof(float) * 9);
}

Matrix3::Matrix3(Matrix4 &matrix)
{
    float *p = matrix.value_ptr();

    m[0] = p[0];
    m[1] = p[1];
    m[2] = p[2];

    m[3] = p[4];
    m[4] = p[5];
    m[5] = p[6];

    m[6] = p[8];
    m[7] = p[9];
    m[8] = p[10];
}

Matrix3::Matrix3(const Matrix4 &matrix)
{
    m[0] = matrix[0];
    m[1] = matrix[1];
    m[2] = matrix[2];

    m[3] = matrix[4];
    m[4] = matrix[5];
    m[5] = matrix[6];

    m[6] = matrix[8];
    m[7] = matrix[9];
    m[8] = matrix[10];
}

void Matrix3::identity() {
    memset(&m, 0, sizeof(float) * 9);
    m[0] = m[4] = m[8] = 1.0f;
}

float * Matrix3::value_ptr()
{
    return m;
}

void Matrix3::print() {
    printf("%0.2f %0.2f %0.2f\n", m[0], m[3], m[6]);
    printf("%0.2f %0.2f %0.2f\n", m[1], m[4], m[7]);
    printf("%0.2f %0.2f %0.2f\n", m[2], m[5], m[8]);
    printf("\n");
}

/////////////////////////////////////////////////////////////////
/// Matrix4 implementation
/////////////////////////////////////////////////////////////////

Matrix4::Matrix4()
{
    identity();
}

Matrix4::Matrix4(float *v)
{
    memcpy(&m, v, sizeof(float) * 16);
}

Matrix4 Matrix4::Load(GLenum matrix)
{
    float m[16];
    glGetFloatv(matrix, m);
    return Matrix4(m);
}

void Matrix4::identity() {
    memset(&m, 0, sizeof(float) * 16);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void Matrix4::invertPt(const Vec3 &from, Vec3 &to) {
    float x = from.x - m[12];
    float y = from.y - m[13];
    float z = from.z - m[14];

    to.x = m[0] * x + m[1] * y + m[2] * z;
    to.y = m[4] * x + m[5] * y + m[6] * z;
    to.z = m[8] * x + m[9] * y + m[10] * z;
}

void Matrix4::transformVector(Vec3 &to) {
    float x = to.x - m[12];
    float y = to.y - m[13];
    float z = to.z - m[14];

    to.x = m[0] * x + m[1] * y + m[2] * z;
    to.y = m[4] * x + m[5] * y + m[6] * z;
    to.z = m[8] * x + m[9] * y + m[10] * z;
}

void Matrix4::translate(float x, float y, float z) {
	m[12] += x;
	m[13] += y;
	m[14] += z;
}

void Matrix4::translate(const Vec3 &v) {
	translate(v.x, v.y, v.z);
}

void Matrix4::rotate(const Vec3 &axis, float degrees)
{
    // Creates a rotation matrix about the specified axis.
    // The axis must be a unit vector. The angle must be in degrees.
    //
    // Let u = axis of rotation = (x, y, z)
    //
    //             | x^2(1 - c) + c  xy(1 - c) + zs  xz(1 - c) - ys   0 |
    // Ru(angle) = | yx(1 - c) - zs  y^2(1 - c) + c  yz(1 - c) + xs   0 |
    //             | zx(1 - c) - ys  zy(1 - c) - xs  z^2(1 - c) + c   0 |
    //             |      0              0                0           1 |
    //
    // where,
    //	c = cos(angle)
    //  s = sin(angle)

    degrees *= DEGREES_TO_RADIANS;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float c = cosf(degrees);
    float s = sinf(degrees);

    m[0] = (x * x) * (1.0f - c) + c;
    m[1] = (x * y) * (1.0f - c) + (z * s);
    m[2] = (x * z) * (1.0f - c) - (y * s);
    m[3] = 0.0f;

    m[4] = (y * x) * (1.0f - c) - (z * s);
    m[5] = (y * y) * (1.0f - c) + c;
    m[6] = (y * z) * (1.0f - c) + (x * s);
    m[7] = 0.0f;

    m[8] = (z * x) * (1.0f - c) + (y * s);
    m[9] = (z * y) * (1.0f - c) - (x * s);
    m[10] = (z * z) * (1.0f - c) + c;
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

void Matrix4::rotateX(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot.m[5] = cos(rads);
    rot.m[6] = sin(rads);
    rot.m[9] = -rot.m[6]; // -sin
    rot.m[10] = rot.m[5];  // cos
    multiply(rot);
}

void Matrix4::rotateY(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot.m[0] = cos(rads);
    rot.m[2] = -sin(rads);
    rot.m[8] = -rot.m[2]; // sin
    rot.m[10] = rot.m[0];  // cos
    multiply(rot);
}

void Matrix4::rotateZ(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot.m[0] = cos(rads);
    rot.m[1] = sin(rads);
    rot.m[4] = -rot.m[1]; // -sin
    rot.m[5] = rot.m[0];  // cos
    multiply(rot);
}

Matrix4 Matrix4::Multiply(const Matrix4 &am, const Matrix4 &bm) {
    float *pA = (float *)&am.m[0];
    float *pB = (float *)&bm.m[0];

    float pM[16];

    memset(pM, 0, 16*sizeof(float));
    for (unsigned short i=0; i<4; i++)
        for (unsigned short j=0; j<4; j++)
            for (unsigned short k=0; k<4; k++)
                pM[4*i+j] += pA[4*i+k] * pB[4*k+j];

    return Matrix4(pM);
}

void Matrix4::multiply(const Matrix4 &m) {
    *this = Multiply(*this, m);
}

Matrix4 Matrix4::operator *(const Matrix4 &m)
{
    return Multiply(*this, m);
}

Matrix4 Matrix4::operator *=(const Matrix4 &m)
{
    multiply(m);
    return *this;
}

Vec4 Matrix4::operator *(const Vec4 &v)
{
    Vec4 result;
    result.x = m[0] * v.x + m[4] + v.y * m[8]  + v.z * m[12] + v.w;
    result.y = m[1] * v.x + m[5] + v.y * m[9]  + v.z * m[13] + v.w;
    result.z = m[2] * v.x + m[6] + v.y * m[10] + v.z * m[14] + v.w;
    result.w = m[3] * v.x + m[7] + v.y * m[10] + v.z * m[15] + v.w;
    return result;
}

Vec4 Matrix4::operator *(Vec4 &v) const
{
    Vec4 result;
    result.x = m[0] * v.x + m[4] + v.y * m[8]  + v.z * m[12] + v.w;
    result.y = m[1] * v.x + m[5] + v.y * m[9]  + v.z * m[13] + v.w;
    result.z = m[2] * v.x + m[6] + v.y * m[10] + v.z * m[14] + v.w;
    result.w = m[3] * v.x + m[7] + v.y * m[10] + v.z * m[15] + v.w;
    return result;
}

float *Matrix4::value_ptr()
{
    return m;
}

const float *Matrix4::value_ptr() const
{
    return m;
}

float & Matrix4::operator()(int row, int column) {
    return m[row * 4 + column];
}


static void
lubksb(Matrix4 & a, int *indx, double *b)
{
	int		i, j, ii=-1, ip;
	double	sum;

	for (i=0; i<4; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii>=0) {
			for (j=ii; j<=i-1; j++) {
				sum -= a(i, j) * b[j];
			}
		} else if (sum != 0.0) {
			ii = i;
		}
		b[i] = sum;
	}
	for (i=3; i>=0; i--) {
		sum = b[i];
		for (j=i+1; j<4; j++) {
			sum -= a(i, j) * b[j];
		}
		b[i] = sum/a(i, i);
	}
} // end lubksb

/*
**-----------------------------------------------------------------------------
**  Name:       ludcmp
**  Purpose:	LU decomposition
**-----------------------------------------------------------------------------
*/

static void
ludcmp(Matrix4 & a, int *indx, double *d)
{
	double	vv[4];               /* implicit scale for each row */
	double	big, dum, sum, tmp;
	int		i, imax, j, k;

	*d = 1.0;
	for (i=0; i<4; i++) {
		big = 0.0;
		for (j=0; j<4; j++) {
			if ((tmp = abs(a(i, j))) > big) {
				big = tmp;
			}
		}
		/*
		if (big == 0.0) {
			printf("ludcmp(): singular matrix found...\n");
			exit(1);
		}
		*/
		vv[i] = 1.0/big;
	}
	for (j=0; j<4; j++) {
		for (i=0; i<j; i++) {
			sum = a(i, j);
			for (k=0; k<i; k++) {
				sum -= a(i, k) * a(k, j);
			}
			a(i, j) = sum;
		}
		big = 0.0;
		for (i=j; i<4; i++) {
			sum = a(i, j);
			for (k=0; k<j; k++) {
				sum -= a(i, k)*a(k, j);
			}
			a(i, j) = sum;
			if ((dum = vv[i] * (double)fabs(sum)) >= big) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) {
			for (k=0; k<4; k++) {
				dum = a(imax, k);
				a(imax, k) = a(j, k);
				a(j, k) = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (a(j, j) == 0.0) {
			a(j, j) = 1.0e-20;      /* can be 0.0 also... */
		}
		if (j != 3) {
			dum = 1.0/a(j, j);
			for (i=j+1; i<4; i++) {
				a(i, j) *= dum;
			}
		}
	}
} // end ludcmp

//-----------------------------------------------------------------------------
// create inverse of argument matrix
void Matrix4::inverse(const Matrix4 &m)
{
	Matrix4 n;
	int i, j, indx[4];
	double d, col[4];

	n = m;
	ludcmp(n, indx, &d);

	for (j=0; j<4; j++)
    {
		for (i=0; i<4; i++)
        {
			col[i] = 0.0;
		}
		col[j] = 1.0;
		lubksb(n, indx, col);
		for (i=0; i<4; i++)
        {
			(*this)(i, j) = col[i];
		}
	}
} // end MatrixInverse

void Matrix4::print() {
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[0], m[4], m[8], m[12]);
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[1], m[5], m[9], m[13]);
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[2], m[6], m[10], m[14]);
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[3], m[7], m[11], m[15]);
    printf("\n");
}

void Matrix4::print() const {
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[0], m[4], m[8], m[12]);
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[1], m[5], m[9], m[13]);
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[2], m[6], m[10], m[14]);
    printf("%0.2f %0.2f %0.2f %0.2f\n", m[3], m[7], m[11], m[15]);
    printf("\n");
}


Matrix4 Matrix4::Perspective(float fovy, float aspect, float zNear, float zFar) {
    if (aspect > 1.0f)
        fovy /= aspect;


    float depth = zFar - zNear;
    float f     = 1.0f/tan(fovy*PI/360);
    Matrix4 M;

    M.m[0]  = f / aspect;
    M.m[5]  = f;
    M.m[10] = -(zFar + zNear) / depth;
    M.m[14] = -2 * (zFar*zNear) / depth;
    M.m[11] = -1.0f;

    return M;
}

void Matrix4::scale(const Vec3 &v)
{
    Matrix4 M;
    M[0]  = v.x;
    M[5]  = v.y;
    M[10] = v.z;

    multiply(M);
}

void Matrix4::scale(float v)
{
    Matrix4 M;
    M[0]  = v;
    M[5]  = v;
    M[10] = v;
    multiply(M);
}
