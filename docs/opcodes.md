# Mango Instruction Set

| Opcode | Instruction      | Stack Transition                                            |
|:------ |:---------------- |:----------------------------------------------------------- |
| 0xC0   | add.f32          | ... value1 value2 &rarr; ... result                         |
| 0xE0   | add.f64          | ... value1 value2 &rarr; ... result                         |
| 0x40   | add.i32          | ... value1 value2 &rarr; ... result                         |
| 0x90   | add.i64          | ... value1 value2 &rarr; ... result                         |
| 0x4B   | and.i32          | ... value1 value2 &rarr; ... result                         |
| 0x9B   | and.i64          | ... value1 value2 &rarr; ... result                         |
| 0x23   | br               | ... &rarr; ...                                              |
| 0x20   | br.s             | ... &rarr; ...                                              |
| 0x01   | break            | ... &rarr; ...                                              |
| 0x24   | brfalse          | ... value &rarr; ...                                        |
| 0x21   | brfalse.s        | ... value &rarr; ...                                        |
| 0x25   | brinst           | ... value &rarr; ...                                        |
| 0x22   | brinst.s         | ... value &rarr; ...                                        |
| 0x24   | brnull           | ... value &rarr; ...                                        |
| 0x21   | brnull.s         | ... value &rarr; ...                                        |
| 0x25   | brtrue           | ... value &rarr; ...                                        |
| 0x22   | brtrue.s         | ... value &rarr; ...                                        |
| 0x1D   | call             | ... argument0 argument1 ... argumentN &rarr; ... result     |
| 0x1C   | call.s           | ... argument0 argument1 ... argumentN &rarr; ... result     |
| 0x1B   | calli            | ... argument0 argument1 ... argumentN ftn &rarr; ... result |
| 0xC6   | ceq.f32          | ... value1 value2 &rarr; ... result                         |
| 0xC7   | ceq.f32.un       | ... value1 value2 &rarr; ... result                         |
| 0xE6   | ceq.f64          | ... value1 value2 &rarr; ... result                         |
| 0xE7   | ceq.f64.un       | ... value1 value2 &rarr; ... result                         |
| 0x4F   | ceq.i32          | ... value1 value2 &rarr; ... result                         |
| 0x9F   | ceq.i64          | ... value1 value2 &rarr; ... result                         |
| 0x4F   | ceq.ref          | ... address1 address2 &rarr; ... result                     |
| 0xCC   | cge.f32          | ... value1 value2 &rarr; ... result                         |
| 0xCD   | cge.f32.un       | ... value1 value2 &rarr; ... result                         |
| 0xEC   | cge.f64          | ... value1 value2 &rarr; ... result                         |
| 0xED   | cge.f64.un       | ... value1 value2 &rarr; ... result                         |
| 0x53   | cge.i32          | ... value1 value2 &rarr; ... result                         |
| 0x54   | cge.i32.un       | ... value1 value2 &rarr; ... result                         |
| 0xA3   | cge.i64          | ... value1 value2 &rarr; ... result                         |
| 0xA4   | cge.i64.un       | ... value1 value2 &rarr; ... result                         |
| 0xCA   | cgt.f32          | ... value1 value2 &rarr; ... result                         |
| 0xCB   | cgt.f32.un       | ... value1 value2 &rarr; ... result                         |
| 0xEA   | cgt.f64          | ... value1 value2 &rarr; ... result                         |
| 0xEB   | cgt.f64.un       | ... value1 value2 &rarr; ... result                         |
| 0x51   | cgt.i32          | ... value1 value2 &rarr; ... result                         |
| 0x52   | cgt.i32.un       | ... value1 value2 &rarr; ... result                         |
| 0xA1   | cgt.i64          | ... value1 value2 &rarr; ... result                         |
| 0xA2   | cgt.i64.un       | ... value1 value2 &rarr; ... result                         |
| 0xD0   | cle.f32          | ... value1 value2 &rarr; ... result                         |
| 0xD1   | cle.f32.un       | ... value1 value2 &rarr; ... result                         |
| 0xF0   | cle.f64          | ... value1 value2 &rarr; ... result                         |
| 0xF1   | cle.f64.un       | ... value1 value2 &rarr; ... result                         |
| 0x57   | cle.i32          | ... value1 value2 &rarr; ... result                         |
| 0x58   | cle.i32.un       | ... value1 value2 &rarr; ... result                         |
| 0xA7   | cle.i64          | ... value1 value2 &rarr; ... result                         |
| 0xA8   | cle.i64.un       | ... value1 value2 &rarr; ... result                         |
| 0xCE   | clt.f32          | ... value1 value2 &rarr; ... result                         |
| 0xCF   | clt.f32.un       | ... value1 value2 &rarr; ... result                         |
| 0xEE   | clt.f64          | ... value1 value2 &rarr; ... result                         |
| 0xEF   | clt.f64.un       | ... value1 value2 &rarr; ... result                         |
| 0x55   | clt.i32          | ... value1 value2 &rarr; ... result                         |
| 0x56   | clt.i32.un       | ... value1 value2 &rarr; ... result                         |
| 0xA5   | clt.i64          | ... value1 value2 &rarr; ... result                         |
| 0xA6   | clt.i64.un       | ... value1 value2 &rarr; ... result                         |
| 0xC8   | cne.f32          | ... value1 value2 &rarr; ... result                         |
| 0xC9   | cne.f32.un       | ... value1 value2 &rarr; ... result                         |
| 0xE8   | cne.f64          | ... value1 value2 &rarr; ... result                         |
| 0xE9   | cne.f64.un       | ... value1 value2 &rarr; ... result                         |
| 0x50   | cne.i32          | ... value1 value2 &rarr; ... result                         |
| 0xA0   | cne.i64          | ... value1 value2 &rarr; ... result                         |
| 0x50   | cne.ref          | ... address1 address2 &rarr; ... result                     |
| 0xDC   | conv.f32.f64     | ... value &rarr; ... result                                 |
| 0xD8   | conv.f32.i32     | ... value &rarr; ... result                                 |
| 0xD9   | conv.f32.i32.un  | ... value &rarr; ... result                                 |
| 0xDA   | conv.f32.i64     | ... value &rarr; ... result                                 |
| 0xDB   | conv.f32.i64.un  | ... value &rarr; ... result                                 |
| 0xFC   | conv.f64.f32     | ... value &rarr; ... result                                 |
| 0xF8   | conv.f64.i32     | ... value &rarr; ... result                                 |
| 0xF9   | conv.f64.i32.un  | ... value &rarr; ... result                                 |
| 0xFA   | conv.f64.i64     | ... value &rarr; ... result                                 |
| 0xFB   | conv.f64.i64.un  | ... value &rarr; ... result                                 |
| 0xD4   | conv.i16.f32     | ... value &rarr; ... result                                 |
| 0xF4   | conv.i16.f64     | ... value &rarr; ... result                                 |
| 0x5B   | conv.i16.i32     | ... value &rarr; ... result                                 |
| 0xAB   | conv.i16.i64     | ... value &rarr; ... result                                 |
| 0xD6   | conv.i32.f32     | ... value &rarr; ... result                                 |
| 0xF6   | conv.i32.f64     | ... value &rarr; ... result                                 |
| 0xAD   | conv.i32.i64     | ... value &rarr; ... result                                 |
| 0xB1   | conv.i64.f32     | ... value &rarr; ... result                                 |
| 0xB3   | conv.i64.f64     | ... value &rarr; ... result                                 |
| 0xAF   | conv.i64.i32     | ... value &rarr; ... result                                 |
| 0xD2   | conv.i8.f32      | ... value &rarr; ... result                                 |
| 0xF2   | conv.i8.f64      | ... value &rarr; ... result                                 |
| 0x59   | conv.i8.i32      | ... value &rarr; ... result                                 |
| 0xA9   | conv.i8.i64      | ... value &rarr; ... result                                 |
| 0xD5   | conv.u16.f32     | ... value &rarr; ... result                                 |
| 0xF5   | conv.u16.f64     | ... value &rarr; ... result                                 |
| 0x5C   | conv.u16.i32     | ... value &rarr; ... result                                 |
| 0xAC   | conv.u16.i64     | ... value &rarr; ... result                                 |
| 0xD7   | conv.u32.f32     | ... value &rarr; ... result                                 |
| 0xF7   | conv.u32.f64     | ... value &rarr; ... result                                 |
| 0xAE   | conv.u32.i64     | ... value &rarr; ... result                                 |
| 0xB2   | conv.u64.f32     | ... value &rarr; ... result                                 |
| 0xB4   | conv.u64.f64     | ... value &rarr; ... result                                 |
| 0xB0   | conv.u64.i32     | ... value &rarr; ... result                                 |
| 0xD3   | conv.u8.f32      | ... value &rarr; ... result                                 |
| 0xF3   | conv.u8.f64      | ... value &rarr; ... result                                 |
| 0x5A   | conv.u8.i32      | ... value &rarr; ... result                                 |
| 0xAA   | conv.u8.i64      | ... value &rarr; ... result                                 |
| 0xC3   | div.f32          | ... value1 value2 &rarr; ... result                         |
| 0xE3   | div.f64          | ... value1 value2 &rarr; ... result                         |
| 0x43   | div.i32          | ... value1 value2 &rarr; ... result                         |
| 0x44   | div.i32.un       | ... value1 value2 &rarr; ... result                         |
| 0x93   | div.i64          | ... value1 value2 &rarr; ... result                         |
| 0x94   | div.i64.un       | ... value1 value2 &rarr; ... result                         |
| 0x06   | dup.f32          | ... value &rarr; ... value value                            |
| 0x07   | dup.f64          | ... value &rarr; ... value value                            |
| 0x06   | dup.i32          | ... value &rarr; ... value value                            |
| 0x07   | dup.i64          | ... value &rarr; ... value value                            |
| 0x06   | dup.ref          | ... value &rarr; ... value value                            |
| 0x10   | ldarg.f32        | ... &rarr; ... value                                        |
| 0x11   | ldarg.f64        | ... &rarr; ... value                                        |
| 0x0E   | ldarg.i16        | ... &rarr; ... value                                        |
| 0x10   | ldarg.i32        | ... &rarr; ... value                                        |
| 0x11   | ldarg.i64        | ... &rarr; ... value                                        |
| 0x0C   | ldarg.i8         | ... &rarr; ... value                                        |
| 0x10   | ldarg.ref        | ... &rarr; ... value                                        |
| 0x0F   | ldarg.u16        | ... &rarr; ... value                                        |
| 0x10   | ldarg.u32        | ... &rarr; ... value                                        |
| 0x11   | ldarg.u64        | ... &rarr; ... value                                        |
| 0x0D   | ldarg.u8         | ... &rarr; ... value                                        |
| 0x12   | ldarga           | ... &rarr; ... address                                      |
| 0x33   | ldc.f32          | ... &rarr; ... value                                        |
| 0x34   | ldc.f64          | ... &rarr; ... value                                        |
| 0x33   | ldc.i32          | ... &rarr; ... value                                        |
| 0x29   | ldc.i32.0        | ... &rarr; ... value                                        |
| 0x2A   | ldc.i32.1        | ... &rarr; ... value                                        |
| 0x2B   | ldc.i32.2        | ... &rarr; ... value                                        |
| 0x2C   | ldc.i32.3        | ... &rarr; ... value                                        |
| 0x2D   | ldc.i32.4        | ... &rarr; ... value                                        |
| 0x2E   | ldc.i32.5        | ... &rarr; ... value                                        |
| 0x2F   | ldc.i32.6        | ... &rarr; ... value                                        |
| 0x30   | ldc.i32.7        | ... &rarr; ... value                                        |
| 0x31   | ldc.i32.8        | ... &rarr; ... value                                        |
| 0x28   | ldc.i32.m1       | ... &rarr; ... value                                        |
| 0x32   | ldc.i32.s        | ... &rarr; ... value                                        |
| 0x34   | ldc.i64          | ... &rarr; ... value                                        |
| 0x82   | ldelem.f32       | ... length array index &rarr; ... value                     |
| 0x83   | ldelem.f64       | ... length array index &rarr; ... value                     |
| 0x80   | ldelem.i16       | ... length array index &rarr; ... value                     |
| 0x82   | ldelem.i32       | ... length array index &rarr; ... value                     |
| 0x83   | ldelem.i64       | ... length array index &rarr; ... value                     |
| 0x7E   | ldelem.i8        | ... length array index &rarr; ... value                     |
| 0x82   | ldelem.ref       | ... length array index &rarr; ... value                     |
| 0x81   | ldelem.u16       | ... length array index &rarr; ... value                     |
| 0x82   | ldelem.u32       | ... length array index &rarr; ... value                     |
| 0x83   | ldelem.u64       | ... length array index &rarr; ... value                     |
| 0x7F   | ldelem.u8        | ... length array index &rarr; ... value                     |
| 0x84   | ldelema          | ... length array index &rarr; ... address                   |
| 0x6C   | ldfld.f32        | ... address &rarr; ... value                                |
| 0x6D   | ldfld.f64        | ... address &rarr; ... value                                |
| 0x6A   | ldfld.i16        | ... address &rarr; ... value                                |
| 0x6C   | ldfld.i32        | ... address &rarr; ... value                                |
| 0x6D   | ldfld.i64        | ... address &rarr; ... value                                |
| 0x68   | ldfld.i8         | ... address &rarr; ... value                                |
| 0x6C   | ldfld.ref        | ... address &rarr; ... value                                |
| 0x6B   | ldfld.u16        | ... address &rarr; ... value                                |
| 0x6C   | ldfld.u32        | ... address &rarr; ... value                                |
| 0x6D   | ldfld.u64        | ... address &rarr; ... value                                |
| 0x69   | ldfld.u8         | ... address &rarr; ... value                                |
| 0x6E   | ldflda           | ... address &rarr; ... address                              |
| 0x35   | ldftn            | ... &rarr; ... ftn                                          |
| 0x04   | ldlen            | ... length array &rarr; ... length                          |
| 0x10   | ldloc.f32        | ... &rarr; ... value                                        |
| 0x11   | ldloc.f64        | ... &rarr; ... value                                        |
| 0x0E   | ldloc.i16        | ... &rarr; ... value                                        |
| 0x10   | ldloc.i32        | ... &rarr; ... value                                        |
| 0x11   | ldloc.i64        | ... &rarr; ... value                                        |
| 0x0C   | ldloc.i8         | ... &rarr; ... value                                        |
| 0x10   | ldloc.ref        | ... &rarr; ... value                                        |
| 0x0F   | ldloc.u16        | ... &rarr; ... value                                        |
| 0x10   | ldloc.u32        | ... &rarr; ... value                                        |
| 0x11   | ldloc.u64        | ... &rarr; ... value                                        |
| 0x0D   | ldloc.u8         | ... &rarr; ... value                                        |
| 0x12   | ldloca           | ... &rarr; ... address                                      |
| 0x29   | ldnull           | ... &rarr; ... null                                         |
| 0xC2   | mul.f32          | ... value1 value2 &rarr; ... result                         |
| 0xE2   | mul.f64          | ... value1 value2 &rarr; ... result                         |
| 0x42   | mul.i32          | ... value1 value2 &rarr; ... result                         |
| 0x92   | mul.i64          | ... value1 value2 &rarr; ... result                         |
| 0xC5   | neg.f32          | ... value &rarr; ... result                                 |
| 0xE5   | neg.f64          | ... value &rarr; ... result                                 |
| 0x47   | neg.i32          | ... value &rarr; ... result                                 |
| 0x97   | neg.i64          | ... value &rarr; ... result                                 |
| 0x61   | newarr           | ... length &rarr; ... length array                          |
| 0x60   | newobj           | ... &rarr; ... address                                      |
| 0x00   | nop              | ... &rarr; ...                                              |
| 0x4E   | not.i32          | ... value &rarr; ... result                                 |
| 0x9E   | not.i64          | ... value &rarr; ... result                                 |
| 0x4C   | or.i32           | ... value1 value2 &rarr; ... result                         |
| 0x9C   | or.i64           | ... value1 value2 &rarr; ... result                         |
| 0x04   | pop.f32          | ... value &rarr; ...                                        |
| 0x05   | pop.f64          | ... value &rarr; ...                                        |
| 0x04   | pop.i32          | ... value &rarr; ...                                        |
| 0x05   | pop.i64          | ... value &rarr; ...                                        |
| 0x04   | pop.ref          | ... value &rarr; ...                                        |
| 0xC4   | rem.f32          | ... value1 value2 &rarr; ... result                         |
| 0xE4   | rem.f64          | ... value1 value2 &rarr; ... result                         |
| 0x45   | rem.i32          | ... value1 value2 &rarr; ... result                         |
| 0x46   | rem.i32.un       | ... value1 value2 &rarr; ... result                         |
| 0x95   | rem.i64          | ... value1 value2 &rarr; ... result                         |
| 0x96   | rem.i64.un       | ... value1 value2 &rarr; ... result                         |
| 0x18   | ret              | ... &rarr; ...                                              |
| 0x19   | ret.f32          | ... value &rarr; ...                                        |
| 0x1A   | ret.f64          | ... value &rarr; ...                                        |
| 0x19   | ret.i16          | ... value &rarr; ...                                        |
| 0x19   | ret.i32          | ... value &rarr; ...                                        |
| 0x1A   | ret.i64          | ... value &rarr; ...                                        |
| 0x19   | ret.i8           | ... value &rarr; ...                                        |
| 0x19   | ret.ref          | ... value &rarr; ...                                        |
| 0x19   | ret.u16          | ... value &rarr; ...                                        |
| 0x19   | ret.u32          | ... value &rarr; ...                                        |
| 0x1A   | ret.u64          | ... value &rarr; ...                                        |
| 0x19   | ret.u8           | ... value &rarr; ...                                        |
| 0x48   | shl.i32          | ... value amount &rarr; ... result                          |
| 0x98   | shl.i64          | ... value amount &rarr; ... result                          |
| 0x49   | shr.i32          | ... value amount &rarr; ... result                          |
| 0x4A   | shr.i32.un       | ... value amount &rarr; ... result                          |
| 0x99   | shr.i64          | ... value amount &rarr; ... result                          |
| 0x9A   | shr.i64.un       | ... value amount &rarr; ... result                          |
| 0x13   | starg.f32        | ... value &rarr; ...                                        |
| 0x14   | starg.f64        | ... value &rarr; ...                                        |
| 0x13   | starg.i16        | ... value &rarr; ...                                        |
| 0x13   | starg.i32        | ... value &rarr; ...                                        |
| 0x14   | starg.i64        | ... value &rarr; ...                                        |
| 0x13   | starg.i8         | ... value &rarr; ...                                        |
| 0x13   | starg.ref        | ... value &rarr; ...                                        |
| 0x13   | starg.u16        | ... value &rarr; ...                                        |
| 0x13   | starg.u32        | ... value &rarr; ...                                        |
| 0x14   | starg.u64        | ... value &rarr; ...                                        |
| 0x13   | starg.u8         | ... value &rarr; ...                                        |
| 0x87   | stelem.f32       | ... length array index value &rarr; ...                     |
| 0x88   | stelem.f64       | ... length array index value &rarr; ...                     |
| 0x86   | stelem.i16       | ... length array index value &rarr; ...                     |
| 0x87   | stelem.i32       | ... length array index value &rarr; ...                     |
| 0x88   | stelem.i64       | ... length array index value &rarr; ...                     |
| 0x85   | stelem.i8        | ... length array index value &rarr; ...                     |
| 0x87   | stelem.ref       | ... length array index value &rarr; ...                     |
| 0x86   | stelem.u16       | ... length array index value &rarr; ...                     |
| 0x87   | stelem.u32       | ... length array index value &rarr; ...                     |
| 0x88   | stelem.u64       | ... length array index value &rarr; ...                     |
| 0x85   | stelem.u8        | ... length array index value &rarr; ...                     |
| 0x71   | stfld.f32        | ... address value &rarr; ...                                |
| 0x72   | stfld.f64        | ... address value &rarr; ...                                |
| 0x70   | stfld.i16        | ... address value &rarr; ...                                |
| 0x71   | stfld.i32        | ... address value &rarr; ...                                |
| 0x72   | stfld.i64        | ... address value &rarr; ...                                |
| 0x6F   | stfld.i8         | ... address value &rarr; ...                                |
| 0x71   | stfld.ref        | ... address value &rarr; ...                                |
| 0x70   | stfld.u16        | ... address value &rarr; ...                                |
| 0x71   | stfld.u32        | ... address value &rarr; ...                                |
| 0x72   | stfld.u64        | ... address value &rarr; ...                                |
| 0x6F   | stfld.u8         | ... address value &rarr; ...                                |
| 0x13   | stloc.f32        | ... value &rarr; ...                                        |
| 0x14   | stloc.f64        | ... value &rarr; ...                                        |
| 0x13   | stloc.i16        | ... value &rarr; ...                                        |
| 0x13   | stloc.i32        | ... value &rarr; ...                                        |
| 0x14   | stloc.i64        | ... value &rarr; ...                                        |
| 0x13   | stloc.i8         | ... value &rarr; ...                                        |
| 0x13   | stloc.ref        | ... value &rarr; ...                                        |
| 0x13   | stloc.u16        | ... value &rarr; ...                                        |
| 0x13   | stloc.u32        | ... value &rarr; ...                                        |
| 0x14   | stloc.u64        | ... value &rarr; ...                                        |
| 0x13   | stloc.u8         | ... value &rarr; ...                                        |
| 0xC1   | sub.f32          | ... value1 value2 &rarr; ... result                         |
| 0xE1   | sub.f64          | ... value1 value2 &rarr; ... result                         |
| 0x41   | sub.i32          | ... value1 value2 &rarr; ... result                         |
| 0x91   | sub.i64          | ... value1 value2 &rarr; ... result                         |
| 0x1E   | syscall          | ... argument0 argument1 ... argumentN &rarr; ... result     |
| 0x4D   | xor.i32          | ... value1 value2 &rarr; ... result                         |
| 0x9D   | xor.i64          | ... value1 value2 &rarr; ... result                         |
|        |                  |                                                             |
| 0x06   | dup              | ... value &rarr; ... value value                            |
| 0x07   | dup2             | ... value1 value2 &rarr; ... value1 value2 value1 value2    |
| 0x0A   | nip              | ... value1 value2 &rarr; ... value2                         |
| 0x08   | over             | ... value1 value2 &rarr; ... value1 value2 value1           |
| 0x04   | pop              | ... value &rarr; ...                                        |
| 0x05   | pop2             | ... value1 value2 &rarr; ...                                |
| 0x09   | rot              | ... value1 value2 value3 &rarr; ... value2 value3 value1    |
| 0x03   | swap             | ... value1 value2 &rarr; ... value2 value1                  |
| 0x0B   | tuck             | ... value1 value2 &rarr; ... value2 value1 value2           |
