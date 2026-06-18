#include "expenses.h"

#include <iostream>

int main() {
    ExpenseList expenses{
        {"2026-06-01", "food", "lunch", 18.50},
        {"2026-06-02", "transport", "bus", 3.20},
        {"2026-06-03", "books", "cpp", 45.00}
    };

    auto by_amount = [](const Expense& a, const Expense& b) {
        return a.amount > b.amount;
    };

    sort_with(expenses, by_amount);

    auto exporter =
        AuditedExporter{
            SummaryExporter{
                CsvExporter{}
            }
        };

    std::cout << exporter.export_expenses(expenses);

    return 0;
}
