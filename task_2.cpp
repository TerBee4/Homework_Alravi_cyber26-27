#include <iostream>
#include <random>
#include <chrono>
#include <format>
#include <cstdio>

namespace Values {

    template<typename Tag>
    class Value {
        double val;

        public:
        constexpr explicit Value(double value) : val(value) {}
        constexpr double get_val() const {return val;}
    };

    struct Amper: Value<Amper> {using Value<Amper>::Value; };
    struct Volt: Value<Volt> {using Value<Volt>::Value; };
    struct Ohm: Value<Ohm> {using Value<Ohm>::Value; };
    struct Joule: Value<Joule> {using Value<Joule>::Value; };
    struct Watt: Value<Watt> {using Value<Watt>::Value; };
    struct Sec: Value<Sec> {using Value<Sec>::Value; };

    //long double пушо так в лекции было
    // суффиксы
    constexpr Amper operator""_mA(long double val) {
        return Amper(static_cast<double>(val) / 1000.0);
    }

    constexpr Volt operator""_V(long double val) {
        return Volt(static_cast<double>(val));
    }

    constexpr Ohm operator""_Ohm(long double val) {
        return Ohm(static_cast<double>(val));
    }

    constexpr Joule operator""_J(long double val) {
        return Joule(static_cast<double>(val));
    }

    constexpr Watt operator""_W(long double val) {
        return Watt(static_cast<double>(val));
    }

    constexpr Sec operator""_S(long double val) {
        return Sec(static_cast<double>(val));
    }

    //перегрузка операторов с физикал законами
    //V=I*R
    constexpr Volt operator*(const Amper& a, const Ohm& b) {
        return Volt(a.get_val() * b.get_val());
    }
    //V=R*I
    constexpr Volt operator*(const Ohm& a, const Amper& b) {
        return Volt(a.get_val() * b.get_val());
    }
    //R=V/I
    constexpr Ohm operator/(const Volt& a, const Amper& b) {
        if (b.get_val() == 0) throw std::runtime_error("Деление на 0 ампер");
        return Ohm(a.get_val() / b.get_val());
    }
    //I=V/R
    constexpr Amper operator/(const Volt& a, const Ohm& b) {
        if (b.get_val() == 0) throw std::runtime_error("Деление на 0 ом");
        return Amper(a.get_val() / b.get_val());
    }
    //P=V*I
    constexpr Watt operator*(const Volt& a, const Amper& b) {
        return Watt(a.get_val() * b.get_val());
    }
    //P=I*V
    constexpr Watt operator*(const Amper& a, const Volt& b) {
        return Watt(a.get_val() * b.get_val());
    }
    //I=P/V
    constexpr Amper operator/(const Watt& a, const Volt& b) {
        if (b.get_val() == 0) throw std::runtime_error("Деление на 0 вольт");
        return Amper(a.get_val() / b.get_val());
    }
    //V=P/I
    constexpr Volt operator/(const Watt& a, const Amper& b) {
        if (b.get_val() == 0) throw std::runtime_error("Деление на 0 ампер");
        return Volt(a.get_val() / b.get_val());
    }
    //E=P*t
    constexpr Joule operator*(const Watt& a, const Sec& b) {
        return Joule(a.get_val() * b.get_val());
    }
    //E=t*P
    constexpr Joule operator*(const Sec& a, const Watt& b) {
        return Joule(a.get_val() * b.get_val());
    }
    //P=E/t
    constexpr Watt operator/(const Joule& a, const Sec& b) {
        if (b.get_val() == 0) throw std::runtime_error("Деление на 0 секунд");
        return Watt(a.get_val() / b.get_val());
    }
    //t=E/P
    constexpr Sec operator/(const Joule& a, const Watt& b) {
        if (b.get_val() == 0) throw std::runtime_error("Деление на 0 ватт");
        return Sec(a.get_val() / b.get_val());
    }
};


template<class Values>
class Measuring {
    std::chrono::system_clock::time_point time;
    Values value;
    Values error;
    public:
    Measuring(std::chrono::system_clock::time_point t, Values val, Values err) : 
    time(t), value(val), error(err) {}

    const auto& get_time() const {return time; }
    const Values& get_value() const {return value; }
    const Values& get_error() const {return error; }
};

template<class Values>
struct std::formatter<Measuring<Values>> {

    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(
        const Measuring<Values>& measurement, std::format_context& ctx) const
         {
        return std::format_to(
            ctx.out(),
            "value = {}, error = {}, time = {}",
            measurement.get_value().get(),
            measurement.get_error().get(),
            std::chrono::duration_cast<std::chrono::seconds>(
                measurement.get_time().time_since_epoch()).count());
    }
};


namespace Meter {
    class Voltmeter {
        Values::Volt min_voltage;
        Values::Volt max_voltage;
        double accuracy;
        std::string identif;

        public:
        Voltmeter(Values::Volt min_val, Values::Volt max_val, double accur, std::string id) : 
        min_voltage(min_val), max_voltage(max_val), accuracy(accur), identif(id) 
        {
            if (min_val.get_val() > max_val.get_val()) {throw std::invalid_argument("Неверный диапазон значений");}
            if (accur <= 0) {throw std::invalid_argument("Класс точности должен быть положителен");}
            if (id.empty()) {throw std::invalid_argument("Пустой идентификатор");}
        }

