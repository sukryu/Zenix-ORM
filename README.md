# Node.js C++ Addon을 활용한 고성능 ORM **Zenix ORM** #

이 프로젝트는 Node.js의 C++ Addon 기능을 활용하여 개발된 고성능 ORM(Object-Relational Mapping)라이브러리입니다.
C++의 성능과 Node.js의 비동기성을 결합하여 빠르고 효율적인 데이터베이스 연동을 제공합니다.

위 라이브러리는 Java의 Hibernate를 모방하여 Node.js 플랫폼에 맞게 변경되었습니다.
Hibernate의 `Session, Transaction ... etc` 등을 지원합니다.

## 목차 ##

1. 소개
2. 기능
3. 설치 방법
4. 사용법
	- 엔티티 정의
	- 엔티티 매핑 등록
	- ORM 초기화
	- CRUD 작업
	- 쿼리 사용
5. 설계 상세
	- C++ 레이어
	- Node.js 연동
	- 비동기 처리
	- 예외 처리 및 로깅
6. 향후 발전 방향
7. 라이센스

## 소개 ##

이 ORM 라이브러리는 C++의 고성능과 Node.js의 개발 편의성을 결합하여, Node.js 애플리케이션에서 효율적으로
데이터베이스를 다룰 수 있도록 설계되었습니다. Node.js의 C++ Addon 기능을 활용하여 네이티브 코드의 성능을 가져오면서도,
JavaScript의 비동기 패턴과 친화적인 API를 제공합니다.

## 기능 ##

- 엔티티 매핑: JavaScript 객체와 데이터베이스 테이블을 매핑하여 객체 지향적인 데이터베이스 작업을 지원합니다.
- CRUD 작업 지원: 엔티티의 생성, 조회, 업데이트, 삭제 기능을 비동기로 제공합니다.
- 트랜잭션 관리: 트랜잭션의 시작, 커밋, 롤백을 지원하며, 트랜잭션 모드와 격리 수준을 설정할 수 있습니다.
- 쿼리 빌더 및 동적 쿼리 실행: 유연한 쿼리 생성을 위한 쿼리 빌더와 파라미터 바인딩을 지원합니다.
- 관계 매핑 지원: 엔티티 간의 관계 (1:1, 1,N)를 매핑하고 지연 로딩을 지원합니다.
- 캐싱 메커니즘: 1차 캐시(세션 캐시)와 2차 캐시(전역 캐시)를 통해 성능을 향상시킵니다.
- 예외 처리 및 로깅: 상세한 예외 처리를 통해 디버깅을 용이하게 하고, 로깅 기능을 제공합니다.

## 설치 방법 ##

1. 필수 요건:
	- Node.js(v12 이상)
	- C++17 이상을 지원하는 컴파일러
	- SQLite3 라이브러리
	- node-gyp 설치

2. 소스 코드 클론:
	```bash
	https://github.com/sukryu/Zenix-ORM
	```

3. 의존성 설치:
	```bash
	cd Zenix-ORM
	npm install
	```

4. 빌드:
	```bash
	npm run build
	```

## 사용법 ##

1. 엔티티 정의
	- 데이터베이스 테이블과 매핑되는 엔티티 클래스를 JavaScript로 정의합니다.

	```javascript
	// models/User.js

	class User {
		constructor() {
			this.id = null;
			this.name = '';
			this.age = 0;
		}
	}

	module.exports = User;
	```

2. 엔티티 매핑 등록
	- 엔티티와 데이터베이스 테이블 간의 매핑 정보를 등록합니다.

	```javascript
	// ormConfig.js

	const orm = require('nodejs-cpp-orm');

	function registerEntities() {
	const userMapping = {
		entityName: 'User',
		tableName: 'users',
		idColumnName: 'id',
		fields: [
		{ fieldName: 'name', columnName: 'name' },
		{ fieldName: 'age', columnName: 'age' },
		],
		entityConstructor: () => new (require('./models/User'))(),
	};

	orm.EntityMapper.registerEntity(userMapping);
	}

	module.exports = registerEntities;
	```

