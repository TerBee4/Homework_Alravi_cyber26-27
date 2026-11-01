#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <stdexcept>
#include <cstdint>

#include "Order.hpp"
#include "Kitchen.hpp"
#include "Simulation.hpp"


std::string convertOrderStatus(OrderStatus status) {
    switch (status) {
        case OrderStatus::Waiting:
            return "Waiting";

        case OrderStatus::Cooking:
            return "Cooking";

        case OrderStatus::Ready:
            return "Ready";
    }
    return "Unknown";
}

std::string convertDishStatus(DishStatus status) {
    switch (status) {
        case DishStatus::Waiting:
            return "Waiting";

        case DishStatus::Cooking:
            return "Cooking";

        case DishStatus::Ready:
            return "Ready";
    }
    return "Unknown";
}

std::string convertSimMode(SimulationMode mode) {
    switch (mode) {
        case SimulationMode::Automatic:
            return "Automatic";

        case SimulationMode::Manual:
            return "Manual";
    }
    return "Unknown";
}

std::string convertPolicy(SchedulingPolicy policy) {
    switch (policy) {
        case SchedulingPolicy::FIFO:
            return "FIFO";

        case SchedulingPolicy::EDF:
            return "EDF";
    }
    return "Unknown";
}

template <typename T>
T readNumber(const std::string& prompt, T min, T max) {
    while (true) {
        std::cout << prompt;

        std::string line;

        if (!std::getline(std::cin >> std::ws, line)) {
            throw std::runtime_error("Ввод завершён");
        }

        std::istringstream input(line);
        T value;
        char extra;

        if ((input >> value) && !(input >> extra) &&
            value >= min && value <= max) {
            return value;
        }
        std::cout << "Некорректное значение. Попробуйте снова.\n";
    }
}


