declare module '*/raycast.wasm' {
  export function get_wall_distance(map: Uint32Array, map_size_x: number, map_size_y: number, pov_x: number, pov_y: number, pov_angle: number): number;
}