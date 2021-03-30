const MAP_W = 10;
const MAP_H = 10;

const SCREEN_W = 640;
const SCREEN_H = 480;
const SCREEN_R = 10;

const START_X = 2;
const START_Y = 2;
const START_ANGLE = 0;

const MOVE_SPEED = 0.1;
const SENSETIVITY = 0.075;

const imports = {
  env: {
    console_log: console.log,
  },
};

interface MyExports extends WebAssembly.Exports {
  map_new: Function;
  game_new: Function;
  game_free: Function;
  game_wall_distances: Function;
  set_wall_distances: Function;
  memory: WebAssembly.Memory;
}

WebAssembly.instantiateStreaming(fetch('/raycast.wasm'), imports).then(({ instance }) => {
  const { map_new, game_new, game_wall_distances, set_wall_distances, memory } = instance.exports as MyExports;

  const mapPointer = map_new(MAP_W, MAP_H);

  const gamePointer = game_new(
    mapPointer,

    SCREEN_W,
    SCREEN_H,
    SCREEN_R,

    START_X,
    START_Y,
    START_ANGLE
  );

  const distancesPointer = game_wall_distances(gamePointer);

  const distances = new Float32Array(memory.buffer, distancesPointer, SCREEN_W / SCREEN_R);

  const canvas = document.body.appendChild(document.createElement('canvas'));
  const ctx = canvas.getContext('2d');
  if (!ctx) throw new Error('ctx is null');
  canvas.width = SCREEN_W;
  canvas.height = SCREEN_H;

  let angle = -Math.PI / 4;
  let x = 5.0;
  let y = 5.0;

  const M_LEFT = 1 << 0;
  const M_RIGHT = 1 << 1;
  const M_UP = 1 << 2;
  const M_DOWN = 1 << 3;
  const R_LEFT = 1 << 4;
  const R_RIGHT = 1 << 5;

  let control = 0;

  (function render() {
    ctx.clearRect(0, 0, SCREEN_W, SCREEN_H);

    const dirX = Math.cos(angle) * MOVE_SPEED;
    const dirY = Math.sin(angle) * MOVE_SPEED;

    if (control & M_UP) {
      x += dirX;
      y += dirY;
    }
    if (control & M_DOWN) {
      x -= dirX;
      y -= dirY;
    }

    if (control & M_LEFT) {
      x += dirY;
      y -= dirX;
    }
    if (control & M_RIGHT) {
      x -= dirY;
      y += dirX;
    }

    if (control & R_LEFT) {
      angle -= SENSETIVITY;
    }
    if (control & R_RIGHT) {
      angle += SENSETIVITY;
    }

    set_wall_distances(gamePointer);

    for (let i = 0; i < SCREEN_W / SCREEN_R; i += 1) {
      const height = SCREEN_H / distances[i];
      ctx.fillRect(i * SCREEN_R, (SCREEN_H - height) / 2, SCREEN_R, height);
    }

    requestAnimationFrame(render);
  })();

  window.addEventListener('keydown', ({ code }) => {
    if (code === 'KeyA') {
      control |= M_LEFT;
    } else if (code === 'KeyD') {
      control |= M_RIGHT;
    } else if (code === 'KeyW') {
      control |= M_UP;
    } else if (code === 'KeyS') {
      control |= M_DOWN;
    } else if (code === 'KeyQ') {
      control |= R_LEFT;
    } else if (code === 'KeyE') {
      control |= R_RIGHT;
    }
  });

  window.addEventListener('keyup', ({ code }) => {
    if (code === 'KeyA') {
      control &= ~M_LEFT;
    } else if (code === 'KeyD') {
      control &= ~M_RIGHT;
    } else if (code === 'KeyW') {
      control &= ~M_UP;
    } else if (code === 'KeyS') {
      control &= ~M_DOWN;
    } else if (code === 'KeyQ') {
      control &= ~R_LEFT;
    } else if (code === 'KeyE') {
      control &= ~R_RIGHT;
    }
  });
});
