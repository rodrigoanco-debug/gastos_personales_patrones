# Gastos personales - Patrones de diseño en C++20

Programa de consola para ordenar y exportar gastos personales.

## Patrones usados

- **Factory Method:** desacopla la creación de los exportadores CSV, JSON y texto.
- **Strategy:** desacopla los criterios usados para ordenar los gastos.
- **Decorator:** permite agregar auditoría y resumen sin modificar los exportadores.

## Archivos principales

- `include/expenses.h`: estructuras, exportadores, decoradores y ordenamiento.
- `src/main.cpp`: ejemplo de uso del programa.
- `tests/test_expenses.cpp`: siete pruebas con `assert`.
- `.github/workflows/tests.yml`: compilación y pruebas automáticas en GitHub Actions.

## Compilar y ejecutar

```bash
cmake -S . -B build
cmake --build build
./build/expenses_app
```

## Ejecutar las pruebas

```bash
ctest --test-dir build --output-on-failure
```
