#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>

struct ZeroDivisionError : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "Cannot devide by zero";
  }
};

class Fraction {
 public:
  Fraction() = default;
  Fraction(int nume, int deno) {
    auto r = std::gcd(deno, nume);
    this->deno = deno / r;
    this->nume = nume / r;
    if (this->deno == 0) {
      throw std::runtime_error("Fraction cannot devide by zero");
    }
    if (this->deno < 0) {
      this->nume *= -1;
      this->deno *= -1;
    }
  };
  Fraction(int n) : deno(1), nume(n) {}
  Fraction(Fraction const &frac) = default;
  Fraction(Fraction &&frac) = default;
  ~Fraction() = default;

 private:
  void add(const Fraction &a) {
    auto ab_ba = a.deno * nume + deno * a.nume;
    auto ab = a.deno * deno;
    auto r = std::gcd(ab_ba, ab);
    nume = ab_ba / r;
    deno = ab / r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  void sub(const Fraction &a) {
    auto ab_ba = a.deno * nume - deno * a.nume;
    auto ab = a.deno * deno;
    auto r = std::gcd(ab_ba, ab);
    nume = ab_ba / r;
    deno = ab / r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  void mul(Fraction a) {
    deno *= a.deno;
    nume *= a.nume;
    auto r = std::gcd(deno, nume);
    nume /= r;
    deno /= r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  void div(Fraction a) {
    deno *= a.nume;
    nume *= a.deno;
    auto r = std::gcd(deno, nume);
    nume /= r;
    deno /= r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  [[nodiscard]] double eval() const { return static_cast<double>(nume) / deno; }

  Fraction &operator=(Fraction &&frac) noexcept = default;
  Fraction &operator=(const Fraction &frac) = default;

  friend std::ostream &operator<<(std::ostream &os, const Fraction &frac);
  friend Fraction operator+(Fraction a, const Fraction &b);
  friend Fraction operator-(Fraction a, const Fraction &b);
  friend Fraction operator*(Fraction a, const Fraction &b);
  friend Fraction operator/(Fraction a, const Fraction &b);

 public:
  void operator++(int a) { add(1); }
  void operator--(int a) { sub(1); }

 private:
  // nume / deno
  int_fast32_t deno;  // 分母
  int_fast32_t nume;  // 分子
};

std::ostream &operator<<(std::ostream &os, const Fraction &frac) {
  if (frac.deno == 1) {
    os << frac.nume;
  } else {
    os << "<frac:" << frac.nume << "/" << frac.deno << ">";
  }
  return os;
}

Fraction operator+(Fraction a, const Fraction &b) {
  a.add(b);
  return a;
}

Fraction operator-(Fraction a, const Fraction &b) {
  a.sub(b);
  return a;
}

Fraction operator*(Fraction a, const Fraction &b) {
  a.mul(b);
  return a;
}

Fraction operator/(Fraction a, const Fraction &b) {
  a.div(b);
  return a;
}

Fraction operator"" _f(unsigned long long a) { return {static_cast<int>(a)}; }

auto main() -> int {
  try {
    auto b = Fraction(4, 0);
  } catch (std::runtime_error e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
