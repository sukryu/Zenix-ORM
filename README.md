📘 Zenix ORM

NestJS 철학을 따르는 C++ 백엔드 프레임워크를 위한 고성능 ORM 모듈

⸻

📌 개요

Zenix++ ORM은 C++20~26의 최신 기능을 기반으로 한 경량 ORM 라이브러리입니다.
Java Hibernate에서 영감을 받은 Session/Transaction 기반 구조, 타입 안전한 DSL(QueryBuilder), 준비된 statement 캐싱, 1차/2차 캐시, 마이그레이션 지원 등 엔터프라이즈급 기능을 제공합니다.
NestJS 스타일의 프레임워크에 자연스럽게 통합될 수 있도록 설계되었습니다.

⸻

🎯 주요 목표

목표	설명
💎 정확성	타입 안전한 쿼리 DSL + 컴파일 타임 검증
⚡ 성능	Prepared Statement 재사용, Batch Insert, Zero-Copy Mapping
🧱 안정성	세션/트랜잭션 단위 관리로 DB 무결성 확보
🧠 생산성	선언형 엔티티 등록, DSL 기반 마이그레이션, 프레임워크 통합
🧪 테스트 용이성	추상화된 DB 계층 및 모의 DB(Mocking) 지원


⸻

🏗️ 아키텍처 구성도

┌──────────────────────────────────────┐
│          Application Layer           │
│ ┌────────────┐  ┌──────────────────┐ │
│ │ Controller │→ │  Service (DI)    │ │
│ └────────────┘  └────────┬─────────┘ │
│                          ↓           │
│                    ┌────────────┐    │
│                    │ Repository │──┐ │
│                    └─────┬──────┘  │ │
└──────────────────────────┼─────────┘ │
                           ↓            │
             ┌──────────────────────────────┐
             │          Zenix++ ORM         │
             │ ┌────────────┐  ┌──────────┐ │
             │ │ Query<T>   │  │ Session  │ │
             │ │ Mapper<T>  │  │ TxScope  │ │
             │ └────┬───────┘  └────┬─────┘ │
             │      ↓               ↓       │
             │  PreparedStmt      Connection│
             └──────┬───────────────────────┘
                    ↓
              ┌────────────┐
              │ PostgreSQL │ (libpq)
              └────────────┘


⸻

📦 기본 사용법

1. 엔티티 정의 및 등록

struct User {
    int id;
    std::string name;
    int age;
};

REGISTER_ENTITY(User, "users",
    std::make_pair("id", &User::id),
    std::make_pair("name", &User::name),
    std::make_pair("age", &User::age));


⸻

2. SELECT 쿼리 (QueryBuilder DSL)

auto users = orm::select<User>()
    .where(field(&User::age) > 20 && field(&User::name) == "진혁")
    .order_by(&User::id)
    .limit(10)
    .exec();


⸻

3. INSERT

User newUser{.id = 1, .name = "홍진혁", .age = 24};
orm::insert(newUser);


⸻

4. 트랜잭션

orm::Session session;
orm::Transaction tx(session);

auto u = orm::select<User>().where(field(&User::id) == 1).exec_one();

u.age += 1;
orm::update(u);

tx.commit(); // or tx.rollback();


⸻

🔧 핵심 기능

기능	설명
🧩 Query DSL	select<T>().where(...) 기반의 타입 안전 SQL 작성
🧠 Entity Registry	런타임 메타데이터 기반, 템플릿 과다 사용 방지
📋 Prepared Statement	자동 캐싱 및 재사용
🔄 Migration DSL	create_table<T>(), alter_table<T>() 형태
🗃️ 1차/2차 캐시	세션별 객체 캐싱, 전역 캐시 (선택)
🔗 프레임워크 통합	@InjectRepository(User)처럼 DI 주입 가능
🧪 MockDB 지원	테스트 환경에서 실제 DB 없이 검증 가능


⸻

🛠️ 기술 스택

항목	선택 기술
언어	C++20 ~ C++26
메타정보	std::type_index, std::function, [[attributes]], 매크로 기반
쿼리 빌더	Expression Template + Operator Overloading
DB 연동	PostgreSQL (libpq) 기반 (MySQL, SQLite는 추후 추가)
비동기	Unifex + ASIO 기반 coroutine 지원 예정
테스트	Catch2, MockDB, Docker 기반 통합 테스트 지원 예정


⸻

🚀 Roadmap

단계	내용
✅ v0.1	Entity 등록 + select/insert DSL + 기본 mapper
🔄 v0.2	Migration DSL + 트랜잭션 + update/delete
🔜 v0.3	Async/Coroutine 지원 + 캐싱 레이어 추가
🔜 v1.0	NestJS 스타일 프레임워크와 완전 통합 (DI, Module 등)


⸻

📄 예제 프로젝트

예제 프로젝트는 /examples/basic_orm_app/ 디렉토리에 위치
CMake, Docker, .env로 PostgreSQL 연동 가능

⸻

📚 문서
	•	Docs: Entity 등록 방식
	•	Docs: Query Builder DSL
	•	Docs: 트랜잭션 처리
	•	Docs: 마이그레이션 시스템

⸻

⏱️ 시작일

이 프로젝트는 2026년 09월 09일부터 본격적으로 시작됩니다.

⸻