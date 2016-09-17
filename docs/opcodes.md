# Mango Instruction Set

| OpCode | Instruction      | Stack Transition                                                        |
|-------:|:-----------------|:------------------------------------------------------------------------|
| 0x30   | add.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x36   | add.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0x20   | add.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x28   | add.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x3F   | and.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x46   | and.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0xB8   | br               | ... &rarr; ...                                                          |  
| 0xB9   | br.s             | ... &rarr; ...                                                          |  
| 0x01   | break            | ... &rarr; ...                                                          |  
| 0xBA   | brfalse          | value ... &rarr; ...                                                    |  
| 0xBB   | brfalse.s        | value ... &rarr; ...                                                    |  
| 0xBC   | brtrue           | value ... &rarr; ...                                                    |  
| 0xBD   | brtrue.s         | value ... &rarr; ...                                                    |  
| 0x1B   | call             | argumentN ... argument1 argument0 ... &rarr; result ...                 |  
| 0x9C   | ceq.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x9D   | ceq.f32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xA8   | ceq.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0xA9   | ceq.f64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x88   | ceq.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x92   | ceq.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0xA2   | cge.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0xA3   | cge.f32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xAE   | cge.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0xAF   | cge.f64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x8C   | cge.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x8D   | cge.i32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x96   | cge.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x97   | cge.i64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xA0   | cgt.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0xA1   | cgt.f32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xAC   | cgt.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0xAD   | cgt.f64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x8A   | cgt.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x8B   | cgt.i32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x94   | cgt.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x95   | cgt.i64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xA6   | cle.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0xA7   | cle.f32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xB2   | cle.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0xB3   | cle.f64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x90   | cle.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x91   | cle.i32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x9A   | cle.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x9B   | cle.i64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xA4   | clt.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0xA5   | clt.f32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xB0   | clt.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0xB1   | clt.f64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x8E   | clt.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x8F   | clt.i32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x98   | clt.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x99   | clt.i64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x9E   | cne.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x9F   | cne.f32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0xAA   | cne.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0xAB   | cne.f64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x89   | cne.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x93   | cne.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x85   | conv.f32.f64     | value ... &rarr; result ...                                             |  
| 0x66   | conv.f32.i32     | value ... &rarr; result ...                                             |  
| 0x67   | conv.f32.i32.un  | value ... &rarr; result ...                                             |  
| 0x70   | conv.f32.i64     | value ... &rarr; result ...                                             |  
| 0x71   | conv.f32.i64.un  | value ... &rarr; result ...                                             |  
| 0x7C   | conv.f64.f32     | value ... &rarr; result ...                                             |  
| 0x68   | conv.f64.i32     | value ... &rarr; result ...                                             |  
| 0x69   | conv.f64.i32.un  | value ... &rarr; result ...                                             |  
| 0x72   | conv.f64.i64     | value ... &rarr; result ...                                             |  
| 0x73   | conv.f64.i64.un  | value ... &rarr; result ...                                             |  
| 0x76   | conv.i16.f32     | value ... &rarr; result ...                                             |  
| 0x7F   | conv.i16.f64     | value ... &rarr; result ...                                             |  
| 0x62   | conv.i16.i32     | value ... &rarr; result ...                                             |  
| 0x6C   | conv.i16.i64     | value ... &rarr; result ...                                             |  
| 0x78   | conv.i32.f32     | value ... &rarr; result ...                                             |  
| 0x81   | conv.i32.f64     | value ... &rarr; result ...                                             |  
| 0x6E   | conv.i32.i64     | value ... &rarr; result ...                                             |  
| 0x7A   | conv.i64.f32     | value ... &rarr; result ...                                             |  
| 0x83   | conv.i64.f64     | value ... &rarr; result ...                                             |  
| 0x64   | conv.i64.i32     | value ... &rarr; result ...                                             |  
| 0x74   | conv.i8.f32      | value ... &rarr; result ...                                             |  
| 0x7D   | conv.i8.f64      | value ... &rarr; result ...                                             |  
| 0x60   | conv.i8.i32      | value ... &rarr; result ...                                             |  
| 0x6A   | conv.i8.i64      | value ... &rarr; result ...                                             |  
| 0x77   | conv.u16.f32     | value ... &rarr; result ...                                             |  
| 0x80   | conv.u16.f64     | value ... &rarr; result ...                                             |  
| 0x63   | conv.u16.i32     | value ... &rarr; result ...                                             |  
| 0x6D   | conv.u16.i64     | value ... &rarr; result ...                                             |  
| 0x79   | conv.u32.f32     | value ... &rarr; result ...                                             |  
| 0x82   | conv.u32.f64     | value ... &rarr; result ...                                             |  
| 0x6F   | conv.u32.i64     | value ... &rarr; result ...                                             |  
| 0x7B   | conv.u64.f32     | value ... &rarr; result ...                                             |  
| 0x84   | conv.u64.f64     | value ... &rarr; result ...                                             |  
| 0x65   | conv.u64.i32     | value ... &rarr; result ...                                             |  
| 0x75   | conv.u8.f32      | value ... &rarr; result ...                                             |  
| 0x7E   | conv.u8.f64      | value ... &rarr; result ...                                             |  
| 0x61   | conv.u8.i32      | value ... &rarr; result ...                                             |  
| 0x6B   | conv.u8.i64      | value ... &rarr; result ...                                             |  
| 0x33   | div.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x39   | div.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0x23   | div.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x24   | div.i32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x2B   | div.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x2C   | div.i64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x06   | dup              | value ... &rarr; value value ...                                        |  
| 0x07   | dup2             | value1 value2 ... &rarr; value1 value2 value1 value2 ...                |  
| 0x5D   | ldc.f32          | ... &rarr; value ...                                                    |  
| 0x5E   | ldc.f64          | ... &rarr; value ...                                                    |  
| 0x5B   | ldc.i32          | ... &rarr; value ...                                                    |  
| 0x51   | ldc.i32.0        | ... &rarr; value ...                                                    |  
| 0x52   | ldc.i32.1        | ... &rarr; value ...                                                    |  
| 0x53   | ldc.i32.2        | ... &rarr; value ...                                                    |  
| 0x54   | ldc.i32.3        | ... &rarr; value ...                                                    |  
| 0x55   | ldc.i32.4        | ... &rarr; value ...                                                    |  
| 0x56   | ldc.i32.5        | ... &rarr; value ...                                                    |  
| 0x57   | ldc.i32.6        | ... &rarr; value ...                                                    |  
| 0x58   | ldc.i32.7        | ... &rarr; value ...                                                    |  
| 0x59   | ldc.i32.8        | ... &rarr; value ...                                                    |  
| 0x50   | ldc.i32.m1       | ... &rarr; value ...                                                    |  
| 0x5A   | ldc.i32.s        | ... &rarr; value ...                                                    |  
| 0x5C   | ldc.i64          | ... &rarr; value ...                                                    |  
| 0x5F   | ldca             | ... &rarr; address ...                                                  |  
| 0xE0   | ldelem.i16       | index array length ... &rarr; value ...                                 |  
| 0xDE   | ldelem.i8        | index array length ... &rarr; value ...                                 |  
| 0xE1   | ldelem.u16       | index array length ... &rarr; value ...                                 |  
| 0xDF   | ldelem.u8        | index array length ... &rarr; value ...                                 |  
| 0xE2   | ldelem.x32       | index array length ... &rarr; value ...                                 |  
| 0xE3   | ldelem.x64       | index array length ... &rarr; value ...                                 |  
| 0xE4   | ldelema          | index array length ... &rarr; address ...                               |  
| 0xE6   | ldelema.x16      | index array length ... &rarr; address ...                               |  
| 0xE7   | ldelema.x32      | index array length ... &rarr; address ...                               |  
| 0xE8   | ldelema.x64      | index array length ... &rarr; address ...                               |  
| 0xE5   | ldelema.x8       | index array length ... &rarr; address ...                               |  
| 0xCA   | ldfld.i16        | address ... &rarr; value ...                                            |  
| 0xC8   | ldfld.i8         | address ... &rarr; value ...                                            |  
| 0xCB   | ldfld.u16        | address ... &rarr; value ...                                            |  
| 0xC9   | ldfld.u8         | address ... &rarr; value ...                                            |  
| 0xCC   | ldfld.x32        | address ... &rarr; value ...                                            |  
| 0xCD   | ldfld.x64        | address ... &rarr; value ...                                            |  
| 0xCE   | ldflda           | address ... &rarr; address ...                                          |  
| 0xC5   | ldlen            | array length ... &rarr; length ...                                      |  
| 0x10   | ldloc.x32        | ... &rarr; value ...                                                    |  
| 0x11   | ldloc.x64        | ... &rarr; value ...                                                    |  
| 0x12   | ldloca           | ... &rarr; address ...                                                  |  
| 0xD5   | ldsfld.i16       | ... &rarr; value ...                                                    |  
| 0xD3   | ldsfld.i8        | ... &rarr; value ...                                                    |  
| 0xD6   | ldsfld.u16       | ... &rarr; value ...                                                    |  
| 0xD4   | ldsfld.u8        | ... &rarr; value ...                                                    |  
| 0xD7   | ldsfld.x32       | ... &rarr; value ...                                                    |  
| 0xD8   | ldsfld.x64       | ... &rarr; value ...                                                    |  
| 0xD9   | ldsflda          | ... &rarr; address ...                                                  |  
| 0xC2   | mkslice          | address length ... &rarr; array length ...                              |  
| 0x32   | mul.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x38   | mul.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0x22   | mul.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x2A   | mul.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x35   | neg.f32          | value ... &rarr; result ...                                             |  
| 0x3B   | neg.f64          | value ... &rarr; result ...                                             |  
| 0x27   | neg.i32          | value ... &rarr; result ...                                             |  
| 0x2F   | neg.i64          | value ... &rarr; result ...                                             |  
| 0xC1   | newarr           | length ... &rarr; array length ...                                      |  
| 0xC0   | newobj           | ... &rarr; address ...                                                  |  
| 0x0C   | nip              | value1 value2 ... &rarr; value1 ...                                     |  
| 0x00   | nop              | ... &rarr; ...                                                          |  
| 0x42   | not.i32          | value ... &rarr; result ...                                             |  
| 0x49   | not.i64          | value ... &rarr; result ...                                             |  
| 0x40   | or.i32           | value2 value1 ... &rarr; result ...                                     |  
| 0x47   | or.i64           | value2 value1 ... &rarr; result ...                                     |  
| 0x0A   | over             | value1 value2 ... &rarr; value2 value1 value2 ...                       |  
| 0x04   | pop              | value ... &rarr; ...                                                    |  
| 0x05   | pop2             | value1 value2 ... &rarr; ...                                            |  
| 0x34   | rem.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x3A   | rem.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0x25   | rem.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x26   | rem.i32.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x2D   | rem.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x2E   | rem.i64.un       | value2 value1 ... &rarr; result ...                                     |  
| 0x18   | ret              | ... &rarr; ...                                                          |  
| 0x19   | ret.x32          | value ... &rarr; ...                                                    |  
| 0x1A   | ret.x64          | value ... &rarr; ...                                                    |  
| 0x0B   | rot              | value1 value2 value3 ... &rarr; value2 value3 value1 ...                |  
| 0x3C   | shl.i32          | amount value ... &rarr; result ...                                      |  
| 0x43   | shl.i64          | amount value ... &rarr; result ...                                      |  
| 0x3D   | shr.i32          | amount value ... &rarr; result ...                                      |  
| 0x3E   | shr.i32.un       | amount value ... &rarr; result ...                                      |  
| 0x44   | shr.i64          | amount value ... &rarr; result ...                                      |  
| 0x45   | shr.i64.un       | amount value ... &rarr; result ...                                      |  
| 0xC3   | slice            | start array length ... &rarr; array length ...                          |  
| 0xC4   | slice2           | length' start array length ... &rarr; array length' ...                 |  
| 0xEA   | stelem.i16       | value index array length ... &rarr; ...                                 |  
| 0xE9   | stelem.i8        | value index array length ... &rarr; ...                                 |  
| 0xEB   | stelem.x32       | value index array length ... &rarr; ...                                 |  
| 0xEC   | stelem.x64       | value index array length ... &rarr; ...                                 |  
| 0xD0   | stfld.i16        | value address ... &rarr; ...                                            |  
| 0xCF   | stfld.i8         | value address ... &rarr; ...                                            |  
| 0xD1   | stfld.x32        | value address ... &rarr; ...                                            |  
| 0xD2   | stfld.x64        | value address ... &rarr; ...                                            |  
| 0x13   | stloc.x32        | value ... &rarr; ...                                                    |  
| 0x14   | stloc.x64        | value ... &rarr; ...                                                    |  
| 0xDB   | stsfld.i16       | value ... &rarr; ...                                                    |  
| 0xDA   | stsfld.i8        | value ... &rarr; ...                                                    |  
| 0xDC   | stsfld.x32       | value ... &rarr; ...                                                    |  
| 0xDD   | stsfld.x64       | value ... &rarr; ...                                                    |  
| 0x31   | sub.f32          | value2 value1 ... &rarr; result ...                                     |  
| 0x37   | sub.f64          | value2 value1 ... &rarr; result ...                                     |  
| 0x21   | sub.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x29   | sub.i64          | value2 value1 ... &rarr; result ...                                     |  
| 0x08   | swap             | value1 value2 ... &rarr; value2 value1 ...                              |  
| 0x09   | swap2            | value1 value2 value3 value4  ... &rarr; value3 value4 value1 value2 ... |  
| 0x1C   | syscall          | argumentN ... argument1 argument0 ... &rarr; result ...                 |  
| 0x0D   | tuck             | value1 value2 ... &rarr; value1 value2 value1 ...                       |  
| 0x41   | xor.i32          | value2 value1 ... &rarr; result ...                                     |  
| 0x48   | xor.i64          | value2 value1 ... &rarr; result ...                                     |  
