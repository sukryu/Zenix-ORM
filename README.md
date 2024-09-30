# Zenix ORM #

해당 프로젝트는 Java의 Hibernate를 모방하여 만들어진 Node.js의 C++ Addon 기능을 활용한 고성능 ORM 입니다.

ORM Feature Roadmap: 초기 단계부터 프로덕트 단계까지 (Hibernate 모방)
1. 초기 단계 (Minimum Viable Product)

기본 CRUD 작업 (Create, Read, Update, Delete)
단일 테이블 쿼리 지원
기본 데이터 타입 매핑 (정수, 문자열, 날짜 등)
간단한 WHERE 조건 지원
기본적인 에러 처리

2. 개발 단계

관계 매핑 (1:1, 1:N, N:M)
트랜잭션 지원
복잡한 WHERE 조건 및 AND, OR 연산자 지원
ORDER BY, LIMIT, OFFSET 지원
집계 함수 지원 (COUNT, SUM, AVG 등)
조인 쿼리 지원
마이그레이션 도구 기초 버전
기본적인 쿼리 빌더
연결 풀링

3. 확장 단계

서브쿼리 지원
복잡한 조인 쿼리 (LEFT JOIN, RIGHT JOIN, FULL JOIN)
고급 데이터 타입 지원 (JSON, ARRAY 등)
인덱스 생성 및 관리
벌크 삽입/업데이트 작업
레이지 로딩과 이거 로딩
캐싱 메커니즘
쿼리 로깅 및 성능 모니터링
고급 마이그레이션 도구 (롤백 지원 등)

4. 최적화 단계

쿼리 최적화 (실행 계획 분석)
prepared statements 지원
동시성 제어 및 락 관리
샤딩 지원
읽기/쓰기 분리
고급 캐싱 전략 (Redis 등과의 통합)
비동기 쿼리 실행

5. 프로덕트 단계

다중 데이터베이스 지원 (MySQL, PostgreSQL, SQLite 등)
보안 기능 (SQL 인젝션 방지, 데이터 암호화 등)
감사 로깅 (Audit logging)
풍부한 쿼리 DSL (Domain Specific Language)
플러그인 시스템
스키마 검증 및 마이그레이션 자동화
성능 튜닝 도구
고가용성 지원 (복제, 페일오버)
확장 가능한 이벤트 시스템 (훅)
다국어 지원
상세한 문서화 및 예제

6. 엔터프라이즈 단계

대규모 분산 트랜잭션 지원
실시간 모니터링 및 알림 시스템
고급 데이터 분석 도구 통합
머신러닝 모델 통합 (예측 쿼리 최적화)
컴플라이언스 및 규제 지원 (GDPR, HIPAA 등)
클라우드 네이티브 기능 (자동 스케일링, 서버리스 지원 등)
고급 보안 기능 (행 수준 보안, 열 수준 암호화 등)
다중 테넌시 지원
글로벌 분산 데이터베이스 지원


## 아키텍처 개요 ##

- ORM은 다음과 같은 계층으로 구성됩니다:
	
	1. 데이터베이스 레이어: 데이터베이스와의 실제 연결과 상호 작용을 처리합니다.
	2. 코어 레이어: 세션 관리, 트랜잭션 관리 등의 핵심 ORM 로직을 포함합니다.
	3. 매핑 레이어: 엔티티와 데이터베이스 테이블 간의 매핑을 관리합니다.
	4. 쿼리 레이어: 유연한 쿼리 빌더와 동적 쿼리 생성을 제공합니다.
	5. API 레이어: Node.js 환경에서 사용할 수 있는 JavaScript API를 제공합니다.

## 디자인 패턴 적용 ##

