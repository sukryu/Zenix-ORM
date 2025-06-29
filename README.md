📘 README.md – Zenix++ ORM

A high-performance, type-safe ORM for modern C++20~26. Inspired by Hibernate and designed for structured backend development with NestJS-like architecture.

⸻

📌 Table of Contents
	1.	Introduction
	2.	Features
	3.	Architecture Overview
	4.	Getting Started
	5.	Usage
	•	Entity Definition
	•	Query Builder
	•	Insert/Update/Delete
	•	Transaction
	6.	Integration
	7.	Performance Considerations
	8.	Future Plans
	9.	License

⸻

🧭 Introduction

Zenix++ ORM is a modern, zero-overhead, and highly extensible ORM designed for C++20 and beyond.
It brings the productivity of Java’s Hibernate to the C++ world without sacrificing compile-time safety, runtime performance, and modular design.

This project is a part of the larger effort to build a NestJS-style backend framework in C++, optimized for real-world backend and microservice development.

⸻

✨ Features
	•	🏷️ Declarative Entity Schema with macros or constexpr DSL
	•	⚡ High-performance query builder with static analysis and prepared statements
	•	🔒 RAII-based transaction management with nested savepoint support
	•	🧠 Reflection-like entity metadata system without relying on RTTI
	•	🔌 Pluggable backend drivers: PostgreSQL, SQLite, MySQL (planned)
	•	📊 Built-in migration, caching, and performance logging
	•	⚙️ Seamless integration with NestJS-style framework in C++

⸻

🏛️ Architecture Overview

[ Entity Registry ]
       ↓
[ Metadata Layer ] -- (Entity<T> → Table, Column[])
       ↓
[ Query Builder ] -- (select<T>().where(...).order_by(...))
       ↓
[ Query Executor ]
       ↓
[ Row Mapper (row → T) ]
       ↓
[ Async I/O / Connection Pool ]


⸻

🚀 Getting Started

Prerequisites
	•	Compiler supporting C++20 or newer (C++23/26 recommended)
	•	CMake 3.20+
	•	Supported DB Driver: PostgreSQL (libpq) or SQLite3

Installation

git clone https://github.com/yourname/zenixpp-orm.git
cd zenixpp-orm
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make


⸻

🧪 Usage

✅ Entity Definition

struct User {
    int id;
    std::string name;
    int age;
};

REGISTER_ENTITY(User, "users",
    std::make_pair("id", &User::id),
    std::make_pair("name", &User::name),
    std::make_pair("age", &User::age)
);


⸻

🔍 Query Builder

auto query = orm::select<User>()
    .where(field(&User::age) > 20 && field(&User::name) == "진혁")
    .order_by(&User::id)
    .limit(10);


⸻

✏️ Insert / Update / Delete

User user{.id = 1, .name = "진혁", .age = 24};
orm::insert(user);

user.age = 25;
orm::update(user);

orm::remove(user);


⸻

🔐 Transaction

orm::DbSession session;

try {
    orm::Transaction tx(session);
    orm::insert(user);
    orm::insert(other_user);
    tx.commit();  // Automatically rolls back if not committed
} catch (const std::exception& e) {
    // rollback triggered automatically
}


⸻

🧩 Integration

Zenix++ ORM is designed to integrate cleanly with NestJS-style modular frameworks in C++.
You can inject repository classes via custom DI containers and reuse entity logic across modules.

class UserRepository {
public:
    std::vector<User> find_adults() {
        return orm::select<User>().where(field(&User::age) >= 18).execute();
    }
};


⸻

🛠️ Performance Considerations

최적화 항목	적용 방식
Prepared Statement	Query pre-compilation + binding
Zero-copy Row Mapping	Struct layout-aware optimization
Async I/O	Coroutine 기반 Unifex + libpq
Connection Pool	Future work (planned module)
Compile-time control	Type-indexed metadata cache, macro DSL


⸻

🧭 Future Plans
	•	Support for JOINs, subqueries, unions
	•	Codegen-based constexpr entity definition DSL
	•	Migration CLI: zenix migrate up/down
	•	Auto schema diffing / version control
	•	Pluggable DB backend (MySQL, Redis view layer)
	•	C++26 std::meta integration (optional)

⸻

📄 License

MIT License.
Feel free to fork, customize, and contribute.

⸻

🗓️ Project Schedule

The Zenix++ ORM project officially kicks off on 2026년 09월 09일.
Stay tuned for alpha releases and module documentation.

⸻