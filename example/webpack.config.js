const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const HtmlWebpackExcludeAssetsPlugin = require('html-webpack-exclude-assets-plugin');

const outputDir = path.join(__dirname, 'build/');

let publicPath = process.env.PUBLIC_PATH || '/';

module.exports = {
  entry: {
    'index': './lib/es6/src/Index.bs.js',
  },
  mode: process.env.NODE_ENV === 'production' ? 'production' : 'development',
  output: {
    path: outputDir,
    publicPath,
    filename: '[name].[chunkhash:4].js'.toLowerCase(),
    chunkFilename: '[name].[chunkhash:4].chunk.js'.toLowerCase()
  },
  stats: 'minimal',
  plugins: [
    new HtmlWebpackPlugin({
      template: 'src/index.html',
      inject: true,
      excludeAssets: [/style.js/]
    }),
    new HtmlWebpackExcludeAssetsPlugin(),
  ],
  devServer: {
    compress: false,
    contentBase: outputDir,
    port: process.env.PORT || 8080,
    historyApiFallback: true
  }
};
