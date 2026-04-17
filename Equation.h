#ifndef EQUATION_H
#define EQUATION_H

#include <iostream>
using namespace std;

class Equation
{
    friend bool operator==(const Equation& lhs, const Equation& rhs);
    friend ostream& operator<<(ostream& out, const Equation& eq);

    friend Equation operator+(const Equation& lhs, const Equation& rhs);
    friend Equation operator-(const Equation& lhs, const Equation& rhs);
    friend Equation operator*(const Equation& lhs, double rhs);
    friend Equation operator*(double lhs, const Equation& rhs);

public:
    Equation(double coefficients[], int number);
    Equation(const Equation& rhs);
    ~Equation();
    int degree() const { return size - 1; }

    Equation& operator=(const Equation& rhs);
    Equation& operator+=(const Equation& rhs);
    Equation& operator-=(const Equation& rhs);
    Equation& operator*=(double rhs);

private:
    int size;
    double* coeff;
};

// oeff는 배열 이름이라 포인터로 선언.
#endif

// 다항식의 계수들을 동적 메모리에 저장
// 다항식을 객체로 다루고 비교,출력,더하기,빼기,실수배 등의 연산을 할 수 있도록
// 연산자 오버로딩
// 배열의 각 원소는 다항식의 계수, 배열의 크기는 다항식의 차수 + 1 (하나는 상수항)
// double* coeff는 각 항의 계수를 저장하는 동적 배열
// 예를 들어 1x^2 + 0x - 2 이면 계수는 [1, 0, -2]
// degree() 함수는 size - 1 반환. 즉 최고차항의 차수 = 항 개수 - 1

// 생성자, 복사 생성자, 소멸자, 대입 연산자, +=, -=, *= 연산자 오버로딩 으로 헤더 구현
// == 비교연산의 경우 차수가 같고 각 계수가 모두 같으면 같다고 보는데
// 계수의 차이가 0.0000000001보다 작으면 같은 것으로 간주 (실수 연산의 오차 고려)

// << 연산의 경우, Equation 객체를 사람이 보는 다항식 형태로 출력
// 예를 들어 1x^2 + 0x - 2 이면 "1x^2 - 2 = 0" 형태로 출력

// + 연산의 경우, 두 다항식의 차수가 다를 수 있으므로, 
// 차수를 맞춰서 더해야 함.
// 예를 들어 1x^2 + 0x - 2 와 3x + 4 를 더하면, 
// 1x^2 + 3x + 2 가 되어야 함. (차수 맞추기 위해 3x + 4 는 0x^2 + 3x + 4 로 생각)

// - 연산도 + 연산과 비슷하게 차수 맞춰서 빼야 함

// * 연산의 경우, 다항식과 실수의 곱이므로, 각 계수에 실수를 곱해주면 됨
// 예를 들어 1x^2 + 0x - 2 에 3 을 곱하면, 3x^2 + 0x - 6 이 되어야 함 (분배법칙)

// 대입 및 복합대입은 멤버함수로 구현

// 복사 생성자는 매우 중요한데, 동적 메모리를 사용하는 클래스에서는 깊은 복사가 필요함
// 소멸자에서는 동적 메모리를 해제해야 함


// ***** 프로그램 흐름 *****
// 첫 번째 방정식 입력
// 최고차항 0 여부 검사
// 잘못되면 재입력
// 두 번째 방정식 입력
// 최고차항 0 여부 검사
// 잘못되면 재입력
// eq1, eq2 출력
// 합 C = eq1 + eq2 출력
// 같으면 equality 메시지 출력
// 다르면 not equal 메시지 출력 후 D = eq1 - eq2 출력
// eq3(eq1) 로 복사 생성
// eq3 += eq2
// eq4 = eq3 * 0.5
// eq5 = 4 * eq3
// 각각 출력