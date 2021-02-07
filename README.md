A browser game in C

## Requirements

- [wasi-sdk-10](https://github.com/WebAssembly/wasi-sdk/releases/tag/wasi-sdk-10) should be placed inside /opt/ directory

## Run the code

```sh
make # compile .c files to .wasm
npm run typings # auto-generate TS typings from .h files
npm run start # start http dev-server
```
