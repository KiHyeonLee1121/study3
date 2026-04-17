#include "Equation.h"
using namespace std;

Equation::Equation(double coefficients[], int number)
{
    size = number;
    coeff = new double[size];

    for (int i = 0; i < size; i++)
    {
        coeff[i] = coefficients[i];
    }
}

Equation::Equation(const Equation& rhs)
{
    size = rhs.size;
    coeff = new double[size];

    for (int i = 0; i < size; i++)
    {
        coeff[i] = rhs.coeff[i];
    }
}

// 그냥 객체 내부에 동적배열을 들고 있는 형태.
// 객체배열이 아니라 계수들을 저장하는 동적 배열이 객체의 멤버로 존재하는 형태.

// 포인터를 멤버로 갖는 복사생성자라서 깊은 복사.
// 여기에선 다항식의 계수들을 저장하는 동적 배열을 복사해야 하므로,
// 복사 생성자에서 새로운 배열을 할당하고 각 계수를 복사하는 방식으로 구현.
// 새로운 배열에 반복문으로 원소를 하나씩 복사
// 따라서 코드에 적힌 Equation의 복사생성자는 깊은 복사로 구현되어 있음.

Equation::~Equation()
{
    delete[] coeff;
}

Equation& Equation::operator=(const Equation& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    // 자기 자신을 자기 자신에게 대입하는 상황을 막음

    delete[] coeff;

    // 기존 coeff는 예전 배열 크기에 맞춰서 잡혀 있는 메모리라서
    // rhs의 크기와 다를 수 있음.

    size = rhs.size;
    coeff = new double[size];

    for (int i = 0; i < size; i++)
    {
        coeff[i] = rhs.coeff[i];
    }

    return *this;
}
// 대입연산자 오버로딩은 사실상 복사생성자랑 로직이 동일함
// 다만, 대입연산자 오버로딩은 기존 메모리 정리 + 자기 대입 방어


Equation& Equation::operator+=(const Equation& rhs)
{
    *this = *this + rhs;
    return *this;
}

Equation& Equation::operator-=(const Equation& rhs)
{
    *this = *this - rhs;
    return *this;
}

Equation& Equation::operator*=(double rhs)
{
    *this = *this * rhs;
    return *this;
}

// +=, -=, *= 연산자 오버로딩은 각각 +, -, * 연산자 오버로딩을 이용해서 구현
// 저런식으로 정의하려면 사칙연산자 오버로딩이 먼저 구현되어 있어야 함

bool operator==(const Equation& lhs, const Equation& rhs)
{
    if (lhs.size != rhs.size)
    {
        return false;
    }

    for (int i = 0; i < lhs.size; i++)
    {
        double diff = lhs.coeff[i] - rhs.coeff[i];
        if (diff < -0.0000000001 || diff > 0.0000000001)
        {
            return false;
        }
    }

    return true;
}

