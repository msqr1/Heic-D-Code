
// An ImageData-like object because ImageData isn't transferrable
export type DecodeOutput = [
  ImageDataArray,
  number,
  number,
];

export type Module = {
  decode(inData: Uint8Array): DecodeOutput;
};

export default function ModuleFactory(options?: unknown): Promise<Module>;
