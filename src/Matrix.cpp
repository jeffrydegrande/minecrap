#include "minecrap.h"
#include "Matrix.h"
#include <cmath>

Matrix4::Matrix4()
{
    loadIdentity();
}

Matrix4::Matrix4(float *v)
{
    memcpy(&_11, v, sizeof(Matrix4));
}


void Matrix4::loadIdentity() {
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = 0.0f;
    _21 = _23 = _24 = 0.0f;
    _31 = _32 = _34 = 0.0f;
    _41 = _42 = _43 = 0.0f;
}

void Matrix4::invertPt(const Vec3 &from, Vec3 &to) {
    float x = from.x - _41;
    float y = from.y - _42;
    float z = from.z - _43;

    to.x = _11 * x + _12 * y + _13 * z;
    to.y = _21 * x + _22 * y + _23 * z;
    to.z = _31 * x + _32 * y + _33 * z;
}

void Matrix4::rotateX(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot._22 = cos(rads);
    rot._23 = sin(rads);
    rot._32 = -rot._23; // -sin
    rot._33 = rot._22;  // cos
    multiply(rot);
}

void Matrix4::rotateY(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot._11 = cos(rads);
    rot._13 = -sin(rads);
    rot._31 = -rot._13; // sin
    rot._33 = rot._11;  // cos
    multiply(rot);
}

void Matrix4::rotateZ(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot._11 = cos(rads);
    rot._12 = sin(rads);
    rot._21 = -rot._12; // -sin
    rot._22 = rot._11;  // cos
    multiply(rot);
}

Matrix4 Matrix4::Multiply(const Matrix4 &am, const Matrix4 &bm) {

    float *pA = (float *)&am._11;
    float *pB = (float *)&bm._11;

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


float *Matrix4::data()
{
    return &_11;
}

float & Matrix4::operator()(int row, int column) {
    return ((float *) this)[row * 4 + column];
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