Equation operator+(const Equation& lhs, const Equation& rhs)
{
    int maxSize = (lhs.size > rhs.size) ? lhs.size : rhs.size;
    double* temp = new double[maxSize];

    for (int i = 0; i < maxSize; i++)
    {
        temp[i] = 0;
    }

    int lhsOffset = maxSize - lhs.size;
    int rhsOffset = maxSize - rhs.size;

    // offset은 차수가 다른 다항식을 뒤쪽 기준으로 맞춰 정렬하기 위한 값
	// 예를 들어 사이즈가 3인 배열1이 있고 사이즈가 4인 배열2가 있다고 하면,
	// maxSize = 4, lhsOffset = 4 - 3 = 1, rhsOffset = 4 - 4 = 0 이런 식인거지
    // 이 경우 lhs는 temp에 넣을 때 한 칸 뒤로 밀어서 넣어야 함
    // rhs는 이미 maxSize랑 길이가 같으므로 그대로 넣으면 됨


    for (int i = 0; i < lhs.size; i++)
    {
        temp[i + lhsOffset] += lhs.coeff[i];
    }

    for (int i = 0; i < rhs.size; i++)
    {
        temp[i + rhsOffset] += rhs.coeff[i];
    }

    // temp의 모든 원소들은 기본적으로 0으로 초기화된 상태이므로
    // offset만큼 칸을 밀어서 저장하기만 해도 자리가 맞춰진 상태로 더해짐
    // maxsize를 기준으로 얼마나 밀어서 저장할지를 정하는 것이기 때문에
    // 뒤쪽 기준으로 맞춰지는 것임

    int start = 0;
    while (start < maxSize - 1)      // 상수항 빼고 최고차항부터 0이 아닌 계수가 나올때까지
    {
        if (temp[start] < -0.0000000001 || temp[start] > 0.0000000001)
        {
            break;
        }
        start++;
    }

    // 이 부분은 앞쪽에 생긴 필요없는 0 계수들을 잘라내기 위한 코드임
    // 왜냐면 같은 차항을 더하거나 빼면 최고차항이 0이 될 수도 있기 때문
	// 반복마다 start를 늘리니까 start에 해당하는 인덱스까지 0인 계수들은 모두 건너뛰게 되는 것임
    

    int newSize = maxSize - start;                  //앞에 불필요한 계수를 제외한 실제 필요한 배열길이
    double* resultCoeff = new double[newSize];

    for (int i = 0; i < newSize; i++)
    {
        resultCoeff[i] = temp[start + i];        //의미있는값이 있는 인덱스부터 시작해서 새로운 크기만큼 배열에 저장
    }

    Equation result(resultCoeff, newSize);        //Equation 인스턴스화


    delete[] temp;
	delete[] resultCoeff;           //temp는 사실상 도구고, resultCoeff는 result 객체가 이미 복사

    return result;
}

Equation operator-(const Equation& lhs, const Equation& rhs)
{
    int maxSize = (lhs.size > rhs.size) ? lhs.size : rhs.size;
    double* temp = new double[maxSize];

    for (int i = 0; i < maxSize; i++)
    {
        temp[i] = 0;
    }

    int lhsOffset = maxSize - lhs.size;
    int rhsOffset = maxSize - rhs.size;

    for (int i = 0; i < lhs.size; i++)
    {
        temp[i + lhsOffset] += lhs.coeff[i];
    }

    for (int i = 0; i < rhs.size; i++)
    {
        temp[i + rhsOffset] -= rhs.coeff[i];
    }

    int start = 0;
    while (start < maxSize - 1)
    {
        if (temp[start] < -0.0000000001 || temp[start] > 0.0000000001)
        {
            break;
        }
        start++;
    }

    int newSize = maxSize - start;
    double* resultCoeff = new double[newSize];

    for (int i = 0; i < newSize; i++)
    {
        resultCoeff[i] = temp[start + i];
    }

    Equation result(resultCoeff, newSize);

    delete[] temp;
    delete[] resultCoeff;

    return result;
}

Equation operator*(const Equation& lhs, double rhs)
{
    double* temp = new double[lhs.size];

    for (int i = 0; i < lhs.size; i++)
    {
        temp[i] = lhs.coeff[i] * rhs;   //객체의 계수를 갖고있는 배열을 호출하여
	}                                  //각 계수에 rhs 곱해서 temp에 저장

    int start = 0;        // 여기부터는 불필요한 0을 제거하는 과정
    while (start < lhs.size - 1)    // 반복해도 결국 하나는 남김
    {
        if (temp[start] < -0.0000000001 || temp[start] > 0.0000000001)
        {
            break;
        }
        start++;
    }

    int newSize = lhs.size - start;
    double* resultCoeff = new double[newSize];


	// 예를 들어 2, 1, 0, -6 이라는 계수 배열을 가진 다항식이 있다고 하면,
	// 0을 곱할 경우 모든 계수가 0이 되어서 temp는 0, 0, 0, 0 이 될 것임
    // 그런데 이렇게 되면 사이즈가 4인 채로 남아버리게 되는데 이건 수학적으로 그냥 0임
	// 따라서 앞의 0들을 잘라서 0 하나만 남도록 하는 것임
    // 꼭 완전한 0이 아니어도 double타입이기 때문에 아주 작은 실수를 곱할 경우에도
    // 0으로 취급될 수 있음

    for (int i = 0; i < newSize; i++)
    {
		resultCoeff[i] = temp[start + i]; // temp에서 의미있는 값이 있던 인덱스부터 resultCoeff의 첫번째 인덱스에 순서대로 저장
    }

    Equation result(resultCoeff, newSize);

    delete[] temp;
    delete[] resultCoeff;

    return result;
}

