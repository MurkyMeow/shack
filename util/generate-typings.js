const fs = require('fs');
const path = require('path');
const { lexUnit } = require('node-c-lexer');

const C_TO_TS_TYPES = {
  int: 'number',
  float: 'number',
  double: 'number',
  char: 'string',
  string: 'string',
};

function parseFunctionDeclaration(decl) {
  const tokens = lexUnit.tokenize(decl);
  const params = [];

  let i = 0;

  while (!(tokens[i].lexeme in C_TO_TS_TYPES)) i++;
  const returnType = C_TO_TS_TYPES[tokens[i].lexeme];

  while (tokens[i].tokenClass !== 'IDENTIFIER') i++;
  const functionName = tokens[i].lexeme;

  while (i < tokens.length) {
    const paramType = tokens[i++];
    const paramName = tokens[i];

    if (paramType.lexeme === ')') break;

    if (paramType.lexeme in C_TO_TS_TYPES && paramName.tokenClass === 'IDENTIFIER') {
      params.push({ type: C_TO_TS_TYPES[paramType.lexeme], name: paramName.lexeme });
    }
  }

  return { returnType, functionName, params };
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
