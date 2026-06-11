# Commit Message Convention

[Conventional Commits](https://www.conventionalcommits.org/) 스펙을 기반으로 한다.

---

## 형식

```
<type>(<scope>): <subject>

[body]

[footer]
```

---

## Type

| Type | 설명 |
|---|---|
| `feat` | 새로운 기능 추가 |
| `fix` | 버그 수정 |
| `refactor` | 기능 변경 없는 코드 구조 개선 |
| `test` | 테스트 코드 추가·수정 |
| `docs` | 문서 추가·수정 (코드 변경 없음) |
| `chore` | 빌드 설정, 패키지, 기타 잡무 |
| `style` | 코드 포맷·네이밍 등 의미 없는 변경 |

---

## Scope

변경 대상 모듈 또는 파일 범위를 소문자로 표기한다.

| Scope | 대상 |
|---|---|
| `main` | `main.cpp` |
| `car` | `Car.h` / `Car.cpp` |
| `validator` | `CarValidator.h` / `CarValidator.cpp` |
| `assembler` | `CarAssembler.h` / `CarAssembler.cpp` |
| `ui` | `ConsoleUI.h` / `ConsoleUI.cpp` |
| `parts` | `parts/` 하위 파일 |
| `types` | `types/` 하위 파일 |
| `tests` | `tests/` 하위 파일 |
| `docs` | 문서 파일 |
| `build` | 프로젝트·빌드 설정 파일 |

---

## Subject

- 명령형 현재 시제 사용 (한국어: `~하다`, 영어: `add`, `remove`, `fix`)
- 첫 글자 대문자 금지
- 끝에 마침표(`.`) 금지
- 50자 이내

---

## Body (선택)

- Subject만으로 의도를 충분히 전달하기 어려울 때 작성
- **무엇을** 했는지가 아니라 **왜** 했는지 설명
- 72자 줄바꿈

---

## Footer (선택)

| 키워드 | 용도 |
|---|---|
| `BREAKING CHANGE:` | 하위 호환성이 깨지는 변경 기술 |
| `Closes #<번호>` | 관련 이슈 자동 닫기 |
| `Refs #<번호>` | 관련 이슈 참조 |

---

## 예시

```
feat(validator): CarValidator 클래스 추가

전역 함수로 흩어진 검증 로직을 단일 클래스로 통합한다.
5가지 제한 조건을 각각 private 메서드로 분리하여
조건 추가 시 영향 범위를 최소화한다.
```

```
refactor(main): delay() busy-wait을 sleep_for로 교체
```

```
test(tests): CarValidator 유닛 테스트 추가

5가지 제한 조건에 대한 PASS/FAIL 케이스 8개 작성

Closes #12
```

```
docs(docs): 리팩토링 계획 문서(PLAN.md) 추가
```