int main() {
    std::vector<Recipe> menu = {
    Recipe("Pizza", {
        {Ingredient::Flour, 200},
        {Ingredient::Tomato, 100},
        {Ingredient::Cheese, 100}
    }, 8),

    Recipe("Omelette", {
        {Ingredient::Egg, 150},
        {Ingredient::Milk, 50}
    }, 4),

    Recipe("Salad", {
        {Ingredient::Cucumber, 100},
        {Ingredient::Tomato, 100},
        {Ingredient::Greens, 100}
    }, 2),

    Recipe("Burger", {
        {Ingredient::Bread, 100},
        {Ingredient::Meat, 150},
        {Ingredient::Tomato, 50},
        {Ingredient::Cheese, 50}
    }, 7),

    Recipe("Fish strips", {
        {Ingredient::Fish, 200},
        {Ingredient::Bread, 250}
    }, 4),

    Recipe("Pinakolada", {
        {Ingredient::Sugar, 30},
        {Ingredient::Milk, 150}
    }, 3),

    Recipe("Cheesecake", {
        {Ingredient::CreamCheese, 150},
        {Ingredient::Sugar, 50},
        {Ingredient::Milk, 50}
    }, 10)
    };

    Storage storage({
        {Ingredient::Bread, 1000}, {Ingredient::Tomato, 1000}, {Ingredient::Cheese, 1000},
        {Ingredient::Egg, 1000}, {Ingredient::Milk, 1000}, {Ingredient::Cucumber, 1000},
        {Ingredient::Greens, 1000}, {Ingredient::Meat, 1000}, {Ingredient::Flour, 1000},
        {Ingredient::Fish, 1000}, {Ingredient::Sugar, 1000}, {Ingredient::CreamCheese, 1000}
        });


    std::cout << "\n========== НАСТРОЙКА РЕСТОРАНА ==========\n";

    int cookCount = readNumber<int>("Количество поваров: ", 1, 100);
    int workplaceCount = readNumber<int>("Количество рабочих мест: ", 1, 100);
    std::int64_t seedInput = readNumber<std::int64_t>("Seed генератора: ", 0,
        std::numeric_limits<std::uint32_t>::max());
    std::uint32_t seed = static_cast<std::uint32_t>(seedInput);
    double probability = readNumber<double>("Вероятность появления заказа (0-1): ", 0.0, 1.0);

    std::vector<Cook> cooks;

    for (int i = 1; i <= cookCount; ++i) {
        cooks.emplace_back(i);
    }

    std::vector<Workplace> workplaces;

    for (int i = 1; i <= workplaceCount; ++i) {
        workplaces.emplace_back(i);
    }
    
    Kitchen kitchen(cooks, workplaces, storage);
    Simulation sim(kitchen);
    OrderGenerator generator(seed, probability, menu);
    sim.setOrderGenerator(generator);

    const std::vector<std::pair<Ingredient, std::string>> ingredients = {
    {Ingredient::Bread, "Bread"},
    {Ingredient::Tomato, "Tomato"},
    {Ingredient::Cheese, "Cheese"},
    {Ingredient::Egg, "Egg"},
    {Ingredient::Milk, "Milk"},
    {Ingredient::Cucumber, "Cucumber"},
    {Ingredient::Greens, "Greens"},
    {Ingredient::Meat, "Meat"},
    {Ingredient::Flour, "Flour"},
    {Ingredient::Fish, "Fish"},
    {Ingredient::Sugar, "Sugar"},
    {Ingredient::CreamCheese, "CreamCheese"}
    };

    int choice = -1;

    while (choice != 0) {
        std::cout << "\n========== РЕСТОРАН ==========\n";
        std::cout << "Текущий тик: " << sim.getCurrentTick() << "\n";
        std::cout << "Режим: " << convertSimMode(sim.getMode()) << '\n';
        std::cout << "Алгоритм: " << convertPolicy(sim.getSchedulingPolicy()) << "\n\n";

        std::cout << "1. Выполнить один тик\n";
        std::cout << "2. Выполнить N тиков\n";
        std::cout << "3. Посмотреть заказы\n";
        std::cout << "4. Изменить режим работы\n";
        std::cout << "5. Запустить приготовление блюда вручную\n";
        std::cout << "6. Пополнить склад\n";
        std::cout << "7. Посмотреть журнал событий\n";
        std::cout << "8. Посмотреть статистику\n";
        std::cout << "9. Изменить алгоритм планирования\n";
        std::cout << "10. Сохранить журнал и статистику\n";
        std::cout << "0. Завершить программу\n";

        std::cout << "\nВыберите действие: ";

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }

            std::cout << "\nОшибка: необходимо целое положительное число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                sim.nextTick();
                std::cout << "Выполнен один тик.\n";
                break;
            }
            case 2: {
                std::string line;
                int n;
                char extra;

                std::cout << "Введите количество тиков: ";

                if (!std::getline(std::cin >> std::ws, line)) {
                    choice = 0;
                    break;
                }

                std::istringstream input(line);

                if (!(input >> n) || n <= 0 || (input >> extra)) {
                    std::cout << "Ошибка: введите целое положительное число.\n";
                    break;
                }

                for (int i = 0; i < n; ++i) {
                    sim.nextTick();
                }

                std::cout << "Выполнено тиков: " << n << '\n';

                break;
            }
            case 3: {
                const auto& orders = sim.getViewOrders();

                std::cout << "\n========== СПИСОК ЗАКАЗОВ ==========\n";

                if (orders.empty()) {
                    std::cout << "Заказов пока нет.\n";
                    break;
                }

                for (const Order& order : orders) {
                    std::cout << "\nЗаказ №" << order.getOrderId() << '\n';
                    std::cout << "Дедлайн: " << order.getDeadline() << '\n';
                    std::cout << "Статус: " << convertOrderStatus(order.getStatus()) << '\n';
                    std::cout << "Блюда:\n";

                    const auto& dishes = order.getOrderDishes();

                    for (std::size_t i = 0; i < dishes.size(); ++i) {
                        std::cout << "  " << i + 1 << ". "
                                << dishes[i].getRecipe().getName() << " — "
                                << convertDishStatus(dishes[i].getStatus()) << '\n';
                    }
                }
                break;
            }
            case 4: {
                std::cout << "\nВыберите режим работы:\n";
                std::cout << "1. Automatic\n";
                std::cout << "2. Manual\n";
                std::cout << "Ваш выбор: ";

                std::string line;

                if (!std::getline(std::cin >> std::ws, line)) {
                    choice = 0;
                    break;
                }
                if (line == "1") {
                    sim.setMode(SimulationMode::Automatic);
                    std::cout << "Установлен автоматический режим.\n";
                }
                else if (line == "2") {
                    sim.setMode(SimulationMode::Manual);
                    std::cout << "Установлен ручной режим.\n";
                }
                else {
                    std::cout << "Неизвестный режим.\n";
                }
                break;
            }
            case 5: {
                if (sim.getMode() != SimulationMode::Manual) {
                    std::cout << "Сначала переключитесь в ручной режим.\n";
                    break;
                }
                if (sim.getCurrentTick() < 0) {
                    std::cout << "Сначала выполните хотя бы один тик.\n";
                    break;
                }
                std::cout << "Введите ID заказа и номер блюда: ";

                std::string line;

                if (!std::getline(std::cin >> std::ws, line)) {
                    choice = 0;
                    break;
                }

                std::istringstream input(line);

                int orderId;
                int dishNumber;
                char extra;

                if (!(input >> orderId >> dishNumber) || orderId <= 0 || dishNumber <= 0 || (input >> extra)) {
                    std::cout << "Ошибка: введите два положительных целых числа.\n";
                    break;
                }

                try {
                    bool started = sim.startDish(orderId, static_cast<std::size_t>(dishNumber - 1)
                    );

                    if (started) {
                        std::cout << "Приготовление блюда запущено.\n";
                    } else {
                        std::cout << "Недостаточно ингредиентов или свободных ресурсов.\n";
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << '\n';
                }
                break;
            }
            case 6: {
                std::cout << "\n========== ПОПОЛНЕНИЕ СКЛАДА ==========\n";

                for (std::size_t i = 0; i < ingredients.size(); ++i) {
                    std::cout << i + 1 << ". " << ingredients[i].second << '\n';
                }
                std::cout << "\nВведите номер ингредиента и количество в граммах: ";

                std::string line;

                if (!std::getline(std::cin >> std::ws, line)) {
                    choice = 0;
                    break;
                }

                std::istringstream input(line);

                int ingredientNumber;
                Grams amount;
                char extra;

                if (!(input >> ingredientNumber >> amount) || ingredientNumber <= 0
                    || static_cast<std::size_t>(ingredientNumber) > ingredients.size()
                    || amount <= 0 || (input >> extra)) {
                    std::cout << "Ошибка: некорректный ингредиент или количество.\n";
                    break;
                }

                Ingredient ingredient = ingredients[ingredientNumber - 1].first;

                try {
                    sim.restock(ingredient, amount);
                    std::cout << "Склад успешно пополнен.\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка пополнения: " << e.what() << '\n';
                }
                break;
            }
            case 7: {
                const auto& entries = sim.getJournal().getEntries();
                std::cout << "\n========== ЖУРНАЛ СОБЫТИЙ ==========\n";
                if (entries.empty()) {
                    std::cout << "Журнал пока пуст.\n";
                    break;
                }
                for (const JournalEntry& entry : entries) {
                    std::cout << "Тик " << entry.tick << ": " << entry.description << '\n';
                }
                break;
            }
            case 8: {
                const auto& history = sim.getStatistics().getStatisticsList();
                std::cout << "\n========== СТАТИСТИКА РЕСТОРАНА ==========\n";

                if (history.empty()) {
                    std::cout << "Статистика пока отсутствует.\n";
                    break;
                }
                const TickStatistics& stat = history.back();

                std::cout << std::fixed << std::setprecision(1);
                std::cout << "Текущий тик: " << stat.tick << "\n\n";
                std::cout << "Ожидающих заказов: " << stat.waitingOrders << '\n';
                std::cout << "Выполнено вовремя: " << stat.onTimeProcent << "%\n";
                std::cout << "Загрузка поваров: " << stat.cookUtilization << "%\n";
                std::cout << "Загрузка рабочих мест: " << stat.workplaceUtilization << "%\n";
                break;
            }
            case 9: {
                std::cout << "\nВыберите алгоритм планирования:\n";
                std::cout << "1. FIFO\n";
                std::cout << "2. EDF\n";
                std::cout << "Ваш выбор: ";

                std::string line;

                if (!std::getline(std::cin >> std::ws, line)) {
                    choice = 0;
                    break;
                }
                if (line == "1") {
                    sim.setScheduler(SchedulingPolicy::FIFO);
                    std::cout << "Установлен алгоритм FIFO.\n";
                }
                else if (line == "2") {
                    sim.setScheduler(SchedulingPolicy::EDF);
                    std::cout << "Установлен алгоритм EDF.\n";
                }
                else {
                    std::cout << "Ошибка: неизвестный алгоритм.\n";
                }
                break;
            }
            case 10: {
                try {
                    sim.getJournal().saveToFile("journal.txt");
                    std::cout << "Журнал сохранён в journal.txt\n";

                    sim.getStatistics().saveToCsv("statistics.csv");
                    std::cout << "Статистика сохранена в statistics.csv\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка сохранения: " << e.what() << '\n';
                }
                std::system("gnuplot plots.gp");
                break;
            }
            case 0: {
                std::cout << "Завершение программы.\n";
                break;
            }

            default: {
                std::cout << "Неизвестная команда.\n";
                break;
            }
        }
    }

    sim.getJournal().saveToFile("journal.txt");
    sim.getStatistics().saveToCsv("statistics.csv");
    
    return 0;
}