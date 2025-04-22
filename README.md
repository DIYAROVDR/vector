# Вектор: 3D Гидродинамический симулятор Black Oil (в разработке)

![Статус](https://img.shields.io/badge/status-в%20разработке-yellow) 
![Лицензия](https://img.shields.io/badge/license-MIT-blue) 
![Язык](https://img.shields.io/badge/C%2B%2B-17%2F20-blueviolet)
![Python](https://img.shields.io/badge/Python-3.10+-brightgreen)

<div align="center">
  <img src="https://github.com/user-attachments/assets/2202a024-6171-45c9-bae3-334801ee1b4d" alt="Логотип Вектор" style="width: 300px; height: auto; border: 1px solid #ddd; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1);"/>
</div>

**Вектор** — это 3D гидродинамический симулятор для моделирования Black Oil с открытым исходным кодом.

## 🛠 Технологический стек

```mermaid
graph TD
    A[Вектор] --> B[Qt 5]
    A --> C[Eigen 3]
    A --> D[HDF5 1.12]
    A --> E[VTK 9]
    B --> F[GUI]
    C --> G[Векторные вычисления]
    D --> H[Хранение данных модели]
    E --> I[3D рендеринг]
    E --> J[Python скриптинг]