3. ORM 초기화
	- ORM을 초기화하고 데이터베이스 설정을 구성합니다.

	```javascript
	// app.js

	const orm = require('nodejs-cpp-orm');
	const registerEntities = require('./ormConfig');

	registerEntities();

	// 데이터베이스 설정
	const config = {
	databaseType: 'SQLite',
	databaseName: 'test.db',
	};

	// SessionFactory 구성
	orm.SessionFactory.configure(config);
	```

4. CRUD 작업
	- 생성 및 저장:
	```javascript
	const orm = require('nodejs-cpp-orm');

	(async () => {
	const session = orm.SessionFactory.openSession();

	const transaction = await session.beginTransaction();

	try {
		const user = new orm.User();
		user.name = 'Alice';
		user.age = 30;

		await session.save(user);
		await transaction.commit();
	} catch (err) {
		await transaction.rollback();
		console.error(err);
	} finally {
		session.close();
	}
	})();
	```

	- 조회:
	```javascript
	const user = await session.find('User', userId);
	```

	- 업데이트:
	```javascript
	user.age = 31;
	await session.update(user);
	```

	- 삭제:
	```javascript
	await session.remove(user);
	```

	- 쿼리 사용
		- 쿼리 빌더 사용:
		```javascript
		const query = session.createQueryBuilder()
			.select('*')
			.from('users', 'u')
			.where('u.age > :age')
			.orderBy('u.name', 'ASC')
			.limit(10)
			.offset(0)
			.getQuery();

		query.setParameter('age', 25);

		const results = await query.list();
		results.forEach((user) => {
		console.log(`Name: ${user.name}, Age: ${user.age}`);
		});

		```

	- 직접 쿼리 실행:
	```javascript
	const queryString = 'SELECT * FROM users WHERE name = :name';
	const query = session.createQuery(queryString);
	query.setParameter('name', 'Alice');

	const user = await query.uniqueResult();
	```

## 설계 상세 ##

- C++ 레이어
	- Core Layer: ORM의 핵심 로직을 담당하며, 세션 관리, 트랜잭션 관리, 엔티티 매핑 등을 처리합니다.
	- Database Layer: SQLite3와의 상호 작용을 담당하며, 쿼리 실행과 결과 처리를 수행합니다.
	- Query Layer: 동적 쿼리 생성과 실행을 지원하며, 파라미터 바인딩과 결과 매핑을 처리합니다.

- Node.js 연동
	- C++ Addon: `N-API`를 활용하여 C++코드와 Node.js 간의 통신을 지원합니다.
	- 비동기 처리: Node.js의 이벤트 루프를 차단하지 않도록 C++ 레이어에서 비동기 작업을 수행하고, 콜백이나 Promise를 통해 결과를 반환합니다.
	- JavaScript API: 사용자가 편리하게 ORM을 사용할 수 있도록 친숙한 JavaScript API를 제공합니다.

- 비동기 처리:
	- Async Worker: C++ 레이어에서 비동기 작업을 처리하기 위해 `napi_async_worker`를 활용합니다.
	- Promise 지원: JavaScript 레이어에서 Promise를 반환하여 `async/awiat`문법을 사용할 수 있습니다.

- 예외 처리 및 로깅:
	- 예외 클래스: C++ 레이어에서 발생한 예외를 JavaScript 예외로 변환하여 전달합니다.
	- 로깅 기능: 디버깅을 위해 로깅 레벨과 출력을 설정할 수 있습니다.

## 향후 발전 방향 ##

- 관계 매핑의 실제 구현: 엔티티 간의 관계를 처리하는 로직을 추가하여 ORM의 완성도를 높입니다.
- 캐싱 최적화: 캐싱 메커니즘을 최적화하여 성능을 더욱 향상시킵니다.
- 다양한 데이터베이스 지원: MySQL, PostgreSQL 등 다른 데이터베이스에 대한 지원을 추가합니다.
- 트랜잭션 격리 수준 강화: 트랜잭션 격리 수준을 더 세밀하게 제어할 수 있도록 개선합니다.
- 테스트 코드 작성: 단위 테스트와 통합 테스트를 통해 안정성을 높입니다.

## 라이선스 ##

이 프로젝트는 MIT 라이센스 하에 배포됩니다. 자세한 내용은 `LICENSE`파일을 참조하세요.

**문의사항이나 제안 사항이 있으시면 언제든지 연락 주세요.**
