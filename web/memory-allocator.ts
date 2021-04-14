export const INT32 = Int32Array;
export const FLOAT32 = Float32Array;
export const UINT8CLAMPED = Uint8ClampedArray;

type CType = typeof INT32 | typeof FLOAT32 | typeof UINT8CLAMPED;

export type CStruct = { [x: string]: CType };
export type Data<T extends CStruct> = { [K in keyof T]: number };

function sizeof(struct: CStruct): number {
  let size = 0;

  for (const key in struct) {
    size += struct[key].BYTES_PER_ELEMENT;
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

  allocNumberArray<T extends CType>(Type: T, array: InstanceType<T>): number {
    const pointer = this._malloc(array.length * Type.BYTES_PER_ELEMENT);
    const type = new Type(this._buffer, pointer);
    type.set(array);
    return pointer;
  }

  allocStructArray<T extends CStruct>(struct: T, data: Data<T>[]): number {
    const keys = Object.keys(struct);
    const structSize = sizeof(struct);
    const pointer = this._malloc(structSize);

    for (let i = 0; i < data.length; i += 1) {
      const datum = data[i];

      let offset = pointer + i * structSize;

      for (let j = 0; j < keys.length; j += 1) {
        const key = keys[j];

        const Type = struct[key];
        const type = new Type(this._buffer, offset, 1);
        type[0] = datum[key];

        offset += struct[key].BYTES_PER_ELEMENT;
      }
    }

    return pointer;
  }
}
