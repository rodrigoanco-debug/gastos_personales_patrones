#ifndef EXPENSES_H
#define EXPENSES_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Expense {
    std::string date;
    std::string category;
    std::string detail;
    double amount{};
};

using ExpenseList = std::vector<Expense>;

template <class T>
concept ExpenseExporter =
    requires(T exporter, const ExpenseList& expenses) {
        { exporter.export_expenses(expenses) } -> std::same_as<std::string>;
    };

using ExportFunction = std::function<std::string(const ExpenseList&)>;

template <class R, class Cmp>
concept SortStrategy =
    std::ranges::random_access_range<R> &&
    std::sortable<std::ranges::iterator_t<R>, Cmp>;

void sort_with(auto& values, auto cmp)
    requires SortStrategy<decltype(values), decltype(cmp)>
{
    std::ranges::sort(values, cmp);
}

template <ExpenseExporter E, class... Args>
ExportFunction make_exporter(Args&&... args) {
    auto exporter = E{std::forward<Args>(args)...};

    return [exporter = std::move(exporter)](const ExpenseList& expenses) {
        return exporter.export_expenses(expenses);
    };
}

inline std::string amount_to_string(double amount) {
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << amount;
    return output.str();
}

struct CsvExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream output;
        output << "date,category,detail,amount\n";

        for (const auto& expense : expenses) {
            output << expense.date << ','
                   << expense.category << ','
                   << expense.detail << ','
                   << amount_to_string(expense.amount) << '\n';
        }

        return output.str();
    }
};

struct JsonExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream output;
        output << "[\n";

        for (std::size_t i = 0; i < expenses.size(); ++i) {
            const auto& expense = expenses[i];

            output << "  {"
                   << "\"date\": \"" << expense.date << "\", "
                   << "\"category\": \"" << expense.category << "\", "
                   << "\"detail\": \"" << expense.detail << "\", "
                   << "\"amount\": " << amount_to_string(expense.amount)
                   << "}";

            if (i + 1 < expenses.size()) {
                output << ',';
            }
            output << '\n';
        }

        output << "]\n";
        return output.str();
    }
};

struct TextExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::ostringstream output;

        for (const auto& expense : expenses) {
            output << expense.date << " | "
                   << expense.category << " | "
                   << expense.detail << " | "
                   << amount_to_string(expense.amount) << '\n';
        }

        return output.str();
    }
};

template <ExpenseExporter Inner>
struct AuditedExporter {
    Inner inner;

    std::string export_expenses(const ExpenseList& expenses) const {
        std::string output = inner.export_expenses(expenses);
        output += "audit: " + std::to_string(expenses.size()) + " expenses exported\n";
        return output;
    }
};

template <ExpenseExporter Inner>
struct SummaryExporter {
    Inner inner;

    std::string export_expenses(const ExpenseList& expenses) const {
        double total = 0.0;

        for (const auto& expense : expenses) {
            total += expense.amount;
        }

        std::string output = inner.export_expenses(expenses);
        output += "total: " + amount_to_string(total) + '\n';
        return output;
    }
};

#endif
