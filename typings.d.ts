declare module '*.wasm' {
  function instantiate(imports: WebAssembly.ModuleImports): Promise<WebAssembly.WebAssemblyInstantiatedSource>;
  export default instantiate;
}
