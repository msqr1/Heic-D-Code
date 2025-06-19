export type DecodeOutput = {
  data: Uint8ClampedArray;
  width: Number;
  height: Number;
};

export type Module = {
  decode(inData: Uint8Array): DecodeOutput;
};

export default function MainModuleFactory(options?: unknown): Promise<Module>;
