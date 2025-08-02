
// An ImageData-like object because ImageData isn't transferrable
export type DecodeOutput = {
  data: ImageDataArray;
  width: number;
  height: number;
};

export type Module = {
  decode(inData: Uint8Array): DecodeOutput;
};

export default function ModuleFactory(options?: unknown): Promise<Module>;
