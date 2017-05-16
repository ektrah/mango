# Mango Instruction Set

| OpCode | Instruction      | Stack Transition                                            |
| ------:|:---------------- |:----------------------------------------------------------- |
| 0xC0   | add.f32          | value2 value1 ... &rarr; result ...                         |
| 0xE0   | add.f64          | value2 value1 ... &rarr; result ...                         |
| 0x40   | add.i32          | value2 value1 ... &rarr; result ...                         |
| 0x90   | add.i64          | value2 value1 ... &rarr; result ...                         |
| 0x4B   | and.i32          | value2 value1 ... &rarr; result ...                         |
| 0x9B   | and.i64          | value2 value1 ... &rarr; result ...                         |
| 0x23   | br               | ... &rarr; ...                                              |
| 0x20   | br.s             | ... &rarr; ...                                              |
| 0x01   | break            | ... &rarr; ...                                              |
| 0x24   | brfalse          | value ... &rarr; ...                                        |
| 0x21   | brfalse.s        | value ... &rarr; ...                                        |
| 0x25   | brtrue           | value ... &rarr; ...                                        |
| 0x22   | brtrue.s         | value ... &rarr; ...                                        |
| 0x1B   | call             | argumentN ... argument1 argument0 ... &rarr; result ...     |
| 0x1C   | calli            | ftn argumentN ... argument1 argument0 ... &rarr; result ... |
| 0xC6   | ceq.f32          | value2 value1 ... &rarr; result ...                         |
| 0xC7   | ceq.f32.un       | value2 value1 ... &rarr; result ...                         |
| 0xE6   | ceq.f64          | value2 value1 ... &rarr; result ...                         |
| 0xE7   | ceq.f64.un       | value2 value1 ... &rarr; result ...                         |
| 0x4F   | ceq.i32          | value2 value1 ... &rarr; result ...                         |
| 0x9F   | ceq.i64          | value2 value1 ... &rarr; result ...                         |
| 0xCC   | cge.f32          | value2 value1 ... &rarr; result ...                         |
| 0xCD   | cge.f32.un       | value2 value1 ... &rarr; result ...                         |
| 0xEC   | cge.f64          | value2 value1 ... &rarr; result ...                         |
| 0xED   | cge.f64.un       | value2 value1 ... &rarr; result ...                         |
| 0x53   | cge.i32          | value2 value1 ... &rarr; result ...                         |
| 0x54   | cge.i32.un       | value2 value1 ... &rarr; result ...                         |
| 0xA3   | cge.i64          | value2 value1 ... &rarr; result ...                         |
| 0xA4   | cge.i64.un       | value2 value1 ... &rarr; result ...                         |
| 0xCA   | cgt.f32          | value2 value1 ... &rarr; result ...                         |
| 0xCB   | cgt.f32.un       | value2 value1 ... &rarr; result ...                         |
| 0xEA   | cgt.f64          | value2 value1 ... &rarr; result ...                         |
| 0xEB   | cgt.f64.un       | value2 value1 ... &rarr; result ...                         |
| 0x51   | cgt.i32          | value2 value1 ... &rarr; result ...                         |
| 0x52   | cgt.i32.un       | value2 value1 ... &rarr; result ...                         |
| 0xA1   | cgt.i64          | value2 value1 ... &rarr; result ...                         |
| 0xA2   | cgt.i64.un       | value2 value1 ... &rarr; result ...                         |
| 0xD0   | cle.f32          | value2 value1 ... &rarr; result ...                         |
| 0xD1   | cle.f32.un       | value2 value1 ... &rarr; result ...                         |
| 0xF0   | cle.f64          | value2 value1 ... &rarr; result ...                         |
| 0xF1   | cle.f64.un       | value2 value1 ... &rarr; result ...                         |
| 0x57   | cle.i32          | value2 value1 ... &rarr; result ...                         |
| 0x58   | cle.i32.un       | value2 value1 ... &rarr; result ...                         |
| 0xA7   | cle.i64          | value2 value1 ... &rarr; result ...                         |
| 0xA8   | cle.i64.un       | value2 value1 ... &rarr; result ...                         |
| 0xCE   | clt.f32          | value2 value1 ... &rarr; result ...                         |
| 0xCF   | clt.f32.un       | value2 value1 ... &rarr; result ...                         |
| 0xEE   | clt.f64          | value2 value1 ... &rarr; result ...                         |
| 0xEF   | clt.f64.un       | value2 value1 ... &rarr; result ...                         |
| 0x55   | clt.i32          | value2 value1 ... &rarr; result ...                         |
| 0x56   | clt.i32.un       | value2 value1 ... &rarr; result ...                         |
| 0xA5   | clt.i64          | value2 value1 ... &rarr; result ...                         |
| 0xA6   | clt.i64.un       | value2 value1 ... &rarr; result ...                         |
| 0xC8   | cne.f32          | value2 value1 ... &rarr; result ...                         |
| 0xC9   | cne.f32.un       | value2 value1 ... &rarr; result ...                         |
| 0xE8   | cne.f64          | value2 value1 ... &rarr; result ...                         |
| 0xE9   | cne.f64.un       | value2 value1 ... &rarr; result ...                         |
| 0x50   | cne.i32          | value2 value1 ... &rarr; result ...                         |
| 0xA0   | cne.i64          | value2 value1 ... &rarr; result ...                         |
| 0xDC   | conv.f32.f64     | value ... &rarr; result ...                                 |
| 0xD8   | conv.f32.i32     | value ... &rarr; result ...                                 |
| 0xD9   | conv.f32.i32.un  | value ... &rarr; result ...                                 |
| 0xDA   | conv.f32.i64     | value ... &rarr; result ...                                 |
| 0xDB   | conv.f32.i64.un  | value ... &rarr; result ...                                 |
| 0xFC   | conv.f64.f32     | value ... &rarr; result ...                                 |
| 0xF8   | conv.f64.i32     | value ... &rarr; result ...                                 |
| 0xF9   | conv.f64.i32.un  | value ... &rarr; result ...                                 |
| 0xFA   | conv.f64.i64     | value ... &rarr; result ...                                 |
| 0xFB   | conv.f64.i64.un  | value ... &rarr; result ...                                 |
| 0xD4   | conv.i16.f32     | value ... &rarr; result ...                                 |
| 0xF4   | conv.i16.f64     | value ... &rarr; result ...                                 |
| 0x5B   | conv.i16.i32     | value ... &rarr; result ...                                 |
| 0xAB   | conv.i16.i64     | value ... &rarr; result ...                                 |
| 0xD6   | conv.i32.f32     | value ... &rarr; result ...                                 |
| 0xF6   | conv.i32.f64     | value ... &rarr; result ...                                 |
| 0xAD   | conv.i32.i64     | value ... &rarr; result ...                                 |
| 0xB1   | conv.i64.f32     | value ... &rarr; result ...                                 |
| 0xB3   | conv.i64.f64     | value ... &rarr; result ...                                 |
| 0xAF   | conv.i64.i32     | value ... &rarr; result ...                                 |
| 0xD2   | conv.i8.f32      | value ... &rarr; result ...                                 |
| 0xF2   | conv.i8.f64      | value ... &rarr; result ...                                 |
| 0x59   | conv.i8.i32      | value ... &rarr; result ...                                 |
| 0xA9   | conv.i8.i64      | value ... &rarr; result ...                                 |
| 0xD5   | conv.u16.f32     | value ... &rarr; result ...                                 |
| 0xF5   | conv.u16.f64     | value ... &rarr; result ...                                 |
| 0x5C   | conv.u16.i32     | value ... &rarr; result ...                                 |
| 0xAC   | conv.u16.i64     | value ... &rarr; result ...                                 |
| 0xD7   | conv.u32.f32     | value ... &rarr; result ...                                 |
| 0xF7   | conv.u32.f64     | value ... &rarr; result ...                                 |
| 0xAE   | conv.u32.i64     | value ... &rarr; result ...                                 |
| 0xB2   | conv.u64.f32     | value ... &rarr; result ...                                 |
| 0xB4   | conv.u64.f64     | value ... &rarr; result ...                                 |
| 0xB0   | conv.u64.i32     | value ... &rarr; result ...                                 |
| 0xD3   | conv.u8.f32      | value ... &rarr; result ...                                 |
| 0xF3   | conv.u8.f64      | value ... &rarr; result ...                                 |
| 0x5A   | conv.u8.i32      | value ... &rarr; result ...                                 |
| 0xAA   | conv.u8.i64      | value ... &rarr; result ...                                 |
| 0xC3   | div.f32          | value2 value1 ... &rarr; result ...                         |
| 0xE3   | div.f64          | value2 value1 ... &rarr; result ...                         |
| 0x43   | div.i32          | value2 value1 ... &rarr; result ...                         |
| 0x44   | div.i32.un       | value2 value1 ... &rarr; result ...                         |
| 0x93   | div.i64          | value2 value1 ... &rarr; result ...                         |
| 0x94   | div.i64.un       | value2 value1 ... &rarr; result ...                         |
| 0x06   | dup.x32          | value ... &rarr; value value ...                            |
| 0x07   | dup.x64          | value ... &rarr; value value ...                            |
| 0x29   | ldc.i32.0        | ... &rarr; value ...                                        |
| 0x2A   | ldc.i32.1        | ... &rarr; value ...                                        |
| 0x2B   | ldc.i32.2        | ... &rarr; value ...                                        |
| 0x2C   | ldc.i32.3        | ... &rarr; value ...                                        |
| 0x2D   | ldc.i32.4        | ... &rarr; value ...                                        |
| 0x2E   | ldc.i32.5        | ... &rarr; value ...                                        |
| 0x2F   | ldc.i32.6        | ... &rarr; value ...                                        |
| 0x30   | ldc.i32.7        | ... &rarr; value ...                                        |
| 0x31   | ldc.i32.8        | ... &rarr; value ...                                        |
| 0x28   | ldc.i32.m1       | ... &rarr; value ...                                        |
| 0x32   | ldc.i32.s        | ... &rarr; value ...                                        |
| 0x33   | ldc.x32          | ... &rarr; value ...                                        |
| 0x34   | ldc.x64          | ... &rarr; value ...                                        |
| 0x80   | ldelem.i16       | index array length ... &rarr; value ...                     |
| 0x7E   | ldelem.i8        | index array length ... &rarr; value ...                     |
| 0x81   | ldelem.u16       | index array length ... &rarr; value ...                     |
| 0x7F   | ldelem.u8        | index array length ... &rarr; value ...                     |
| 0x82   | ldelem.x32       | index array length ... &rarr; value ...                     |
| 0x83   | ldelem.x64       | index array length ... &rarr; value ...                     |
| 0x84   | ldelema          | index array length ... &rarr; address ...                   |
| 0x86   | ldelema.x16      | index array length ... &rarr; address ...                   |
| 0x87   | ldelema.x32      | index array length ... &rarr; address ...                   |
| 0x88   | ldelema.x64      | index array length ... &rarr; address ...                   |
| 0x85   | ldelema.x8       | index array length ... &rarr; address ...                   |
| 0x6A   | ldfld.i16        | address ... &rarr; value ...                                |
| 0x68   | ldfld.i8         | address ... &rarr; value ...                                |
| 0x6B   | ldfld.u16        | address ... &rarr; value ...                                |
| 0x69   | ldfld.u8         | address ... &rarr; value ...                                |
| 0x6C   | ldfld.x32        | address ... &rarr; value ...                                |
| 0x6D   | ldfld.x64        | address ... &rarr; value ...                                |
| 0x6E   | ldflda           | address ... &rarr; address ...                              |
| 0x35   | ldftn            | ... &rarr; ftn ...                                          |
| 0x65   | ldlen            | array length ... &rarr; length ...                          |
| 0x10   | ldloc.x32        | ... &rarr; value ...                                        |
| 0x11   | ldloc.x64        | ... &rarr; value ...                                        |
| 0x12   | ldloca           | ... &rarr; address ...                                      |
| 0x75   | ldsfld.i16       | ... &rarr; value ...                                        |
| 0x73   | ldsfld.i8        | ... &rarr; value ...                                        |
| 0x76   | ldsfld.u16       | ... &rarr; value ...                                        |
| 0x74   | ldsfld.u8        | ... &rarr; value ...                                        |
| 0x77   | ldsfld.x32       | ... &rarr; value ...                                        |
| 0x78   | ldsfld.x64       | ... &rarr; value ...                                        |
| 0x79   | ldsflda          | ... &rarr; address ...                                      |
| 0x62   | mkslice          | address length ... &rarr; array length ...                  |
| 0xC2   | mul.f32          | value2 value1 ... &rarr; result ...                         |
| 0xE2   | mul.f64          | value2 value1 ... &rarr; result ...                         |
| 0x42   | mul.i32          | value2 value1 ... &rarr; result ...                         |
| 0x92   | mul.i64          | value2 value1 ... &rarr; result ...                         |
| 0xC5   | neg.f32          | value ... &rarr; result ...                                 |
| 0xE5   | neg.f64          | value ... &rarr; result ...                                 |
| 0x47   | neg.i32          | value ... &rarr; result ...                                 |
| 0x97   | neg.i64          | value ... &rarr; result ...                                 |
| 0x61   | newarr           | length ... &rarr; array length ...                          |
| 0x60   | newobj           | ... &rarr; address ...                                      |
| 0x0C   | nip              | value1 value2 ... &rarr; value1 ...                         |
| 0x00   | nop              | ... &rarr; ...                                              |
| 0x4E   | not.i32          | value ... &rarr; result ...                                 |
| 0x9E   | not.i64          | value ... &rarr; result ...                                 |
| 0x4C   | or.i32           | value2 value1 ... &rarr; result ...                         |
| 0x9C   | or.i64           | value2 value1 ... &rarr; result ...                         |
| 0x0A   | over             | value1 value2 ... &rarr; value2 value1 value2 ...           |
| 0x04   | pop.x32          | value ... &rarr; ...                                        |
| 0x05   | pop.x64          | value ... &rarr; ...                                        |
| 0xC4   | rem.f32          | value2 value1 ... &rarr; result ...                         |
| 0xE4   | rem.f64          | value2 value1 ... &rarr; result ...                         |
| 0x45   | rem.i32          | value2 value1 ... &rarr; result ...                         |
| 0x46   | rem.i32.un       | value2 value1 ... &rarr; result ...                         |
| 0x95   | rem.i64          | value2 value1 ... &rarr; result ...                         |
| 0x96   | rem.i64.un       | value2 value1 ... &rarr; result ...                         |
| 0x18   | ret              | ... &rarr; ...                                              |
| 0x19   | ret.x32          | value ... &rarr; ...                                        |
| 0x1A   | ret.x64          | value ... &rarr; ...                                        |
| 0x0B   | rot              | value1 value2 value3 ... &rarr; value2 value3 value1 ...    |
| 0x48   | shl.i32          | amount value ... &rarr; result ...                          |
| 0x98   | shl.i64          | amount value ... &rarr; result ...                          |
| 0x49   | shr.i32          | amount value ... &rarr; result ...                          |
| 0x4A   | shr.i32.un       | amount value ... &rarr; result ...                          |
| 0x99   | shr.i64          | amount value ... &rarr; result ...                          |
| 0x9A   | shr.i64.un       | amount value ... &rarr; result ...                          |
| 0x63   | slice            | start array length ... &rarr; array length ...              |
| 0x64   | slice2           | length' start array length ... &rarr; array length' ...     |
| 0x8A   | stelem.i16       | value index array length ... &rarr; ...                     |
| 0x89   | stelem.i8        | value index array length ... &rarr; ...                     |
| 0x8B   | stelem.x32       | value index array length ... &rarr; ...                     |
| 0x8C   | stelem.x64       | value index array length ... &rarr; ...                     |
| 0x70   | stfld.i16        | value address ... &rarr; ...                                |
| 0x6F   | stfld.i8         | value address ... &rarr; ...                                |
| 0x71   | stfld.x32        | value address ... &rarr; ...                                |
| 0x72   | stfld.x64        | value address ... &rarr; ...                                |
| 0x13   | stloc.x32        | value ... &rarr; ...                                        |
| 0x14   | stloc.x64        | value ... &rarr; ...                                        |
| 0x7B   | stsfld.i16       | value ... &rarr; ...                                        |
| 0x7A   | stsfld.i8        | value ... &rarr; ...                                        |
| 0x7C   | stsfld.x32       | value ... &rarr; ...                                        |
| 0x7D   | stsfld.x64       | value ... &rarr; ...                                        |
| 0xC1   | sub.f32          | value2 value1 ... &rarr; result ...                         |
| 0xE1   | sub.f64          | value2 value1 ... &rarr; result ...                         |
| 0x41   | sub.i32          | value2 value1 ... &rarr; result ...                         |
| 0x91   | sub.i64          | value2 value1 ... &rarr; result ...                         |
| 0x08   | swap.x32         | value1 value2 ... &rarr; value2 value1 ...                  |
| 0x09   | swap.x64         | value1 value2 ... &rarr; value2 value1 ...                  |
| 0x1D   | syscall          | argumentN ... argument1 argument0 ... &rarr; result ...     |
| 0x0D   | tuck             | value1 value2 ... &rarr; value1 value2 value1 ...           |
| 0x4D   | xor.i32          | value2 value1 ... &rarr; result ...                         |
| 0x9D   | xor.i64          | value2 value1 ... &rarr; result ...                         |
