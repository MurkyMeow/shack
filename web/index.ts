import { get_wall_distance } from '../wasm/raycast.wasm';

const mapSizeX = 5;
const mapSizeY = 5;

const povX = 2;
const povY = 2;
const povAngle = 0;

const map = new Uint32Array(
  [1, 1, 1, 1, 1].concat([1, 0, 0, 0, 1]).concat([1, 0, 0, 0, 1]).concat([1, 0, 0, 0, 1]).concat([1, 1, 1, 1, 1])
);

console.log(get_wall_distance(map, mapSizeX, mapSizeY, povX, povY, povAngle));
