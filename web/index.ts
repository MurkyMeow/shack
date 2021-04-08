import { FLOAT32, INT32, MemoryAllocator } from './memory-allocator';

const MAP_W = 10;
const MAP_H = 10;

const SCREEN_W = 640;
const SCREEN_H = 480;
const SCREEN_R = 10;

const START_X = 2;
const START_Y = 2;
const START_ANGLE = -Math.PI / 4;

// 1Mb for now
const TOTAL_MEMORY = 1024 * 1024 * 1024 * 1;

// NOTE a dangerous thing
// We assume that we won't allocate further this index on the C side
// So we can safely allocate after this index on the JS side
const C_MEMORY_OFFSET = TOTAL_MEMORY / 2;

const SPRITES = [
  { x: 3.3, y: 3.1, texid: 0 },
  { x: 5.3, y: 5.1, texid: 1 },
];

const SPRITE_SCHEMA = {
  x: FLOAT32,
  y: FLOAT32,
  texid: INT32,
};

const imports = {
  env: {
    console_log: console.log,
  },
};

interface MyExports extends WebAssembly.Exports {
  game_new: Function;
  game_tick: Function;
  game_controls: Function;
  game_pbuffer: Function;
  memory: WebAssembly.Memory;
}

WebAssembly.instantiateStreaming(fetch('/game.wasm'), imports).then(({ instance }) => {
  const { game_new, game_tick, game_controls, game_pbuffer, memory } = instance.exports as MyExports;

  memory.grow(TOTAL_MEMORY / memory.buffer.byteLength);

  const allocator = new MemoryAllocator(memory.buffer, C_MEMORY_OFFSET);

  const spritesPointer = allocator.allocStructArray(SPRITE_SCHEMA, SPRITES);

  const gamePointer = game_new(
    MAP_W,
    MAP_H,

    SCREEN_W,
    SCREEN_H,
    SCREEN_R,

    START_X,
    START_Y,
    START_ANGLE,

    SPRITES.length,
    spritesPointer
  );

  const screenBufferPointer = game_pbuffer(gamePointer);
  const screenBuffer = new Uint8ClampedArray(memory.buffer, screenBufferPointer, SCREEN_W * SCREEN_H * 4);

  const controlsPointer = game_controls(gamePointer);
  const controls = new Uint32Array(memory.buffer, controlsPointer, 6);

  const canvas = document.body.appendChild(document.createElement('canvas'));
  const ctx = canvas.getContext('2d');
  if (!ctx) throw new Error('ctx is null');
  canvas.width = SCREEN_W;
  canvas.height = SCREEN_H;

  const imageData = ctx.getImageData(0, 0, SCREEN_W, SCREEN_H);

  (function render() {
    game_tick(gamePointer);

    imageData.data.set(screenBuffer);

    ctx.putImageData(imageData, 0, 0);

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
