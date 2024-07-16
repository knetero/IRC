#include "convert.hpp"

bool is_int(std::string param)
{
    int result = 0;
    int sign = 1;
    size_t index = 0;

    if (index < param.length() && (param[index] == '-' || param[index] == '+')) {
        if((param[index] = '-'))
            sign = -1;
        index++;
    }

    while (index < param.length()) {
        if (param[index] >= '0' && param[index] <= '9')
            result = result * 10 + (param[index] - '0');
        else
            return (false);
        index++;
    }
    // if ((sign * result) > 2147483647 || (sign * result) < -2147483648)
	// 	return (false);
    return true;
}

int ft_atoi(std::string str) {
    int result = 0;
    int sign = 1;
    size_t index = 0;

    if (index < str.length() && (str[index] == '-' || str[index] == '+')) {
        if((str[index] = '-'))
            sign = -1;
        index++;
    }

    while (index < str.length() && str[index] >= '0' && str[index] <= '9') {
        result = result * 10 + (str[index] - '0');
        index++;
    }
    return sign * result;
}

void cast_int(std::string param)
{
    char c = '\0';
    int n;
    float f;
    double d;
    
    n = static_cast<int>(ft_atoi(param));
    c = static_cast<char>(c);
    if(c < 32 || c == 127)
        std::cout << "char : " << "Non displayable" << std::endl;
    else
        std::cout << "char : " << c << std::endl;
    if (n > 2147483647 || n < -2147483648)
        std::cout << "int : " << "Impossible" << std::endl;
    else
        std::cout << "int : " << n << std::endl;
    f = static_cast<float>(c);
    std::cout << "float : " << f <<".0f"<< std::endl;
    d = static_cast<double>(c);
    std::cout << "double : " << d <<".0"<< std::endl;
}


void convert(std::string param)
{
     if (is_int(param))
           cast_int(param);
        else
            return;
}