- 추상 팩토리 패턴(Abstract Factory): 데이터베이스 연결 객체 생성 시 사용하여 다양한 데이터베이스 지원을 용이하게 합니다.
- 전략 패턴(Strategy): 데이터베이스별로 다른 구현체를 선택할 수 있도록 합니다.
- 싱글톤 패턴(Singleton): 전역적으로 하나의 인스턴스만 존재해야 하는 클래스에 적용합니다.
- 데이터 매퍼 매턴(Data Mapper): 엔티티와 데이터베이스 간의 매핑을 담당합니다.
- 빌더 패턴(Builder): 복잡한 객체나 쿼리를 단계별로 생성합니다.
- 프록시 패턴(Proxy): 엔티티의 지연 로딩을 지원합니다.
- 옵저버 패턴(Observer): 이벤트 발생 시 리스너에게 알림을 보냅니다.
- 의존성 주입(DI): 컴포넌트 간의 결합도를 낮추고 테스트 용이성을 높입니다.

## 컴포넌트 상세 설계 ##

1. 데이터베이스 레이어:
	1. IDatabaseConnection 인터페이스
		- 역할: 데이터베이스 연결에 대한 공통 인터페이스를 제공합니다.
		- 메서드:
			- `connect()`
			- `disconnect()`
			- `executeQuery(query)`
			- `beginTransaction()`
			- `commit()`
			- `rollback()`

	2. SQLiteConnection 클래스
		- 역할: `IDatabaseConnection`을 구현하여 SQLite3에 특화된 기능을 제공합니다.
		- 디자인 패턴: 전략 패턴과 추상 팩토리 패턴을 적용하여 데이터베이스별 구현체를 선택합니다.

	3. DatabaseConnectionFactory 클래스
		- 역할: `IDatabaseConnection`의 인스턴스를 생성합니다.
		- 디자인 패턴: 추상 팩토리 패턴

		```cpp
		class DatabaseConnectionFactory {
		public:
			static std::shared_ptr<IDatabaseConnection> createConnection(DatabaseConfig config) {
				if (config.type == DatabaseType::SQLite) {
					return std::make_shared<SQLiteConnection>(config);
				}
				// 다른 데이터베이스 지원 추가 가능
			}
		};
	    ```

2. 코어 레이어
	1. ISession 인터페이스
		- 역할: 세션에 대한 공통 인터페이스를 제공합니다.
		- 메서드:
			- `save(entity)`
			- `update(entity)`
			- `delete(entity)`
			- `find(entityClass, id)`
			- `createQuery()`

	2. Session 클래스
		- 역할: `ISession`을 구현하여 세션 관리를 담당합니다.
		- 디자인 패턴: 팩토리 패턴을 적용한 `SessionFactory`에서 생성됩니다.
		- Unit of Work 패턴을 적용하여 엔티티의 변경 내역을 추적하고 일괄 처리합니다.


	3. Session Factory 클래스
		- 역할: `Session`인스턴스를 생성하고 관리합니다.
		- 디자인 패턴: 싱글톤 패턴과 팩토리 패턴

		```cpp
		class SessionFactory {
		public:
			static SessionFactory& getInstance() {
				static SessionFactory instance;
				return instance;
			}

			void configure(DatabaseConfig config) {
				this->config = config;
			}

			std::shared_ptr<ISession> openSession() {
				auto connection = DatabaseConnectionFactory::createConnection(config);
				return std::make_shared<Session>(connection);
			}

		private:
			SessionFactory() = default;
			DatabaseConfig config;
		};
		```

3. 트랜잭션 관리
	1. ITransaction 인터페이스
		- 역할: 트랜잭션에 대한 공통 인터페이스를 제공합니다.
		- 메서드:
			- `bigin()`
			- `commit()`
			- `rollback()`

	2. Transaction 클래스
		- 역할: `ITransaction`을 구현하여 트랜잭션 관리를 담당합니다.
		- 디자인 패턴: 데코레이터 패턴을 사용하여 트랜잭션 기능을 동적으로 추가합니다.


