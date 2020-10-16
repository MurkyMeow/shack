const fs = require('fs');
const path = require('path');

const C_TO_TS_TYPES = {
  int: 'number',
  'unsigned int': 'number',
  float: 'number',
  double: 'number',
  char: 'string',
  string: 'string',
};

const C_TO_TS_ARRAY_TYPES = {
  'unsigned int': 'Uint32Array',
};

function parseFunctionDeclaration(decl) {
  const [signature, paramsString] = decl.split('(');

  const lastSignatureSpaceIdx = signature.lastIndexOf(' ');
  const returnType = signature.slice(0, lastSignatureSpaceIdx);
  const functionName = signature.slice(lastSignatureSpaceIdx + 1);

  const params = paramsString
    .replace(');', '')
    .split(', ')
    .map((paramString) => {
      const lastParamSpaceIdx = paramString.lastIndexOf(' ');
      const paramType = paramString.slice(0, lastParamSpaceIdx).replace('const ', '');
      const paramName = paramString.slice(lastParamSpaceIdx + 1);
      const paramIsArray = paramName.includes('[]');

      if (paramIsArray) {
        const actualType = C_TO_TS_ARRAY_TYPES[paramType] || `${C_TO_TS_TYPES[paramType]}[]`;
        return { name: paramName.replace('[]', ''), type: actualType };
      }

      return { name: paramName, type: C_TO_TS_TYPES[paramType] };
    });

  return { returnType: C_TO_TS_TYPES[returnType], functionName, params };
}

function* getTSModules(hFilesDir) {
  for (const file of fs.readdirSync(hFilesDir)) {
    if (!file.includes('.h')) continue;

    const fullPath = path.join(hFilesDir, file);

    const cDeclarations = fs
      .readFileSync(fullPath)
      .toString()
      .split('\n')
      .map((line) => line.trim())
      .filter(Boolean)
      .map(parseFunctionDeclaration);

    const tsExports = cDeclarations.map(
      (decl) =>
        `  export function ${decl.functionName}(${decl.params
          .map((param) => `${param.name}: ${param.type}`)
          .join(', ')}): ${decl.returnType};`
    );

    const tsDeclaration = [`declare module '*/${file.replace('.h', '.wasm')}' {`, tsExports.join('\n'), '}'].join('\n');

    yield tsDeclaration;
  }
}

(function main() {
  const H_FILES_DIR = 'core';
  const OUT_FILE = 'typings.d.ts';

  const tsModules = [...getTSModules(H_FILES_DIR)].join('\n');

  fs.writeFileSync(OUT_FILE, tsModules);
})();
