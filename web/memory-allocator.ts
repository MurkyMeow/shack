type Container = Int32ArrayConstructor | Float32ArrayConstructor;

export type Layout = { [x: string]: Container };
export type Data<T extends Layout> = { [K in keyof T]: number };

export const INT32: Container = Int32Array;
export const FLOAT32: Container = Float32Array;

function sizeof(layout: Layout): number {
  let size = 0;

  for (const key in layout) {
    size += layout[key].BYTES_PER_ELEMENT;
  }

  return size;
}

export class MemoryAllocator {
  _buffer: ArrayBuffer;
  _offset: number;

  constructor(buffer: ArrayBuffer, offset: number) {
    this._buffer = buffer;
    this._offset = offset;
  }

  _malloc(bytes: number): number {
    const { _offset } = this;
    this._offset += bytes;
    return _offset;
  }

  allocStructArray<T extends Layout>(layout: T, data: Data<T>[]): number {
    const keys = Object.keys(layout);
    const structSize = sizeof(layout);
    const pointer = this._malloc(structSize);

    for (let i = 0; i < data.length; i += 1) {
      const datum = data[i];

      let offset = pointer + i * structSize;

      for (let j = 0; j < keys.length; j += 1) {
        const key = keys[j];

        const Type = layout[key];
        const type = new Type(this._buffer, offset, 1);
        type[0] = datum[key];

        offset += layout[key].BYTES_PER_ELEMENT;
      }
    }

    return pointer;
  }
}
