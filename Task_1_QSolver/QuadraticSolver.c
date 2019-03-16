#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <assert.h>

enum CasesOfEquation
{
    INF, ///< The case of an infinite number of roots
    LIN_0, ///< The case of a linear equation with no roots
    LIN_1, ///< The case of a linear equation with one root
    SQU_0, ///< The case of a quadratic equation with no roots
    SQU_1, ///< The case of a quadratic equation with one root
    SQU_2, ///< The case of a quadratic equation with two roots
};

const double PRECISION = 0.00001; ///< The precision value for comparing real numbers with zero

/**
    \brief The function for finding the roots of the quadratic equation
    @param [in] a coefficient
    @param [in] b coefficient
    @param [in] c coefficient
    @param [out] x1 point to the first root of the equation
    @param [out] x2 point to the second root of the equation

    @return roots of equation
    @note result of the function in case of an error in calculating the roots
*/

int SolverQuadratic (double a, double b, double c, double *x1, double *x2);

/**
    \brief The function for finding the roots of the linear equation

    @param [in] b coefficient
    @param [in] c coefficient
    @param [out] x point to the root of the equation

    @return root of equation
    @note result of the function in case of an error in calculating the roots
*/

int SolverLinear (double b, double c, double *x);

/**
    \brief The function for finding the discriminant of the equation

    @param [in] a coefficient
    @param [in] b coefficient
    @param [in] c coefficient
    @param [out] discr discriminant of the equation

    @return discriminant of equation
*/

double funcDiscr (double a, double b, double c);

/**
    \brief The function for comparing real numbers with zero

    @param [in] a real number
    @return True if the number is zero with a given precision, else False
*/

int CompareWithZero (double a);

int main ()
{
    setlocale (LC_ALL, "Russian");

    printf ("�������� ���������� ���������\n");
    printf ("������� ������������ a, b, c ����������� ���������:\n");


    double a = NAN, b = NAN, c = NAN;
    char last_symb = 0;
    int coeffNum = scanf ("%lf%lf%lf%c", &a, &b, &c, &last_symb);

    if (!isfinite (a) || !isfinite (b) || !isfinite (c))
    {
        printf ("������!\n�������� �� ����� ������������ ������������\n������������ ������ ���� ��������������� �������\n������������� ���������");
        return -2;
    }
    if (4 != coeffNum || '\n' != last_symb )
    {
        printf ("������!\n�������� �� ����� ������������ ����� �������������\n����������� ����� �������������: 3\n������������� ���������\n");
        return -1;
    }


    assert (!isnan(a));
    assert (!isnan(b));
    assert (!isnan(c));

    double x1 = 0, x2 = 0, x = 0;

    if (CompareWithZero(a))
    {
        int solutionLinear = SolverLinear (b, c, &x);
        switch(solutionLinear)
        {
            case INF:
                printf ("��������� ����� ����������� ����� �������");
                break;

            case LIN_0:
                printf ("��������� ������� � �� ����� ������");
                break;

            case LIN_1:
                printf ("��������� ������� � ����� ���� ������: %lg", x);
                break;

            default:
                printf("�������� ������ ��� ���������� ������ ��������� ���������, ���������: %d", solutionLinear);
        }
    }
    else
    {
        int solutionQuadratic = SolverQuadratic (a, b, c, &x1, &x2);
        switch (solutionQuadratic)
        {
            case SQU_0:
                printf ("������������ ������ ����, ��������� �� ����� ������");
                break;

            case SQU_1:
                printf ("��������� ���������� � ����� ���� ������: %lg", x1);
                break;

            case SQU_2:
                printf ("��������� ����� ��� �����: %lg, %lg", x1, x2);
                break;

            default:
                printf("�������� ������ ��� ���������� ������ ����������� ���������, ���������: %d", solutionQuadratic);
        }
    }

   return 0;
}

int SolverQuadratic (double a, double b, double c, double *x1, double *x2)
{
    assert (x1 != x2);
    assert (x1 != NULL);
    assert (x2 != NULL);
    assert (!isnan(a));
    assert (!isnan(b));
    assert (!isnan(c));

    if (CompareWithZero (c))
    {
        *x1 = 0;
        *x2 = -b / a;
        return SQU_2;
    }

    double discr = b*b - 4*a*c;

    if (CompareWithZero (discr))
    {
        *x1 = -b / (2*a);
        return SQU_1;
    }

    if (discr < 0) return SQU_0;

    double sqrtDiscr = sqrt (discr);

    *x1 = (-b + sqrtDiscr) / (2*a);
    *x2 = (-b - sqrtDiscr) / (2*a);
    return SQU_2;
}

int SolverLinear (double b, double c, double *x)
{
    assert (!isnan(b));
    assert (!isnan(c));
    assert (x != NULL);

    if (CompareWithZero (b) && CompareWithZero (c)) return INF;
    if (CompareWithZero (b) && !CompareWithZero (c)) return LIN_0;

    *x = -c / b;
    return LIN_1;
}
/*
double funcDiscr (double a, double b, double c)
{
    assert (!isnan(a));
    assert (!isnan(b));
    assert (!isnan(c));

    return b*b - 4*a*c;
}
*/
int CompareWithZero (double a)
{
    assert (!isnan(a));

    return fabs(a) <= PRECISION;
}

