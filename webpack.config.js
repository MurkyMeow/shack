const path = require('path');

const PRODUCTION = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './web/index.ts',
  mode: PRODUCTION ? 'production' : 'development',
  output: {
    path: path.resolve(__dirname, './dist'),
    publicPath: '/',
    filename: 'bundle.js',
  },
  devServer: {
    contentBase: path.resolve(__dirname, './dist'),
  },
  module: {
    rules: [
      {
        test: /\.ts$/,
        loader: 'ts-loader',
        exclude: /node_modules/,
      },
      {
        test: /\.wasm$/,
        loader: 'wasm-loader',
        exclude: /node_modules/,
      },
    ],
  },
  resolve: {
    extensions: ['.ts', '.js', '.wasm'],
  },
};