Equation operator*(double lhs, const Equation& rhs) // 그냥 순서바꾸는 건데 위에 이미 오버로딩 해놓음
{
    return rhs * lhs;
}

ostream& operator<<(ostream& out, const Equation& eq)   
{
    bool first = true;      // 지금 출력하려는 항이 첫번째 항인지를 기억하는 용도
                                               // 첫 항과 다음 항은 출력 양식이 다르기 때문
    for (int i = 0; i < eq.size; i++)             // 최고차항부터 차례대로 확인
    {
        double current = eq.coeff[i];          // 현재 보고 있는 항의 계수를 current에 담는다

        if (current > -0.0000000001 && current < 0.0000000001) 
        {
            continue;                     // 거의 0인 값은 건너 뛰기
        }

        int power = eq.size - 1 - i;    // 현재 항의 차수 계산

		if (!first)                  // 첫번째 항이 아니면, 양수인 경우 앞에 + 붙여주기
        {
            if (current > 0)
            {
                out << "+";
            }
        }

        out << current;        // 이건 상수항에 다다르면 상수항을 출력하는 부분이기도 함.

        if (power >= 2)
        {
            out << "x^" << power;
        }
        else if (power == 1)
        {
            out << "x";
        }

		first = false;   // 첫 반복 이후엔 무조건 first는 false가 되도록 설정
    }

    if (first)
    {
        out << 0;
    }

    out << "=0";
    return out;
}

int main()
{
    int n1;
    cin >> n1;    // 첫번째 방정식 항 개수 입력받기

	double* input1 = new double[n1];     // 항 개수만큼의 크기를 가진 배열 동적 할당
                                         // Equation 객체를 만들기 전, 사용자 입력을 임시로 담는 버퍼

    while (true)            // 정답 입력이 나올 때까지 반복
    {
        for (int i = 0; i < n1; i++)
        {
            cin >> input1[i];
        }

        if (input1[0] == 0)       // 최고차항 계수 검사하고
        {
            cout << "Error: The highest order coefficient shouldn't be zero. Please enter again." << endl;
        }
        else
        {
			break;           // 최고차항 계수가 0이 아니면 입력받는 과정 종료
        }
    }

    Equation eq1(input1, n1);    
    delete[] input1;      // 객체 만들었으니까 input1은 메모리 제거

    int n2;
    cin >> n2;

    double* input2 = new double[n2];

    while (true)
    {
        for (int i = 0; i < n2; i++)
        {
            cin >> input2[i];
        }

        if (input2[0] == 0)
        {
            cout << "Error: The highest order coefficient shouldn't be zero. Please enter again." << endl;
        }
        else
        {
            break;
        }
    }

    Equation eq2(input2, n2);
    delete[] input2;

    cout << "eq1: " << eq1 << endl;
    cout << "eq2: " << eq2 << endl;

    Equation C = eq1 + eq2;
    cout << "eq1+eq2: " << C << endl;

    if (eq1 == eq2)
    {
        cout << "eq1 and eq2 are equal" << endl;
    }
    else
    {
        cout << "eq1 and eq2 are not equal" << endl;
        Equation D = eq1 - eq2;
        cout << "eq1-eq2: " << D << endl;
    }

    Equation eq3(eq1);
    cout << "eq3 after eq3(eq1): " << eq3 << endl;

    eq3 += eq2;
    cout << "eq3 after eq3+=eq2: " << eq3 << endl;

    Equation eq4 = eq3 * 0.5;
    cout << "eq3*0.5: " << eq4 << endl;

    Equation eq5 = 4 * eq3;
    cout << "4*eq3: " << eq5 << endl;

    return 0;
}