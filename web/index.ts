const MAP_W = 10;
const MAP_H = 10;

const SCREEN_W = 640;
const SCREEN_H = 480;
const SCREEN_R = 10;

const START_X = 2;
const START_Y = 2;
const START_ANGLE = -Math.PI / 4;

const imports = {
  env: {
    console_log: console.log,
  },
};

interface MyExports extends WebAssembly.Exports {
  game_new: Function;
  game_tick: Function;
  game_controls: Function;
  game_wall_distances: Function;
  memory: WebAssembly.Memory;
}

WebAssembly.instantiateStreaming(fetch('/raycast.wasm'), imports).then(({ instance }) => {
  const { game_new, game_tick, game_controls, game_wall_distances, memory } = instance.exports as MyExports;

  const gamePointer = game_new(
    MAP_W,
    MAP_H,

    SCREEN_W,
    SCREEN_R,

    START_X,
    START_Y,
    START_ANGLE
  );

  const distancesPointer = game_wall_distances(gamePointer);
  const distances = new Float32Array(memory.buffer, distancesPointer, SCREEN_W / SCREEN_R);

  const controlsPointer = game_controls(gamePointer);
  const controls = new Uint32Array(memory.buffer, controlsPointer, 6);

  const canvas = document.body.appendChild(document.createElement('canvas'));
  const ctx = canvas.getContext('2d');
  if (!ctx) throw new Error('ctx is null');
  canvas.width = SCREEN_W;
  canvas.height = SCREEN_H;

  (function render() {
    ctx.clearRect(0, 0, SCREEN_W, SCREEN_H);

    game_tick(gamePointer);

    for (let i = 0; i < SCREEN_W / SCREEN_R; i += 1) {
      const height = SCREEN_H / distances[i];
      ctx.fillRect(i * SCREEN_R, (SCREEN_H - height) / 2, SCREEN_R, height);
    }

    requestAnimationFrame(render);
  })();

  window.addEventListener('keydown', ({ code }) => {
    if (code === 'KeyA') {
      controls[0] = 1;
    } else if (code === 'KeyD') {
      controls[1] = 1;
    } else if (code === 'KeyW') {
      controls[2] = 1;
    } else if (code === 'KeyS') {
      controls[3] = 1;
    } else if (code === 'KeyQ') {
      controls[4] = 1;
    } else if (code === 'KeyE') {
      controls[5] = 1;
    }
  });

  window.addEventListener('keyup', ({ code }) => {
    if (code === 'KeyA') {
      controls[0] = 0;
    } else if (code === 'KeyD') {
      controls[1] = 0;
    } else if (code === 'KeyW') {
      controls[2] = 0;
    } else if (code === 'KeyS') {
      controls[3] = 0;
    } else if (code === 'KeyQ') {
      controls[4] = 0;
    } else if (code === 'KeyE') {
      controls[5] = 0;
    }
  });
});