4. 매핑 레이어
	1. IEntityManager 인터페이스
		- 역할: 엔티티와 데이터베이스 간의 매핑을 담당합니다.
		- 메서드:
			- `map(entity)`
			- `unmap(rowData)`

	2. EntityManager 클래스
		- 역할: `IEntityManager`를 구현하여 실제 매핑 로직을 처리합니다.
		- 디자인 패턴: 데이터 매퍼 패턴과 메타데이터 매핑

	3. 매핑 구성 (Mapping Configuration)
		- XML, JSON 또는 어노테이션 기반으로 엔티티와 테이블 간의 매핑 정보를 정의합니다.
		- 확장성을 위해 **의존성 주입(DI)**를 활용하여 매핑 구성을 주입합니다.


5. 쿼리 레이어
	1. IQuery 인터페이스
		- 역할: 쿼리 객체에 대한 공통 인터페이스를 제공합니다.
		- 메서드:
			- `setParameter(name, value)`
			- `execute()`
			- `list()`
			- `uniqueResult()`

	2. Query 클래스
		- 역할: `IQuery`를 구현하여 쿼리 실행을 담당합니다.
		
	3. QueryBuilder 클래스
		- 역할: 유연한 쿼리 생성을 지원합니다.
		- 디자인 패턴: 빌더 패턴

		```cpp
		class QueryBuilder {
		public:
			QueryBuilder& select(const std::vector<std::string>& fields);
			QueryBuilder& from(const std::string& table);
			QueryBuilder& where(const std::string& condition);
			QueryBuilder& orderBy(const std::string& field, bool ascending = true);
			std::shared_ptr<IQuery> build();

		private:
			std::string selectClause;
			std::string fromClause;
			std::string whereClause;
			std::string orderByClause;
		};
		```


6. 유틸리티 및 공통 모듈
	- Logger 클래스: 로그 관리를 담당하며, 싱글톤 패턴을 적용합니다.
	- Exception 클래스 계층: ORM에서 발생하는 예외를 체계적으로 관리합니다.


### 확장성을 위한 고려 사항 ###

1. 인터페이스 기반 설계
	- 모든 주요 컴포넌트는 인터페이스를 통해 정의되어 구현체를 교체하거나 확장하기 쉽습니다.

2. 의존성 역전 원칙 (DIP)
	- 상위 모듈을 하위 모듈에 의존하지 않고, 추상화에 의존합니다.

3. 개방-폐쇄 원칙(OCP)
	- 컴포넌트 확장에는 열려 있고, 수정에는 닫혀 있습니다.
	- 새로운 기능 추가 시 기존 코드를 수정하지 않고도 확장할 수 있습니다.

4. 설정 기반 확장
	- 외부 설정 파일이나 주입된 설정을 통해 동작을 변경할 수 있습니다.


### 디자인 패턴의 적용 이유 ###

1. 추상 팩토리 패턴: 다양한 데이터베이스 지원을 위해 연결 객체 생성을 추상화합니다.
2. 전략 패턴: 런타임에 데이터베이스별 구현체를 교체할 수 있습니다.
3. 싱글톤 패턴: 전역 상태를 관리하는 객체(SessionFactory, Logger 등)에 적용합니다.
4. 데이터 매퍼 패턴: 엔티티와 데이터베이스 간의 매핑을 분리하여 유지보수성을 높입니다.
5. 빌더 패턴: 복잡한 쿼리를 유연하고 가독성 높게 생성할 수 있습니다.
6. 데코레이터 패턴: 트랜잭션 기능을 동적으로 추가하여 유연한 트랜잭션 관리를 지원합니다.
7. 프록시 패턴: 지연 로딩을 구현하여 필요한 시점에 데이터베이스에서 데이터를 가져옵니다.
8. 옵저버 패턴: 이벤트 시스템을 구현하여 확장 가능한 이벤트 처리가 가능합니다.
9. 의존성 주입(DI): 컴포넌트 간의 결합도를 낮추고 테스트 용이성을 높입니다.
