# Korlang

Korlang은 한국어 문법을 기반으로 설계된 프로그래밍 언어 컴파일러 및 런타임 프로젝트입니다. C++로 작성되었으며, 한국어 키워드를 사용하여 코드를 작성하고 실행할 수 있습니다.

[Demo 페이지 보러가기](https://Yoon6.github.io/korlang)

---

## 구조 및 파이프라인

Korlang은 일반적인 컴파일러 파이프라인을 따르며, 다음과 같은 단계로 코드를 처리합니다.

```mermaid
flowchart LR
    SourceCode["소스 코드"] --> Scanner
    Scanner["스캐너 (Scanner)<br/>Lexical Analysis"] --> Tokens["토큰 (Tokens)"]
    Tokens --> Parser
    Parser["파서 (Parser)<br/>Syntax Analysis"] --> AST["추상 구문 트리 (AST)"]
    AST --> Generator
    Generator["생성기 (Generator)<br/>Code Generation"] --> Bytecode["바이트코드 (Bytecode)<br/>or Object Code"]
    Bytecode --> Machine
    Machine["가상 머신 (Machine)<br/>Runtime Execution"] --> Result["실행 결과"]
```

### 각 모듈 설명
- **Scanner**: 소스 코드를 읽어 의미 있는 단위인 토큰(Token)으로 분리합니다. 한국어 키워드와 식별자를 처리합니다.
- **Parser**: 토큰 목록을 분석하여 문법 구조를 파악하고 추상 구문 트리(AST)를 생성합니다.
- **Generator**: AST를 순회하며 실행 가능한 명령어 코드(Bytecode)를 생성합니다.
- **Machine**: 스택 기반 가상 머신으로, 생성된 코드를 실행하고 메모리를 관리합니다.

---

## 문법

Korlang은 직관적인 한국어 키워드를 사용합니다.

### 1. 자료형 및 리터럴
- **빈값**: `null`
- **참 / 거짓**: `true` / `false`
- **숫자**: 정수 및 실수 (예: `123`, `3.14`)
- **문자열**: 큰따옴표 사용 (예: `"안녕"`)
- **리스트/맵**: `[1, 2, 3]`, `{"키": "값"}`

### 2. 변수 선언
`변수` 키워드를 사용하여 변수를 선언합니다.
```korlang
변수 숫자 = 10;
변수 이름 = "홍길동";
```

### 3. 입출력
- `출력(...)`: 내용을 출력합니다.
- `출력줄바꿈(...)`: 내용을 출력하고 줄을 바꿉니다.

### 4. 제어문 (조건문)
`만약`, `그게아니라`, `아니면`을 사용합니다.
```korlang
만약 (점수 >= 90) {
    출력줄바꿈("A학점");
} 그게아니라 (점수 >= 80) {
    출력줄바꿈("B학점");
} 아니면 {
    출력줄바꿈("C학점");
}
```

### 5. 반복문
`반복` 키워드를 사용하며 C스타일의 for문과 유사합니다.
```korlang
반복 (변수 i = 0; i < 10; i = i + 1) {
    출력줄바꿈(i);
    만약 (i == 5) {
        끊기; // break
    }
}
```
- **끊기**: `break`
- **계속하기**: `continue`

### 6. 함수 정의
`함수` 키워드로 정의하고 `반환`으로 값을 반환합니다.
```korlang
함수 더하기(a, b) {
    반환 a + b;
}

출력줄바꿈(더하기(10, 20));
```

### 7. 논리 연산자
- **그리고**: `&&` (AND)
- **또는**: `||` (OR)

---

## 예제 코드

```korlang
함수 시작() {
    출력줄바꿈(11 + 2);
    출력줄바꿈("Hello Korlang!");
    출력줄바꿈(팩토리얼(5));
}

함수 팩토리얼(숫자) {
    만약 (숫자 < 2) {
        반환 1;
    }
    반환 숫자 * 팩토리얼(숫자 - 1);
}
```

---

## 빌드 및 실행

이 프로젝트는 C++17 표준을 따르며 CMake를 사용하여 빌드할 수 있습니다.

### Native Build (CMake)
```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
./korlang
```

### WebAssembly (Emscripten)
웹 환경에서 실행하기 위해 Emscripten을 사용하여 WASM으로 빌드할 수 있습니다.
```bash
em++ main.cpp Parser.cpp Scanner.cpp Generator.cpp Machine.cpp DataType.cpp Kind.cpp Printer.cpp BuiltinFunctionTable.cpp \
    -o docs/index.html \
    -s EXPORTED_FUNCTIONS='["_run_wrapper"]' \
    -s EXPORTED_RUNTIME_METHODS='["cwrap"]' \
    -s WASM=1 \
    -std=c++17 \
    -O2
```

---

## 라이선스
MIT License