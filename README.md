# korlang
korlang compiler.


# Example
```
함수 시작() {
    출력줄바꿈(11 + 2);
    출력줄바꿈("Hello");
    출력줄바꿈(팩토리얼(3));
}

함수 팩토리얼(수) {
  만약 (수 < 2) {
    반환 1;
  }
  반환 수 * 팩토리얼(수 - 1);
}
```

# Build wasm
```
em++ main.cpp Parser.cpp Scanner.cpp Generator.cpp Machine.cpp DataType.cpp Kind.cpp Printer.cpp BuiltinFunctionTable.cpp -o index.html -s EXPORTED_FUNCTIONS='["_run_wrapper"]' -s EXPORTED_RUNTIME_METHODS='["cwrap"]' -s WASM=1 -std=c++17 -O2
```