        Measuring<Values::Volt> measure() const {
            static std::random_device random_device;
            static std::mt19937 generator(random_device());

            std::uniform_real_distribution<double> distrib(min_voltage.get_val(),
             max_voltage.get_val());

            double measured_val = distrib(generator);
            double error_val = max_voltage.get_val() * accuracy / 100.0;
            
            return Measuring<Values::Volt>(std::chrono::system_clock::now(), 
                Values::Volt(measured_val), Values::Volt(error_val));
        };
    };

    class Ampermeter {
        Values::Amper min_amper;
        Values::Amper max_amper;
        double accuracy;
        std::string identif;

        public:
        Ampermeter(Values::Amper min_val, Values::Amper max_val, double accur, std::string id) : 
        min_amper(min_val), max_amper(max_val), accuracy(accur), identif(id)
        {
            if (min_val.get_val() > max_val.get_val()) {throw std::invalid_argument("Неверный диапазон значений");}
            if (accur <= 0) {throw std::invalid_argument("Класс точности должен быть положителен");}
            if (id.empty()) {throw std::invalid_argument("Пустой идентификатор");}
        }

        Measuring<Values::Amper> measure() const {
            static std::random_device random_device;
            static std::mt19937 generator(random_device());

            std::uniform_real_distribution<double> distrib(min_amper.get_val(),
             max_amper.get_val());

            double measured_val = distrib(generator);
            double error_val = max_amper.get_val() * accuracy / 100.0;
            
            return Measuring<Values::Amper>(std::chrono::system_clock::now(), 
                Values::Amper(measured_val), Values::Amper(error_val));
        };
    };

    template<class Values>
    class Multimeter {
        Values min_value;
        Values max_value;
        double accuracy;
        std::string identif;

        public:
        Multimeter(Values min_val, Values max_val, double accur, std::string id) :
         min_value(min_val), max_value(max_val), accuracy(accur), identif(id)  {
            if (min_val.get_val() > max_val.get_val()) {
            throw std::invalid_argument("Неверный диапазон");}

            if (accur <= 0) {throw std::invalid_argument("Класс точности должен быть положителен");}

            if (id.empty()) {throw std::invalid_argument("Идентификатор не может быть пустым");}
        }
        Measuring<Values> measure() const {
        
        static std::random_device random_device;
        static std::mt19937 generator(random_device());

        std::uniform_real_distribution<double> distribution(
            min_value.get_val(), max_value.get_val());

        double measured_value = distribution(generator);
        double error_value = max_value.get_val() * accuracy / 100.0;

        return Measuring<Values>(std::chrono::system_clock::now(),
            Values(measured_value), Values(error_value));
        };
    };

};



void test_values() {
    using namespace Values;

    auto voltage = 220.0_V;
    auto resistance = 110.0_Ohm;

    auto current = voltage / resistance;

    std::cout << "Величины\n";

    std::cout << "Напряжение: "
              << voltage.get_val()
              << " V\n";

    std::cout << "Сопротивление: "
              << resistance.get_val()
              << " Ohm\n";

    std::cout << "Ток: "
              << current.get_val()
              << " A\n";
}

void test_operators() {
    using namespace Values;

    auto voltage = 220.0_V;
    auto tok = 2000.0_mA;
    auto power = voltage * tok;
    auto time = 10.0_S;
    auto energy = power * time;

    std::cout << "\n Операторы \n";

    std::cout << "Мощность: "
              << power.get_val()
              << " W\n";

    std::cout << "Энергия: "
              << energy.get_val()
              << " J\n";
}

void test_Measuring() {

    using namespace Values;

    Measuring<Volt> measurement(
        std::chrono::system_clock::now(),
        220.0_V,
        3.3_V
    );
    std::cout << "\n Измерение \n";

    std::cout << "Значение: "
              << measurement.get_value().get_val()
              << " V\n";

    std::cout << "Погрешность: "
              << measurement.get_error().get_val()
              << " V\n";
}

void test_Devices() {

    using namespace Values;
    using namespace Meter;

    std::cout << "\n Приборы \n";

    Voltmeter voltmeter(
        0.0_V,
        250.0_V,
        1.5,
        "V-001"
    );

    auto voltage_measurement =
        voltmeter.measure();

    std::printf(
        "Вольтметр: %.2f V +- %.2f V\n",
        voltage_measurement.get_value().get_val(),
        voltage_measurement.get_error().get_val()
    );


    Ampermeter ammeter(
        0.0_mA,
        5000.0_mA,
        2.0,
        "A-001"
    );

    auto current_measurement =
        ammeter.measure();

    std::printf(
        "Амперметр: %.2f A +- %.2f A\n",
        current_measurement.get_value().get_val(),
        current_measurement.get_error().get_val()
    );


    Multimeter<Volt> multimeter(
        0.0_V,
        600.0_V,
        1.0,
        "MM-001"
    );

    auto multimeter_measurement =
        multimeter.measure();

    std::printf(
        "Мультиметр: %.2f V +- %.2f V\n",
        multimeter_measurement.get_value().get_val(),
        multimeter_measurement.get_error().get_val()
    );
};

int main() {

    test_values();
    test_operators();
    test_Measuring();
    test_Devices();

    return 0;
};